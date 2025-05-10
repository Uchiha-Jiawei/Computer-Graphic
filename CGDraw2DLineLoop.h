#pragma once 
#include "UIEventHandler.h" 
#include "glIncludes.h" 
#include "vector"
class CGDraw2DLineLoop : public UIEventHandler
{
public:
    CGDraw2DLineLoop(GLFWwindow* window = nullptr);
    ~CGDraw2DLineLoop();
    virtual EventType GetType() override;      //命令类型 
    //假定鼠标左键单击拾取线段起点、终点，移动鼠标时画橡皮筋线 
    virtual int OnMouseButton(GLFWwindow* window, int button, int action, int mods) override;
    virtual int OnCursorPos(GLFWwindow* window, double xpos, double ypos) override;
    //取消正在执行的命令（如按ESC键） 
    virtual int Cancel(GLFWwindow* window) override;
protected:
    std::vector<glm::dvec3> mPoints; // 存储闭合线的所有顶点 
    glm::dvec3 mStart;   //点击的第一点作为闭合线起点 
    glm::dvec3 mPre;   //记录终点前的上一个点 
    glm::dvec3 mNext;     //记录下一个点 
    //临时存放当前位置的前一、二光标位置（因为双缓存的原因，橡皮线可能绘制到了前两帧） 
    glm::dvec3 mPrePos1, mPrePos2;
    int mMoveCount = 0;
};