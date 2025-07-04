#pragma once
#include "CGObject.h"
#include "CGTransform.h"
#include "CG2022112454冷家苇Doc.h"
//节点变换自定义参数，可以根据更新需要定义参数 
class RobotBodyTransformParam : public CGObject
{
public:
    void setRotateDegree(float degree) { mRotateDegree = degree; }
    float rotateDegree() const { return mRotateDegree; }
    glm::vec3 rotate;
    float range;
    float now;
	int turn = 1; //1表示顺时针，0表示逆时针
protected:
    float mRotateDegree = 1.0f;
    
};

//旋转更新回调示例 
class RobotBodyRotate : public CGCallback
{
public:
    virtual bool run(CGObject* object, void* data) override

    {
        if (!mEnabled || !object||!CCG2022112454冷家苇Doc::doRecallR)
            return false;
        CGTransform* body = dynamic_cast<CGTransform*>(object);  //要求变换节点 
        if (body) {
            RobotBodyTransformParam* d = nullptr;
            if (data) { //是否传入自定义参数 
                d = dynamic_cast<RobotBodyTransformParam*>((CGObject*)data);
            }
            if (d) {
				if (d->turn) {
                    d->now += d->rotateDegree();
					body->rotate(d->rotateDegree(), d->rotate.x, d->rotate.y, d->rotate.z);
                    if (d->now >= d->range) {
                        d->turn = 0;
                    }
                }
                else {
                    d->now -= d->rotateDegree();
                    body->rotate(-d->rotateDegree(), d->rotate.x, d->rotate.y, d->rotate.z);
                    if (d->now <= 0) {
						d->turn = 1;
                    }
                }
                
            }
            else { //如果没传入自定参数，按默认参数进行 
                body->rotate(1.0f, 0.0f, 1.0f, 0.0f);
            }
            return true;
        }
        return false;
    }
};

