// CInputDialog1.cpp: 实现文件
//

#include "pch.h"
#include "CG2022112454冷家苇.h"
#include "afxdialogex.h"
#include "CInputDialog1.h"


// CInputDialog1 对话框

IMPLEMENT_DYNAMIC(CInputDialog1, CDialogEx)

CInputDialog1::CInputDialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_DIALOG1, pParent)
	, type(Type::CUBE)
	, mTitle(_T(" "))
	, x(100)
	, y(0)
	, z(0)
	, l(100)
	, w(100)
	, h(100)
	, rd(100)
	, ru(100)
	, h1(100)
	, slice(30)
	, stack(10)
	, angle(45)
	, x1(1)
	, y1(1)
	, z1(1)
	, m(1)
{

}

CInputDialog1::~CInputDialog1()
{
}

void CInputDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_VALUE1, x);
	DDX_Text(pDX, IDC_INPUT_VALUE2, y);
	DDX_Text(pDX, IDC_INPUT_VALUE3, z);
	DDX_Text(pDX, IDC_INPUT_VALUE4, l);
	DDX_Text(pDX, IDC_INPUT_VALUE5, w);
	DDX_Text(pDX, IDC_INPUT_VALUE6, h);
	DDX_Text(pDX, IDC_INPUT_VALUE7, rd);
	DDX_Text(pDX, IDC_INPUT_VALUE8, ru);
	DDX_Text(pDX, IDC_INPUT_VALUE9, h1);
	DDX_Text(pDX, IDC_INPUT_VALUE10, slice);
	DDX_Text(pDX, IDC_INPUT_VALUE11, stack);
	DDX_Text(pDX, IDC_INPUT_VALUE12, angle);
	DDX_Text(pDX, IDC_INPUT_VALUE13, x1);
	DDX_Text(pDX, IDC_INPUT_VALUE14, y1);
	DDX_Text(pDX, IDC_INPUT_VALUE15, z1);
	DDX_Text(pDX, IDC_INPUT_VALUE16, m);
}


BEGIN_MESSAGE_MAP(CInputDialog1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputDialog1::OnBnClickedOk)
END_MESSAGE_MAP()

void CInputDialog1::hideCube() {
	int ids[] = { IDC_INPUT_VALUE4,IDC_INPUT_VALUE5 ,IDC_INPUT_VALUE6 ,IDC_STATIC_3 ,IDC_STATIC_4 ,IDC_STATIC_5 };

	hideItems(ids, 6);
	// 刷新对话框以确保控件立即隐藏
	Invalidate();
	UpdateWindow();
}
void CInputDialog1::hideCylinder() {
	int ids[] = { IDC_INPUT_VALUE7 ,IDC_INPUT_VALUE8 ,IDC_INPUT_VALUE9,IDC_INPUT_VALUE10 ,IDC_INPUT_VALUE11,
		IDC_STATIC_6,IDC_STATIC_7,IDC_STATIC_8,IDC_STATIC_9,IDC_STATIC_10 };

	hideItems(ids, 10);
	// 刷新对话框以确保控件立即隐藏
	Invalidate();
	UpdateWindow();
}
void CInputDialog1::hideItems(int ids[], int n) {
	for (int i = 0;i < n;i++) {
		CWnd* item = GetDlgItem(ids[i]);
		if (item)
			item->ShowWindow(SW_HIDE);
	}
}
void CInputDialog1::setType(Type t) {
	type = t;
}
// CInputDialog1 消息处理程序

void CInputDialog1::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true); //取回输入框中数据到成员变量 
	CDialogEx::OnOK();
}

BOOL CInputDialog1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化 
	if (type == Type::CUBE)
	{
		this->hideCylinder();
	}
	else if (type == Type::CYLINDER)
	{
		this->hideCube();
	}
	return TRUE;  // return TRUE unless you set the focus to a control 
	// 异常: OCX 属性页应返回 FALSE 
}

