#pragma once
#include "CGObject.h"
#include "CGTransform.h"
#include "CG2022112454���έDoc.h"
//�ڵ�任�Զ�����������Ը��ݸ�����Ҫ������� 
class RobotBodyTransformParam : public CGObject
{
public:
    void setRotateDegree(float degree) { mRotateDegree = degree; }
    float rotateDegree() const { return mRotateDegree; }
    glm::vec3 rotate;
    float range;
    float now;
	int turn = 1; //1��ʾ˳ʱ�룬0��ʾ��ʱ��
protected:
    float mRotateDegree = 1.0f;
    
};

//��ת���»ص�ʾ�� 
class RobotBodyRotate : public CGCallback
{
public:
    virtual bool run(CGObject* object, void* data) override

    {
        if (!mEnabled || !object||!CCG2022112454���έDoc::doRecallR)
            return false;
        CGTransform* body = dynamic_cast<CGTransform*>(object);  //Ҫ��任�ڵ� 
        if (body) {
            RobotBodyTransformParam* d = nullptr;
            if (data) { //�Ƿ����Զ������ 
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
            else { //���û�����Զ���������Ĭ�ϲ������� 
                body->rotate(1.0f, 0.0f, 1.0f, 0.0f);
            }
            return true;
        }
        return false;
    }
};

