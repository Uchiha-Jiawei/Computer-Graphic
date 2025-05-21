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
	//���л� 
	virtual void Serialize(CArchive& ar) override;
	//���ƶ���������������д�� 
	virtual bool Render(CGRenderContext* pRC, CGCamera* pCamera);

	virtual void Translate(double tx, double ty); //ƽ��

	virtual void Rotate(double angle, double cx, double cy);

	virtual void Scale(double sx, double sy, double cx, double cy);
protected:
	std::vector<glm::dvec3> mPoints; // �洢�պ����ϵĵ�
	double M_PI = 3.14159;
	
};

