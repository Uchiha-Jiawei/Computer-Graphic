#include "pch.h" 
#include "CGRenderContext.h" 
#include <vector>
#include <algorithm>
#include<queue>
double M_PI = 3.1415926;
CCGRenderContext::CCGRenderContext()
{
}
CCGRenderContext::~CCGRenderContext()
{
}
void Swap(int*x, int*y) {
	int t = *x;
	*x = *y;
	*y = t;
}
void CCGRenderContext::drawTest(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(0, 0);
	glVertex2d(100, 100);
	glVertex2d(100, 0);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2d(100, 100);
	glVertex2d(300, 300);
	glVertex2d(300, 100);
	glEnd();
}
void CCGRenderContext::DDALine(int xStart, int yStart, int xEnd, int yEnd, glm::vec3 color) //DDA算法
{
	//以下是绘制代码示例：用于其他算法参考，可根据自己的涉及进行修改 
	int steps;
	int dx = xEnd - xStart;
	int dy = yEnd - yStart;
	if (abs(dx) >= abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	if (steps == 0)
		return;
	float x = (float)xStart, y = (float)yStart, xincre = (float)dx / steps, yincre = (float)dy /
		steps;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	for (int i = 0; i <= steps; i++)
	{
		glVertex2d((int)(x + 0.5), (int)(y + 0.5)); //在(x,y)处，以颜色color绘像素 
		x = x + xincre;
		y = y + yincre;
	}
	glEnd();
}

void CCGRenderContext::MidLine(int x0, int y0, int x1, int y1, glm::vec3 color) { //中点画线算法
	bool steep = (fabs(y1 - y0) > fabs(x1 - x0));
	if (steep) {//确保画图是斜率咋-1<k<1
		Swap(&x0, &y0);
		Swap(&x1, &y1);
	}
	if (x0 > x1) {//保证x是递增的
		Swap(&x0, &x1);
		Swap(&y0, &y1);
	}
	int a = 0 - fabs(y1 - y0);
	int b = x1 - x0;
	int d = 2 * a + b;
	int ystep = (y0 < y1) ? 1 : -1;// 根据斜率确定y是增加还是减少
	int y = y0;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			glVertex2d(y, x); // 注意这里是(y,x)，因为坐标系已经交换
		}
		else {
			glVertex2d(x, y);
		}
		if (d < 0) {
			y += ystep;
			d += 2 * b;
		}
		d += 2 * a;
	}
	glEnd();
}
void CCGRenderContext::Bresenham(int x0, int y0, int x1, int y1, glm::vec3 color) {//Bresenham画线算法
	bool steep = (fabs(y1 - y0) > fabs(x1 - x0));
	if (steep) {
		Swap(&x0, &y0);
		Swap(&x1, &y1);
	}
	if (x0 > x1) {
		Swap(&x0, &x1);
		Swap(&y0, &y1);
	}
	int dx = x1 - x0;
	int dy = fabs(y1 - y0);
	int d = 2*dy-dx;
	int ystep = (y0 < y1) ? 1 : -1;
	int y = y0;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			glVertex2d(y, x); // 注意这里是(y,x)，因为坐标系已经交换
		}
		else {
			glVertex2d(x, y);
		}
		if (d >= 0) {
			y += ystep;
			d -= 2 * dx;
		}
		d += 2 * dy;
	}
	glEnd();
}

void CCGRenderContext::MidCircle(int x0, int y0, int radius, glm::vec3 color) {//中点画圆算法
	int x = 0;
	int y = radius;
	float d = 1 - radius;//初始决策参数
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	while (x <= y) {
		// 利用圆的八对称性绘制像素点
		glVertex2d(x0+x, y0+y);
		glVertex2d(x0+y, y0+x);
		glVertex2d(x0-x, y0+y);
		glVertex2d(x0-y, y0+x);
		glVertex2d(x0-x, y0-y);
		glVertex2d(x0-y, y0-x);
		glVertex2d(x0+x, y0-y);
		glVertex2d(x0+y, y0-x);
		if (d < 0) {//考虑正右方、正右下方
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;	
		}
		x++;
	}
	glEnd();
}

void CCGRenderContext::BresenhamCircle(int x0, int y0, int radius, glm::vec3 color) {//Bresenham画圆算法
	int x = 0;
	int y = radius;
	float d = 2 *(1- radius);//初始决策参数
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	while (x <= y) {
		// 利用圆的八对称性绘制像素点
		glVertex2d(x0 + x, y0 + y);
		glVertex2d(x0 + y, y0 + x);
		glVertex2d(x0 - x, y0 + y);
		glVertex2d(x0 - y, y0 + x);
		glVertex2d(x0 - x, y0 - y);
		glVertex2d(x0 - y, y0 - x);
		glVertex2d(x0 + x, y0 - y);
		glVertex2d(x0 + y, y0 - x);
		if (d > 0) {//考虑正右方、正右下方、正下方
			if (2 * (d - x) - 1 <= 0) {
				d += 2 * (x - y + 3);
				x++;
				y--;
			}
			else {
				d -= 2 * y - 3;
				y--;
			}
		}
		else if(d==0){
			d += 2 * (x - y + 3);
			x++;
			y--;
		}
		else { //d<0
			if (2 * (d - x) - 1 <= 0) {
				d += 2 * x + 3;
				x++;
			}
			else {
				d += 2 *(x - y +3);
				x++;
				y--;
			}
		}
	}
	glEnd();
}
void bresenham_optimize(int x0, int y0, int radius, glm::vec3 color) {
	int x = 0;
	int y = radius;
	float d = 3 - 2 * radius;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	while (x <= y) {
		// 利用圆的八对称性绘制像素点
		glVertex2d(x0 + x, y0 + y);
		glVertex2d(x0 + y, y0 + x);
		glVertex2d(x0 - x, y0 + y);
		glVertex2d(x0 - y, y0 + x);
		glVertex2d(x0 - x, y0 - y);
		glVertex2d(x0 - y, y0 - x);
		glVertex2d(x0 + x, y0 - y);
		glVertex2d(x0 + y, y0 - x);
		if (d > 0) { //只考虑正右方和正右下方
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}
		x++;
	}
	glEnd();
}
void CCGRenderContext::draw_arc(int x0, int y0,int radius, double startAngle,double endAngle, glm::vec3 color) {//圆弧算法
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
	auto norm = [](double a)->double {
		while (a < 0)   a += 360;
		while (a >= 360) a -= 360;
		return a;
	};
	startAngle = norm(startAngle);
	endAngle = norm(endAngle);
	if (endAngle < startAngle)
		std::swap(endAngle, startAngle);
	double startRad = startAngle * M_PI / 180.0; // 转换为弧度
	double endRad = endAngle * M_PI / 180.0;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2d(x0, y0);
	while (x <= y) {
		// 将当前点转换为极坐标
		if (startRad < atan2(y, x) && atan2(y, x) <= endRad) //判断该圆弧应该在哪些八分之一园内存在
			glVertex2d(x0 + x, y0 + y);
		if(startRad < atan2(x,y) && atan2(x,y) <= endRad)
			glVertex2d(x0 + y, y0 + x);
		if(startRad < atan2(y,-x) && atan2(y, -x) <= endRad)
			glVertex2d(x0 - x, y0 + y);
		if(startRad < atan2(x,-y)  && atan2(x,-y) <= endRad)
			glVertex2d(x0 - y, y0 + x);
		if(startRad < atan2(-y,-x)+2*M_PI&& atan2(-y,-x) + 2 * M_PI <= endRad) //这里开始需要+2*M_PI使其角度范围在0-2*M_PI
			glVertex2d(x0 - x, y0 - y);
		if(startRad < atan2(-x,-y) + 2 * M_PI && atan2(-x,-y) + 2 * M_PI <= endRad)
			glVertex2d(x0 - y, y0 - x);
		if(startRad < atan2(-y,x) + 2 * M_PI && atan2(- y, x) + 2 * M_PI <= endRad)
			glVertex2d(x0 + x, y0 - y);
		if(startRad < atan2(-x, y) + 2 * M_PI && atan2(-x,y) + 2 * M_PI <= endRad)
			glVertex2d(x0 + y, y0 - x);
		if(d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}
		x++;
	}
	glEnd();
}

struct Edge { //定义边表
	int ymin;
	int ymax;
	float x;
	float invslope; // 倒数斜率
	Edge * next;
	Edge(int ymin, int ymax, float x, float invslope) : ymin(ymin), ymax(ymax), x(x), invslope(invslope), next(nullptr) {}
};

void insertSortedByX(Edge*& list, Edge* edge) {//按照x大小顺序将新的边插入进边表
	if (!list || list->x >= edge->x) {//初始链表为空或者新边的x最小
		edge->next = list;
		list = edge;
	}
	else {
		Edge* current = list;
		while (current->next && current->next->x < edge->x) {//遍历到插入位置
			current = current->next;
		}
		edge->next = current->next;
		current->next = edge;
	}
}

void  CCGRenderContext::scanlineFill(std::vector<glm::ivec2> polygon, glm::vec3 color) {//扫描线填充算法
	Edge* ET = nullptr; //初始化边表为空
	int yStart = INT_MAX; //用来记录所有边的最下边
	int yEnd = INT_MIN; //记录最上边
	// 构建边表
	for (int i = 0; i < polygon.size(); i++) {
		auto& p1 = polygon[i];
		auto& p2 = polygon[(i + 1) % polygon.size()];
		int yMin = p1.y;
		int yMax = p2.y;
		if (yMin == yMax) 
		{
			DDALine(p1.x, p1.y, p2.x, p2.y, color);//水平直接扫描线填充
			continue; // 跳过水平边
		}
		int x = p1.x;
		if (yMin > yMax) {
			x = p2.x;
			std::swap(yMin, yMax);
		}
		float xStart = (float)(x);
		float slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
		float invSlope = slope != 0 ? 1.0f / slope : 0; 
		Edge* newEdge = new Edge(yMin, yMax, xStart, invSlope); //创建新边
		insertSortedByX(ET, newEdge); //插入边表
		yStart = std::min(yStart, yMin);
		yEnd = std::max(yMax, yEnd);
	}

	Edge* AET = nullptr; //初始化活动边表为空

	// 遍历每一行
	for (int y = yStart; y <= yEnd; y++) { //从最小y到最大y扫描
		// 添加新进入的边
		Edge* etCurrent = ET;
		Edge* etPrev = nullptr;
		while (etCurrent) {		
			if(etCurrent->ymin <= y)
			{
				insertSortedByX(AET, new Edge(etCurrent->ymin, etCurrent->ymax, etCurrent->x, etCurrent->invslope));
				if (etPrev) { //在边表中删除这条边
					etPrev->next = etCurrent->next;
				}
				else {
					ET = etCurrent->next;
				}
				Edge* temp = etCurrent;
				etCurrent = etCurrent->next;
				delete temp; // 释放不再有效的边
			}
			else {
				etPrev = etCurrent;
				etCurrent = etCurrent->next;
			}
		}
		Edge* current = AET;
		// 填充
		std::vector<int> xCoords;//记录现在所有的交点x
		while (current) {
			xCoords.push_back(static_cast<int>(current->x));
			current = current->next;
		}
		for (int i = 0; i < xCoords.size(); i += 2) { //两点间填充像素
			if (i + 1 < xCoords.size()) {
				int x1 = xCoords[i];
				int x2 = xCoords[i + 1];
				DDALine(x1, y, x2, y, color); // 绘制水平线段做扫描线
			}
		}
		// 更新活动边表，删除yMax小于y的边
		current = AET;
		Edge* prev = nullptr;
		while (current) {
			if (current->ymax <= y+1) {
				if (prev) {
					prev->next = current->next;
				}
				else {
					AET = current->next;
				}
				Edge* temp = current;
				current = current->next;
				delete temp; // 释放不再有效的边
			}
			else {
				prev = current;
				current = current->next;
			}
		}
		// 更新活动边表中的x坐标
		current = AET;
		while (current) {
			current->x += current->invslope;
			current = current->next;
		}
	}

	// 清理内存
	while (ET) {
		Edge* temp = ET;
		ET = ET->next;
		delete temp;
	}
	while (AET) {
		Edge* temp = AET;
		AET = AET->next;
		delete temp;
	}
}

glm::vec3 GetPixelColor(int x, int y) { //获取当前像素颜色
	// 读取当前点的颜色数据
	GLfloat pixel[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixel);
	// 提取当前像素的颜色
	glm::vec3 currentColor(pixel[0], pixel[1], pixel[2]);
	return currentColor;
}
void CCGRenderContext::FloodFill4(int x, int y, glm::vec3 oldcolor, glm::vec3 newcolor) { //内点表示的种子填充算法
	std::queue<std::pair<int, int>> q;
	if (GetPixelColor(x, y) == oldcolor) { //判断当前像素是否为内点颜色
		q.push({ x, y });
	}
	while (!q.empty()){
		auto p = q.front();
		q.pop();
		// 确保仍在内点，并且颜色匹配
		if ( GetPixelColor(p.first, p.second) == oldcolor) {
			glColor3f(newcolor.r, newcolor.g, newcolor.b);
			glBegin(GL_POINTS);
			glVertex2d(p.first, p.second);
			glEnd();
			// 将相邻像素加入队列
			q.push({ p.first + 1, p.second });
			q.push({ p.first - 1, p.second });
			q.push({ p.first, p.second + 1 });
			q.push({ p.first, p.second - 1 });
		}
	}
}

void CCGRenderContext::BoundaryFill4(int x, int y, glm::vec3 boundarycolor, glm::vec3 newcolor) { //边界表示的种子填充算法
	std::queue<std::pair<int, int>> q;
	if (GetPixelColor(x, y) != boundarycolor && GetPixelColor(x, y) != newcolor) { //判断当前像素是否不是边界颜色并且未被填充
		q.push({ x, y });
	}
	while (!q.empty()) {
		auto p = q.front();
		q.pop();
		// 判断当前像素是否不是边界颜色并且未被填充
		if (GetPixelColor(p.first, p.second) != boundarycolor && GetPixelColor(p.first, p.second) != newcolor) {
			glColor3f(newcolor.r, newcolor.g, newcolor.b);
			glBegin(GL_POINTS);
			glVertex2d(p.first, p.second);
			glEnd();
			// 将相邻像素加入队列
			q.push({ p.first + 1, p.second });
			q.push({ p.first - 1, p.second });
			q.push({ p.first, p.second + 1 });
			q.push({ p.first, p.second - 1 });
		}
	}
}


void CCGRenderContext::draw_points(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		120, 50, 
		90, 70,
		140, 70,
		160, 90
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_POINTS);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]);
	}
	glEnd();
}


void CCGRenderContext::draw_lines(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		200, 70,
		160, 60,
		250, 45,
		120, 50,
		220, 30,
		110, 35,
		220, 50
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_LINES);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+90);
	}
	glEnd();
}
void CCGRenderContext::draw_line_strip(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		200, 70,
		160, 60,
		250, 45,
		120, 50,
		220, 30,
		110, 35,
		220, 50
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_LINE_STRIP);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+160);
	}
	glEnd();
}
void CCGRenderContext::draw_line_loop(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		200, 70,
		250, 20,
		280, 45,
		300, 10,
		150, 10,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_LINE_LOOP);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+230);
	}
	glEnd();
}
void CCGRenderContext::draw_triangles(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		100, 20,
		200, 20,
		150, 70,
		200, 70,
		175, 30
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_TRIANGLES);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]-20);
	}
	glEnd();
}
void CCGRenderContext::draw_triangle_strip(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		100, 20,
		200, 50,
		220, 20,
		240, 50,
		260, 10
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_TRIANGLE_STRIP);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+50);
	}
	glEnd();
}
void CCGRenderContext::draw_triangle_fan(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		100, 100,
		150, 80,
		160, 60,
		155, 30,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_TRIANGLE_FAN);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+100);
	}
	glEnd();
}
void CCGRenderContext::draw_quads(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		100, 100,
		150, 80,
		160, 60,
		155, 30,
		150, 20,
		200, 25,
		210, 40
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_QUADS);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+200);
	}
	glEnd();
}
void CCGRenderContext::draw_quad_strip(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		90, 100,
		150, 50,
		150, 105,
		200, 60,
		180, 110,
		220, 70,
		215, 120,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_QUAD_STRIP);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+300);
	}
	glEnd();
}
void CCGRenderContext::draw_polygon(void) {
	int vertices[][2] = { //各点坐标
		// x,   y, 
		100, 50,
		90, 100,
		150, 50,
		100, 20,
		60, 20,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//将当前颜色设置为红色
	glBegin(GL_POLYGON);
	//绘制红色的点
	for (auto& x : vertices) {
		glVertex2d(x[0]+300, x[1]);
	}
	glEnd();
}
void CCGRenderContext::draw_star(void) {
	float vertices[11][2] = {};//各点坐标
	float R = 50; // 外接圆半径
	float cx = 400, cy = 400; // 中心点坐标
	float M_PI = 3.14159;
	float angleStep = 2* M_PI / 5; // 每个顶点之间的角度差
	float innerRadiusRatio = 0.381966; // 内圈半径与外圈半径的比例，黄金分割比
	int cnt = 1;
	vertices[0][0] = cx;
	vertices[0][1]= cy;
	for (int i = 0; i < 5; ++i) {
		// 计算外部点坐标
		float xOut = cx + R * sin(i * angleStep);
		float yOut = cy + R * cos(i * angleStep);
		vertices[cnt][0] = xOut;
		vertices[cnt][1] = yOut;
		cnt++;
		// 计算内部点坐标
		float xIn = cx + R * innerRadiusRatio * sin(i * angleStep + angleStep / 2);
		float yIn = cy + R * innerRadiusRatio * cos(i * angleStep + angleStep / 2);
		vertices[cnt][0] = xIn;
		vertices[cnt][1] = yIn;
		cnt++;
	}
	// 记录完各点坐标后开始绘图
	glColor3f(1.0f, 0.65f, 0.0f);
	cnt = 1;
	//将当前颜色设置为橙色
	glBegin(GL_TRIANGLES);
	while (cnt < 10) {
		glVertex2f(vertices[0][0],vertices[0][1]);
		glVertex2f(vertices[cnt][0], vertices[cnt][1]);
		cnt++;
		glVertex2f(vertices[cnt][0], vertices[cnt][1]);
		cnt++;
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f);
	cnt = 2;
	//将当前颜色设置为黄色
	glBegin(GL_TRIANGLES);
	while (cnt <= 10) {
		glVertex2f(vertices[0][0], vertices[0][1]);
		glVertex2f(vertices[cnt][0], vertices[cnt][1]);
		cnt++;
		glVertex2f(vertices[cnt%10][0], vertices[cnt%10][1]);
		cnt++;
	}
	glEnd();
	// 记录完各点坐标后开始绘图
	glColor3f(0.0f, 0.0f, 1.0f);
	cnt = 1;
	//将当前颜色设置为蓝色
	glBegin(GL_TRIANGLES);
	while (cnt < 10) {
		glVertex2f(vertices[0][0]+100, vertices[0][1]);
		glVertex2f(vertices[cnt][0]+100, vertices[cnt][1]);
		cnt++;
		glVertex2f(vertices[cnt][0]+100, vertices[cnt][1]);
		cnt++;
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	cnt = 2;
	//将当前颜色设置为白色
	glBegin(GL_TRIANGLES);
	while (cnt <= 10) {
		glVertex2f(vertices[0][0]+100, vertices[0][1]);
		glVertex2f(vertices[cnt][0]+100, vertices[cnt][1]);
		cnt++;
		glVertex2f(vertices[cnt % 10][0]+100, vertices[cnt % 10][1]);
		cnt++;
	}
	glEnd();
}

