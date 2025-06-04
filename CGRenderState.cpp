#include "pch.h"
#include "CGRenderState.h"

void CGColor::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);
}

void CGPointSize::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glPointSize(mPointSize);
}

void CGLineWidth::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glLineWidth(mLineWidth);
}

void CGLineStipple::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glLineStipple(mFactor, mPattern);
}

void CGPolygonMode::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	if (mFrontFace == mBackFace) {
		glPolygonMode(GL_FRONT_AND_BACK, (GLenum)mFrontFace);
	}
	else {
		glPolygonMode(GL_FRONT, (GLenum)mFrontFace);
		glPolygonMode(GL_BACK, (GLenum)mBackFace);
	}
}

void CGRenderStateSet::setRenderState(std::shared_ptr<CGRenderState> renderstate, int index)
{
	if (renderstate) {
		for (unsigned i = 0; i < mRenderStates.size(); ++i) {
			if (mRenderStates[i].mRS->type() == renderstate->type() && mRenderStates[i].mIndex == index) {
				mRenderStates[i].mRS = renderstate;
				//mRenderStates[i].mIndex = index;
				return;
			}
		}
		mRenderStates.push_back(CGRenderStateSlot(renderstate, index));
	}
}
CGRenderState* CGRenderStateSet::renderState(ERenderState type, int index)
{
	for (unsigned i = 0; i < mRenderStates.size(); ++i)
		if (mRenderStates[i].mRS->type() == type && mRenderStates[i].mIndex == index)
			return mRenderStates[i].mRS.get();
	return nullptr;
}
const CGRenderState* CGRenderStateSet::renderState(ERenderState type, int index) const
{
	for (unsigned i = 0; i < mRenderStates.size(); ++i)
		if (mRenderStates[i].mRS->type() == type && mRenderStates[i].mIndex == index)
			return mRenderStates[i].mRS.get();
	return nullptr;
}
void CGRenderStateSet::eraseRenderState(ERenderState type, int index)
{
	for (unsigned i = 0; i < mRenderStates.size(); ++i) {
		if (mRenderStates[i].mRS->type() == type && (index == mRenderStates[i].mIndex || index == -1)) {
			mRenderStates.erase(mRenderStates.begin() + i);
			if (index == -1)
				continue;
			else
				return;
		}
	}
}

void CGRenderStateSet::apply(const CGCamera* camera, CGRenderContext* ctx)
{
	for (auto itr = mModes.begin(); itr != mModes.end(); ++itr)
	{
		if (itr->second) {
			glEnable(itr->first);
		}
		else {
			glDisable(itr->first);
		}
	}
	for (auto itr = mRenderStates.begin(); itr != mRenderStates.end(); ++itr) {
		if (itr->mRS->GetUpdateCallback()) {
			itr->mRS->GetUpdateCallback()->run(itr->mRS.get(), itr->mRS->userData());
		}
		itr->apply(camera, ctx);
	}
}

CGLightModel::CGLightModel()
	: mAmbientColor(0.2f, 0.2f, 0.2f, 1.0f), mLocalViewer(false), mTwoSide(false)
{
}
void CGLightModel::apply(const CGCamera*, CGRenderContext*, int) const
{
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, mLocalViewer ? 1.0f : 0.0f);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &mAmbientColor.r);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, mTwoSide ? 1.0f : 0.0f);
}

CGShadeModel::CGShadeModel(EShadeModel shademodel)
	: mShadeModel(shademodel)
{
}
void CGShadeModel::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glShadeModel((GLenum)mShadeModel);
}

CGMaterial::CGMaterial()
	: mAmbient(0.2f, 0.2f, 0.2f, 1.0f)
	, mDiffuse(0.8f, 0.8f, 0.8f, 1.0f)
	, mSpecular(0.0f, 0.0f, 0.0f, 1.0f)
	, mEmission(0.0f, 0.0f, 0.0f, 1.0f)
	, mShininess(0.0f)
	, mColorMaterialMode(CM_AMBIENT_AND_DIFFUSE) // 默认glColor影响环境光和漫反射光
{
}

void CGMaterial::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &mAmbient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &mDiffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mSpecular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &mEmission.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);

	GLenum face = GL_FRONT_AND_BACK;
	GLenum mode = GL_AMBIENT_AND_DIFFUSE; // 默认值
	switch (mColorMaterialMode)
	{
	case CM_AMBIENT:
		mode = GL_AMBIENT;
		break;
	case CM_DIFFUSE:
		mode = GL_DIFFUSE;
		break;
	case CM_SPECULAR:
		mode = GL_SPECULAR;
		break;
	case CM_EMISSION:
		mode = GL_EMISSION;
		break;
	case CM_AMBIENT_AND_DIFFUSE:
		mode = GL_AMBIENT_AND_DIFFUSE;
		break;
	}
	{
		glColorMaterial(face, mode);
	}
}

CGLight::CGLight(int lightIndex)
	: mLightIndex(lightIndex)
	, mEnabled(true) // 默认启用
	, mAmbient(0.0f, 0.0f, 0.0f, 1.0f)       // 默认无环境光
	, mDiffuse(1.0f, 1.0f, 1.0f, 1.0f)       // 默认白色漫反射光
	, mSpecular(1.0f, 1.0f, 1.0f, 1.0f)      // 默认白色镜面光
	, mPosition(0.0f, 0.0f, 1.0f, 0.0f)      // 默认方向光源，沿Z轴正向
	, mSpotDirection(0.0f, 0.0f, -1.0f)    // 默认聚光灯方向，沿Z轴负向
	, mSpotExponent(0.0f)                  // 默认无聚光效果
	, mSpotCutoff(180.0f)                  // 默认是点光源 (180度意味着不是聚光灯)
	, mConstantAttenuation(0.0f)
	, mLinearAttenuation(0.0f)
	, mQuadraticAttenuation(0.0f)
{
}

ERenderState CGLight::type() const
{
	// 根据 mLightIndex 返回对应的 ERenderState 类型
	// 例如，如果 mLightIndex 是 0, 返回 RS_Light0, 如果是 1, 返回 RS_Light1, 等等。
	// 这里需要确保 ERenderState 枚举中有足够的 RS_LightX 定义。
	if (mLightIndex >= 0 && mLightIndex < 8) // OpenGL通常支持至少8个光源
	{
		return static_cast<ERenderState>(RS_Light0 + mLightIndex);
	}
	return RS_NONE; // 或者抛出异常，表示无效的光源索引
}

void CGLight::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	// index 参数在这里通常与 mLightIndex 相同，或者可以忽略 index 而使用 mLightIndex
	// 为保持一致性，我们使用 mLightIndex
	GLenum light = GL_LIGHT0 + mLightIndex;

	if (mEnabled)
	{
		glEnable(light);

		glLightfv(light, GL_AMBIENT, &mAmbient.r);
		glLightfv(light, GL_DIFFUSE, &mDiffuse.r);
		glLightfv(light, GL_SPECULAR, &mSpecular.r);
		glLightfv(light, GL_POSITION, &mPosition.r); // 位置受当前ModelView矩阵影响

		// 聚光灯参数 (仅当 mSpotCutoff < 180.0f 时这些参数才有意义)
		if (mSpotCutoff < 180.0f)
		{
			glLightfv(light, GL_SPOT_DIRECTION, &mSpotDirection.x);
			glLightf(light, GL_SPOT_EXPONENT, mSpotExponent);
			glLightf(light, GL_SPOT_CUTOFF, mSpotCutoff);
		}
		else // 如果是点光源或方向光源，确保聚光灯参数被设置为默认值，以避免意外行为
		{
			// 对于非聚光灯，通常将SPOT_CUTOFF设为180，SPOT_DIRECTION和SPOT_EXPONENT可以不设置或设为默认
			// OpenGL规范中，如果GL_SPOT_CUTOFF是180，则光源被视为全向光源，其他聚光灯参数被忽略。
			glLightf(light, GL_SPOT_CUTOFF, 180.0f);
		}

		// 衰减参数
		glLightf(light, GL_CONSTANT_ATTENUATION, mConstantAttenuation);
		glLightf(light, GL_LINEAR_ATTENUATION, mLinearAttenuation);
		glLightf(light, GL_QUADRATIC_ATTENUATION, mQuadraticAttenuation);
	}
	else
	{
		glDisable(light);
	}

	// 注意：glEnable(GL_LIGHTING)应该在渲染循环的更高层级进行管理，
	// 而不是在单个光源的apply函数中。这里只负责启用/禁用特定的GL_LIGHTi。
}
