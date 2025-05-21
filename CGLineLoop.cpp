#include "pch.h" 
#include "CGLineLoop.h" 
#include"CGRenderContext.h"
IMPLEMENT_SERIAL(CGLineLoop, CGGeometry, 1)

CGLineLoop::CGLineLoop()
{
}

CGLineLoop::CGLineLoop(const std::vector<glm::dvec3> mPoints):mPoints(mPoints)
{
}

CGLineLoop::~CGLineLoop()
{
}
void CGLineLoop::Serialize(CArchive& ar)
{

	CGRenderable::Serialize(ar);  //�ȵ��û�������л������������л��Լ��ĳ�Ա��������Ҫ�� 

	if (ar.IsStoring())
		//���� 
	{
		//ar << ;  
		//����������Ҫ��������ݳ�Ա��<<�����֧�ֵ����Ͳ���CArchiveʹ��˵�� 
	}
	else //��ȡ 
	{
		//ar >> ; 
	}
}

//��Ⱦ 
bool CGLineLoop::Render(CGRenderContext* pRC, CGCamera* pCamera)
{
	if (pRC == nullptr || pCamera == nullptr)
		return false;
	for (auto itr = mPoints.begin(); itr != mPoints.end(); ++itr)
	{
		if (itr + 1 != mPoints.end())
		{
			(new CCGRenderContext())->MidLine((int)itr->x, (int)itr->y, (int)(itr + 1)->x, (int)(itr+1)->y, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}
	//��Ҫ�պϣ��������һ�㵽��һ����
	(new CCGRenderContext())->MidLine((int)mPoints.front().x, (int)mPoints.front().y, (int)mPoints.back().x, (int)mPoints.back().y, glm::vec3(1.0f, 1.0f, 1.0f));
	return true;
}


void CGLineLoop::Translate(double tx, double ty) {
	// �������е㲢Ӧ��ƽ��
	for (auto& point : mPoints) {
		point.x += tx; // x ����ƽ��
		point.y += ty; // y ����ƽ��
	}
}

void CGLineLoop::Rotate(double angle, double cx, double cy)
{
	// ���Ƕ�ת��Ϊ���ȣ���Ϊ���Ǻ���ʹ�û��ȣ�
	double radians = angle * (M_PI / 180.0);

	// ������ת�����Ԫ��
	double cosTheta = cos(radians);
	double sinTheta = sin(radians);
	//1. ����ƽ�ƾ��󣨽����ĵ��ƶ���ԭ�㣩
	Translate(-cx, -cy);
	for (auto& point : mPoints) {//��ÿ���㶼������ת�任
		// 2. ��ת
		double x1 = point.x;
		point.x = x1 * cosTheta - point.y * sinTheta;
		point.y = x1 * sinTheta + point.y * cosTheta;
	}
	// 3. �����ĵ��ƻ�ԭλ��
	Translate(cx, cy);
}


void CGLineLoop::Scale(double sx, double sy, double cx, double cy)
{
	// 1.���������ĵ��ƶ���ԭ��
	Translate(-cx, -cy);

	// 2. ����
	for (auto& point : mPoints) {//��ÿ���㶼�������ű任
		// 2. ��ת
		
		point.x*=sx;
		point.y*=sy;
	}

	// 3. �����ĵ��ƻ�ԭλ��
	Translate(cx, cy);
}