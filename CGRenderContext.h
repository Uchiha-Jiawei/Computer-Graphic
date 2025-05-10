#pragma once
#include "glIncludes.h" 
#include<vector>
class CCGRenderContext
{
public:

	CCGRenderContext();
	void drawTest(void);
	void DDALine(int xStart, int yStart, int xEnd, int yEnd, glm::vec3 color);
	void MidLine(int x0, int y0, int x1, int y1, glm::vec3 color);
	void Bresenham(int x0, int y0, int x1, int y1, glm::vec3 color);
	void MidCircle(int x0, int y0, int radius, glm::vec3 color);
	void BresenhamCircle(int x0, int y0, int radius, glm::vec3 color);
	void draw_arc(int x0, int y0, int radius, double startAngle, double endAngle, glm::vec3 color);
	void scanlineFill(std::vector<glm::ivec2> polygon, glm::vec3 color);
	void FloodFill4(int x, int y, glm::vec3 oldcolor, glm::vec3 newcolor);
	void BoundaryFill4(int x, int y, glm::vec3 boundarycolor, glm::vec3 newcolor);
	void draw_points(void);
	void draw_lines(void);
	void draw_line_strip(void);
	void draw_line_loop(void);
	void draw_triangles(void);
	void draw_triangle_strip(void);
	void draw_triangle_fan(void);
	void draw_quads(void);
	void draw_quad_strip(void);
	void draw_polygon(void);
	void draw_star(void);
	virtual ~CCGRenderContext();
};

