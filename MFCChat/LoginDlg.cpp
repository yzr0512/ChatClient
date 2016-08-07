// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "RegistDlg.h"
#include "MFCChatDlg.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_csID(_T(""))
	, m_csKey(_T(""))
{
	m_pParentWnd = pParent;
	m_pRegDlg = NULL;
}

CLoginDlg::~CLoginDlg()
{}

// 界面控件与绑定变量的数据交换
void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 账号 字符上限ID_MAX	
	DDX_Text(pDX, IDC_EDIT_ID_LOGIN, m_csID);
	DDV_MaxChars(pDX, m_csID, ID_MAX - 1);

	// 密码 字符上限KEY_MAX
	DDX_Text(pDX, IDC_EDIT_KEY_LOGIN, m_csKey);
	DDV_MaxChars(pDX, m_csKey, KEY_MAX - 1);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGIST_DLG, &CLoginDlg::OnBnClickedButtonRegDlg)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

/*********************************************************
函数名称：OnBnClickedButtonLogin
功能描述：发送登录消息
*********************************************************/
void CLoginDlg::OnBnClickedButtonLogin()
{
	


	//更新变量的数据
	UpdateData(TRUE);
	
	// 判断账号或密码是否为空
	if(m_csID == "")
	{
		MessageBox(_T("账号不能为空"));
		return;
	}
	if(m_csKey == "")
	{
		MessageBox(_T("密码不能为空"));
		return;
	}

	// 将要发送的结构体
	struct MSG_LOGIN msg_lg;

	msg_lg.nType = LOGIN; // 这是登录信息结构体

	size_t i; // 已转换的字符数
	wcstombs_s(&i, msg_lg.nID, m_csID, m_csID.GetLength() + 1); // 宽字符转多字符
	wcstombs_s(&i, msg_lg.Key, m_csKey, m_csKey.GetLength() + 1); // 宽字符转多字符
	msg_lg.nStatus = IDS_STATUS_ONLINE;
	// 发送信息
	((CMFCChatDlg *)m_pParentWnd)->SendMsg(&msg_lg, sizeof(msg_lg));
	
}


/*********************************************************
函数名称：OnBnClickedButtonRegDlg
功能描述：打开注册窗口
*********************************************************/
void CLoginDlg::OnBnClickedButtonRegDlg()
{		
	if(m_pRegDlg == NULL)
	{
		m_pRegDlg = new CRegistDlg(this);
		m_pRegDlg->Create (IDD_REGIST_DLG);
		m_pRegDlg->ShowWindow(SW_SHOW);	
	}

}


/*********************************************************
函数名称：OnSysCommand
功能描述：请求系统命令
*********************************************************/
void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID == SC_CLOSE)
	{
		if(((CMFCChatDlg*)m_pParentWnd)->m_csMyID != m_csID || m_csID == "")
		{// 在没有登录的情况下关闭登录窗口同时将主窗口关掉
			((CMFCChatDlg*)m_pParentWnd)->OnCancel();
		}
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


/*********************************************************
函数名称：OnCancel
功能描述：关闭一个非模态对话框时调用
备    注：与PostNcDestroy搭配
*********************************************************/
void CLoginDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//DestroyWindow();
	CDialogEx::OnCancel();
}

/*********************************************************
函数名称：PostNcDestroy
功能描述：关闭一个非模态对话框时调用
*********************************************************/
void CLoginDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	//delete this;
	CDialogEx::PostNcDestroy();
}
