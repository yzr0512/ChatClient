

/************************************************
�� �� ����FileRecordDlg.cpp
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;���ļ���¼���ڵ�ʵ���ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

// FileRecordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "FileRecordDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

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
	ON_WM_TIMER()
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
	OnBnClickedRadioSend();
	
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
	m_ListCtrlSend.InsertColumn(0, _T(""), LVCFMT_CENTER, int(nColInterval  * 0.05));
	m_ListCtrlSend.InsertColumn(1, _T("�ļ���"), LVCFMT_CENTER, int(nColInterval  * 0.3));
	m_ListCtrlSend.InsertColumn(2, _T("���͸�"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlSend.InsertColumn(3, _T("״̬"), LVCFMT_CENTER, int(nColInterval  * 0.15));
	m_ListCtrlSend.InsertColumn(4, _T("��С"), LVCFMT_CENTER, int(nColInterval  * 0.2));

	m_ListCtrlRecv.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlRecv.InsertColumn(0, _T(""), LVCFMT_CENTER, int(nColInterval  * 0.05));
	m_ListCtrlRecv.InsertColumn(1, _T("�ļ���"), LVCFMT_CENTER, int(nColInterval  * 0.3));
	m_ListCtrlRecv.InsertColumn(2, _T("����"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.InsertColumn(3, _T("״̬"), LVCFMT_CENTER, int(nColInterval  * 0.15));
	m_ListCtrlRecv.InsertColumn(4, _T("��С"), LVCFMT_CENTER, int(nColInterval  * 0.2));
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
	m_bIsSendList = true;
	RefreshListCtrlData();
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
	m_bIsSendList = false;
	RefreshListCtrlData();
	m_BtnRadioSend.SetCheck(FALSE);
	m_BtnRadioRecv.SetCheck(TRUE);
	m_ListCtrlSend.ShowWindow(SW_HIDE);
	m_ListCtrlRecv.ShowWindow(SW_SHOW);
}

/*********************************************************
�������ƣ�RefreshListCtrlData
����������ˢ���б�ؼ����������
���ߣ�	  ��־��
����ʱ�䣺2016-08-31
����˵����
�� �� ֵ��
*********************************************************/
int CFileRecordDlg::RefreshListCtrlData(void)
{
	if (m_bIsSendList)
	{
		m_ListCtrlSend.DeleteAllItems();
	} 
	else
	{
		m_ListCtrlRecv.DeleteAllItems();
	}
	int nSend = 0, nRecv = 0; // ��Ŀ���
	POSITION pos = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetHeadPosition();
	while (pos != NULL)
	{
		FILE_RECORD *pRecord = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetAt(pos);
		((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetNext(pos); // pos������һ��	
		if (pRecord->bType == true)
		{// ���ͼ�¼
			if (!m_bIsSendList)
			{// ��ǰ���Ƿ����б� ֱ������
				continue;
			}
			// ���
			CString strItem;
			strItem.Format(_T("%d"),  nSend + 1);
			m_ListCtrlSend.InsertItem(nSend, strItem); 
			// �ļ���
			CString strName;
			strName = pRecord->FileName;
			m_ListCtrlSend.SetItemText(nSend, 1, strName);

			// ������
			CString strID;
			strID = pRecord->ToID;
			m_ListCtrlSend.SetItemText(nSend, 2, strID);
			// ״̬
			CString strState;
			double dbCent; // �ٷ���
			switch(pRecord->nTransState)
			{
			case 0:
				strState = "�ȴ�����";
				break;
			case 1:
				dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // ���͵İٷֱ�
				strState.Format(L"%.2lf%%", dbCent);
				break;
			case 2:
				strState = "��ͣ";
				break;
			case 3:
				strState = "�����";
				break;
			case 4:
				strState = "ȡ��";
				break;
			case 5:
				strState = "����";
				break;
			case 6:
				strState = "�ܾ�";
				break;
			default:
				strState = "what?";
				break;
			}
			m_ListCtrlSend.SetItemText(nSend, 3, strState);
			
			// �ļ���С
			CString strFileSize;
			if(pRecord->nFileSize >= 1073741824) // ����1GB
			{
				strFileSize.Format(L" %.2lf GB", pRecord->nFileSize / 1073741824.0);
			}
			else if(pRecord->nFileSize >= 1048576) // С��1GB����1MB
			{
				strFileSize.Format(L" %.2lf MB", pRecord->nFileSize / 1048576.0);
			}
			else
			{
				strFileSize.Format(L" %.2lf KB", pRecord->nFileSize / 1024.0);
			}
			m_ListCtrlSend.SetItemText(nSend, 4, strFileSize);

			++nSend;
		}
		else
		{// ���ռ�¼
			if (m_bIsSendList)
			{// ��ǰ���ǽ����б� ֱ������
				continue;
			}
			// ���
			CString strItem;
			strItem.Format(_T("%d"),  nRecv + 1);
			m_ListCtrlRecv.InsertItem(nRecv, strItem); 
			// �ļ���
			CString strName;
			strName = pRecord->FileName;
			m_ListCtrlRecv.SetItemText(nRecv, 1, strName);
			// ������
			CString strID;
			strID = pRecord->FromID;
			m_ListCtrlRecv.SetItemText(nRecv, 2, strID);
			// ״̬
			CString strState;
			double dbCent; // �ٷ���
			switch(pRecord->nTransState)
			{
			case 0:
				strState = "�ȴ�����";
				break;
			case 1:
				dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // ���͵İٷֱ�
				strState.Format(L"%.2lf%%", dbCent);
				break;
			case 2:
				strState = "��ͣ";
				break;
			case 3:
				strState = "�����";
				break;
			case 4:
				strState = "ȡ��";
				break;
			case 5:
				strState = "����";
				break;
			case 6:
				strState = "�ܾ�";
				break;
			default:
				strState = "what?";
				break;
			}
			m_ListCtrlRecv.SetItemText(nRecv, 3, strState);

			CString strFileSize;
			if(pRecord->nFileSize >= 1073741824) // ����1GB
			{
				strFileSize.Format(L" %.2lf GB", pRecord->nFileSize / 1073741824.0);
			}
			else if(pRecord->nFileSize >= 1048576) // С��1GB����1MB
			{
				strFileSize.Format(L" %.2lf MB", pRecord->nFileSize / 1048576.0);
			}
			else
			{
				strFileSize.Format(L" %.2lf KB", pRecord->nFileSize / 1024.0);
			}
			m_ListCtrlRecv.SetItemText(nRecv, 4, strFileSize);
			++nRecv;
		}
	}
	return 0;
}


void CFileRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	// ��ʱˢ���б�ؼ������� ��ʱ���������ڿ���
	RefreshListCtrlData();

	CDialogEx::OnTimer(nIDEvent);
}
