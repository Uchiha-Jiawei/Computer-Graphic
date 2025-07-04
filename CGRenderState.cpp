#include "pch.h"
#include "CGRenderState.h"

void CGColor::apply(const CGCamera* camera, CGRenderContext* ctx, int index) const
{
	glColor4f(mColor.r, mColor.g, mColor.b, mColor.a);
}

void CGColor::setType(int i)
{
	switch (i) {
	case 1: // 金属材质（黄铜）
		setValue(glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f));
		t = 1;
		break;

	case 2: // 塑料材质（白色塑料）
		setValue(glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));
		t = 2;
		break;

	case 3: // 木材材质（松木）
		setValue(glm::vec4(0.78f, 0.53f, 0.25f, 1.0f));
		t = 3;
		break;

	case 4: // 陶瓷材质（白色）
		setValue(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		t = 4;
		break;

	case 5: // 玻璃材质（透明蓝色）
		setValue(glm::vec4(0.4f, 0.4f, 0.8f, 0.5f));
		t = 5;
		break;
	default: // 金属材质（铬）
		setValue(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		t = 6;
		break;
	}
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
void  CGMaterial::setType(int i) {
	switch (i) {
	case 1: // 金属材质（黄铜）
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f));
		setDiffuse(glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f));
		setSpecular(glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f));
		setShininess(27.8974f);
		setColorMaterial(CM_AMBIENT_AND_DIFFUSE);
		t = 1;
		break;

	case 2: // 塑料材质（白色塑料）
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setDiffuse(glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));//
		setSpecular(glm::vec4(0.70f, 0.70f, 0.70f, 1.0f));
		setShininess(32.0f);
		setColorMaterial(CM_DIFFUSE);
		t = 2;
		break;

	case 3: // 木材材质（松木）
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.2f, 0.13f, 0.05f, 1.0f));
		setDiffuse(glm::vec4(0.78f, 0.53f, 0.25f, 1.0f));
		setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		setShininess(5.0f);
		setColorMaterial(CM_AMBIENT_AND_DIFFUSE);
		t = 3;
		break;

	case 4: // 陶瓷材质
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		setDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		setSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		setShininess(100.0f);
		setColorMaterial(CM_AMBIENT_AND_DIFFUSE);
		t = 4;
		break;

	case 5: // 玻璃材质
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
		setDiffuse(glm::vec4(0.4f, 0.4f, 0.4f, 0.5f));
		setSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		setShininess(96.0f);
		setColorMaterial(CM_AMBIENT_AND_DIFFUSE);
		t = 5;
		break;

	default: // 金属材质（铬）
		setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		setAmbient(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
		setDiffuse(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		setSpecular(glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f));
		setShininess(76.8f);
		setColorMaterial(CM_AMBIENT_AND_DIFFUSE);
		t = 6;
		break;
	}
}