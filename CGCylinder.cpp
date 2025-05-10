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

    // 1. ���Ʋ���
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

                // ���߷���ָ����࣬Բ��Ĭ���� Z ������
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

    // 2. ���Ƶ���
    if (createBottom && radiusBottom > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f); // Բ��
        for (int j = 0; j <= slices; ++j) {
            float angle = j * angleStep;
            float x = cos(angle);
            float y = sin(angle);
            glTexCoord2f((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f);
            glVertex3f(radiusBottom * x, radiusBottom * y, 0.0f);
        }
        glEnd();
    }

    // 3. ���ƶ���
    if (createTop && radiusTop > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, height); // Բ��
        for (int j = 0; j <= slices; ++j) {
            float angle = -j * angleStep; // ע�ⶥ������������෴
            float x = cos(angle);
            float y = sin(angle);
            glTexCoord2f((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f);
            glVertex3f(radiusTop * x, radiusTop * y, height);
        }
        glEnd();
    }
}

