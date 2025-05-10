#pragma once
#include "afxdialogex.h"

enum class Type
{
	CUBE,
	CYLINDER
};
// CInputDialog1 对话框

class CInputDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDialog1)

public:
	CInputDialog1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInputDialog1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void hideCube();
	void hideCylinder();
	void hideItems(int ids[],int n);
	void setType(Type t);
	CString mTitle;
	Type type;
	double x;
	double y;
	double z;
	double l;
	double w;
	double h;
	double rd;
	double ru;
	double h1;
	int slice;
	int stack;
	double angle;
	int x1;
	int y1;
	int z1;
};
