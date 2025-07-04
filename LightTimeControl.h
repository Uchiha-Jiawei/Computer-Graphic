#pragma once
#include "CGObject.h"
#include "CGRenderState.h"
#include "CGLight.h"
#include "CG2022112454���έDoc.h"
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

//����λ�ûص�
class LightChange : public CGCallback
{
public:
    virtual bool run(CGObject* object, void* data) override
    {
        if (!mEnabled || !object ||!CCG2022112454���έDoc::doRecallL)
            return false;
        CGLight* body = dynamic_cast<CGLight*>(object);  //Ҫ��任�ڵ� 
        if (body) {
            LightTimeControl* d = nullptr;
            if (data) { //�Ƿ����Զ������ 
                d = dynamic_cast<LightTimeControl*>((CGObject*)data);
            }
            glm::vec3 position = glm::vec3(body->mPosition); // ��ȡ��ǰλ��
            if (d) {
                float xAngle = glm::radians(d->x); // ת��Ϊ����
                float yAngle = glm::radians(d->y);
                float zAngle = glm::radians(d->z);

                // ��X����ת
                position = glm::rotateX(position, xAngle);

                // ��Y����ת
                position = glm::rotateY(position, yAngle);

                // ��Z����ת
                position = glm::rotateZ(position, zAngle);
            }
            else { //���û�����Զ���������Ĭ�ϲ������� 
                // Ĭ�ϣ��Ƹ�����ת10��
                float angle = glm::radians(1.0f);
                position = glm::rotateX(position, angle);
                position = glm::rotateY(position, angle);
                position = glm::rotateZ(position, angle);
            }
            body->setPosition(glm::vec4(position, body->mPosition.w)); // ���� w ����
            return true;
        }
        return false;
    }
};


