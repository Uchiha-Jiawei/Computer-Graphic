#pragma once
#include "CGRenderable.h" 
#include "CGCube.h"
class TessellationHints;
class CGRobot : public CGRenderable
{
	DECLARE_SERIAL(CGRobot)
public:
	CGRobot();
	virtual ~CGRobot();
	void setTessellationHints(std::shared_ptr<TessellationHints> hints);
	TessellationHints* tessellationHints() { return mTessellationHints.get(); }
	const TessellationHints* tessellationHints() const { return mTessellationHints.get(); }
//protected:
	virtual std::shared_ptr<CGTransform> buildRobot(); //重写基类虚函数 
	virtual std::shared_ptr<CGTransform> DrawCube(std::shared_ptr<CGCube> cube, glm::vec3 trans, glm::vec3 scale, glm::vec4 rotate, glm::vec4 color, CString name); //绘制立方体
	void moveRobot(std::shared_ptr<CGTransform> node, glm::vec3 rotate, float to,  float now,  int turn); //移动机器人
protected:
	std::shared_ptr<TessellationHints> mTessellationHints = nullptr;
};


