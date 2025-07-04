#pragma once
#include "CGRenderState.h"
#include "CG2022112454���έDoc.h"
class MaterialControl : public CGObject
{
public:
    int cnt = 0;
};

class changeMaterial : public CGCallback
{
public:
	virtual bool run(CGObject* object, void* data) override
	{
        if (!mEnabled || !object ||!CCG2022112454���έDoc::doRecallM)
            return false;
        CGMaterial*body  = dynamic_cast<CGMaterial*>(object);  //Ҫ��任�ڵ� 
        if (body) {
            MaterialControl* d = nullptr;
            if (data) { //�Ƿ����Զ������ 
                d = dynamic_cast<MaterialControl*>((CGObject*)data);
            }
            if (d) {
                if (d->cnt < 50) {
                    d->cnt++;
                }
                else
                {
                    int t = body->t % 6 + 1;
                    body->setType(t); //��������ѡ�����
                    d->cnt = 0;
                }
            }
            else { //���û�����Զ���������Ĭ�ϲ������� 
                // Ĭ�ϣ��Ƹ�����ת10��
                if (d->cnt < 50) {
                    d->cnt++;
                }
                else
                {
                    int t = body->t % 6 + 1;
                    body->setType(t); //��������ѡ�����
                    d->cnt = 0;
                }
            }
            
            return true;
        }
        return false;
	}
};

