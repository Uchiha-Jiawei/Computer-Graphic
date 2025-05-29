#include "pch.h"
#include "CGCameraControl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CG2022112454���έDoc.h" // ����View֮ǰҪ����Doc
#include "CG2022112454���έView.h" // �û��������ƣ�����ͼ�ζ���ͨ��Doc��ӵ�����
#include <iostream>
#include "CGCamera.h" // ��� CGCamera �İ����ļ�
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
	mPivotPoint = glm::vec3(0.0f, 0.0f, 0.0f); // ��ʼ����ת���ĵ�
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
        CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
        if (!view) return 0;

        float zoomFactor = 1.25f;
        float trans = 10.0f;

        switch (key)
        {
        case GLFW_KEY_EQUAL: // + �� (ͨ����Ҫ shift)
            if (mods == GLFW_MOD_SHIFT) // ���� + (Shift + =)
            {
                camera->mLeft *= zoomFactor;
                camera->mRight *= zoomFactor;
                camera->mBottom *= zoomFactor;
                camera->mTop *= zoomFactor;
                view->ShowPrompt("+���۲촰�ڷŴ�ԭ1.25��");
            }
            break;
        case GLFW_KEY_MINUS: // - ��
            camera->mLeft /= zoomFactor;
            camera->mRight /= zoomFactor;
            camera->mBottom /= zoomFactor;
            camera->mTop /= zoomFactor;
            view->ShowPrompt("-���۲촰����С��ԭ0.8��");
            break;
        case GLFW_KEY_L:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+L: ����ͼ
                camera->mEye = glm::vec3(-400, 0, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("��Ctrl+L������ͼ");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mLeft += trans;
                view->ShowPrompt("l���۲촰����߽�����");
            }
            else
            { // L: ��ƽ��
                camera->mLeft -= trans;
                view->ShowPrompt("L���۲촰����߽�����");
            }
            break;
        case GLFW_KEY_R:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+R: ����ͼ
                camera->mEye = glm::vec3(400, 0, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+R������ͼ");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mRight -= trans;
                view->ShowPrompt("R���۲촰���ұ߽�����");
            }
            else
            { // R: ��ƽ�� (��С�ұ߽�)
                camera->mRight += trans;
                view->ShowPrompt("r���۲촰���ұ߽�����");
            }
            break;
        case GLFW_KEY_B:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+B: ����ͼ
                camera->mEye = glm::vec3(0, 0, -400);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+B������ͼ");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mBottom -= trans;
                view->ShowPrompt("B: �۲촰���±߽�����");
            }
            else
            { // B: ��ƽ��
                camera->mBottom += trans;
                view->ShowPrompt("b: �۲촰���±߽�����");
            }
            break;
        case GLFW_KEY_T:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+T: ����ͼ (����ͼ)
                camera->mEye = glm::vec3(0, 400, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 0, -1); // ���ڶ���ͼ��up ͨ���� -Z �� +Z������ȡ����Լ��
                view->ShowPrompt("Ctrl+T������ͼ");
            }
            else if (mods == GLFW_MOD_SHIFT) {
                camera->mTop -= trans;
                view->ShowPrompt("T���۲촰���ϱ߽�����");
            }
            else
            { // t: ��ƽ��
                camera->mTop += trans;
                view->ShowPrompt("t���۲촰���ϱ߽�����");
            }
            break;
        case GLFW_KEY_P:
            camera->SetProjectionMode(1); // ͸��ͶӰ
            view->ShowPrompt("͸��ͶӰ");
            break;
        case GLFW_KEY_O:
            camera->SetProjectionMode(0); // ����ͶӰ
            view->ShowPrompt("����ͶӰ");
            break;
        case GLFW_KEY_F:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+F: ǰ��ͼ
                camera->mEye = glm::vec3(0, 0, 400);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 1, 0);
                view->ShowPrompt("Ctrl+F��ǰ��ͼ");
            }
            break;
        case GLFW_KEY_D:
            if (mods == GLFW_MOD_CONTROL)
            { // Ctrl+D: ����ͼ (����ͼ)
                camera->mEye = glm::vec3(0, -400, 0);
                camera->mTarget = glm::vec3(0, 0, 0);
                camera->mUp = glm::vec3(0, 0, 1); // ���ڵ���ͼ��up ͨ���� +Z �� -Z
                view->ShowPrompt("Ctrl+D������ͼ ");
            }
            break;
        default:
            return 0; // δ����
        }
        view->Invalidate();
        view->UpdateWindow();
        return 1; // �Ѵ���
    }
    return 0; // δ����
}

int CGCameraControl::OnMouseButton(GLFWwindow* window, int button, int action, int mods) { //shift���������趨��꽻����׼�㣬Ĭ�ϣ�0��0��
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

int CGCameraControl::OnCursorPos(GLFWwindow* window, double xpos, double ypos) {
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

int CGCameraControl::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)//ctrl+����ʵ������
{
	// ����Ƿ��� Ctrl ��
	CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
	if (!view || !camera) return 0;

    // �������� (�����Ŀ���֮��ľ���)
    float zoomSensitivity = 20.0f;
    glm::vec3 viewDir = glm::normalize(camera->mTarget - camera->mEye);
    camera->mEye += viewDir * (float)yoffset * zoomSensitivity;

    // �����������ģʽ����ͬʱ��������ͶӰ�߽�
    // ��ʹ��������ģʽ�¹������Ÿо�����Ȼ��ͬʱ�ı��˹۲���Ĵ�С
    if (camera->ProjectionMode() == 0 || camera->ProjectionMode() == 2) {
        double scaleFactor = (yoffset > 0) ? 0.9 : 1.1; // ���ڹ�����ת�����Ϲ��� = �Ŵ� = �߽��С
        camera->mLeft *= scaleFactor;
        camera->mRight *= scaleFactor;
        camera->mBottom *= scaleFactor;
        camera->mTop *= scaleFactor;
    }

	view->ShowPrompt("��������");
	view->Invalidate();
	view->UpdateWindow();
	return 1; // �¼��Ѵ���
}

int CGCameraControl::Cancel(GLFWwindow* window)
{
	return 1;
}
