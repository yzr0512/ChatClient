
/************************************************
File name：AddFriendDlg.cpp
Create by：余志荣
CreateDate：2016-08-27
Use：“添加好友”对话框的实现文件
Change by: 2016-08-27 by 余志荣 创建
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
	// TODO: 在此添加专用代码和/或调用基类

	//((CMFCChatDlg*)this->GetParent())->m_pAddFriendDlg = NULL;
	((CMFCChatDlg*)theApp.m_pMainWnd)->m_pAddFriendDlg = NULL;

	DestroyWindow();
	//CDialogEx::OnCancel();
}
void CAddFriendDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	// 删除当前对象
	delete this;

	CDialogEx::PostNcDestroy();
}


/*********************************************************
函数名称：OnInitDialog
功能描述：初始化对话框
作者：    余志荣
创建时间：2016-08-27
返 回 值：成功返回TRUE
*********************************************************/
BOOL CAddFriendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetBackgroundColor(RGB(255, 255, 255)); // 设置背景颜色
	InitListCtrlStyle();

	//RefreshListCtrlData();  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


/*********************************************************
函数名称：InitListCtrlStyle
功能描述：初始化列表控件
作者：    余志荣
创建时间：2016-08-27
返 回 值：无意义
*********************************************************/
int CAddFriendDlg::InitListCtrlStyle(void)
{
	// **获取列表控件的宽度
	CRect rect;
	m_listctrlFriend.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// **设置ListCtrl的样式
	//  LVS_EX_GRIDLINES 网格线
	//  LVS_EX_FULLROWSELECT 整行选中
	m_listctrlFriend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//m_lstFindFriend.SetExtendedStyle(m_lstFindFriend.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	// **插入表头
	// 参数分别为：列索引（第几列）、列名、列的文字格式、列宽、与列相关联子项的索引
	//  LVCFMT_CENTER 居中
	//  LVCFMT_LEFT   左对齐
	//  LVCFMT_RIGHT  右对齐
	m_listctrlFriend.InsertColumn(0, _T("账号"), LVCFMT_CENTER, int(nColInterval / 5));
	m_listctrlFriend.InsertColumn(1, _T("昵称"), LVCFMT_CENTER, int(nColInterval / 5)); 
	m_listctrlFriend.InsertColumn(2, _T("邮箱"), LVCFMT_CENTER, int(nColInterval / 5)); 
    m_listctrlFriend.InsertColumn(3, _T("性别"), LVCFMT_CENTER, int(nColInterval / 5));   
	m_listctrlFriend.InsertColumn(4, _T("状态"), LVCFMT_CENTER, int(nColInterval / 5));   
	return 0;
}


/*********************************************************
函数名称：ShowStrangerInfo
功能描述：输出陌生人信息到列表控件中
作者：    余志荣
创建时间：2016-08-27
返 回 值：无意义
*********************************************************/
int CAddFriendDlg::ShowStrangerInfo(MSG_USERINFO* msg_info)
{
	if(msg_info == NULL)
	{
		m_listctrlFriend.DeleteAllItems();
		//m_listctrlFriend.InsertItem(0, _T("请输入对方ID后按查找"));   	
	}
	CString csTemp;
	
	csTemp = msg_info->nID;
	m_listctrlFriend.InsertItem(0, csTemp);
	
	csTemp = (LPCTSTR)msg_info->Name;
	m_listctrlFriend.SetItemText(0, 1, csTemp);

	csTemp = msg_info->Email;
	m_listctrlFriend.SetItemText(0, 2, csTemp);
	
	if(msg_info->Sex == 'm')
		m_listctrlFriend.SetItemText(0, 3, _T("男"));
	else
		m_listctrlFriend.SetItemText(0, 3, _T("女"));

	csTemp.LoadStringW(msg_info->nStatus);
	m_listctrlFriend.SetItemText(0, 4, csTemp);

	return 0;
}


/*********************************************************
函数名称：OnBnClickedButtonFind
功能描述：向服务器发送查找信息
作者：    余志荣
创建时间：2016-08-27
*********************************************************/
void CAddFriendDlg::OnBnClickedButtonFind()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	struct MSG_USERINFO msg_info;
	msg_info.nType = GET_STRANGER_INFO;
	size_t i;
	wcstombs_s(&i, msg_info.nID, m_csFriendID, m_csFriendID.GetLength() + 1);
	((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_info, sizeof(msg_info));	
	m_listctrlFriend.DeleteAllItems(); // 清空列表信息
}


/*********************************************************
函数名称：OnBnClickedButtonAddFriend
功能描述：发送添加好友请求
作者：    余志荣
创建时间：2016-08-27
*********************************************************/
void CAddFriendDlg::OnBnClickedButtonAddFriend()
{
	// TODO: 在此添加控件通知处理程序代码
	//此处方法不知可不可以 需要测试
	POSITION pos = m_listctrlFriend.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox(_T("请选中一个用户！"));
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


