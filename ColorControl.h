#pragma once
#include "CGRenderState.h"
#include "CG2022112454冷家苇Doc.h"
class ColorControl : public CGObject
{
public:
    int cnt = 0;
};

class changeColor : public CGCallback
{
public:
    virtual bool run(CGObject* object, void* data) override
    {
        if (!mEnabled || !object||!CCG2022112454冷家苇Doc::doRecallM)
            return false;
        CGColor* body = dynamic_cast<CGColor*>(object);  //要求变换节点 
        if (body) {
            ColorControl* d = nullptr;
            if (data) { //是否传入自定义参数 
                d = dynamic_cast<ColorControl*>((CGObject*)data);
            }
            if (d) {
                if (d->cnt < 50) {
                    d->cnt++;
                }           
                else
                {
                    int t = body->t % 6 + 1;
                     body->setType(t); //根据类型选择材质
                     d->cnt = 0;
                }
            }
            else { //如果没传入自定参数，按默认参数进行 
                // 默认：绕各轴旋转10度
                if (d->cnt < 50) {
                    d->cnt++;
                }
                else
                {
                    int t = body->t % 6 + 1;
                    body->setType(t); //根据类型选择材质
                    d->cnt = 0;
                }
            }
            return true;
        }
        return false;
    }
};



