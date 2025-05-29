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
	DECLARE_SERIAL(Viewport) // 声明序列化
public:
	Viewport();
	Viewport(int x, int y, int w, int h);
	void activate() const; // 激活视口

	void set(int x, int y, int w, int h) { mX = x; mY = y; mWidth = w; mHeight = h; } // 设置视口位置和大小
	void setX(int x) { mX = x; } // 设置视口X坐标
	int x() const { return mX; } // 获取视口X坐标
	void setY(int y) { mY = y; } // 设置视口Y坐标
	int y() const { return mY; } // 获取视口Y坐标

		void setWidth(int width) { mWidth = width; } // 设置视口宽度
	int width() const { return mWidth; } // 获取视口宽度
	void setHeight(int height) { mHeight = height; } // 设置视口高度
	int height() const { return mHeight; } // 获取视口高度
	glm::vec2 center() const { return glm::vec2(mX + mWidth / 2.0f, mY + mHeight / 2.0f); } // 获取视口中心点

	void setClearColor(float r, float g, float b, float a) { mClearColor = glm::vec4(r, g, b, a); } // 设置清除颜色
	void setClearColor(const glm::vec4& color) { mClearColor = color; } // 设置清除颜色
	const glm::vec4& clearColor() const { return mClearColor; } // 获取清除颜色

	void setClearStencil(int stencil) { mClearStencil = stencil; } // 设置清除模板值
	int clearStencil() const { return mClearStencil; } // 获取清除模板值

	void setClearDepth(GLfloat depth) { mClearDepth = depth; } // 设置清除深度值
	GLfloat clearDepth() const { return mClearDepth; } // 获取清除深度值

	void setClearFlags(EClearFlags clear_flags) { mClearFlags = clear_flags; } // 设置清除标志
	EClearFlags clearFlags() const { return mClearFlags; } // 获取清除标志

	void setScissorEnabled(bool enable) { mScissorEnabled = enable; } // 设置裁剪测试是否启用
	bool isScissorEnabled() const { return mScissorEnabled; } // 获取裁剪测试是否启用

protected:
	int mX = 0; // 视口X坐标
	int mY = 0; // 视口Y坐标
	int mWidth = 0; // 视口宽度
	int mHeight = 0; // 视口高度
	glm::vec4 mClearColor = glm::vec4(0, 0, 0, 1); // 清除颜色

	GLfloat mClearDepth = 1.0f; // 清除深度值
	int mClearStencil = 0; // 清除模板值

	EClearFlags mClearFlags = CF_CLEAR_COLOR_DEPTH; // 清除标志
	bool mScissorEnabled = true; // 裁剪测试是否启用
};

class CGCamera : public CGTransform
{

	DECLARE_SERIAL(CGCamera) // 声明序列化
public:

	CGCamera();

	virtual ~CGCamera();

	//序列化 

	virtual void Serialize(CArchive& ar) override;


	//mode=0为平行投影（三维），mode=1为透视投影，mode=2表示二维平行投影（二维） 

	virtual void Projection(int mode);

	int ProjectionMode() { return mProjectionMode; } // 获取投影模式

	void SetProjectionMode(int mode) { if (mode >= 0 && mode <= 2) mProjectionMode = mode; } // 设置投影模式

//protected: // 已更改为 public
public: // 从 protected 更改为 public 以便 CGCameraControl 更轻松地访问

	int mProjectionMode = 0; //投影模式，0：平行投影，1：透视投影，2：二维平行投影
//protected: // 已更改为 public
//std::shared_ptr<Viewport> mViewport; // 这个已经通过 viewport() getter/setter 变为有效的 public
	//观察体定义（在观察坐标系中定义），观察窗口在投影平面上的左右下上，观察窗口的近裁面、远裁面 
	GLfloat mLeft = -200.0f; // 左边界
	GLfloat mBottom = -200.0f; // 下边界
	GLfloat mRight = 200.0f; // 右边界
	GLfloat mTop = 200.0f; // 上边界
	GLfloat mNearPlane = 100.0f;  //近裁面（距离为Eye在观察方向的投影） 
	GLfloat mFarPlane = 10000.0f;  //远裁面（最远能看到的远方） 
	glm::vec3 mEye = glm::vec3(0, 0, 400); //相机位置，透视投影和正交投影 
	glm::vec3 mTarget = glm::vec3(0, 0, 0); //被观察点 
	glm::vec3 mUp = glm::vec3(0, 1, 0); //相机向上方向，一般Y轴向上，对应屏幕的Y轴

public:
	std::shared_ptr<Viewport> mViewport; // 从 protected 移至 public 以便在需要时直接访问，但首选 viewport()
	void setViewport(Viewport* viewport) { mViewport = std::shared_ptr<Viewport>(viewport); } // 设置视口
	Viewport* viewport() { return mViewport.get(); } // 获取视口
	const Viewport * viewport() const { return mViewport.get(); } // 获取常量视口
};