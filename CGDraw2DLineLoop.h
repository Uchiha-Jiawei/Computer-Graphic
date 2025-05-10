#pragma once 
#include "UIEventHandler.h" 
#include "glIncludes.h" 
#include "vector"
class CGDraw2DLineLoop : public UIEventHandler
{
public:
    CGDraw2DLineLoop(GLFWwindow* window = nullptr);
    ~CGDraw2DLineLoop();
    virtual EventType GetType() override;      //�������� 
    //�ٶ�����������ʰȡ�߶���㡢�յ㣬�ƶ����ʱ����Ƥ���� 
    virtual int OnMouseButton(GLFWwindow* window, int button, int action, int mods) override;
    virtual int OnCursorPos(GLFWwindow* window, double xpos, double ypos) override;
    //ȡ������ִ�е�����簴ESC���� 
    virtual int Cancel(GLFWwindow* window) override;
protected:
    std::vector<glm::dvec3> mPoints; // �洢�պ��ߵ����ж��� 
    glm::dvec3 mStart;   //����ĵ�һ����Ϊ�պ������ 
    glm::dvec3 mPre;   //��¼�յ�ǰ����һ���� 
    glm::dvec3 mNext;     //��¼��һ���� 
    //��ʱ��ŵ�ǰλ�õ�ǰһ�������λ�ã���Ϊ˫�����ԭ����Ƥ�߿��ܻ��Ƶ���ǰ��֡�� 
    glm::dvec3 mPrePos1, mPrePos2;
    int mMoveCount = 0;
};