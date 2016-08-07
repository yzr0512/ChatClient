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
	: CDialogEx(CChatDlg::IDD, NULL)
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
	SetWindowText(_T("Chatting with ") + m_csID);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// �ر����촰��ʱͬʱɾ���������е�Ԫ��
// ��Ҫ����OnCancel()��PostNcDestroy()
void CChatDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	// ������ڵķ���
	// ��������ɾ�������е�Ԫ��
	std::vector<CChatDlg*>& vecpChatDlg = ((CMFCChatDlg*)this->GetParent())->m_vecpChatDlg;
	
	std::vector<CChatDlg*>::iterator it = vecpChatDlg.begin();


	for(; it != vecpChatDlg.end(); ++it)
	{
		CChatDlg* pChatDlg = *it;

		if(pChatDlg->m_csID == m_csID)
		{
			// ����������itָ���Ԫ��
			vecpChatDlg.erase(it);	
			
			break;
		}
	}
	// ɾ������
	DestroyWindow();

	////�˷���ֻ�ܴ򿪵������촰��
	//// �Ѹ������е�m_pChatDlg��NULL
	//((CMFCChatDlg*)this->GetParent())->m_pChatDlg = NULL;
	//
	//// ɾ������
	//DestroyWindow();

}
void CChatDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ɾ����ǰ����
	delete this;

	CDialogEx::PostNcDestroy();
}


void CChatDlg::OnBnClickedButtonSendMsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_csInputMsg == "") // �༭����δ��������
	{
		// ������ʾ���༭������ݲ���Ϊ��
		// 
	}
	
	struct MSG_TRANSPOND chatmsg;
	chatmsg.nType = CHATING_TEXT_MSG;
	
	memset(chatmsg.ToID, '\0', sizeof(chatmsg.ToID));
	memset(chatmsg.FromID, '\0', sizeof(chatmsg.FromID));
	memset(chatmsg.Date, '\0', sizeof(chatmsg.Date));
	memset(chatmsg.Time, '\0', sizeof(chatmsg.Time));
	memset(chatmsg.Data, '\0', sizeof(chatmsg.Data));
	
	CString csMyID; // �ҵ�ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // ����ʱ��
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	

	memcpy(chatmsg.ToID, m_csID, m_csID.GetLength() * 2);
	memcpy(chatmsg.FromID, csMyID, csMyID.GetLength() * 2);
	memcpy(chatmsg.Date, csDate, csDate.GetLength() * 2);
	memcpy(chatmsg.Time, csTime, csTime.GetLength() * 2);
	memcpy(chatmsg.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2);
	
	// ������Ϣ
	
	if(((CMFCChatDlg*)m_pParentWnd)->SendMsg(&chatmsg, sizeof(chatmsg)))
	{
		// ���ͳɹ��� ��ʾ�ҷ�������
		AddMessage(_T("�ң� "), csTime, m_csInputMsg);
		// �������������
		m_csInputMsg = "";
	}

	// ����Ϣ���µ���Ļ
	UpdateData(FALSE);
	
}

// �����Ϣ
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

