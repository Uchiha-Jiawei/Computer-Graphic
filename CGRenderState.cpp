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
	, mColorMaterialMode(CM_AMBIENT_AND_DIFFUSE) // Ĭ��glColorӰ�컷������������
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
	GLenum mode = GL_AMBIENT_AND_DIFFUSE; // Ĭ��ֵ
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
	, mEnabled(true) // Ĭ������
	, mAmbient(0.0f, 0.0f, 0.0f, 1.0f)       // Ĭ���޻�����
	, mDiffuse(1.0f, 1.0f, 1.0f, 1.0f)       // Ĭ�ϰ�ɫ�������
	, mSpecular(1.0f, 1.0f, 1.0f, 1.0f)      // Ĭ�ϰ�ɫ�����
	, mPosition(0.0f, 0.0f, 1.0f, 0.0f)      // Ĭ�Ϸ����Դ����Z������
	, mSpotDirection(0.0f, 0.0f, -1.0f)    // Ĭ�Ͼ۹�Ʒ�����Z�Ḻ��
	, mSpotExponent(0.0f)                  // Ĭ���޾۹�Ч��
	, mSpotCutoff(180.0f)                  // Ĭ���ǵ��Դ (180����ζ�Ų��Ǿ۹��)
	, mConstantAttenuation(0.0f)
	, mLinearAttenuation(0.0f)
	, mQuadraticAttenuation(0.0f)
{
}

ERenderState CGLight::type() const
{
	// ���� mLightIndex ���ض�Ӧ�� ERenderState ����
	// ���磬��� mLightIndex �� 0, ���� RS_Light0, ����� 1, ���� RS_Light1, �ȵȡ�
	// ������Ҫȷ�� ERenderState ö�������㹻�� RS_LightX ���塣
	if (mLightIndex >= 0 && mLightIndex < 8) // OpenGLͨ��֧������8����Դ
	{
		return static_cast<ERenderState>(RS_Light0 + mLightIndex);
	}
	return RS_NONE; // �����׳��쳣����ʾ��Ч�Ĺ�Դ����
}

void CGLight::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	// index ����������ͨ���� mLightIndex ��ͬ�����߿��Ժ��� index ��ʹ�� mLightIndex
	// Ϊ����һ���ԣ�����ʹ�� mLightIndex
	GLenum light = GL_LIGHT0 + mLightIndex;

	if (mEnabled)
	{
		glEnable(light);

		glLightfv(light, GL_AMBIENT, &mAmbient.r);
		glLightfv(light, GL_DIFFUSE, &mDiffuse.r);
		glLightfv(light, GL_SPECULAR, &mSpecular.r);
		glLightfv(light, GL_POSITION, &mPosition.r); // λ���ܵ�ǰModelView����Ӱ��

		// �۹�Ʋ��� (���� mSpotCutoff < 180.0f ʱ��Щ������������)
		if (mSpotCutoff < 180.0f)
		{
			glLightfv(light, GL_SPOT_DIRECTION, &mSpotDirection.x);
			glLightf(light, GL_SPOT_EXPONENT, mSpotExponent);
			glLightf(light, GL_SPOT_CUTOFF, mSpotCutoff);
		}
		else // ����ǵ��Դ�����Դ��ȷ���۹�Ʋ���������ΪĬ��ֵ���Ա���������Ϊ
		{
			// ���ڷǾ۹�ƣ�ͨ����SPOT_CUTOFF��Ϊ180��SPOT_DIRECTION��SPOT_EXPONENT���Բ����û���ΪĬ��
			// OpenGL�淶�У����GL_SPOT_CUTOFF��180�����Դ����Ϊȫ���Դ�������۹�Ʋ��������ԡ�
			glLightf(light, GL_SPOT_CUTOFF, 180.0f);
		}

		// ˥������
		glLightf(light, GL_CONSTANT_ATTENUATION, mConstantAttenuation);
		glLightf(light, GL_LINEAR_ATTENUATION, mLinearAttenuation);
		glLightf(light, GL_QUADRATIC_ATTENUATION, mQuadraticAttenuation);
	}
	else
	{
		glDisable(light);
	}

	// ע�⣺glEnable(GL_LIGHTING)Ӧ������Ⱦѭ���ĸ��߲㼶���й���
	// �������ڵ�����Դ��apply�����С�����ֻ��������/�����ض���GL_LIGHTi��
}
