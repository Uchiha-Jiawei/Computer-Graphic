#pragma once 
#include "CGGeometry.h" 
#include "vector"
class CGLineLoop : public CGGeometry
{
	DECLARE_SERIAL(CGLineLoop)
public:
	CGLineLoop();
	CGLineLoop(const std::vector<glm::dvec3> mPoints);
	virtual ~CGLineLoop();
	//序列化 
	virtual void Serialize(CArchive& ar) override;
	//绘制对象（在派生类中重写） 
	virtual bool Render(CGRenderContext* pRC, CGCamera* pCamera);

	virtual void Translate(double tx, double ty); //平移

	virtual void Rotate(double angle, double cx, double cy);

	virtual void Scale(double sx, double sy, double cx, double cy);
protected:
	std::vector<glm::dvec3> mPoints; // 存储闭合线上的点
	double M_PI = 3.14159;
	
};

