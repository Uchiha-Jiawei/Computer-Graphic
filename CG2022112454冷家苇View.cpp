// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CG2022112454冷家苇View.cpp: CCG2022112454冷家苇View 类的实现
//

#include "pch.h"
#include "framework.h"
#include<cmath>
#include<vector>
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG2022112454冷家苇.h"
#endif

#include "CG2022112454冷家苇Doc.h"
#include "CG2022112454冷家苇View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h" 
#include "UIEventHandler.h" 

// CCG2022112454冷家苇View

IMPLEMENT_DYNCREATE(CCG2022112454冷家苇View, CView)

BEGIN_MESSAGE_MAP(CCG2022112454冷家苇View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCG2022112454冷家苇View 构造/析构

CCG2022112454冷家苇View::CCG2022112454冷家苇View() noexcept
{
	// TODO: 在此处添加构造代码

}

CCG2022112454冷家苇View::~CCG2022112454冷家苇View()
{
	if (mTimer) KillTimer(mTimer);  //关闭定时器 
	if (m_glfwWindow) {


		glfwDestroyWindow(m_glfwWindow);


		glfwTerminate();

	}
}

bool CCG2022112454冷家苇View::InitGLFW()
{

	if (!glfwInit()) {


		AfxMessageBox(_T("GLFW初始化失败"));


		return false;

	}


	// 设置GLFW窗口无边框、透明（避免覆盖MFC控件） 

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);	// 设置OpenGL版本 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //如果使用OpenGL核心模式 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);  //如果使用OpenGL兼容模式 
	// 创建隐藏的GLFW窗口 

	m_glfwWindow = glfwCreateWindow(100, 100, "", NULL, NULL);

	if (!m_glfwWindow) {
		AfxMessageBox(_T("Failed to glfwCreateWindow"));
		glfwTerminate();
		return false;
	}


	// 获取GLFW窗口的HWND并设置为MFC视图的子窗口 

	m_hWndGL = glfwGetWin32Window(m_glfwWindow);

	SetWindowLong(m_hWndGL, GWL_STYLE, WS_VISIBLE);

	::SetParent(m_hWndGL, GetSafeHwnd());

	::ShowWindow(m_hWndGL, SW_SHOW);


	// 初始化OpenGL上下文 

	glfwMakeContextCurrent(m_glfwWindow);

#if defined(_USE_GLEW) 

	glewInit();
#else 

	//使用GLAD来加载OpenGL的函数地址 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

	{


		AfxMessageBox(_T("Failed to initialize GLAD"));


		return false;

	}
#endif 


	// 设置视口 

	CRect rect;

	GetClientRect(&rect);

	glViewport(0, 0, rect.Width(), rect.Height());


	m_bGLInitialized = true;

	//设置回调函数用到的指针（this-当前view） 

	glfwSetWindowUserPointer(m_glfwWindow, this);
	//this也是CCGRenderContext*类型 

	//设置键盘输入回调 

	glfwSetKeyCallback(m_glfwWindow, UIEventHandler::KeyCallback);

	//设置鼠标按键回调 

	glfwSetMouseButtonCallback(m_glfwWindow, UIEventHandler::MouseButtonCallback);

	//设置光标移动回调 

	glfwSetCursorPosCallback(m_glfwWindow, UIEventHandler::CursorPosCallback);

	//设置鼠标滚轮回调 

	glfwSetScrollCallback(m_glfwWindow, UIEventHandler::ScrollCallback);
	return true;
}


void calculateVertices(int sides, double radius, std::vector<std::vector<double>>& vertices,int x0,int y0) {
	double M_PI = 3.1415926;
	for (int k = 0; k < sides; ++k) {
		
		double angle = k * 2 * M_PI / sides; // Angle in radians
		vertices.push_back({ x0 + radius * cos(angle),y0 + radius * sin(angle) });
	}
}
//状态栏上显示提示信息 
void CCG2022112454冷家苇View::ShowPrompt(const std::string& str)
{

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();

	if (pMainWnd != nullptr)

	{
		pMainWnd->ShowPrompt(CString(str.c_str()));

	}
}
//状态栏上显示坐标信息 
void CCG2022112454冷家苇View::ShowCoord(double x, double y)
{

	CString str;

	glm::dvec3 v = DCS2WCS(glm::dvec3(x, y, 0.0));

	str.Format(_T("视口坐标 (%.0f, %.0f) : 世界坐标 (%.2f, %.2f) "), x, y, v.x, v.y);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();

	if (pMainWnd != nullptr)

	{


		pMainWnd->ShowCoord(str);  //显示到状态栏 

	}
}

glm::dvec3 CCG2022112454冷家苇View::DCS2WCS(const glm::dvec3& p)
{

	//暂时使用屏幕设备坐标 

	int w, h;

	glfwGetWindowSize(m_glfwWindow, &w, &h);

	/*return glm::dvec3(p.x, double(h) - p.y, 0.0);*/
	return glm::dvec3(p.x - double(w) / 2, double(h) / 2 - p.y, 0.0);
}

glm::dvec3 CCG2022112454冷家苇View::WCS2DCS(const glm::dvec3& p)
{

	//待实现 

	return glm::dvec3();
}
bool CCG2022112454冷家苇View::AddRenderable(std::shared_ptr<CGNode> r) const
{

	CCG2022112454冷家苇Doc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc)


		return false;

	return pDoc->AddRenderable(r);
}

UINT CCG2022112454冷家苇View::toggleFrameTimer()
{
	if (!mTimer) {
		mTimer = SetTimer(1, 20, NULL);// 启动50FPS定时器（20毫秒间隔） 
	}
	else {
		KillTimer(mTimer);
		mTimer = 0;
	}
	return mTimer;
}

void CCG2022112454冷家苇View::RenderScene()
{
	if (!m_bGLInitialized)
		return;
	//启用渲染环境 
	glfwMakeContextCurrent(m_glfwWindow);
	//此处添加自定义OpenGL渲染代码... 
	// // 此处添加自定义OpenGL渲染代码... 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); 
	CCG2022112454冷家苇Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc) {
		pDoc->RenderScene((CGRenderContext*)this);

	}
	//drawTest();
	/*std::vector<std::vector<double>> vertices;
	calculateVertices(14, 100, vertices, 100, 100);
	int n = size(vertices);
	for (int i = 0; i < n;i++) {
		for (int j = i + 1;j < n; j++)
			DDALine((int)vertices[i][0], (int)vertices[i][1], (int)vertices[j][0], (int)vertices[j][1], glm::vec3(1.0f, 0.0f, 0.0f));
	}
	for (int i = 0 ; i < n;i++) {
		for(int j = i+1;j< n; j++)
			MidLine((int)vertices[i][0]+200, (int)vertices[i][1], (int)vertices[j][0]+200, (int)vertices[j][1], glm::vec3(0.0f, 1.0f, 0.0f));
	}
	for (int i = 0; i < n;i++) {
		for (int j = i + 1;j < n; j++)
			Bresenham((int)vertices[i][0]+400, (int)vertices[i][1], (int)vertices[j][0]+400, (int)vertices[j][1], glm::vec3(0.0f, 0.0f, 1.0f));
	}
	for(int i = 0;i<110;i+=10)
	{
		MidCircle(100, 320, 10 + i, glm::vec3(1.0f, 0.0f, 0.0f));
		MidCircle(250+i, 320, 50, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	for (int i = 0;i < 110;i += 10)
	{
		BresenhamCircle(550, 320, 10 + i, glm::vec3(0.0f, 1.0f, 0.0f));
		BresenhamCircle(700 + i, 320, 50, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	draw_arc(100, 500, 50, 20, 90, glm::vec3(0.0f, 1.0f, 0.0f));
	draw_arc(200, 500, 50, 70, 180, glm::vec3(0.0f, 1.0f, 0.0f));
	draw_arc(300, 500, 50, 50, 200, glm::vec3(0.0f, 1.0f, 0.0f));
	draw_arc(400, 500, 50, -20, 100, glm::vec3(0.0f, 1.0f, 0.0f));
	draw_arc(500, 500, 50, 280, 350, glm::vec3(0.0f, 1.0f, 0.0f));
	std::vector<glm::ivec2> polygon = { {640, 10}, {620, 40}, {640, 100}, {680, 80}, {720, 70},{720,10} };
	scanlineFill(polygon, glm::vec3(0.0f, 1.0f, 0.0f));
	std::vector<glm::ivec2> polygon1 = { {730, 10}, {755, 20}, {765, 45}, {750, 65}, {725, 65} ,{740, 45},{725, 20} };
	scanlineFill(polygon1, glm::vec3(1.0f, 1.0f, 0.0f));
	std::vector<glm::ivec2> polygon2 = { {780, 10}, {780, 100}, {800, 50}, {840,140}, {830, 20} ,{800,40} };
	scanlineFill(polygon2, glm::vec3(1.0f, 1.0f, 1.0f));
	std::vector<glm::ivec2> polygon3 = { {850, 10}, {870, 100}, {845, 120}, {900,100},{920,60},{920, 100},{940, 80},{900,20} };
	scanlineFill(polygon3, glm::vec3(1.0f, 0.0f, 0.0f));
	MidCircle(850, 200, 50 , glm::vec3(1.0f, 0.0f, 0.0f));
	FloodFill4(850, 200, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	MidCircle(970, 200, 60, glm::vec3(0.0f, 1.0f, 0.0f));
	FloodFill4(970, 200, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	MidCircle(1090, 200, 50, glm::vec3(1.0f, 1.0f, 1.0f));
	BoundaryFill4(1090, 200, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	MidCircle(1210, 200, 60, glm::vec3(1.0f, 1.0f, 0.0f));
	BoundaryFill4(1210, 200, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));*/
	/*draw_points();
	draw_lines();
	draw_line_strip();
	draw_line_loop();
	draw_triangles();
	draw_triangle_strip();
	draw_triangle_fan();
	draw_quads();
	draw_quad_strip();
	draw_polygon();
	draw_star();*/
	//绘制完毕，交换缓存 
	glfwSwapBuffers(m_glfwWindow);
}

BOOL CCG2022112454冷家苇View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCG2022112454冷家苇View 绘图

void CCG2022112454冷家苇View::OnDraw(CDC* /*pDC*/)
{

	CCG2022112454冷家苇Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码 
	RenderScene();
}

void CCG2022112454冷家苇View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCG2022112454冷家苇View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCG2022112454冷家苇View 诊断

#ifdef _DEBUG
void CCG2022112454冷家苇View::AssertValid() const
{
	CView::AssertValid();
}

void CCG2022112454冷家苇View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCG2022112454冷家苇Doc* CCG2022112454冷家苇View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇Doc)));
	return (CCG2022112454冷家苇Doc*)m_pDocument;
}
#endif //_DEBUG


// CCG2022112454冷家苇View 消息处理程序

int CCG2022112454冷家苇View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CView::OnCreate(lpCreateStruct) == -1)

		return -1;
	// TODO:  在此添加您专用的创建代码 

	if (!InitGLFW())
		return -1;
	return 0;
}

void CCG2022112454冷家苇View::OnDestroy()
{

	if (m_glfwWindow) {


		glfwDestroyWindow(m_glfwWindow);


		glfwTerminate();


		m_glfwWindow = nullptr;

	}


	CView::OnDestroy();


	// TODO: 在此处添加消息处理程序代码 
}

BOOL CCG2022112454冷家苇View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值 


	//return CView::OnEraseBkgnd(pDC); 

	return TRUE; // 禁用背景擦除 
}

void CCG2022112454冷家苇View::OnSize(UINT nType, int cx, int cy)
{

	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码 
	if (m_hWndGL) {
		// 调整GLFW子窗口大小与视图客户区匹配 
		::SetWindowPos(m_hWndGL, NULL, 0, 0, cx, cy, SWP_NOZORDER);
		glfwMakeContextCurrent(m_glfwWindow);
		//通过文档类操作相机 
		CCG2022112454冷家苇Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc) {
			pDoc->OnViewResize(cx, cy);
		}
		else {
			//+0 
			glViewport(0, 0, cx, cy);
			glMatrixMode(GL_PROJECTION);
		    glLoadIdentity();
			// 
			//基本图形生成算法实验部分，假定观察窗口与视口一致。（左下角为原点，右上角为(cx,cy)） 
			//使用正交平行投影 
			//glOrtho(0, cx,    //x范围 
			// 0, cy,     //y范围 
			// -10000.0f, 10000.0f); //z范围（深度） 
			//调到屏幕中心 
			glOrtho(-cx / 2.0f, cx / 2.0f, //x范围 
				-cy / 2.0f, cy / 2.0f,  //y范围 
				-10000.0f, 10000.0f); //z范围（深度） 
			// 
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
	} //
}

void CCG2022112454冷家苇View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值 
	if (nIDEvent == mTimer) {
		RenderScene();
	}
	__super::OnTimer(nIDEvent);
}
