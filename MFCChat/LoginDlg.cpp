

/************************************************
�� �� ����LoginDlg.cpp
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;����½���ڵ�ʵ���ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "RegistDlg.h"
#include "MFCChatDlg.h"


// CLoginDlg �Ի���

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

// ����ؼ���󶨱��������ݽ���
void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// �˺� �ַ�����ID_MAX	
	DDX_Text(pDX, IDC_EDIT_ID_LOGIN, m_csID);
	DDV_MaxChars(pDX, m_csID, ID_MAX - 1);

	// ���� �ַ�����KEY_MAX
	DDX_Text(pDX, IDC_EDIT_KEY_LOGIN, m_csKey);
	DDV_MaxChars(pDX, m_csKey, KEY_MAX - 1);

	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_BtnLogin);
	DDX_Control(pDX, IDC_BUTTON_REGIST_DLG, m_BtnRegist);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
	
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGIST_DLG, &CLoginDlg::OnBnClickedButtonRegDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

/*********************************************************
�������ƣ�OnBnClickedButtonLogin
�������������͵�¼��Ϣ
*********************************************************/
void CLoginDlg::OnBnClickedButtonLogin()
{
	//���±���������
	UpdateData(TRUE);
	
	// �ж��˺Ż������Ƿ�Ϊ��
	if(m_csID == "")
	{
		MessageBox(_T("�˺Ų���Ϊ��"));
		return;
	}
	if(m_csKey == "")
	{
		MessageBox(_T("���벻��Ϊ��"));
		return;
	}

	// ��Ҫ���͵Ľṹ��
	struct MSG_LOGIN msg_lg;

	msg_lg.nType = LOGIN; // ���ǵ�¼��Ϣ�ṹ��

	size_t i; // ��ת�����ַ���
	wcstombs_s(&i, msg_lg.nID, m_csID, m_csID.GetLength() + 1); // ���ַ�ת���ַ�
	wcstombs_s(&i, msg_lg.Key, m_csKey, m_csKey.GetLength() + 1); // ���ַ�ת���ַ�
	msg_lg.nStatus = IDS_STATUS_ONLINE;
	// ������Ϣ
	((CMFCChatDlg *)m_pParentWnd)->SendMsg(&msg_lg, sizeof(msg_lg));
	
}


/*********************************************************
�������ƣ�OnBnClickedButtonRegDlg
������������ע�ᴰ��
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
�������ƣ�OnSysCommand
��������������ϵͳ����
*********************************************************/
void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nID == SC_CLOSE)
	{
		if(((CMFCChatDlg*)m_pParentWnd)->m_csMyID != m_csID || m_csID == "")
		{// ��û�е�¼������¹رյ�¼����ͬʱ�������ڹص�
			((CMFCChatDlg*)m_pParentWnd)->OnCancel();
		}
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


/*********************************************************
�������ƣ�OnCancel
�����������ر�һ����ģ̬�Ի���ʱ����
��    ע����PostNcDestroy����
*********************************************************/
void CLoginDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	//((CMFCChatDlg*)theApp.m_pMainWnd)->m_pLoginDlg = NULL;
	//DestroyWindow();
	CDialogEx::OnCancel();
}

/*********************************************************
�������ƣ�PostNcDestroy
�����������ر�һ����ģ̬�Ի���ʱ����
*********************************************************/
void CLoginDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	//delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ���Ͻǹرհ�ť
	//CRect rcClient;
	//GetClientRect(&rcClient);	
	//m_BtnClose.SetImage(IDB_PNG_CLOSE, L"PNG");
	//CRect rcBtn;
	//m_BtnClose.GetClientRect(&rcBtn);
	//m_BtnClose.SetWindowPos(NULL, rcClient.Width() - rcBtn.Width(), 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	this->SetBackgroundColor(RGB(255, 255, 255)); // ���ñ�����ɫ

	return TRUE;  // return TRUE unless you set the focus to a control
	
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//CRect rcTop(rcClient.TopLeft(), CPoint(rcClient.Width(), rcClient.Height() * 0.4));	
	//CBrush BlueBrush(RGB(4, 142, 255));
	//dc.FillRect(&rcTop, &BlueBrush);


}
