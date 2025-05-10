#include "pch.h" 
#include "CGLineSegment.h" 


IMPLEMENT_SERIAL(CGLineSegment, CGGeometry, 1)

CGLineSegment::CGLineSegment()

	: mStart(glm::dvec3(0.0, 0.0, 0.0)), mEnd(glm::dvec3(0.0, 0.0, 0.0))
{
}

CGLineSegment::CGLineSegment(const glm::dvec3& start, const glm::dvec3& end)

	: mStart(start), mEnd(end)
{

}

CGLineSegment::~CGLineSegment()
{
}
void CGLineSegment::Serialize(CArchive& ar)
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
bool CGLineSegment::Render(CGRenderContext* pRC, CGCamera* pCamera)
{
	if (pRC == nullptr || pCamera == nullptr)
		return false;
	glColor3f(1.0f, 1.0f, 1.0f); // 白色 
	glBegin(GL_LINES);
	glVertex3f(mStart.x, mStart.y, mStart.z);
	glVertex3f(mEnd.x, mEnd.y, mEnd.z);
	glEnd();
	return true;
}
void CGLineSegment::Translate(double tx, double ty)
{
	mStart.x += tx;
	mStart.y += ty;
	mEnd.x += tx;
	mEnd.y += ty;
}

void CGLineSegment::Rotate(double angle, double cx, double cy)
{
	// 将角度转换为弧度（因为三角函数使用弧度）
	double radians = angle * (M_PI / 180.0);

	// 计算旋转矩阵的元素
	double cosTheta = cos(radians);
	double sinTheta = sin(radians);

	// 1.将旋转中心点移动到原点
	Translate(-cx, -cy);

	// 2. 旋转
	double x1 = mStart.x, x2 = mEnd.x;
	mStart.x = x1*cosTheta - mStart.y*sinTheta;
	mStart.y = x1 * sinTheta + mStart.y * cosTheta;
	mEnd.x = x2 * cosTheta - mEnd.y * sinTheta;
	mEnd.y = x2 * sinTheta + mEnd.y * cosTheta;

	// 3. 将中心点移回原位置
	Translate(cx, cy);
}

void CGLineSegment::Scale(double sx, double sy, double cx, double cy)
{
	// 1.将缩放中心点移动到原点
	Translate(-cx, -cy);

	// 2. 缩放
	mStart.x *= sx;
	mStart.y *= sy;
	mEnd.x *= sx;
	mEnd.y *= sy;

	// 3. 将中心点移回原位置
	Translate(cx, cy);

}