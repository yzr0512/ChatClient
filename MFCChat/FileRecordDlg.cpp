// FileRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "FileRecordDlg.h"
#include "afxdialogex.h"


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
	m_BtnRadioSend.SetCheck(TRUE);
	m_BtnRadioRecv.SetCheck(FALSE);
	
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
	m_ListCtrlSend.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, int(nColInterval  * 0.5));
	m_ListCtrlSend.InsertColumn(1, _T("发送给"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlSend.InsertColumn(2, _T("时间"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrlRecv.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, int(nColInterval  * 0.5));
	m_ListCtrlRecv.InsertColumn(1, _T("来自"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	m_ListCtrlRecv.InsertColumn(2, _T("时间"), LVCFMT_CENTER, int(nColInterval  * 0.25));
	
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
	m_BtnRadioSend.SetCheck(FALSE);
	m_BtnRadioRecv.SetCheck(TRUE);
	m_ListCtrlSend.ShowWindow(SW_HIDE);
	m_ListCtrlRecv.ShowWindow(SW_SHOW);
}
