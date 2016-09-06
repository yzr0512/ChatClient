

/************************************************
文 件 名：FileRecordDlg.cpp
作    者：余志荣
创建日期：2016-08-29
用    途：文件记录窗口的实现文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

// FileRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "FileRecordDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

// CFileRecordDlg 对话框

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


// CFileRecordDlg 消息处理程序

/*********************************************************
函数名称：OnInitDialog
功能描述：初始化窗口
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
*********************************************************/
BOOL CFileRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置其中一个单选框为选中状态
	OnBnClickedRadioSend();
	
	// 初始化列表控件
	CRect rcClient;
	GetClientRect(&rcClient);
	CWnd* pWnd;
	pWnd = this->GetDlgItem(IDC_RADIO_SEND);
	CRect rcBtn; // 单选按钮的矩形
	pWnd->GetClientRect(&rcBtn);
	// 重新设置两个列表控件的位置和大小
	m_ListCtrlSend.SetWindowPos(NULL, rcBtn.Width() + 3, 3, rcClient.Width() - rcBtn.Width() - 6, rcClient.Height() - 6, SWP_NOZORDER);
	m_ListCtrlRecv.SetWindowPos(NULL, rcBtn.Width() + 3, 3, rcClient.Width() - rcBtn.Width() - 6, rcClient.Height() - 6, SWP_NOZORDER);
	CRect rcList; // 列表控件的矩形大小
	m_ListCtrlSend.GetClientRect(&rcList);
	int nColInterval = rcList.Width();
	// 设置列表的风格和表头
	m_ListCtrlSend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlSend.InsertColumn(0, _T(""), LVCFMT_CENTER, int(nColInterval  * 0.05));
	m_ListCtrlSend.InsertColumn(1, _T("文件名"), LVCFMT_CENTER, int(nColInterval  * 0.3));
	m_ListCtrlSend.InsertColumn(2, _T("发送给"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlSend.InsertColumn(3, _T("状态"), LVCFMT_CENTER, int(nColInterval  * 0.15));
	m_ListCtrlSend.InsertColumn(4, _T("大小"), LVCFMT_CENTER, int(nColInterval  * 0.2));

	m_ListCtrlRecv.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlRecv.InsertColumn(0, _T(""), LVCFMT_CENTER, int(nColInterval  * 0.05));
	m_ListCtrlRecv.InsertColumn(1, _T("文件名"), LVCFMT_CENTER, int(nColInterval  * 0.3));
	m_ListCtrlRecv.InsertColumn(2, _T("来自"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.InsertColumn(3, _T("状态"), LVCFMT_CENTER, int(nColInterval  * 0.15));
	m_ListCtrlRecv.InsertColumn(4, _T("大小"), LVCFMT_CENTER, int(nColInterval  * 0.2));
	this->SetBackgroundColor(RGB(237, 238, 238));// 设置窗口背景颜色

	return TRUE;  // return TRUE unless you set the focus to a control

}

/*********************************************************
函数名称：OnBnClickedRadioSend
功能描述：一个单选框按下 将另一个弹起 并调整显示的列表
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
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
函数名称：OnBnClickedRadioRecv
功能描述：一个单选框按下 将另一个弹起 并调整显示的列表
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
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
函数名称：RefreshListCtrlData
功能描述：刷新列表控件里面的内容
作者：	  余志荣
创建时间：2016-08-31
参数说明：
返 回 值：
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
	int nSend = 0, nRecv = 0; // 项目序号
	POSITION pos = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetHeadPosition();
	while (pos != NULL)
	{
		FILE_RECORD *pRecord = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetAt(pos);
		((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetNext(pos); // pos跳到下一个	
		if (pRecord->bType == true)
		{// 发送记录
			if (!m_bIsSendList)
			{// 当前不是发送列表 直接跳过
				continue;
			}
			// 序号
			CString strItem;
			strItem.Format(_T("%d"),  nSend + 1);
			m_ListCtrlSend.InsertItem(nSend, strItem); 
			// 文件名
			CString strName;
			strName = pRecord->FileName;
			m_ListCtrlSend.SetItemText(nSend, 1, strName);

			// 接收者
			CString strID;
			strID = pRecord->ToID;
			m_ListCtrlSend.SetItemText(nSend, 2, strID);
			// 状态
			CString strState;
			double dbCent; // 百分数
			switch(pRecord->nTransState)
			{
			case 0:
				strState = "等待接收";
				break;
			case 1:
				dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // 发送的百分比
				strState.Format(L"%.2lf%%", dbCent);
				break;
			case 2:
				strState = "暂停";
				break;
			case 3:
				strState = "已完成";
				break;
			case 4:
				strState = "取消";
				break;
			case 5:
				strState = "出错";
				break;
			case 6:
				strState = "拒绝";
				break;
			default:
				strState = "what?";
				break;
			}
			m_ListCtrlSend.SetItemText(nSend, 3, strState);
			
			// 文件大小
			CString strFileSize;
			if(pRecord->nFileSize >= 1073741824) // 大于1GB
			{
				strFileSize.Format(L" %.2lf GB", pRecord->nFileSize / 1073741824.0);
			}
			else if(pRecord->nFileSize >= 1048576) // 小于1GB大于1MB
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
		{// 接收记录
			if (m_bIsSendList)
			{// 当前不是接收列表 直接跳过
				continue;
			}
			// 序号
			CString strItem;
			strItem.Format(_T("%d"),  nRecv + 1);
			m_ListCtrlRecv.InsertItem(nRecv, strItem); 
			// 文件名
			CString strName;
			strName = pRecord->FileName;
			m_ListCtrlRecv.SetItemText(nRecv, 1, strName);
			// 发送者
			CString strID;
			strID = pRecord->FromID;
			m_ListCtrlRecv.SetItemText(nRecv, 2, strID);
			// 状态
			CString strState;
			double dbCent; // 百分数
			switch(pRecord->nTransState)
			{
			case 0:
				strState = "等待接收";
				break;
			case 1:
				dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // 发送的百分比
				strState.Format(L"%.2lf%%", dbCent);
				break;
			case 2:
				strState = "暂停";
				break;
			case 3:
				strState = "已完成";
				break;
			case 4:
				strState = "取消";
				break;
			case 5:
				strState = "出错";
				break;
			case 6:
				strState = "拒绝";
				break;
			default:
				strState = "what?";
				break;
			}
			m_ListCtrlRecv.SetItemText(nRecv, 3, strState);

			CString strFileSize;
			if(pRecord->nFileSize >= 1073741824) // 大于1GB
			{
				strFileSize.Format(L" %.2lf GB", pRecord->nFileSize / 1073741824.0);
			}
			else if(pRecord->nFileSize >= 1048576) // 小于1GB大于1MB
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
	// 定时刷新列表控件的内容 定时器在主窗口开启
	RefreshListCtrlData();

	CDialogEx::OnTimer(nIDEvent);
}
