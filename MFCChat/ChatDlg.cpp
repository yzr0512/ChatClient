// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

#include <afxsock.h>  
#include <afxwin.h>

// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_pParentWnd(pParent)
{
	m_csInputMsg = _T("");
	m_csOutputMsg = _T("");
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_MSG, m_csInputMsg);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_MSG, m_csOutputMsg);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND_MSG, &CChatDlg::OnBnClickedButtonSendMsg)
END_MESSAGE_MAP()


// CChatDlg 消息处理程序


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString csName;
	csName = m_Name;
	SetWindowText(_T("Chatting with ") + csName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 关闭聊天窗口时同时删除主窗口中的元素
// 需要重载OnCancel()和PostNcDestroy()
void CChatDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	((CMFCChatDlg*)m_pParentWnd)->CloseChatDlg(m_nID);

	// 删除窗口
	DestroyWindow();
}

void CChatDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 删除当前对象
	delete this;

	CDialogEx::PostNcDestroy();
}


void CChatDlg::OnBnClickedButtonSendMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_csInputMsg == "") // 编辑框中未输入内容
	{
		// 气泡提示：编辑框的内容不能为空
		// 
	}
	
	struct MSG_TRANSPOND chatmsg;
	chatmsg.nType = CHATING_TEXT_MSG;
	
	memset(chatmsg.ToID, '\0', sizeof(chatmsg.ToID));
	memset(chatmsg.FromID, '\0', sizeof(chatmsg.FromID));
	memset(chatmsg.Date, '\0', sizeof(chatmsg.Date));
	memset(chatmsg.Time, '\0', sizeof(chatmsg.Time));
	memset(chatmsg.Data, '\0', sizeof(chatmsg.Data));
	
	CString csMyID; // 我的ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // 现在时间
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	

	memcpy(chatmsg.ToID, m_csID, m_csID.GetLength() * 2);
	memcpy(chatmsg.FromID, csMyID, csMyID.GetLength() * 2);
	memcpy(chatmsg.Date, csDate, csDate.GetLength() * 2);
	memcpy(chatmsg.Time, csTime, csTime.GetLength() * 2);
	memcpy(chatmsg.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2);
	
	// 发送消息
	
	if(((CMFCChatDlg*)m_pParentWnd)->SendMsg(&chatmsg, sizeof(chatmsg)))
	{
		// 发送成功后 显示我发的内容
		AddMessage(_T("我： "), csTime, m_csInputMsg);
		// 清空输入框的内容
		m_csInputMsg = "";
	}

	// 将信息更新到屏幕
	UpdateData(FALSE);
	
}


// 输出信息
int CChatDlg::AddMessage(const CString& csName, const CString& csTime, const CString& csMsg)
{		
		m_csOutputMsg += csName;
		m_csOutputMsg += csTime;
		m_csOutputMsg += "\r\n";
		m_csOutputMsg += csMsg;
		m_csOutputMsg += "\r\n";
		UpdateData(FALSE);
		return 0;
}

// 此处可设置快捷键
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) // 按下回车键发送消息
		{
			OnBnClickedButtonSendMsg();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

