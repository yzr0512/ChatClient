
/************************************************
File name��AddFriendDlg.cpp
Create by����־��
CreateDate��2016-08-27
Use������Ӻ��ѡ��Ի����ʵ���ļ�
Change by: 2016-08-27 by ��־�� ����
************************************************/ 

// AddFriendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCChat.h"
#include "AddFriendDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"


// CAddFriendDlg dialog

IMPLEMENT_DYNAMIC(CAddFriendDlg, CDialogEx)

CAddFriendDlg::CAddFriendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddFriendDlg::IDD, pParent), 
	m_pParentWnd(pParent)
{

	m_csFriendID = _T("");
}

CAddFriendDlg::~CAddFriendDlg()
{
}

void CAddFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIND_FRIEND, m_listctrlFriend);
	DDX_Text(pDX, IDC_EDIT_FRIEND_NAME, m_csFriendID);
	// UI	
	DDX_Control(pDX, IDC_BUTTON_ADD_FRIEND, m_BtnAddFrnd);

	DDX_Control(pDX, IDC_BUTTON_FIND, m_BtnFind);
}


BEGIN_MESSAGE_MAP(CAddFriendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CAddFriendDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FRIEND, &CAddFriendDlg::OnBnClickedButtonAddFriend)
END_MESSAGE_MAP()


// CAddFriendDlg message handlers



void CAddFriendDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//((CMFCChatDlg*)this->GetParent())->m_pAddFriendDlg = NULL;
	((CMFCChatDlg*)theApp.m_pMainWnd)->m_pAddFriendDlg = NULL;

	DestroyWindow();
	//CDialogEx::OnCancel();
}
void CAddFriendDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	// ɾ����ǰ����
	delete this;

	CDialogEx::PostNcDestroy();
}


/*********************************************************
�������ƣ�OnInitDialog
������������ʼ���Ի���
���ߣ�    ��־��
����ʱ�䣺2016-08-27
�� �� ֵ���ɹ�����TRUE
*********************************************************/
BOOL CAddFriendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetBackgroundColor(RGB(255, 255, 255)); // ���ñ�����ɫ
	InitListCtrlStyle();

	//RefreshListCtrlData();  

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


/*********************************************************
�������ƣ�InitListCtrlStyle
������������ʼ���б�ؼ�
���ߣ�    ��־��
����ʱ�䣺2016-08-27
�� �� ֵ��������
*********************************************************/
int CAddFriendDlg::InitListCtrlStyle(void)
{
	// **��ȡ�б�ؼ��Ŀ��
	CRect rect;
	m_listctrlFriend.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// **����ListCtrl����ʽ
	//  LVS_EX_GRIDLINES ������
	//  LVS_EX_FULLROWSELECT ����ѡ��
	m_listctrlFriend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//m_lstFindFriend.SetExtendedStyle(m_lstFindFriend.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	// **�����ͷ
	// �����ֱ�Ϊ�����������ڼ��У����������е����ָ�ʽ���п�������������������
	//  LVCFMT_CENTER ����
	//  LVCFMT_LEFT   �����
	//  LVCFMT_RIGHT  �Ҷ���
	m_listctrlFriend.InsertColumn(0, _T("�˺�"), LVCFMT_CENTER, int(nColInterval / 5));
	m_listctrlFriend.InsertColumn(1, _T("�ǳ�"), LVCFMT_CENTER, int(nColInterval / 5)); 
	m_listctrlFriend.InsertColumn(2, _T("����"), LVCFMT_CENTER, int(nColInterval / 5)); 
    m_listctrlFriend.InsertColumn(3, _T("�Ա�"), LVCFMT_CENTER, int(nColInterval / 5));   
	m_listctrlFriend.InsertColumn(4, _T("״̬"), LVCFMT_CENTER, int(nColInterval / 5));   
	return 0;
}


/*********************************************************
�������ƣ�ShowStrangerInfo
�������������İ������Ϣ���б�ؼ���
���ߣ�    ��־��
����ʱ�䣺2016-08-27
�� �� ֵ��������
*********************************************************/
int CAddFriendDlg::ShowStrangerInfo(MSG_USERINFO* msg_info)
{
	if(msg_info == NULL)
	{
		m_listctrlFriend.DeleteAllItems();
		//m_listctrlFriend.InsertItem(0, _T("������Է�ID�󰴲���"));   	
	}
	CString csTemp;
	
	csTemp = msg_info->nID;
	m_listctrlFriend.InsertItem(0, csTemp);
	
	csTemp = (LPCTSTR)msg_info->Name;
	m_listctrlFriend.SetItemText(0, 1, csTemp);

	csTemp = msg_info->Email;
	m_listctrlFriend.SetItemText(0, 2, csTemp);
	
	if(msg_info->Sex == 'm')
		m_listctrlFriend.SetItemText(0, 3, _T("��"));
	else
		m_listctrlFriend.SetItemText(0, 3, _T("Ů"));

	csTemp.LoadStringW(msg_info->nStatus);
	m_listctrlFriend.SetItemText(0, 4, csTemp);

	return 0;
}


/*********************************************************
�������ƣ�OnBnClickedButtonFind
��������������������Ͳ�����Ϣ
���ߣ�    ��־��
����ʱ�䣺2016-08-27
*********************************************************/
void CAddFriendDlg::OnBnClickedButtonFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	struct MSG_USERINFO msg_info;
	msg_info.nType = GET_STRANGER_INFO;
	size_t i;
	wcstombs_s(&i, msg_info.nID, m_csFriendID, m_csFriendID.GetLength() + 1);
	((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_info, sizeof(msg_info));	
	m_listctrlFriend.DeleteAllItems(); // ����б���Ϣ
}


/*********************************************************
�������ƣ�OnBnClickedButtonAddFriend
����������������Ӻ�������
���ߣ�    ��־��
����ʱ�䣺2016-08-27
*********************************************************/
void CAddFriendDlg::OnBnClickedButtonAddFriend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�˴�������֪�ɲ����� ��Ҫ����
	POSITION pos = m_listctrlFriend.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox(_T("��ѡ��һ���û���"));
		return;
	}

	int nRow = m_listctrlFriend.GetNextSelectedItem(pos);
	CString csToID, csMyID;
	csToID = m_listctrlFriend.GetItemText(nRow, 0);
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;
	struct MSG_TRANSPOND msg_add;
	msg_add.nType = ADD_FRIEND_REQUEST;
	
	size_t i;
	wcstombs_s(&i, msg_add.ToID, csToID, csToID.GetLength() + 1);
	wcstombs_s(&i, msg_add.FromID, csMyID, csMyID.GetLength() + 1);
	CTime tm = CTime::GetCurrentTime();
	CString csDate = tm.Format("%Y%m%d");
	wcstombs_s(&i, msg_add.Date, csDate, csDate.GetLength() + 1);
	CString csTime = tm.Format("%H%M%S");
	wcstombs_s(&i, msg_add.Time, csTime, csTime.GetLength() + 1);

	memset(msg_add.Data, 0, 200);
	msg_add.nReturn = 0;

	((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_add, sizeof(msg_add));
}


