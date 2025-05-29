#include "pch.h"
#include "CGTraceBoll.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CG2022112454冷家苇Doc.h" // 包含View之前要包含Doc
#include "CG2022112454冷家苇View.h" // 用户交互绘制，并将图形对象通过Doc添加到场景
#include <iostream>
#include "CGCamera.h" // 添加 CGCamera 的包含文件
CGTraceBoll::CGTraceBoll(
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


CGTraceBoll::~CGTraceBoll() = default;

EventType CGTraceBoll::GetType()
{
    return EventType::InteractByKey;
}


int CGTraceBoll::OnMouseButton(GLFWwindow* window, int button, int action, int mods) { //shift加鼠标左键设定鼠标交互基准点，默认（0，0）
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

int CGTraceBoll::OnCursorPos(GLFWwindow* window, double xpos, double ypos) {
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



int CGTraceBoll::Cancel(GLFWwindow* window)
{
    return 1;
}
