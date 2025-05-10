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
void CCGRenderContext::DDALine(int xStart, int yStart, int xEnd, int yEnd, glm::vec3 color) //DDA�㷨
{
	//�����ǻ��ƴ���ʾ�������������㷨�ο����ɸ����Լ����漰�����޸� 
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
		glVertex2d((int)(x + 0.5), (int)(y + 0.5)); //��(x,y)��������ɫcolor������ 
		x = x + xincre;
		y = y + yincre;
	}
	glEnd();
}

void CCGRenderContext::MidLine(int x0, int y0, int x1, int y1, glm::vec3 color) { //�е㻭���㷨
	bool steep = (fabs(y1 - y0) > fabs(x1 - x0));
	if (steep) {//ȷ����ͼ��б��զ-1<k<1
		Swap(&x0, &y0);
		Swap(&x1, &y1);
	}
	if (x0 > x1) {//��֤x�ǵ�����
		Swap(&x0, &x1);
		Swap(&y0, &y1);
	}
	int a = 0 - fabs(y1 - y0);
	int b = x1 - x0;
	int d = 2 * a + b;
	int ystep = (y0 < y1) ? 1 : -1;// ����б��ȷ��y�����ӻ��Ǽ���
	int y = y0;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			glVertex2d(y, x); // ע��������(y,x)����Ϊ����ϵ�Ѿ�����
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
void CCGRenderContext::Bresenham(int x0, int y0, int x1, int y1, glm::vec3 color) {//Bresenham�����㷨
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
			glVertex2d(y, x); // ע��������(y,x)����Ϊ����ϵ�Ѿ�����
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

void CCGRenderContext::MidCircle(int x0, int y0, int radius, glm::vec3 color) {//�е㻭Բ�㷨
	int x = 0;
	int y = radius;
	float d = 1 - radius;//��ʼ���߲���
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	while (x <= y) {
		// ����Բ�İ˶Գ��Ի������ص�
		glVertex2d(x0+x, y0+y);
		glVertex2d(x0+y, y0+x);
		glVertex2d(x0-x, y0+y);
		glVertex2d(x0-y, y0+x);
		glVertex2d(x0-x, y0-y);
		glVertex2d(x0-y, y0-x);
		glVertex2d(x0+x, y0-y);
		glVertex2d(x0+y, y0-x);
		if (d < 0) {//�������ҷ��������·�
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

void CCGRenderContext::BresenhamCircle(int x0, int y0, int radius, glm::vec3 color) {//Bresenham��Բ�㷨
	int x = 0;
	int y = radius;
	float d = 2 *(1- radius);//��ʼ���߲���
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	while (x <= y) {
		// ����Բ�İ˶Գ��Ի������ص�
		glVertex2d(x0 + x, y0 + y);
		glVertex2d(x0 + y, y0 + x);
		glVertex2d(x0 - x, y0 + y);
		glVertex2d(x0 - y, y0 + x);
		glVertex2d(x0 - x, y0 - y);
		glVertex2d(x0 - y, y0 - x);
		glVertex2d(x0 + x, y0 - y);
		glVertex2d(x0 + y, y0 - x);
		if (d > 0) {//�������ҷ��������·������·�
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
		// ����Բ�İ˶Գ��Ի������ص�
		glVertex2d(x0 + x, y0 + y);
		glVertex2d(x0 + y, y0 + x);
		glVertex2d(x0 - x, y0 + y);
		glVertex2d(x0 - y, y0 + x);
		glVertex2d(x0 - x, y0 - y);
		glVertex2d(x0 - y, y0 - x);
		glVertex2d(x0 + x, y0 - y);
		glVertex2d(x0 + y, y0 - x);
		if (d > 0) { //ֻ�������ҷ��������·�
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
void CCGRenderContext::draw_arc(int x0, int y0,int radius, double startAngle,double endAngle, glm::vec3 color) {//Բ���㷨
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
	double startRad = startAngle * M_PI / 180.0; // ת��Ϊ����
	double endRad = endAngle * M_PI / 180.0;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2d(x0, y0);
	while (x <= y) {
		// ����ǰ��ת��Ϊ������
		if (startRad < atan2(y, x) && atan2(y, x) <= endRad) //�жϸ�Բ��Ӧ������Щ�˷�֮һ԰�ڴ���
			glVertex2d(x0 + x, y0 + y);
		if(startRad < atan2(x,y) && atan2(x,y) <= endRad)
			glVertex2d(x0 + y, y0 + x);
		if(startRad < atan2(y,-x) && atan2(y, -x) <= endRad)
			glVertex2d(x0 - x, y0 + y);
		if(startRad < atan2(x,-y)  && atan2(x,-y) <= endRad)
			glVertex2d(x0 - y, y0 + x);
		if(startRad < atan2(-y,-x)+2*M_PI&& atan2(-y,-x) + 2 * M_PI <= endRad) //���￪ʼ��Ҫ+2*M_PIʹ��Ƕȷ�Χ��0-2*M_PI
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

struct Edge { //����߱�
	int ymin;
	int ymax;
	float x;
	float invslope; // ����б��
	Edge * next;
	Edge(int ymin, int ymax, float x, float invslope) : ymin(ymin), ymax(ymax), x(x), invslope(invslope), next(nullptr) {}
};

void insertSortedByX(Edge*& list, Edge* edge) {//����x��С˳���µı߲�����߱�
	if (!list || list->x >= edge->x) {//��ʼ����Ϊ�ջ����±ߵ�x��С
		edge->next = list;
		list = edge;
	}
	else {
		Edge* current = list;
		while (current->next && current->next->x < edge->x) {//����������λ��
			current = current->next;
		}
		edge->next = current->next;
		current->next = edge;
	}
}

void  CCGRenderContext::scanlineFill(std::vector<glm::ivec2> polygon, glm::vec3 color) {//ɨ��������㷨
	Edge* ET = nullptr; //��ʼ���߱�Ϊ��
	int yStart = INT_MAX; //������¼���бߵ����±�
	int yEnd = INT_MIN; //��¼���ϱ�
	// �����߱�
	for (int i = 0; i < polygon.size(); i++) {
		auto& p1 = polygon[i];
		auto& p2 = polygon[(i + 1) % polygon.size()];
		int yMin = p1.y;
		int yMax = p2.y;
		if (yMin == yMax) 
		{
			DDALine(p1.x, p1.y, p2.x, p2.y, color);//ˮƽֱ��ɨ�������
			continue; // ����ˮƽ��
		}
		int x = p1.x;
		if (yMin > yMax) {
			x = p2.x;
			std::swap(yMin, yMax);
		}
		float xStart = (float)(x);
		float slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
		float invSlope = slope != 0 ? 1.0f / slope : 0; 
		Edge* newEdge = new Edge(yMin, yMax, xStart, invSlope); //�����±�
		insertSortedByX(ET, newEdge); //����߱�
		yStart = std::min(yStart, yMin);
		yEnd = std::max(yMax, yEnd);
	}

	Edge* AET = nullptr; //��ʼ����߱�Ϊ��

	// ����ÿһ��
	for (int y = yStart; y <= yEnd; y++) { //����Сy�����yɨ��
		// ����½���ı�
		Edge* etCurrent = ET;
		Edge* etPrev = nullptr;
		while (etCurrent) {		
			if(etCurrent->ymin <= y)
			{
				insertSortedByX(AET, new Edge(etCurrent->ymin, etCurrent->ymax, etCurrent->x, etCurrent->invslope));
				if (etPrev) { //�ڱ߱���ɾ��������
					etPrev->next = etCurrent->next;
				}
				else {
					ET = etCurrent->next;
				}
				Edge* temp = etCurrent;
				etCurrent = etCurrent->next;
				delete temp; // �ͷŲ�����Ч�ı�
			}
			else {
				etPrev = etCurrent;
				etCurrent = etCurrent->next;
			}
		}
		Edge* current = AET;
		// ���
		std::vector<int> xCoords;//��¼�������еĽ���x
		while (current) {
			xCoords.push_back(static_cast<int>(current->x));
			current = current->next;
		}
		for (int i = 0; i < xCoords.size(); i += 2) { //������������
			if (i + 1 < xCoords.size()) {
				int x1 = xCoords[i];
				int x2 = xCoords[i + 1];
				DDALine(x1, y, x2, y, color); // ����ˮƽ�߶���ɨ����
			}
		}
		// ���»�߱�ɾ��yMaxС��y�ı�
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
				delete temp; // �ͷŲ�����Ч�ı�
			}
			else {
				prev = current;
				current = current->next;
			}
		}
		// ���»�߱��е�x����
		current = AET;
		while (current) {
			current->x += current->invslope;
			current = current->next;
		}
	}

	// �����ڴ�
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

glm::vec3 GetPixelColor(int x, int y) { //��ȡ��ǰ������ɫ
	// ��ȡ��ǰ�����ɫ����
	GLfloat pixel[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixel);
	// ��ȡ��ǰ���ص���ɫ
	glm::vec3 currentColor(pixel[0], pixel[1], pixel[2]);
	return currentColor;
}
void CCGRenderContext::FloodFill4(int x, int y, glm::vec3 oldcolor, glm::vec3 newcolor) { //�ڵ��ʾ����������㷨
	std::queue<std::pair<int, int>> q;
	if (GetPixelColor(x, y) == oldcolor) { //�жϵ�ǰ�����Ƿ�Ϊ�ڵ���ɫ
		q.push({ x, y });
	}
	while (!q.empty()){
		auto p = q.front();
		q.pop();
		// ȷ�������ڵ㣬������ɫƥ��
		if ( GetPixelColor(p.first, p.second) == oldcolor) {
			glColor3f(newcolor.r, newcolor.g, newcolor.b);
			glBegin(GL_POINTS);
			glVertex2d(p.first, p.second);
			glEnd();
			// ���������ؼ������
			q.push({ p.first + 1, p.second });
			q.push({ p.first - 1, p.second });
			q.push({ p.first, p.second + 1 });
			q.push({ p.first, p.second - 1 });
		}
	}
}

void CCGRenderContext::BoundaryFill4(int x, int y, glm::vec3 boundarycolor, glm::vec3 newcolor) { //�߽��ʾ����������㷨
	std::queue<std::pair<int, int>> q;
	if (GetPixelColor(x, y) != boundarycolor && GetPixelColor(x, y) != newcolor) { //�жϵ�ǰ�����Ƿ��Ǳ߽���ɫ����δ�����
		q.push({ x, y });
	}
	while (!q.empty()) {
		auto p = q.front();
		q.pop();
		// �жϵ�ǰ�����Ƿ��Ǳ߽���ɫ����δ�����
		if (GetPixelColor(p.first, p.second) != boundarycolor && GetPixelColor(p.first, p.second) != newcolor) {
			glColor3f(newcolor.r, newcolor.g, newcolor.b);
			glBegin(GL_POINTS);
			glVertex2d(p.first, p.second);
			glEnd();
			// ���������ؼ������
			q.push({ p.first + 1, p.second });
			q.push({ p.first - 1, p.second });
			q.push({ p.first, p.second + 1 });
			q.push({ p.first, p.second - 1 });
		}
	}
}


void CCGRenderContext::draw_points(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		120, 50, 
		90, 70,
		140, 70,
		160, 90
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_POINTS);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]);
	}
	glEnd();
}


void CCGRenderContext::draw_lines(void) {
	int vertices[][2] = { //��������
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
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_LINES);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+90);
	}
	glEnd();
}
void CCGRenderContext::draw_line_strip(void) {
	int vertices[][2] = { //��������
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
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_LINE_STRIP);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+160);
	}
	glEnd();
}
void CCGRenderContext::draw_line_loop(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		200, 70,
		250, 20,
		280, 45,
		300, 10,
		150, 10,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_LINE_LOOP);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]-90, x[1]+230);
	}
	glEnd();
}
void CCGRenderContext::draw_triangles(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		100, 20,
		200, 20,
		150, 70,
		200, 70,
		175, 30
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_TRIANGLES);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]-20);
	}
	glEnd();
}
void CCGRenderContext::draw_triangle_strip(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		100, 20,
		200, 50,
		220, 20,
		240, 50,
		260, 10
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_TRIANGLE_STRIP);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+50);
	}
	glEnd();
}
void CCGRenderContext::draw_triangle_fan(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		100, 100,
		150, 80,
		160, 60,
		155, 30,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_TRIANGLE_FAN);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+100);
	}
	glEnd();
}
void CCGRenderContext::draw_quads(void) {
	int vertices[][2] = { //��������
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
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_QUADS);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+200);
	}
	glEnd();
}
void CCGRenderContext::draw_quad_strip(void) {
	int vertices[][2] = { //��������
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
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_QUAD_STRIP);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+100, x[1]+300);
	}
	glEnd();
}
void CCGRenderContext::draw_polygon(void) {
	int vertices[][2] = { //��������
		// x,   y, 
		100, 50,
		90, 100,
		150, 50,
		100, 20,
		60, 20,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_POLYGON);
	//���ƺ�ɫ�ĵ�
	for (auto& x : vertices) {
		glVertex2d(x[0]+300, x[1]);
	}
	glEnd();
}
void CCGRenderContext::draw_star(void) {
	float vertices[11][2] = {};//��������
	float R = 50; // ���Բ�뾶
	float cx = 400, cy = 400; // ���ĵ�����
	float M_PI = 3.14159;
	float angleStep = 2* M_PI / 5; // ÿ������֮��ĽǶȲ�
	float innerRadiusRatio = 0.381966; // ��Ȧ�뾶����Ȧ�뾶�ı������ƽ�ָ��
	int cnt = 1;
	vertices[0][0] = cx;
	vertices[0][1]= cy;
	for (int i = 0; i < 5; ++i) {
		// �����ⲿ������
		float xOut = cx + R * sin(i * angleStep);
		float yOut = cy + R * cos(i * angleStep);
		vertices[cnt][0] = xOut;
		vertices[cnt][1] = yOut;
		cnt++;
		// �����ڲ�������
		float xIn = cx + R * innerRadiusRatio * sin(i * angleStep + angleStep / 2);
		float yIn = cy + R * innerRadiusRatio * cos(i * angleStep + angleStep / 2);
		vertices[cnt][0] = xIn;
		vertices[cnt][1] = yIn;
		cnt++;
	}
	// ��¼����������ʼ��ͼ
	glColor3f(1.0f, 0.65f, 0.0f);
	cnt = 1;
	//����ǰ��ɫ����Ϊ��ɫ
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
	//����ǰ��ɫ����Ϊ��ɫ
	glBegin(GL_TRIANGLES);
	while (cnt <= 10) {
		glVertex2f(vertices[0][0], vertices[0][1]);
		glVertex2f(vertices[cnt][0], vertices[cnt][1]);
		cnt++;
		glVertex2f(vertices[cnt%10][0], vertices[cnt%10][1]);
		cnt++;
	}
	glEnd();
	// ��¼����������ʼ��ͼ
	glColor3f(0.0f, 0.0f, 1.0f);
	cnt = 1;
	//����ǰ��ɫ����Ϊ��ɫ
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
	//����ǰ��ɫ����Ϊ��ɫ
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

