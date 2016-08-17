// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

#include <afxsock.h>  
#include <afxwin.h>

// CChatDlg �Ի���

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


// CChatDlg ��Ϣ�������


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString csName;
	csName.Format(L"%s", m_Name);
	SetWindowText(_T("Chatting with ") + csName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// �ر����촰��ʱͬʱɾ���������е�Ԫ��
// ��Ҫ����OnCancel()��PostNcDestroy()
void CChatDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	((CMFCChatDlg*)m_pParentWnd)->CloseChatDlg(m_nID);

	// ɾ������
	DestroyWindow();
}

void CChatDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ɾ����ǰ����
	delete this;

	CDialogEx::PostNcDestroy();
}

// �˴������ÿ�ݼ�
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) // ���»س���������Ϣ
		{
			OnBnClickedButtonSendMsg();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


// ������Ϣ
void CChatDlg::OnBnClickedButtonSendMsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_csInputMsg == "") // �༭����δ��������
	{
		// ������ʾ���༭������ݲ���Ϊ��
		// 
		return;
	}
	
	struct MSG_TRANSPOND msg_chat;
	msg_chat.nType = CHATING_TEXT_MSG;

	CString csMyID; // �ҵ�ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // ����ʱ��
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	
	
	memcpy(msg_chat.ToID, m_nID, ID_MAX);
	memcpy(msg_chat.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2 + 2);
	wcstombs(msg_chat.FromID, csMyID, csMyID.GetLength() + 1);
	wcstombs(msg_chat.Date, csDate, csDate.GetLength() + 1);
	wcstombs(msg_chat.Time, csTime, csTime.GetLength() + 1);

	/*
	memset(msg_chat.ToID, '\0', sizeof(msg_chat.ToID));
	memset(msg_chat.FromID, '\0', sizeof(msg_chat.FromID));
	memset(msg_chat.Date, '\0', sizeof(msg_chat.Date));
	memset(msg_chat.Time, '\0', sizeof(msg_chat.Time));
	memset(msg_chat.Data, '\0', sizeof(msg_chat.Data));
	
	CString csMyID; // �ҵ�ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // ����ʱ��
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	

	memcpy(msg_chat.ToID, m_csID, m_csID.GetLength() * 2);
	memcpy(msg_chat.FromID, csMyID, csMyID.GetLength() * 2);
	memcpy(msg_chat.Date, csDate, csDate.GetLength() * 2);
	memcpy(msg_chat.Time, csTime, csTime.GetLength() * 2);
	memcpy(msg_chat.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2);
	*/
	// ������Ϣ
	
	if(((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_chat, sizeof(msg_chat)))
	{
		// ���ͳɹ��� ��ʾ�ҷ�������
		AddMessage(_T("��"), csTime, m_csInputMsg);
		// �������������
		m_csInputMsg = "";
	}

	// ����Ϣ���µ���Ļ
	UpdateData(FALSE);
	
}


// �����Ϣ
int CChatDlg::AddMessage(const CString& csName, const CString& csTime, const CString& csMsg)
{	
	// �����������csName�Ǻ��ѵ�ID ��ID��������
	CString csTemp;
	csTemp = m_nID;
	if(csTemp == csName)	
		csTemp.Format(L"%s", m_Name);			
	else
		csTemp = csName;
	m_csOutputMsg += csTemp;
	m_csOutputMsg += ": ";
	m_csOutputMsg += csTime;
	m_csOutputMsg += "\r\n";
	m_csOutputMsg += csMsg;
	m_csOutputMsg += "\r\n";
	UpdateData(FALSE);
	return 0;
}

