#include "pch.h" 
#include "CGTransform.h" 

IMPLEMENT_SERIAL(CGTransform, CGGroup, 1)

CGTransform::CGTransform()
{
}

CGTransform::~CGTransform()
{
}
void CGTransform::Serialize(CArchive& ar)
{
    CGGroup::Serialize(ar);
    //�ȵ��û�������л������������л��Լ��ĳ�Ա��������Ҫ�� 
    if (ar.IsStoring())

        //���� 
    {
        //ar << ; //����������Ҫ��������ݳ�Ա��<<�����֧�ֵ����Ͳ���CArchiveʹ��˵�� 
    }
    else //��ȡ 
    {
        //ar >> ; 
    }
}

bool CGTransform::Render(CGRenderContext* pRC, CGCamera* pCamera)
{
    if (pRC == nullptr || pCamera == nullptr)
        return false;

    glPushMatrix(); //���� 
    glMultMatrixf(glm::value_ptr(localMatrix())); //�����һ���任 
    if (getRenderStateSet()) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        getRenderStateSet()->apply(pCamera, pRC);
    }
    for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
    {
        (*itr)->Render(pRC, pCamera);
    }
    if (getRenderStateSet()) {
        glPopAttrib();
    }
    glPopMatrix();  //�ָ� 

    return true;
}

void CGTransform::dirtyWorldMatrix()
{
    mWorldMatrixDirty = true;
    for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
    {
        CGTransform* trans = (*itr != nullptr) ? (*itr)->asTransform() : 0;
        if (trans) {
            trans->dirtyWorldMatrix();
        }

    }
    dirtyBound();
}
void CGTransform::setLocalMatrix(const glm::mat4& m)
{
    mLocalMatrix = m;
    dirtyWorldMatrix(); //�ֲ�����仯�ᵼ�µ���������ϵ�ľ���ı䡣 
}
void CGTransform::translate(float tx, float ty, float tz)
{
    //���в���ʵ�ֹ��� 
    translate(glm::vec3(tx, ty, tz));

}
void CGTransform::translate(const glm::vec3& t)
{
    //���в���ʵ�ֹ��� 
	// ƽ�ƾ���
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), t);
	// ����������Ϊ��Ҫ����
    postMultiply(trans);
}
void CGTransform::scale(float sx, float sy, float sz)
{
    //���в���ʵ�ֹ��� 
	// ���ž���
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
	// ����������Ϊ��Ҫ����
    postMultiply(scaleMat);
}
void CGTransform::rotate(float degrees, float x, float y, float z)
{
    //���в���ʵ�ֹ��� 
	// ��ת����
    glm::mat4 rotMat = glm::rotate(glm::radians(degrees), glm::vec3(x, y, z));
    // ����������Ϊ��Ҫ����
    postMultiply(rotMat);
}
void CGTransform::preMultiply(const glm::mat4& m)
{
    //���в���ʵ�ֹ��� 
	// ���¾ֲ����󣺽�����ľ�����˵��ֲ�������
    mLocalMatrix = m * mLocalMatrix;
	// ����������Ϊ��Ҫ����
    dirtyWorldMatrix();
}
void CGTransform::postMultiply(const glm::mat4& m)
{
    //���в���ʵ�ֹ��� 
     // ���¾ֲ����󣺽�����ľ����ҳ˵��ֲ�������
    mLocalMatrix = mLocalMatrix * m;
    // ����������Ϊ��Ҫ����
    dirtyWorldMatrix();
}