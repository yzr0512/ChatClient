// FileRecordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "FileRecordDlg.h"
#include "afxdialogex.h"


// CFileRecordDlg �Ի���

IMPLEMENT_DYNAMIC(CFileRecordDlg, CDialogEx)

CFileRecordDlg::CFileRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileRecordDlg::IDD, pParent)
{

}

CFileRecordDlg::~CFileRecordDlg()
{
}

void CFileRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SEND, m_BtnRadioSend);
	DDX_Control(pDX, IDC_RADIO_RECV, m_BtnRadioRecv);
	DDX_Control(pDX, IDC_LIST_RECV_RECORD, m_ListCtrlRecv);
	DDX_Control(pDX, IDC_LIST_SEND_RECORD, m_ListCtrlSend);
}


BEGIN_MESSAGE_MAP(CFileRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_SEND, &CFileRecordDlg::OnBnClickedRadioSend)
	ON_BN_CLICKED(IDC_RADIO_RECV, &CFileRecordDlg::OnBnClickedRadioRecv)
END_MESSAGE_MAP()


// CFileRecordDlg ��Ϣ�������

/*********************************************************
�������ƣ�OnInitDialog
������������ʼ������
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
BOOL CFileRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������һ����ѡ��Ϊѡ��״̬
	m_BtnRadioSend.SetCheck(TRUE);
	m_BtnRadioRecv.SetCheck(FALSE);
	
	// ��ʼ���б�ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	CWnd* pWnd;
	pWnd = this->GetDlgItem(IDC_RADIO_SEND);
	CRect rcBtn; // ��ѡ��ť�ľ���
	pWnd->GetClientRect(&rcBtn);
	// �������������б�ؼ���λ�úʹ�С
	m_ListCtrlSend.SetWindowPos(NULL, rcBtn.Width() + 3, 3, rcClient.Width() - rcBtn.Width() - 6, rcClient.Height() - 6, SWP_NOZORDER);
	m_ListCtrlRecv.SetWindowPos(NULL, rcBtn.Width() + 3, 3, rcClient.Width() - rcBtn.Width() - 6, rcClient.Height() - 6, SWP_NOZORDER);
	CRect rcList; // �б�ؼ��ľ��δ�С
	m_ListCtrlSend.GetClientRect(&rcList);
	int nColInterval = rcList.Width();
	// �����б�ķ��ͱ�ͷ
	m_ListCtrlSend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlSend.InsertColumn(0, _T("�ļ���"), LVCFMT_CENTER, int(nColInterval  * 0.5));
	m_ListCtrlSend.InsertColumn(1, _T("���͸�"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlSend.InsertColumn(2, _T("ʱ��"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlRecv.InsertColumn(0, _T("�ļ���"), LVCFMT_CENTER, int(nColInterval  * 0.5));
	m_ListCtrlRecv.InsertColumn(1, _T("����"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.InsertColumn(2, _T("ʱ��"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	
	this->SetBackgroundColor(RGB(237, 238, 238));// ���ô��ڱ�����ɫ

	return TRUE;  // return TRUE unless you set the focus to a control

}

/*********************************************************
�������ƣ�OnBnClickedRadioSend
����������һ����ѡ���� ����һ������ ��������ʾ���б�
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
void CFileRecordDlg::OnBnClickedRadioSend()
{
	m_BtnRadioSend.SetCheck(TRUE);
	m_BtnRadioRecv.SetCheck(FALSE);
	m_ListCtrlSend.ShowWindow(SW_SHOW);
	m_ListCtrlRecv.ShowWindow(SW_HIDE);
}

/*********************************************************
�������ƣ�OnBnClickedRadioRecv
����������һ����ѡ���� ����һ������ ��������ʾ���б�
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
void CFileRecordDlg::OnBnClickedRadioRecv()
{
	m_BtnRadioSend.SetCheck(FALSE);
	m_BtnRadioRecv.SetCheck(TRUE);
	m_ListCtrlSend.ShowWindow(SW_HIDE);
	m_ListCtrlRecv.ShowWindow(SW_SHOW);
}
