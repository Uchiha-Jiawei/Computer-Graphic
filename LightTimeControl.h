#pragma once
#include "CGObject.h"
#include "CGRenderState.h"
#include "CGLight.h"
#include "CG2022112454冷家苇Doc.h"
class LightTimeControl : public CGObject
{
public:
    void setxDegree(float degree) { x = degree; }
    void setyDegree(float degree) { y = degree; }
    void setzDegree(float degree) { z = degree; }
    float x = 10.0f;
    float y = 10.0f;
    float z = 10.0f;

};

//更新位置回调
class LightChange : public CGCallback
{
public:
    virtual bool run(CGObject* object, void* data) override
    {
        if (!mEnabled || !object ||!CCG2022112454冷家苇Doc::doRecallL)
            return false;
        CGLight* body = dynamic_cast<CGLight*>(object);  //要求变换节点 
        if (body) {
            LightTimeControl* d = nullptr;
            if (data) { //是否传入自定义参数 
                d = dynamic_cast<LightTimeControl*>((CGObject*)data);
            }
            glm::vec3 position = glm::vec3(body->mPosition); // 获取当前位置
            if (d) {
                float xAngle = glm::radians(d->x); // 转换为弧度
                float yAngle = glm::radians(d->y);
                float zAngle = glm::radians(d->z);

                // 绕X轴旋转
                position = glm::rotateX(position, xAngle);

                // 绕Y轴旋转
                position = glm::rotateY(position, yAngle);

                // 绕Z轴旋转
                position = glm::rotateZ(position, zAngle);
            }
            else { //如果没传入自定参数，按默认参数进行 
                // 默认：绕各轴旋转10度
                float angle = glm::radians(1.0f);
                position = glm::rotateX(position, angle);
                position = glm::rotateY(position, angle);
                position = glm::rotateZ(position, angle);
            }
            body->setPosition(glm::vec4(position, body->mPosition.w)); // 保留 w 分量
            return true;
        }
        return false;
    }
};


