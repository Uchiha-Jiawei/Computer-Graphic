#include "pch.h"
#include "CGCameraControl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CG2022112454冷家苇Doc.h" // 包含View之前要包含Doc
#include "CG2022112454冷家苇View.h" // 用户交互绘制，并将图形对象通过Doc添加到场景
#include <iostream>
#include "CGCamera.h" // 添加 CGCamera 的包含文件
CGCameraControl::CGCameraControl(
	CGCamera* camera,
	GLFWwindow* window
) : UIEventHandler(window),
camera(camera)
{
	if (window) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glfwFocusWindow(window);
		mLastCursorPos = glm::dvec2(x, y);
	}
	mPivotPoint = glm::vec3(0.0f, 0.0f, 0.0f); // 初始化旋转中心点
}


CGCameraControl::~CGCameraControl() = default;

EventType CGCameraControl::GetType()
{
	return EventType::InteractByKey;
}

int CGCameraControl::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (!camera) return 0;
        CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
        if (!view) return 0;

        float zoomFactor = 1.25f;
        float trans = 10.0f;

        switch (key)
        {
        case GLFW_KEY_EQUAL: // + 键 (通常需要 shift)
            if (mods == GLFW_MOD_SHIFT) // 对于 + (Shift + =)
            {
                camera->mLeft *= zoomFactor;
                camera->mRight *= zoomFactor;
                camera->mBottom *= zoomFactor;
                camera->mTop *= zoomFactor;
                view->ShowPrompt("+：观察窗口放大到原1.25倍");
            }
            break;
        case GLFW_KEY_MINUS: // - 键
            camera->mLeft /= zoomFactor;
            camera->mRight /= zoomFactor;
            camera->mBottom /= zoomFactor;
            camera->mTop /= zoomFactor;
            view->ShowPrompt("-：观察窗口缩小到原0.8倍");
            break;
        case GLFW_KEY_L:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+L: 左视图
                camera->mEye = glm::vec3(-400, 0, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("：Ctrl+L：左视图");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mLeft += trans;
                view->ShowPrompt("l：观察窗口左边界右移");
            }
            else
            { // L: 左平移
                camera->mLeft -= trans;
                view->ShowPrompt("L：观察窗口左边界左移");
            }
            break;
        case GLFW_KEY_R:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+R: 右视图
                camera->mEye = glm::vec3(400, 0, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+R：右视图");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mRight -= trans;
                view->ShowPrompt("R：观察窗口右边界左移");
            }
            else
            { // R: 右平移 (减小右边界)
                camera->mRight += trans;
                view->ShowPrompt("r：观察窗口右边界右移");
            }
            break;
        case GLFW_KEY_B:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+B: 后视图
                camera->mEye = glm::vec3(0, 0, -400);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+B：后视图");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mBottom -= trans;
                view->ShowPrompt("B: 观察窗口下边界下移");
            }
            else
            { // B: 下平移
                camera->mBottom += trans;
                view->ShowPrompt("b: 观察窗口下边界上移");
            }
            break;
        case GLFW_KEY_T:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+T: 顶视图 (俯视图)
                camera->mEye = glm::vec3(0, 400, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 0, -1); // 对于顶视图，up 通常是 -Z 或 +Z，具体取决于约定
                view->ShowPrompt("Ctrl+T：俯视图");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mTop -= trans;
                view->ShowPrompt("T：观察窗口上边界下移");
            }
            else
            { // t: 上平移
                camera->mTop += trans;
                view->ShowPrompt("t：观察窗口上边界上移");
            }
            break;
        case GLFW_KEY_P:
            camera->SetProjectionMode(1); // 透视投影
            view->ShowPrompt("透视投影");
            break;
        case GLFW_KEY_O:
            camera->SetProjectionMode(0); // 正交投影
            view->ShowPrompt("正交投影");
            break;
        case GLFW_KEY_F:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+F: 前视图
                camera->mEye = glm::vec3(0, 0, 400);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+F：前视图");
            }
            break;
        case GLFW_KEY_D:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+D: 底视图 (仰视图)
                camera->mEye = glm::vec3(0, -400, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 0, 1); // 对于底视图，up 通常是 +Z 或 -Z
                view->ShowPrompt("Ctrl+D：顶视图 ");
            }
            break;
        default:
            return 0; // 未处理
        }
        view->Invalidate();
        view->UpdateWindow();
        return 1; // 已处理
    }
    return 0; // 未处理
}

int CGCameraControl::OnMouseButton(GLFWwindow* window, int button, int action, int mods) { //shift加鼠标左键设定鼠标交互基准点，默认（0，0）
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            mLeftMouseButtonDown = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            mRightMouseButtonDown = true;
        }
        glfwGetCursorPos(window, &mLastCursorPos.x, &mLastCursorPos.y);
    }
    else if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            mLeftMouseButtonDown = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            mRightMouseButtonDown = false;
        }
    }
    return 0; // 让其他处理程序也处理此事件，如果已独占处理则返回 1
}

int CGCameraControl::OnCursorPos(GLFWwindow* window, double xpos, double ypos) {
    if (!camera) return 0;
    CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
    if (!view) return 0;

    glm::dvec2 currentPos(xpos, ypos);
    glm::dvec2 delta = currentPos - mLastCursorPos;
    mLastCursorPos = currentPos;

    if (mLeftMouseButtonDown)
    {
        // 围绕 Y 轴旋转 (水平移动)
        float yawAngle = static_cast<float>(delta.x) * 0.005f; // 调整灵敏度
        // 围绕 X 轴旋转 (垂直移动)
        float pitchAngle = static_cast<float>(delta.y) * 0.005f; // 调整灵敏度

        // 应用偏航 (围绕世界 Y 轴)
        glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), yawAngle, glm::vec3(0, 1, 0));
        camera->mEye = glm::vec3(yaw * glm::vec4(camera->mEye - mPivotPoint, 1.0f)) + mPivotPoint;
        camera->mUp = glm::vec3(yaw * glm::vec4(camera->mUp, 0.0f)); // 同时旋转向上向量

        // 应用俯仰 (围绕相机局部 X 轴)
        glm::vec3 right = glm::normalize(glm::cross(camera->mTarget - camera->mEye, camera->mUp));
        glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), pitchAngle, right);
        camera->mEye = glm::vec3(pitch * glm::vec4(camera->mEye - mPivotPoint, 1.0f)) + mPivotPoint;
        camera->mUp = glm::vec3(pitch * glm::vec4(camera->mUp, 0.0f));

        view->ShowPrompt("相机旋转");
        view->Invalidate();
        view->UpdateWindow();
        return 1;
    }
    else if (mRightMouseButtonDown)
    {
        // 使用鼠标右键拖动缩放 (垂直移动)
        float zoomFactor = 1.0f - static_cast<float>(delta.y) * 0.01f; // 调整灵敏度
        camera->mLeft *= zoomFactor;
        camera->mRight *= zoomFactor;
        camera->mBottom *= zoomFactor;
        camera->mTop *= zoomFactor;
        view->ShowPrompt("鼠标缩放");
        view->Invalidate();
        view->UpdateWindow();
        return 1;
    }

	return 0;
}

int CGCameraControl::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)//ctrl+滚轮实现缩放
{
	// 检查是否按下 Ctrl 键
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (!view || !camera) return 0;

    // 滚动缩放 (相机与目标点之间的距离)
    float zoomSensitivity = 20.0f;
    glm::vec3 viewDir = glm::normalize(camera->mTarget - camera->mEye);
    camera->mEye += viewDir * (float)yoffset * zoomSensitivity;

    // 如果处于正交模式，则同时调整正交投影边界
    // 这使得在正交模式下滚动缩放感觉更自然，同时改变了观察体的大小
    if (camera->ProjectionMode() == 0 || camera->ProjectionMode() == 2) {
        double scaleFactor = (yoffset > 0) ? 0.9 : 1.1; // 对于滚动反转：向上滚动 = 放大 = 边界变小
        camera->mLeft *= scaleFactor;
        camera->mRight *= scaleFactor;
        camera->mBottom *= scaleFactor;
        camera->mTop *= scaleFactor;
    }

	view->ShowPrompt("滚轮缩放");
	view->Invalidate();
	view->UpdateWindow();
	return 1; // 事件已处理
}

int CGCameraControl::Cancel(GLFWwindow* window)
{
	return 1;
}
