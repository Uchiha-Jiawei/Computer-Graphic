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

	CGRenderable::Serialize(ar);  //先调用基类的序列化函数，再序列化自己的成员（根据需要） 

	if (ar.IsStoring())
		//保存 
	{
		//ar << ;  
		//保存自身需要保存的数据成员。<<运算符支持的类型查阅CArchive使用说明 
	}
	else //读取 
	{
		//ar >> ; 
	}
}

//渲染 
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
	//需要闭合，绘制最后一点到第一个点
	(new CCGRenderContext())->MidLine((int)mPoints.front().x, (int)mPoints.front().y, (int)mPoints.back().x, (int)mPoints.back().y, glm::vec3(1.0f, 1.0f, 1.0f));
	return true;
}


void CGLineLoop::Translate(double tx, double ty) {
	// 遍历所有点并应用平移
	for (auto& point : mPoints) {
		point.x += tx; // x 坐标平移
		point.y += ty; // y 坐标平移
	}
}

void CGLineLoop::Rotate(double angle, double cx, double cy)
{
	// 将角度转换为弧度（因为三角函数使用弧度）
	double radians = angle * (M_PI / 180.0);

	// 计算旋转矩阵的元素
	double cosTheta = cos(radians);
	double sinTheta = sin(radians);
	//1. 创建平移矩阵（将中心点移动到原点）
	Translate(-cx, -cy);
	for (auto& point : mPoints) {//对每个点都进行旋转变换
		// 2. 旋转
		double x1 = point.x;
		point.x = x1 * cosTheta - point.y * sinTheta;
		point.y = x1 * sinTheta + point.y * cosTheta;
	}
	// 3. 将中心点移回原位置
	Translate(cx, cy);
}


void CGLineLoop::Scale(double sx, double sy, double cx, double cy)
{
	// 1.将缩放中心点移动到原点
	Translate(-cx, -cy);

	// 2. 缩放
	for (auto& point : mPoints) {//对每个点都进行缩放变换
		// 2. 旋转
		
		point.x*=sx;
		point.y*=sy;
	}

	// 3. 将中心点移回原位置
	Translate(cx, cy);
}