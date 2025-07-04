﻿// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CG2022112454冷家苇Doc.h: CCG2022112454冷家苇Doc 类的接口
//


#pragma once
#include <memory> 
#include "CGLight.h"
class CGScene;
class CGRenderContext;
class CGNode;
class CGGroup;
class CCGSceneGraphView;
class CCG2022112454冷家苇Doc : public CDocument
{
protected: // 仅从序列化创建
	CCG2022112454冷家苇Doc() noexcept;
	DECLARE_DYNCREATE(CCG2022112454冷家苇Doc)

	// 特性 
public:

	std::shared_ptr<CGScene> mScene = nullptr;
	// 操作 
public:

	bool RenderScene(CGRenderContext* pRC);

	bool AddRenderable(std::shared_ptr<CGNode> r);

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCG2022112454冷家苇Doc();
	static bool doRecallR;
	static bool doRecallM;
	static bool doRecallL;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnDraw2dLineseg();
	afx_msg void OnUpdateDraw2dLineseg(CCmdUI* pCmdUI);
	afx_msg void OnDraw2dLineloop();
	afx_msg void OnUpdateDraw2dLineloop(CCmdUI* pCmdUI);
protected:
	CGGroup* mSelectedGroup = nullptr;
	HTREEITEM mSelectedItem = nullptr;
public:
	CCGSceneGraphView* GetSceneGraphView();
	void InstToSceneTree(CTreeCtrl* pTree);//实列节点加入场景树 
	void InstToSceneTree(CTreeCtrl* pTree, HTREEITEM hInst, CGNode* node);
	void OnSelectSceneTreeItem(CTreeCtrl* pTree, HTREEITEM hItem); //场景树中选中节点
	void setLight(shared_ptr<CGLight> light);
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnRotateRight();
	afx_msg void OnRotateLeft();
	afx_msg void OnBigger();
	afx_msg void OnSmaller();
	afx_msg void OnBiggerX();
	afx_msg void OnSmallerX();
	afx_msg void OnBiggerY();
	afx_msg void OnSmallerY();
	afx_msg void OnRotateByMouse();
	afx_msg void OnScaleByMouse();
	afx_msg void OnMoveByMouse();
	afx_msg void OnDrawCube();
	afx_msg void OnDrawCylinder();
	afx_msg void OnDraw3DCube();
	afx_msg void OnDraw3DCylinber();
protected:
	UINT mTimer = 0; //定时器
public:
	afx_msg void OnBtnTimer();
	afx_msg void OnUpdateBtnTimer(CCmdUI* pCmdUI);
	afx_msg void OnBuildRobot();

public:
	void OnViewResize(int cx, int cy);
	afx_msg void OnCameraControl();
	afx_msg void OnTraceBoll();
	afx_msg void OnPointLight();
	afx_msg void OnparallelLight();
	afx_msg void OnSpotLight();
	afx_msg void OnTimeVaryingControl();
	afx_msg void OnUpdateTimeControl(CCmdUI* pCmdUI);
	afx_msg void OnMer();
	afx_msg void OnMertrialChange();
};
