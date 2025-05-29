#pragma once
#include "UIEventHandler.h"
#include "CGRenderable.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CGTraceBoll : public UIEventHandler
{
public:
    CGTraceBoll(
        CGCamera* camera,
        GLFWwindow* window // Fixed missing type specifier issue by completing the parameter list
    );
    ~CGTraceBoll();

    virtual EventType GetType() override;
    virtual int OnMouseButton(GLFWwindow* window, int button, int action, int mods) override;
    virtual int OnCursorPos(GLFWwindow* window, double xpos, double ypos) override;
    virtual int Cancel(GLFWwindow* window) override;

protected:
    CGCamera* camera;
    glm::vec3 mPivotPoint;

    glm::dvec2 mLastCursorPos;
    bool mLeftMouseButtonDown = false;
    bool mRightMouseButtonDown = false;
    const double M_PI = 3.1415926;
};
