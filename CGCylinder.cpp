#include "pch.h"
#include "CGCylinder.h"
#include "TessellationHints.h"

IMPLEMENT_SERIAL(CGCylinder, CGRenderable, 1)

void CGCylinder::setTessellationHints(std::shared_ptr<TessellationHints> hints)
{
    if (mTessellationHints.get() != hints.get()) {
        mTessellationHints = hints;
        mDisplayListDirty = true;
    }
}

void CGCylinder::buildDisplayList()
{
    TessellationHints* hints = tessellationHints();
    bool createBody = (hints ? hints->createBody() : true);
    bool createTop = (hints ? hints->createTop() : true);
    bool createBottom = (hints ? hints->createBottom() : true);

    float radiusBottom = mRadiusBottom;
    float radiusTop = mRadiusTop;
    float height = mHeight;
    int slices = mSlices;
    int stacks = mStacks;

    const float stepZ = height / stacks;
    const float stepRadius = (radiusTop - radiusBottom) / stacks;
    const float angleStep = 2.0f * glm::pi<float>() / slices;

    // 1. 绘制侧面
    if (createBody) {
        for (int i = 0; i < stacks; ++i) {
            float z0 = i * stepZ;
            float z1 = (i + 1) * stepZ;
            float r0 = radiusBottom + stepRadius * i;
            float r1 = radiusBottom + stepRadius * (i + 1);

            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; ++j) {
                float angle = j * angleStep;
                float x = cos(angle);
                float y = sin(angle);

                // 法线方向指向外侧，圆柱默认沿 Z 轴拉伸
                glNormal3f(x, y, 0.0f);
                glTexCoord2f(j / (float)slices, (i + 1) / (float)stacks);
                glVertex3f(r1 * x, r1 * y, z1);

                glNormal3f(x, y, 0.0f);
                glTexCoord2f(j / (float)slices, i / (float)stacks);
                glVertex3f(r0 * x, r0 * y, z0);
            }
            glEnd();
        }
    }

    // 2. 绘制底面
    if (createBottom && radiusBottom > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f); // 圆心
        for (int j = 0; j <= slices; ++j) {
            float angle = j * angleStep;
            float x = cos(angle);
            float y = sin(angle);
            glTexCoord2f((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f);
            glVertex3f(radiusBottom * x, radiusBottom * y, 0.0f);
        }
        glEnd();
    }

    // 3. 绘制顶面
    if (createTop && radiusTop > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, height); // 圆心
        for (int j = 0; j <= slices; ++j) {
            float angle = -j * angleStep; // 注意顶面绕向与底面相反
            float x = cos(angle);
            float y = sin(angle);
            glTexCoord2f((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f);
            glVertex3f(radiusTop * x, radiusTop * y, height);
        }
        glEnd();
    }
}

