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

// CG2022112454冷家苇Doc.cpp: CCG2022112454冷家苇Doc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG2022112454冷家苇.h"
#endif

#include "CG2022112454冷家苇Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CG2022112454冷家苇View.h" 
#include "CGScene.h" 
#include "CGCamera.h" 
#include "CGTransform.h" 
#include "CGGeode.h" 
#include "CGLineSegment.h" 
#include "CGLineLoop.h" 
#include "CGRenderContext.h" 
#include "UIEventHandler.h" 
#include "CGDraw2DLineSeg.h" 
#include "CGDraw2DLineLoop.h" 
#include "CGSceneGraphView.h" 
#include "CGModel2DTransform.h"
#include "CGCube.h"
#include "TessellationHints.h"
#include "CInputDialog1.h"
#include "CGCylinder.h"
#include"RobotBodyTransform.h"
#include "CGRobot.h"
#include "CGCameraControl.h"
#include "CGTraceBoll.h"
// CCG2022112454冷家苇Doc

IMPLEMENT_DYNCREATE(CCG2022112454冷家苇Doc, CDocument)

BEGIN_MESSAGE_MAP(CCG2022112454冷家苇Doc, CDocument)
	ON_COMMAND(ID_BUTTON2, &CCG2022112454冷家苇Doc::OnDraw2dLineseg)
	ON_UPDATE_COMMAND_UI(ID_BUTTON2, &CCG2022112454冷家苇Doc::OnUpdateDraw2dLineseg)
	ON_COMMAND(ID_BUTTON3, &CCG2022112454冷家苇Doc::OnDraw2dLineloop)
	ON_UPDATE_COMMAND_UI(ID_BUTTON3, &CCG2022112454冷家苇Doc::OnUpdateDraw2dLineloop)
	ON_COMMAND(ID_BUTTON4, &CCG2022112454冷家苇Doc::OnMoveLeft)
	ON_COMMAND(ID_BUTTON5, &CCG2022112454冷家苇Doc::OnMoveRight)
	ON_COMMAND(ID_BUTTON7, &CCG2022112454冷家苇Doc::OnMoveUp)
	ON_COMMAND(ID_BUTTON6, &CCG2022112454冷家苇Doc::OnMoveDown)
	ON_COMMAND(ID_BUTTON8, &CCG2022112454冷家苇Doc::OnRotateRight)
	ON_COMMAND(ID_BUTTON9, &CCG2022112454冷家苇Doc::OnRotateLeft)
	ON_COMMAND(ID_BUTTON10, &CCG2022112454冷家苇Doc::OnBigger)
	ON_COMMAND(ID_BUTTON11, &CCG2022112454冷家苇Doc::OnSmaller)
	ON_COMMAND(ID_BUTTON12, &CCG2022112454冷家苇Doc::OnBiggerX)
	ON_COMMAND(ID_BUTTON13, &CCG2022112454冷家苇Doc::OnSmallerX)
	ON_COMMAND(ID_BUTTON14, &CCG2022112454冷家苇Doc::OnBiggerY)
	ON_COMMAND(ID_BUTTON15, &CCG2022112454冷家苇Doc::OnSmallerY)
	ON_COMMAND(ID_BUTTON16, &CCG2022112454冷家苇Doc::OnRotateByMouse)
	ON_COMMAND(ID_BUTTON18, &CCG2022112454冷家苇Doc::OnMoveByMouse)
	ON_COMMAND(ID_BUTTON20, &CCG2022112454冷家苇Doc::OnDraw3DCube)
	ON_COMMAND(ID_BUTTON21, &CCG2022112454冷家苇Doc::OnDraw3DCylinber)
	ON_COMMAND(ID_BUTTON17, &CCG2022112454冷家苇Doc::OnBtnTimer)
	ON_UPDATE_COMMAND_UI(ID_BUTTON17, &CCG2022112454冷家苇Doc::OnUpdateBtnTimer)
	ON_COMMAND(ID_BUTTON19, &CCG2022112454冷家苇Doc::OnBuildRobot)
	ON_COMMAND(ID_BUTTON34, &CCG2022112454冷家苇Doc::OnCameraControl)
	ON_COMMAND(ID_BUTTON22, &CCG2022112454冷家苇Doc::OnTraceBoll)
	ON_COMMAND(ID_BUTTON23, &CCG2022112454冷家苇Doc::OnPointLight)
	ON_COMMAND(ID_BUTTON26, &CCG2022112454冷家苇Doc::OnparallelLight)
	ON_COMMAND(ID_BUTTON24, &CCG2022112454冷家苇Doc::OnSpotLight)
	ON_COMMAND(ID_BUTTON27, &CCG2022112454冷家苇Doc::OnTimeVaryingControl)
END_MESSAGE_MAP()

bool CCG2022112454冷家苇Doc::doRecall = false; // 或你需要的初始值
// CCG2022112454冷家苇Doc 构造/析构

CCG2022112454冷家苇Doc::CCG2022112454冷家苇Doc() noexcept
{
	// TODO: 在此添加一次性构造代码

	mScene = std::make_shared<CGScene>();

	//mScene->SetMainCamera(std::make_shared<CGCamera>());

	//auto e = std::make_shared<CGGeode>();

	///*auto line = std::make_shared<CGLineSegment>(glm::dvec3(100, 100, 0), glm::dvec3(400, 300, 0));

	//e->AddChild(line);*/

	//auto g = std::make_shared<CGTransform>();

	//g->AddChild(e);

	//mScene->SetSceneData(g);
	//长方体（模型） 
}

CCG2022112454冷家苇Doc::~CCG2022112454冷家苇Doc()
{
}
BOOL CCG2022112454冷家苇Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}
bool CCG2022112454冷家苇Doc::RenderScene(CGRenderContext* pRC)
{

	if (pRC == nullptr)


		return false;

	if (mScene == nullptr)


		return false;

	CGCamera* pCamera = mScene->GetMainCamera();

	if (pCamera == nullptr)


		return false;


	return mScene->Render(pRC, pCamera);
}
bool CCG2022112454冷家苇Doc::AddRenderable(std::shared_ptr<CGNode> r)
{
	if (mSelectedGroup) { //需要先选中一各组节点 
		//模型加入实例节点后加入场景 
		auto ge = std::make_shared<CGGeode>();
		ge->AddChild(r);
		mSelectedGroup->AddChild(ge);
		CTreeCtrl& tree = GetSceneGraphView()->GetTreeCtrl();
		InstToSceneTree(&tree, mSelectedItem, ge.get());
		return true;
	}
	else {
		AfxMessageBox(_T("请先选择添加子节点的组节点！"));
	}
	return false;
}
CCGSceneGraphView* CCG2022112454冷家苇Doc::GetSceneGraphView()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCGSceneGraphView))) {
			CCGSceneGraphView* view = dynamic_cast<CCGSceneGraphView*>(pView);
			return view;
		}
	}
	return nullptr;
}
void CCG2022112454冷家苇Doc::InstToSceneTree(CTreeCtrl* pTree)
{
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM hInst;
	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvinsert.item.hItem = NULL;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 40;
	tvinsert.item.cChildren = 0;
	tvinsert.item.lParam = NULL;// 
	CString str(_T("场景"));
	tvinsert.item.pszText = str.GetBuffer();
	str.ReleaseBuffer();
	hInst = pTree->InsertItem(&tvinsert);
	pTree->SetItemData(hInst, DWORD_PTR(mScene.get()));
	InstToSceneTree(pTree, hInst, mScene->GetSceneData());
	pTree->Expand(hInst, TVE_EXPAND);
}
void CCG2022112454冷家苇Doc::InstToSceneTree(CTreeCtrl* pTree, HTREEITEM hParent, CGNode* node)
{
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM hTree;
	tvinsert.hParent = hParent;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvinsert.item.hItem = NULL;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 40;
	tvinsert.item.cChildren = 0;
	tvinsert.item.lParam = LPARAM(&node);// 
	if (node->asGeode()) {
		CString str(_T(node->Name()));
		tvinsert.item.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		hTree = pTree->InsertItem(&tvinsert);
		pTree->SetItemData(hTree, DWORD_PTR(node));
		//叶子实例节点不再显示模型节点 
	}
	else 
	if (node->asTransform()) {
		CString str(node->Name());
		tvinsert.item.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		hTree = pTree->InsertItem(&tvinsert);
		pTree->SetItemData(hTree, DWORD_PTR(node));
		unsigned int childs = node->asTransform()->GetNumChildren();
		for (unsigned int i = 0; i < childs; i++) {
			InstToSceneTree(pTree, hTree, node->asTransform()->GetChild(i));
		}
	}
	else if (node->asGroup()) {
		CString str(node->Name());
		tvinsert.item.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		hTree = pTree->InsertItem(&tvinsert);
		pTree->SetItemData(hTree, DWORD_PTR(node));
		unsigned int childs = node->asGroup()->GetNumChildren();
		for (unsigned int i = 0; i < childs; i++) {
			InstToSceneTree(pTree, hTree, node->asGroup()->GetChild(i));
		}
	}
}
void CCG2022112454冷家苇Doc::OnSelectSceneTreeItem(CTreeCtrl* pTree, HTREEITEM hItem)
{
	mSelectedItem = hItem;
	if (!mSelectedItem) {
		mSelectedGroup = nullptr;
		return;
	}
	HTREEITEM hRoot = pTree->GetRootItem();
	if (mSelectedItem == hRoot) {
		mSelectedGroup = nullptr;
	}
	else {
		CGGroup* node = (CGGroup*)(pTree->GetItemData(mSelectedItem));
		if (node && node->asGroup() && !(node->asGeode())) { //不允许叶子节点上再 
			mSelectedGroup = dynamic_cast<CGGroup*>(node);
		}
		else {
			mSelectedGroup = nullptr;
		}
	}
}

// CCG2022112454冷家苇Doc 序列化

void CCG2022112454冷家苇Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

void CCG2022112454冷家苇Doc::OnViewResize(int cx, int cy) { mScene->GetMainCamera()->viewport()->set(0, 0, cx, cy); }
#ifdef SHARED_HANDLERS

// 缩略图的支持
void CCG2022112454冷家苇Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CCG2022112454冷家苇Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CCG2022112454冷家苇Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCG2022112454冷家苇Doc 诊断

#ifdef _DEBUG
void CCG2022112454冷家苇Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCG2022112454冷家苇Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCG2022112454冷家苇Doc 命令

void CCG2022112454冷家苇Doc::OnDraw2dLineseg()
{
	// TODO: 在此添加命令处理程序代码 
	if (!mSelectedGroup) {
		AfxMessageBox(_T("请先选择添加子节点的组节点！"));
		return;
	}
	CCG2022112454冷家苇View* view = nullptr;

	POSITION pos = GetFirstViewPosition();

	while (pos != NULL)

	{


		CView* pView = GetNextView(pos);


		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {



			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);



			break;


		}

	}


	if (UIEventHandler::CurCommand()) {


		UIEventHandler::DelCommand();

	}

	if (view != nullptr) {


		UIEventHandler::SetCommand(new CGDraw2DLineSeg(view->glfwWindow())); //创建绘制直线段的命令对象

	}
}

void CCG2022112454冷家苇Doc::OnUpdateDraw2dLineseg(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(UIEventHandler::CurCommand() && UIEventHandler::CurCommand()->GetType() ==
		EventType::Draw2DLineSeg);
}

void CCG2022112454冷家苇Doc::OnDraw2dLineloop()
{
	// TODO: 在此添加命令处理程序代码
	if (!mSelectedGroup) {
		AfxMessageBox(_T("请先选择添加子节点的组节点！"));
		return;
	}
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}

	}
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		UIEventHandler::SetCommand(new CGDraw2DLineLoop(view->glfwWindow())); //创建绘制直线段的命令对象
	}
}

void CCG2022112454冷家苇Doc::OnUpdateDraw2dLineloop(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(UIEventHandler::CurCommand() && UIEventHandler::CurCommand()->GetType() ==
		EventType::Draw2DLineLoop);
}

void CCG2022112454冷家苇Doc::OnMoveLeft()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		// 直接应用向左平移变换（每次移动10个单位）
		const float translateAmount = -10.0f; // 负值表示向左移动
		child->Translate(translateAmount, 0);
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已向左移动");
	}
}

void CCG2022112454冷家苇Doc::OnMoveRight()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		// 直接应用向右平移变换（每次移动10个单位）
		const float translateAmount = 10.0f; // 正值表示向右移动
		child->Translate(translateAmount, 0);
		// 更新所有视图
		UpdateAllViews(NULL);
		// 设置状态栏提示
		view->ShowPrompt("对象已向右移动");
	}
}

void CCG2022112454冷家苇Doc::OnMoveUp()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		// 直接应用向上平移变换（每次移动10个单位）
		const float translateAmount = 10.0f; // 正值表示向上移动
		child->Translate(0, translateAmount);
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已向左移动");
	}
}

void CCG2022112454冷家苇Doc::OnMoveDown()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		// 直接应用向下平移变换（每次移动10个单位）
		const float translateAmount = -10.0f; // 负值表示向下移动
		child->Translate(0, translateAmount);
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已向左移动");
	}
}

void CCG2022112454冷家苇Doc::OnMoveByMouse()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要移动的子节点！"));
			return;
		}
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(),false,false,true)); //创建鼠标交互命令对象
		// 更新所有视图
		UpdateAllViews(NULL);
	}
}

void CCG2022112454冷家苇Doc::OnRotateRight()//默认基准点为原点（0,0）
{
	// TODO: 在此添加命令处理程序代码
	// 顺时针旋转操作
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		// 直接应用旋转变换（每次旋转30度）
		const float rotateAmount = -30.0f; // 负值表示顺时针
		child->Rotate(rotateAmount,0,0);
		// 更新所有视图
		UpdateAllViews(NULL);
		// 设置状态栏提示
		view->ShowPrompt("对象已向顺时针旋转");
	}
}

void CCG2022112454冷家苇Doc::OnRotateLeft()
{
	// TODO: 在此添加命令处理程序代码
	// 逆时针旋转操作
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		// 直接应用旋转变换（每次旋转30度）
		const float rotateAmount = 30.0f; // 正值表示逆时针
		child->Rotate(rotateAmount, 0, 0);
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已逆时针旋转");
	}

}

void CCG2022112454冷家苇Doc::OnRotateByMouse()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要旋转的子节点！"));
			return;
		}
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow())); //创建鼠标交互命令对象
		// 更新所有视图
		UpdateAllViews(NULL);
	}
	
}

void CCG2022112454冷家苇Doc::OnBigger() //默认基准点为原点（0,0）
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}

	if (view != nullptr) {
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		// 直接应用缩放（每次放大0.5倍）
		const float zoomAmount = 1.5f; 
		child->Scale(zoomAmount, zoomAmount,0,0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), true, true)); //创建鼠标交互命令对象
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已放大");
	}
}

void CCG2022112454冷家苇Doc::OnSmaller()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		// UIEventHandler::SetCommand(new CGModel2DTransform((CGRenderable*)pTree->GetItemData(mSelectedItem),view->glfwWindow())); 
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		// 直接应用缩放（每次缩小1倍）
		const float zoomAmount = 0.5f;
		child->Scale(zoomAmount, zoomAmount, 0, 0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), true, true)); //创建鼠标交互命令对象
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已缩小");
	}
}

void CCG2022112454冷家苇Doc::OnBiggerX()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		// UIEventHandler::SetCommand(new CGModel2DTransform((CGRenderable*)pTree->GetItemData(mSelectedItem),view->glfwWindow())); 
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		// 直接应用缩放（每次缩小1倍）
		const float zoomAmount = 1.5f;
		child->Scale(zoomAmount, 1, 0, 0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), true, false)); //创建鼠标交互命令对象

		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已在X方向放大");
	}
}

void CCG2022112454冷家苇Doc::OnSmallerX()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		// UIEventHandler::SetCommand(new CGModel2DTransform((CGRenderable*)pTree->GetItemData(mSelectedItem),view->glfwWindow())); 
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		// 直接应用缩放（每次缩小1倍）
		const float zoomAmount = 0.5f;
		child->Scale(zoomAmount, 1, 0, 0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), true, false)); //创建鼠标交互命令对象

		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已在X方向缩小");
	}
}

void CCG2022112454冷家苇Doc::OnBiggerY()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		// UIEventHandler::SetCommand(new CGModel2DTransform((CGRenderable*)pTree->GetItemData(mSelectedItem),view->glfwWindow())); 
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要放大的子节点！"));
			return;
		}
		// 直接应用缩放（每次缩小1倍）
		const float zoomAmount = 1.5f;
		child->Scale(1, zoomAmount, 0, 0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), false, true)); //创建鼠标交互命令对象

		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已在Y方向放大");
	}
}

void CCG2022112454冷家苇Doc::OnSmallerY()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	CTreeCtrl* pTree = &pSceneGraphView->GetTreeCtrl();
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	if (view != nullptr) {
		// UIEventHandler::SetCommand(new CGModel2DTransform((CGRenderable*)pTree->GetItemData(mSelectedItem),view->glfwWindow())); 
		CGGeode* renderable = (CGGeode*)pTree->GetItemData(mSelectedItem);
		if (!renderable) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		CGNode* child = renderable->GetChild(0);
		if (!child) {
			AfxMessageBox(_T("请先选择需要缩小的子节点！"));
			return;
		}
		// 直接应用缩放（每次缩小1倍）
		const float zoomAmount = 0.5f;
		child->Scale(1, zoomAmount, 0, 0);
		UIEventHandler::SetCommand(new CGModel2DTransform(child, view->glfwWindow(), false, true)); //创建鼠标交互命令对象
		// 更新所有视图
		UpdateAllViews(NULL);

		// 设置状态栏提示
		view->ShowPrompt("对象已在Y方向缩小");
	}
}
std::shared_ptr<CGMaterial> choseMaterial(int i) {
	auto material = std::make_shared<CGMaterial>();
	switch (i) {
	case 2: // 塑料材质
		material->setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		material->setAmbient(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)); // 较低的环境光吸收
		material->setDiffuse(glm::vec4(0.58f, 0.58f, 0.75f, 1.0f)); // 更接近塑料的颜色
		material->setSpecular(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)); // 略微增加镜面反射
		material->setShininess(64.0f); // 中等光泽度
		material->setColorMaterial(CM_DIFFUSE);
		break;

	case 3: // 木材材质
		material->setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		material->setAmbient(glm::vec4(0.05f, 0.025f, 0.0f, 1.0f)); // 较暗淡的环境光吸收
		material->setDiffuse(glm::vec4(0.55f, 0.275f, 0.09f, 1.0f)); // 深棕色色调
		material->setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 很少的镜面反射
		material->setShininess(5.0f); // 非常低的光泽度
		material->setColorMaterial(CM_AMBIENT_AND_DIFFUSE); // 结合环境光与漫反射颜色
		break;

	default: // 金属材质（如钢）
		material->setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		material->setAmbient(glm::vec4(0.02f, 0.02f, 0.02f, 1.0f)); // 极低的环境光吸收
		material->setDiffuse(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)); // 对于金属，漫反射颜色通常很暗
		material->setSpecular(glm::vec4(0.97f, 0.97f, 0.97f, 1.0f)); // 强烈的镜面反射
		material->setShininess(128.0f); // 高光泽度
		material->setColorMaterial(CM_SPECULAR); // 使用镜面反射颜色作为主颜色来源
		break;
	}
	return material;
}
std::shared_ptr<CGColor> choseColor(int i) {
	auto c = std::make_shared<CGColor>();
	switch (i) {
	case 2: // 塑料材质
		c->setValue(glm::vec4(0.58f, 0.58f, 0.75f, 1.0f)); //塑料色
		break;
	case 3: // 木材材质
		c->setValue(glm::vec4(0.55f, 0.275f, 0.09f, 1.0f)); //棕色
		break;	
	default: // 金属材质
		c->setValue(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); //黄色 
		break;
	}
	return c;
}
int cubeNum = 0;
void CCG2022112454冷家苇Doc::OnDraw3DCube()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog1 dlg;
	dlg.mTitle = _T("请输入立方体坐标位置和长宽高还有旋转角度"); //根据需要设置对话框标题 
	double x,y,z,length, width, height,angle;
	int x1, y1, z1,m;
	dlg.setType(Type::CUBE);
	if (dlg.DoModal() == IDOK)
	{
		x = dlg.x;
		y = dlg.y;
		z = dlg.z;
		length = dlg.l;
		height = dlg.h;
		width = dlg.w;
		angle = dlg.angle;
		x1 = dlg.x1;
		y1 = dlg.y1;
		z1 = dlg.z1;
		m = dlg.m;
		auto c = std::make_shared<CGCube>();
		auto h = std::make_shared<TessellationHints>();
		c->setTessellationHints(h);
		c->setDisplayListEnabled(true);

		//右长方体实例节点 
		auto t1 = std::make_shared<CGTransform>(); //实列组节点 
		auto e1 = std::make_shared<CGGeode>();  //实列叶节点 
		e1->setName(CString(("Cube" + std::to_string(cubeNum)).c_str())); // 节点名称
		e1->gocRenderStateSet()->setRenderState(choseColor(m), -1); //设置节点属性 
		t1->translate(x, y, z);
		t1->rotate(angle, x1, y1, z1);
		t1->scale(length, width, height);
		e1->AddChild(c);
		t1->AddChild(e1);
		t1->gocRenderStateSet()->setRenderState(choseMaterial(m), -1);
		mScene->GetSceneData()->asGroup()->AddChild(t1);

		//左长方体节点 
		auto t2 = std::make_shared<CGTransform>(); //实列组节点 
		auto e2 = std::make_shared<CGGeode>();  //实列叶节点 
		e2->setName(CString(("Cube" + std::to_string(cubeNum++)).c_str())); // 节点名称
		auto color2 = std::make_shared<CGColor>(); //属性 
		color2->setValue(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); //蓝色 
		e2->gocRenderStateSet()->setRenderState(color2, -1); //设置节点属性 
		auto p = std::make_shared<CGPolygonMode>(PM_LINE, PM_LINE); //设置线框模式 
		e2->gocRenderStateSet()->setRenderState(p, -1); //设置节点属性 
		t2->translate(0-x, y, z);
		t2->rotate(angle, x1, y1, z1);
		t2->scale(length, width, height);
		e2->AddChild(c);
		t2->AddChild(e2);
		mScene->GetSceneData()->asGroup()->AddChild(t2);

		 //更新所有视图
		UpdateAllViews(NULL);

	}
	

}

int cylinberNum = 0;
void CCG2022112454冷家苇Doc::OnDraw3DCylinber()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog1 dlg;
	dlg.mTitle = _T("请输入圆柱体坐标位置和下底半径、上顶半径、高度、环Z轴细分数、沿Z轴方向细分数还有旋转角度"); //根据需要设置对话框标题 
	double x,y,z,rd, ru, h1, angle;
	int slice, stack,x1,y1,z1,m;
	dlg.setType(Type::CYLINDER);
	if (dlg.DoModal() == IDOK)
	{
		x = dlg.x;
		y = dlg.y;
		z = dlg.z;
		rd = dlg.rd;
		ru = dlg.ru;
		h1 = dlg.h1;
		slice = dlg.slice;
		stack = dlg.stack;
		angle = dlg.angle;
		x1 = dlg.x1;
		y1 = dlg.y1;
		z1 = dlg.z1;
		m = dlg.m;
		// 创建共享的圆柱体几何
		auto cyl = std::make_shared<CGCylinder>(rd, ru, h1, slice, stack);
		auto hint = std::make_shared<TessellationHints>();
		cyl->setTessellationHints(hint);
		cyl->setDisplayListEnabled(true);

		// 圆柱体实例 1：红色实心
		auto t1 = std::make_shared<CGTransform>();
		auto e1 = std::make_shared<CGGeode>();
		e1->setName(CString(("Cylinber" + std::to_string(cylinberNum)).c_str())); // 节点名称
		auto color1 = std::make_shared<CGColor>();
		color1->setValue(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // 红色
		e1->gocRenderStateSet()->setRenderState(color1, -1);

		t1->translate(x, y, z);
		t1->rotate(angle, x1, y1, z1);
		t1->scale(1, 1, 1); // 可根据需要缩放
		e1->AddChild(cyl);
		t1->AddChild(e1);	
		t1->gocRenderStateSet()->setRenderState(choseMaterial(m), -1);
		mScene->GetSceneData()->asGroup()->AddChild(t1);

		// 圆柱体实例 2：绿色线框
		auto t2 = std::make_shared<CGTransform>();
		auto e2 = std::make_shared<CGGeode>();
		e2->setName(CString(("Cylinber" + std::to_string(cylinberNum++)).c_str())); // 节点名称
		auto color2 = std::make_shared<CGColor>();
		color2->setValue(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // 绿色
		e2->gocRenderStateSet()->setRenderState(color2, -1);

		auto wire = std::make_shared<CGPolygonMode>(PM_LINE, PM_LINE);
		e2->gocRenderStateSet()->setRenderState(wire, -1);

		t2->translate(0 - x, y, z);
		t2->rotate(angle, x1, y1, z1);
		t2->scale(1, 1, 1);
		e2->AddChild(cyl);
		t2->AddChild(e2);
		mScene->GetSceneData()->asGroup()->AddChild(t2);
		// 更新所有视图
		UpdateAllViews(NULL);
	}	
}

void CCG2022112454冷家苇Doc::OnBtnTimer()
{
	// TODO: 在此添加命令处理程序代码
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	if (view != nullptr) {
		mTimer = view->toggleFrameTimer();// 启动定时器 
	}
	doRecall = doRecall ? false : true;
}

void CCG2022112454冷家苇Doc::OnUpdateBtnTimer(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(mTimer != 0);
}

int robotNum = 0;
void CCG2022112454冷家苇Doc::OnBuildRobot()
{
	// TODO: 在此添加命令处理程序代码
	glEnable(GL_COLOR_MATERIAL);//启用颜色作为材质 
	//// 创建材质对象
	//auto material = std::make_shared<CGMaterial>();

	//// 设置材质属性
	//material->setAmbient(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
	//material->setDiffuse(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
	//material->setSpecular(glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f));
	//material->setShininess(76.8f);
	// 创建CGRobot对象
	 // 定义一个函数来创建带材质和颜色的对象
	// 设置材质（每个对象可以不同，比如金属/塑料）
	auto material = std::make_shared<CGMaterial>();
	material->setEmission(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	material->setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	material->setDiffuse(glm::vec4(0.9f, 0.9f, 0.7f, 1.0f));
	material->setSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	material->setShininess(32.0f);
	material->setColorMaterial(CM_DIFFUSE);

	auto robot = std::make_shared<CGRobot>();
	// 将材质应用到机器人节点
	robot->gocRenderStateSet()->setRenderState(material, -1);
	// 生成机器人结构
	auto robotRoot = robot->buildRobot();
	robotRoot->setName(CString(("Robot" + std::to_string(robotNum++)).c_str()));
	// 添加到场景
	mScene->GetSceneData()->asGroup()->AddChild(robotRoot);

	// 更新所有视图
	UpdateAllViews(NULL);
	
}


void CCG2022112454冷家苇Doc::OnCameraControl()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	UIEventHandler::SetCommand(new CGCameraControl(mScene->GetMainCamera(), view->glfwWindow())); //创建鼠标交互命令对象
}

void CCG2022112454冷家苇Doc::OnTraceBoll()
{
	// TODO: 在此添加命令处理程序代码
	CCGSceneGraphView* pSceneGraphView = GetSceneGraphView();
	CCG2022112454冷家苇View* view = nullptr;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCG2022112454冷家苇View))) {
			view = dynamic_cast<CCG2022112454冷家苇View*>(pView);
			break;
		}
	}
	// 如果当前有正在执行的命令，先删除它
	if (UIEventHandler::CurCommand()) {
		UIEventHandler::DelCommand();
	}
	UIEventHandler::SetCommand(new CGTraceBoll(mScene->GetMainCamera(), view->glfwWindow())); //创建鼠标交互命令对象
}

void CCG2022112454冷家苇Doc::OnPointLight()
{
	// TODO: 在此添加命令处理程序代码
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0); // 关闭之前可能开启的光源
	glEnable(GL_NORMALIZE); // 确保法线正确
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 启用光照与深度测试（必须）
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST); //启用深度测试，确保物体前后关系正确显示。
	glEnable(GL_COLOR_MATERIAL); //启用颜色追踪材质（GL_COLOR_MATERIAL），让材质属性跟随 glColor 的变化而变化。

	// 设置光源（点光源）
	GLfloat light0_position[] = { 100.0f, 100.0f, 100.0f, 1.0f }; // w=1 表示点光源
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 合理白光强度
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); //设置光源的漫反射颜色为白色（RGB: 1,1,1）
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position); //

	// 重置聚光灯参数，确保点光源行为正确
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f);
	GLfloat spot_direction[] = {0.0f, 0.0f, -1.0f}; // 虽然点光源不使用，但为保持一致性设置
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	// 创建光照模型（设置一次即可）
	auto lightModel = std::make_shared<CGLightModel>();
	lightModel->setAmbientColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 更暗的环境光
	lightModel->setLocalViewer(true);  // 启用局部观察者，意味着视点位置会影响高光计算（更真实）
	lightModel->setTwoSide(true);      // 启用双面光照，使物体背面也能被正确照亮（否则背面可能看起来是黑的）。

	// 将光照模型设置到场景根节点（防止多次覆盖）
	mScene->GetSceneData()->asGroup()->gocRenderStateSet()->setRenderState(lightModel, -1);
	// 更新所有视图
	UpdateAllViews(NULL);
}

void CCG2022112454冷家苇Doc::OnparallelLight()
{
	// TODO: 在此添加命令处理程序代码
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0); // 关闭之前可能开启的光源
	glEnable(GL_NORMALIZE); // 确保法线正确
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 启用光照与深度测试（必须）
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST); //启用深度测试，确保物体前后关系正确显示。
	glEnable(GL_COLOR_MATERIAL); //启用颜色追踪材质（GL_COLOR_MATERIAL），让材质属性跟随 glColor 的变化而变化。

	// 设置光源（平行光）
	GLfloat light0_position[] = { 100.0f, 100.0f, 100.0f, 0.0f }; // w=0 表示平行光
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 合理白光强度
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); //设置光源的漫反射颜色为白色（RGB: 1,1,1）
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position); //

	// 重置聚光灯参数，确保平行光源行为正确
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f);
	GLfloat spot_direction[] = {0.0f, 0.0f, -1.0f}; // 虽然平行光源不使用，但为保持一致性设置
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	// 创建光照模型（设置一次即可）
	auto lightModel = std::make_shared<CGLightModel>();
	lightModel->setAmbientColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 更暗的环境光
	lightModel->setLocalViewer(true);  // 启用局部观察者，意味着视点位置会影响高光计算（更真实）
	lightModel->setTwoSide(true);      // 启用双面光照，使物体背面也能被正确照亮（否则背面可能看起来是黑的）。

	// 将光照模型设置到场景根节点（防止多次覆盖）
	mScene->GetSceneData()->asGroup()->gocRenderStateSet()->setRenderState(lightModel, -1);
	// 更新所有视图
	UpdateAllViews(NULL);
}

void CCG2022112454冷家苇Doc::OnSpotLight()
{
	// TODO: 在此添加命令处理程序代码
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0); // 关闭之前可能开启的光源
	glEnable(GL_NORMALIZE); // 确保法线正确
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 启用光照与深度测试（必须）
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST); //启用深度测试，确保物体前后关系正确显示。
	glEnable(GL_COLOR_MATERIAL); //启用颜色追踪材质（GL_COLOR_MATERIAL），让材质属性跟随 glColor 的变化而变化。

	// 光源位置和颜色
	GLfloat light0_position[] = { 0.0f, 0.0f, 100.0f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 设置光源的基本属性
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// 设置聚光灯属性
	GLfloat light0_spotDirection[] = { 0.0f, 0.0f, -1.0f };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0f);      // 照射角度为 30 度
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);     // 集中程度

	// 创建光照模型（设置一次即可）
	auto lightModel = std::make_shared<CGLightModel>();
	lightModel->setAmbientColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 更暗的环境光
	lightModel->setLocalViewer(true);  // 启用局部观察者，意味着视点位置会影响高光计算（更真实）
	lightModel->setTwoSide(true);      // 启用双面光照，使物体背面也能被正确照亮（否则背面可能看起来是黑的）。

	// 将光照模型设置到场景根节点（防止多次覆盖）
	mScene->GetSceneData()->asGroup()->gocRenderStateSet()->setRenderState(lightModel, -1);
	// 更新所有视图
	UpdateAllViews(NULL);
}

void CCG2022112454冷家苇Doc::OnTimeVaryingControl()
{
	// TODO: 在此添加命令处理程序代码

}
