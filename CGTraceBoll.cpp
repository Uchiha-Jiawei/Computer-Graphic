#include "pch.h"
#include "CGTraceBoll.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CG2022112454���έDoc.h" // ����View֮ǰҪ����Doc
#include "CG2022112454���έView.h" // �û��������ƣ�����ͼ�ζ���ͨ��Doc��ӵ�����
#include <iostream>
#include "CGCamera.h" // ��� CGCamera �İ����ļ�
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
    mPivotPoint = glm::vec3(0.0f, 0.0f, 0.0f); // ��ʼ����ת���ĵ�
}


CGTraceBoll::~CGTraceBoll() = default;

EventType CGTraceBoll::GetType()
{
    return EventType::InteractByKey;
}


int CGTraceBoll::OnMouseButton(GLFWwindow* window, int button, int action, int mods) { //shift���������趨��꽻����׼�㣬Ĭ�ϣ�0��0��
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
    return 0; // �������������Ҳ������¼�������Ѷ�ռ�����򷵻� 1
}

int CGTraceBoll::OnCursorPos(GLFWwindow* window, double xpos, double ypos) {
    if (!camera) return 0;
    CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
    if (!view) return 0;

    glm::dvec2 currentPos(xpos, ypos);
    glm::dvec2 delta = currentPos - mLastCursorPos;
    mLastCursorPos = currentPos;

    if (mLeftMouseButtonDown)
    {
        // Χ�� Y ����ת (ˮƽ�ƶ�)
        float yawAngle = static_cast<float>(delta.x) * 0.005f; // ����������
        // Χ�� X ����ת (��ֱ�ƶ�)
        float pitchAngle = static_cast<float>(delta.y) * 0.005f; // ����������

        // Ӧ��ƫ�� (Χ������ Y ��)
        glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), yawAngle, glm::vec3(0, 1, 0));
        camera->mEye = glm::vec3(yaw * glm::vec4(camera->mEye - mPivotPoint, 1.0f)) + mPivotPoint;
        camera->mUp = glm::vec3(yaw * glm::vec4(camera->mUp, 0.0f)); // ͬʱ��ת��������

        // Ӧ�ø��� (Χ������ֲ� X ��)
        glm::vec3 right = glm::normalize(glm::cross(camera->mTarget - camera->mEye, camera->mUp));
        glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), pitchAngle, right);
        camera->mEye = glm::vec3(pitch * glm::vec4(camera->mEye - mPivotPoint, 1.0f)) + mPivotPoint;
        camera->mUp = glm::vec3(pitch * glm::vec4(camera->mUp, 0.0f));

        view->ShowPrompt("�����ת");
        view->Invalidate();
        view->UpdateWindow();
        return 1;
    }
    else if (mRightMouseButtonDown)
    {
        // ʹ������Ҽ��϶����� (��ֱ�ƶ�)
        float zoomFactor = 1.0f - static_cast<float>(delta.y) * 0.01f; // ����������
        camera->mLeft *= zoomFactor;
        camera->mRight *= zoomFactor;
        camera->mBottom *= zoomFactor;
        camera->mTop *= zoomFactor;
        view->ShowPrompt("�������");
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
