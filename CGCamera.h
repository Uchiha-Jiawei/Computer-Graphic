#pragma once 
#include "CGTransform.h" 


typedef enum
{
	CF_DO_NOT_CLEAR = 0,
	CF_CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
	CF_CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
	CF_CLEAR_STENCIL = GL_STENCIL_BUFFER_BIT,
	CF_CLEAR_COLOR_DEPTH = CF_CLEAR_COLOR + CF_CLEAR_DEPTH,
	CF_CLEAR_COLOR_STENCIL = CF_CLEAR_COLOR + CF_CLEAR_STENCIL,
	CF_CLEAR_DEPTH_STENCIL = CF_CLEAR_DEPTH + CF_CLEAR_STENCIL,
	CF_CLEAR_COLOR_DEPTH_STENCIL = CF_CLEAR_COLOR + CF_CLEAR_DEPTH + CF_CLEAR_STENCIL,
} EClearFlags;

class Viewport : public CGObject
{
	DECLARE_SERIAL(Viewport) // �������л�
public:
	Viewport();
	Viewport(int x, int y, int w, int h);
	void activate() const; // �����ӿ�

	void set(int x, int y, int w, int h) { mX = x; mY = y; mWidth = w; mHeight = h; } // �����ӿ�λ�úʹ�С
	void setX(int x) { mX = x; } // �����ӿ�X����
	int x() const { return mX; } // ��ȡ�ӿ�X����
	void setY(int y) { mY = y; } // �����ӿ�Y����
	int y() const { return mY; } // ��ȡ�ӿ�Y����

		void setWidth(int width) { mWidth = width; } // �����ӿڿ��
	int width() const { return mWidth; } // ��ȡ�ӿڿ��
	void setHeight(int height) { mHeight = height; } // �����ӿڸ߶�
	int height() const { return mHeight; } // ��ȡ�ӿڸ߶�
	glm::vec2 center() const { return glm::vec2(mX + mWidth / 2.0f, mY + mHeight / 2.0f); } // ��ȡ�ӿ����ĵ�

	void setClearColor(float r, float g, float b, float a) { mClearColor = glm::vec4(r, g, b, a); } // ���������ɫ
	void setClearColor(const glm::vec4& color) { mClearColor = color; } // ���������ɫ
	const glm::vec4& clearColor() const { return mClearColor; } // ��ȡ�����ɫ

	void setClearStencil(int stencil) { mClearStencil = stencil; } // �������ģ��ֵ
	int clearStencil() const { return mClearStencil; } // ��ȡ���ģ��ֵ

	void setClearDepth(GLfloat depth) { mClearDepth = depth; } // ����������ֵ
	GLfloat clearDepth() const { return mClearDepth; } // ��ȡ������ֵ

	void setClearFlags(EClearFlags clear_flags) { mClearFlags = clear_flags; } // ���������־
	EClearFlags clearFlags() const { return mClearFlags; } // ��ȡ�����־

	void setScissorEnabled(bool enable) { mScissorEnabled = enable; } // ���òü������Ƿ�����
	bool isScissorEnabled() const { return mScissorEnabled; } // ��ȡ�ü������Ƿ�����

protected:
	int mX = 0; // �ӿ�X����
	int mY = 0; // �ӿ�Y����
	int mWidth = 0; // �ӿڿ��
	int mHeight = 0; // �ӿڸ߶�
	glm::vec4 mClearColor = glm::vec4(0, 0, 0, 1); // �����ɫ

	GLfloat mClearDepth = 1.0f; // ������ֵ
	int mClearStencil = 0; // ���ģ��ֵ

	EClearFlags mClearFlags = CF_CLEAR_COLOR_DEPTH; // �����־
	bool mScissorEnabled = true; // �ü������Ƿ�����
};

class CGCamera : public CGTransform
{

	DECLARE_SERIAL(CGCamera) // �������л�
public:

	CGCamera();

	virtual ~CGCamera();

	//���л� 

	virtual void Serialize(CArchive& ar) override;


	//mode=0Ϊƽ��ͶӰ����ά����mode=1Ϊ͸��ͶӰ��mode=2��ʾ��άƽ��ͶӰ����ά�� 

	virtual void Projection(int mode);

	int ProjectionMode() { return mProjectionMode; } // ��ȡͶӰģʽ

	void SetProjectionMode(int mode) { if (mode >= 0 && mode <= 2) mProjectionMode = mode; } // ����ͶӰģʽ

//protected: // �Ѹ���Ϊ public
public: // �� protected ����Ϊ public �Ա� CGCameraControl �����ɵط���

	int mProjectionMode = 0; //ͶӰģʽ��0��ƽ��ͶӰ��1��͸��ͶӰ��2����άƽ��ͶӰ
//protected: // �Ѹ���Ϊ public
//std::shared_ptr<Viewport> mViewport; // ����Ѿ�ͨ�� viewport() getter/setter ��Ϊ��Ч�� public
	//�۲��嶨�壨�ڹ۲�����ϵ�ж��壩���۲촰����ͶӰƽ���ϵ��������ϣ��۲촰�ڵĽ����桢Զ���� 
	GLfloat mLeft = -200.0f; // ��߽�
	GLfloat mBottom = -200.0f; // �±߽�
	GLfloat mRight = 200.0f; // �ұ߽�
	GLfloat mTop = 200.0f; // �ϱ߽�
	GLfloat mNearPlane = 100.0f;  //�����棨����ΪEye�ڹ۲췽���ͶӰ�� 
	GLfloat mFarPlane = 10000.0f;  //Զ���棨��Զ�ܿ�����Զ���� 
	glm::vec3 mEye = glm::vec3(0, 0, 400); //���λ�ã�͸��ͶӰ������ͶӰ 
	glm::vec3 mTarget = glm::vec3(0, 0, 0); //���۲�� 
	glm::vec3 mUp = glm::vec3(0, 1, 0); //������Ϸ���һ��Y�����ϣ���Ӧ��Ļ��Y��

public:
	std::shared_ptr<Viewport> mViewport; // �� protected ���� public �Ա�����Ҫʱֱ�ӷ��ʣ�����ѡ viewport()
	void setViewport(Viewport* viewport) { mViewport = std::shared_ptr<Viewport>(viewport); } // �����ӿ�
	Viewport* viewport() { return mViewport.get(); } // ��ȡ�ӿ�
	const Viewport * viewport() const { return mViewport.get(); } // ��ȡ�����ӿ�
};