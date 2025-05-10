#include "pch.h"
#include "CGModel2DTransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CG2022112454冷家苇Doc.h" // 包含View之前要包含Doc
#include "CG2022112454冷家苇View.h" // 用户交互绘制，并将图形对象通过Doc添加到场景
#include <iostream>
CGModel2DTransform::CGModel2DTransform(
	CGNode* node,
	GLFWwindow* window,
	bool isScaleX,
	bool isScaleY,
	bool isMoving,
	glm::vec3 pivotPoint,
	bool showPivot
) : UIEventHandler(window),
mNode(node),
mIsScaleX(isScaleX),
mIsScaleY(isScaleY),
mPivotPoint(pivotPoint),
mShowPivot(showPivot),
mIsRotating(false),      // 固定默认值
mIsMoving(isMoving),
mLastCursorPos(0.0),
mIsDragging(false)
{
	if (window) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glfwFocusWindow(window);
		mLastCursorPos = glm::dvec2(x, y);
	}
}
bool Moving = false;

CGModel2DTransform::~CGModel2DTransform() = default;

EventType CGModel2DTransform::GetType()
{
	return EventType::InteractByMouse;
}

int CGModel2DTransform::OnMouseButton(GLFWwindow* window, int button, int action, int mods) { //shift加鼠标左键设定鼠标交互基准点，默认（0，0）
	if (!mNode || !window) return 0;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !mIsMoving) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (mods & GLFW_MOD_SHIFT) {
			// Shift+左键：设置旋转中心（需转换坐标）
			CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
			if (view) {
				// 转换屏幕坐标 (DCS) → 世界坐标 (WCS)
				glm::dvec3 pivotDCS(x, y, 0.0);
				mPivotPoint = view->DCS2WCS(pivotDCS); // 存储为世界坐标
				mShowPivot = true;
				std::string coordText = "基准点: (" +
					std::to_string(mPivotPoint.x) + ", " +
					std::to_string(mPivotPoint.y) + ", " +
					std::to_string(mPivotPoint.z) + ")";
				// 在界面上显示信息
				view->ShowPrompt(coordText.c_str());
				view->Invalidate();
				view->UpdateWindow();
			}
		}
	}
	// 右键直接旋转
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			mIsRotating = true; // 开始旋转
			glm::dvec3 pivotDCS(x, y, 0.0);
			mLastCursorPos = pivotDCS;
			glfwGetCursorPos(window, &mLastCursorPos.x, &mLastCursorPos.y);
		}
		else if (action == GLFW_RELEASE) {
			mIsRotating = false; // 停止旋转
		}
	}
	// 左键直接移动
	else if (button == GLFW_MOUSE_BUTTON_LEFT && mIsMoving) {
		if (action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			Moving = true;
			glm::dvec3 pivotDCS(x, y, 0.0);
			mLastCursorPos = pivotDCS;
			glfwGetCursorPos(window, &mLastCursorPos.x, &mLastCursorPos.y);
		}
		else if (action == GLFW_RELEASE) {
			Moving = false; // 停止旋转
		}
	}
	return 1;
}

int CGModel2DTransform::OnCursorPos(GLFWwindow* window, double xpos, double ypos) {
	if (mNode && window && Moving) { //拖动状态
		CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
		if (!view) return 0;
		glm::dvec3 currentPosDCS(xpos, ypos, 0.0);
		glm::dvec3 currentPosWCS = view->DCS2WCS(currentPosDCS);
		glm::dvec3 lastPosDCS(mLastCursorPos.x, mLastCursorPos.y, 0.0);
		glm::dvec3 lastPosWCS = view->DCS2WCS(lastPosDCS);
		double dx = currentPosWCS.x - lastPosWCS.x;
		double dy = currentPosWCS.y - lastPosWCS.y;
		std::string coordText = "X方向移动: (" + std::to_string(dx)+ "），Y方向移动:(" + std::to_string(dy) + ")";
		view->ShowPrompt(coordText.c_str());
		// 移动
		mNode->Translate(dx,dy);
		view->Invalidate();
		view->UpdateWindow();
		mLastCursorPos = glm::dvec2(xpos, ypos);
		return 1;
	}
	if (!mNode || !window || !mIsRotating) return 0;
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (!view) return 0;
	glm::dvec3 currentPosDCS(xpos, ypos, 0.0);
	glm::dvec3 currentPosWCS = view->DCS2WCS(currentPosDCS);
	glm::dvec3 lastPosDCS(mLastCursorPos.x, mLastCursorPos.y, 0.0);
	glm::dvec3 lastPosWCS = view->DCS2WCS(lastPosDCS);
	//利用双点计算出旋转角度
	double currentDx = currentPosWCS.x - mPivotPoint.x;
	double currentDy = currentPosWCS.y - mPivotPoint.y;
	double currentAngle = atan2(currentDy, currentDx);
	double lastDx = lastPosWCS.x - mPivotPoint.x;
	double lastDy = lastPosWCS.y - mPivotPoint.y;
	double lastAngle = atan2(lastDy, lastDx);
	double deltaAngleByPosition = currentAngle - lastAngle;
	if (deltaAngleByPosition > M_PI) {//保证旋转角度正常
		deltaAngleByPosition -= 2.0 * M_PI;
	}
	else if (deltaAngleByPosition < -M_PI) {
		deltaAngleByPosition += 2.0 * M_PI;
	}
	double deltaAngle = deltaAngleByPosition*45.0;
	// 显示
	std::string coordText = "旋转中心: (" + std::to_string(mPivotPoint.x) + std::to_string(mPivotPoint.y) + "），旋转角度:(" + std::to_string(deltaAngle) + ")";
	view->ShowPrompt(coordText.c_str());
	// 旋转
	mNode->Rotate(deltaAngle, mPivotPoint.x, mPivotPoint.y);
	view->Invalidate();
	view->UpdateWindow();
	mLastCursorPos = glm::dvec2(xpos, ypos);
	return 1;
}

int CGModel2DTransform::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)//ctrl+滚轮实现缩放
{
	// 检查是否按下 Ctrl 键
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (!view) return 0;
	bool isCtrlPressed = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
	if (isCtrlPressed && mNode != nullptr)
	{
		// 计算缩放因子（滚轮向上放大，向下缩小）
		double scaleFactor = (yoffset > 0) ? 1.1 : 0.9; // 10% 缩放
		// 直接调用节点的Scale方法
		mNode->Scale(
			mIsScaleX ? scaleFactor : 1.0,//这里判断是否右某方向缩放
			mIsScaleY ? scaleFactor : 1.0,
			mPivotPoint.x,
			mPivotPoint.y
		);
		view->ShowPrompt("已实现缩放！");
		view->Invalidate();
		view->UpdateWindow();
		return 1; // 事件已处理
	}
	return 0; // 事件未处理
}

int CGModel2DTransform::Cancel(GLFWwindow* window)
{
	mIsRotating = false;
	return 1;
}
