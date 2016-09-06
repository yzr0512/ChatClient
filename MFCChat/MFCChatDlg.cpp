




// MFCChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "MFCChatDlg.h"
#include "afxdialogex.h"
#include "RegistDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_HEARTBEAT 123
#define TIMER_UPDATE_FRIEND_INFO 456
#define TIMER_UPDATE_FILE_DLG 666

#define SERVER_IP   "127.0.0.1"		// 服务器IP
#define SERVER_PORT 8080			// 传文件用的端口号在此基础上+1

//定义一个全局变量，用于控制线程是否退出
volatile BOOL IsRun = FALSE;
UINT AFX_CDECL SendFile(LPVOID lpParam);

// CMFCChatDlg 对话框

CMFCChatDlg::CMFCChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCChatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_csMyName = _T("");

	m_pAddFriendDlg = NULL;
	m_pLoginDlg = NULL;
	m_pFileRcrdDlg = NULL;

	m_csMyID = _T("");

	m_pSocketChat = NULL;
	m_pSocketFileTrans = NULL;

	m_FileTransThread = NULL;
}

void CMFCChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_lstctlFriend);
	DDX_Control(pDX, IDC_COMBO_STATE, m_cbState);
	DDX_Text(pDX, IDC_STATIC_USERNAME, m_csMyName);
	DDV_MaxChars(pDX, m_csMyName, 20);
	DDX_Control(pDX, IDC_BUTTON_ADD_FRIEND_DLG, m_BtnAddFriend);
	DDX_Control(pDX, IDC_BT_DEL_FRIEND, m_BtnDelFriend);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_BT_FILE_DLG, m_BtnFileDlg);
}

BEGIN_MESSAGE_MAP(CMFCChatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIEND, &CMFCChatDlg::OnNMDblclkListFriend)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FRIEND_DLG, &CMFCChatDlg::OnBnClickedButtonAddFriendDlg)
	ON_BN_CLICKED(IDC_BT_DEL_FRIEND, &CMFCChatDlg::OnBnClickedBtDelFriend)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BT_FILE_DLG, &CMFCChatDlg::OnBnClickedBtFileDlg)
ON_NOTIFY(NM_CLICK, IDC_LIST_FRIEND, &CMFCChatDlg::OnNMClickListFriend)
END_MESSAGE_MAP()


// CMFCChatDlg 消息处理程序

BOOL CMFCChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 打开登录窗口
	if(m_pLoginDlg == NULL)
	{
		m_pLoginDlg = new CLoginDlg(this);	
		m_pLoginDlg->Create(IDD_LOGIN_DLG, GetDesktopWindow());
		m_pLoginDlg->CenterWindow(); // 登陆窗口居中显示
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
	//ShowWindow(SW_SHOW);
	CenterWindow(); // 主窗口居中
	this->SetBackgroundColor(RGB(255, 255, 255)); // 设置背景颜色

	UpdateData(TRUE);
	
	// 显示用户名
	//m_csMyName = m_dlgLogin.m_csID;
	
	// 更新好友列表的数据
	RefreshListCtrlData();

	// 设置在线状态
	//m_cbState.SelectString(0, _T("在线"));
		
	UpdateData(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCChatDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 对话框关闭时调用下面两个函数
void CMFCChatDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pSocketChat != NULL)
	{
		if(m_csMyID != "")
		{
			LoginOut();
		}
	}
	
	DestroyWindow();
	//CDialogEx::OnCancel();
}
void CMFCChatDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


/*********************************************************
函数名称：SocketInit
功能描述：初始化Socket
返 回 值：成功返回TRUE
备    注：
*********************************************************/
bool CMFCChatDlg::SocketInit(void)
{
	if(m_pSocketChat != NULL)
	{
		return TRUE;
	}

	m_pSocketChat = new CChatSocket(this);

	if(m_pSocketChat == NULL)
	{
		//套接字初始化失败
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("套接字初始化失败！"));
		return FALSE;
	}

	if(!m_pSocketChat->Create())
	{
		//创建失败
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("套接字创建失败！"));
		return FALSE;
	}
	
	if(m_pSocketChat->Connect(_T(SERVER_IP), SERVER_PORT) == 0)
	{
		//连接服务器失败
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("连接服务器失败！"));
		return FALSE;
	}
	return TRUE;
}

/*********************************************************
函数名称：CloseChatSocket
功能描述：关闭Socket
返 回 值：成功返回TRUE
备    注：
*********************************************************/
void CMFCChatDlg::CloseChatSocket(void)
{
	//delete m_pSocketChat;
	m_pSocketChat = NULL;
	MessageBox(L"与服务器连接已断开，请重新登陆");
	OnCancel(); // 关闭程序
	//return TRUE;
}

/*********************************************************
函数名称：RecvMsg
功能描述：接收消息
创建时间：2016-08-03
备    注：
*********************************************************/
int CMFCChatDlg::RecvMsg(void)
{
	int nBugLen = 4096;
	struct MSG_T *msg_recv = new struct MSG_T; // 接收到的数据
	int nRes; // 接收到的字节数
	nRes = m_pSocketChat->Receive((char*)msg_recv, nBugLen);

	if(nRes == 0) // 没有接收到数据
	{
		return 0;
	}
	
	switch(msg_recv->nType)
	{
	case LOGIN_SUCCESS: // 登录成功
		LoginSuccess((struct MSG_USERINFO*)msg_recv);
		break;	

	case HEARTBEAT:
		break;

	case REGISTER_SUCCESS: // 注册成功
		//m_dlgLogin.m_dlgRegist.RegisterSuccess((struct MSG_REGISTER*)&msg_recv);
		m_pLoginDlg->m_pRegDlg->RegisterSuccess((struct MSG_REGISTER*)msg_recv);
		break;
	case REGISTER_FAILED: // 注册失败
		/*CString csMsg;
		csMsg.Format(_T("%s"),msg_recv.Data);		
		MessageBox(csMsg);*/
		break;
	case IDS_SYSTEM_MESSAGE: // 系统消息
		SystemMessage((struct MSG_SYS*)msg_recv);
		break;
	case GET_STRANGER_INFO:
		m_pAddFriendDlg->ShowStrangerInfo((struct MSG_USERINFO *)msg_recv);
		break;
	case ADD_FRIEND_REQUEST:
		RecvAddFriendRequest((struct MSG_TRANSPOND*)msg_recv);
		break;
	case GET_ALL_FRIEND_INFO:
		UpdateFriendInfo((MSG_FRND_INFO*)msg_recv);
		break;

		// 一对一聊天消息
	case CHATING_TEXT_MSG:
		struct MSG_TRANSPOND *chatmsg;
		chatmsg = (MSG_TRANSPOND *)msg_recv;
		m_listChatMsg.AddTail(chatmsg);	
		m_Friend.SetIsHaveMsg(chatmsg->FromID, TRUE);
		RefreshChatDlgMsg();
		//RefreshListCtrlData();
		break;
	case GET_FRIEND_INFO: // 一个人的详细信息
		UpdateOneFriendInfo((MSG_USERINFO*)msg_recv);
		break;


		// 文件传输请求
	case MESSAGE_FILE_REQUEST: // 接收到请求
		AnswerFileRequest((MSG_FILE_REQUEST*)msg_recv);
		break;
	case MESSAGE_FILE_AGREE: // 同意
		StartFileTrans((MSG_FILE_REQUEST*)msg_recv);
		break;
	case MESSAGE_FILE_REFUSE: // 拒绝		
	case MESSAGE_FILE_ERROR:
		RecvFileRequestAnswer((MSG_FILE_REQUEST *)msg_recv);
		break;


	default:
		//msg_recv;
		//GetDesktopWindow()->MessageBox(L"收到未定义的数据类型！", L"系统提示");
		break;
	}

	return 0;
}


/*********************************************************
函数名称：SendMsg
功能描述：发送消息
创建时间：2016-08-03
备    注：
*********************************************************/
int CMFCChatDlg::SendMsg(void *msg, int nBufLen)
{
	if(m_pSocketChat == NULL)
	{
		if(!SocketInit())
		{// Socket初始化失败
			return 0;
		}
	}
	//MSG_HEAD msg_head;
	//msg_head.nType = MESSAGE_HEAD;
	//msg_head.nMsgSize = nBufLen;
	//m_pSocketChat->Send(msg_head, sizeof(MSG_HEAD));

	int nRes;
	nRes = m_pSocketChat->Send(msg, nBufLen);
	if(nRes == 0)
	{
		MessageBox(_T("消息发送失败！"));
	}
	return nRes;
}



// 从服务器获取好友信息(弃用)
int CMFCChatDlg::InitFriendInfo(struct msg_friend *pMsgFriend)
{
	/*
	// 如果参数为NULL 则清空m_listFriendInfo
	// 然后向服务器发送一条获取好友信息的消息
	if(pMsgFriend == NULL)
	{
		m_listFriendInfo.RemoveAll(); // 删除所有好友
		struct msg_friend_num msg;
		msg.nType = GET_FRIEND_INFO_ALL;
		SendMsg(&msg);
		return 0;
	}

	// 如果参数不为NULL 则往m_listFriendInfo里面添加元素
	class CFriendInfo friendinfo;
	friendinfo.csID.Format(_T("%s"), pMsgFriend->ID); // ID
	friendinfo.csName.Format(_T("%s"), pMsgFriend->Name);// 名字
	friendinfo.nStatus = pMsgFriend->nStatus; // 在线状态
	friendinfo.num = m_listFriendInfo.GetCount(); // 序号
	m_listFriendInfo.AddTail(&friendinfo);
	*/
	return 0;
}



// 登陆
//
/*********************************************************
函数名称：LoginSuccess
功能描述：登录成功
参数说明：msg_info-登陆成功时服务器返回的用户信息
返 回 值：
备    注：
*********************************************************/
int CMFCChatDlg::LoginSuccess(MSG_USERINFO* msg_info)
{
	UpdateData(TRUE);
	m_csMyID = m_pLoginDlg->m_csID;
	//m_dlgLogin.EndDialog(LOGIN_SUCCESS);
	m_pLoginDlg->OnCancel(); // 关闭登录窗口
	//delete m_pLoginDlg;
	m_pLoginDlg = NULL;
	m_csMyName = (LPCTSTR)msg_info->Name; // 用户昵称
	SetWindowTextW(m_csMyName);

	ShowWindow(SW_SHOW); // 显示主窗口
	UpdateData(FALSE); 
	UpdateFriendInfo(); // 更新好友信息

	LoadFileRecord(); // 加载记录数据
	
	SetTimer(TIMER_HEARTBEAT, 2000, NULL);
	SetTimer(TIMER_UPDATE_FRIEND_INFO, 10000, NULL);
	delete msg_info; // 释放内存
	return 0;
}

/*********************************************************
函数名称：LoginOut
功能描述：退出登录
参数说明：
返 回 值：
备    注：
**************** *****************************************/
int CMFCChatDlg::LoginOut(void)
{
	SaveFileRecord(); // 保存记录数据

	struct MSG_LOGIN msg_lg;
	msg_lg.nType = LOGIN_OUT;
	msg_lg.nStatus = IDS_STATUS_OFFLINE;
	size_t i;
	wcstombs_s(&i, msg_lg.nID, m_csMyID, m_csMyID.GetLength() + 1);
	SendMsg(&msg_lg, sizeof(msg_lg));
	m_csMyID = "";
	m_pSocketChat->Close();
	delete m_pSocketChat;
	m_pSocketChat = NULL;



	return 0;
}

/*********************************************************
函数名称：UpdateFriendInfo
功能描述：更新全部好友信息1
参数说明：msg_info-全部好友的ID、名字和状态
创建日期：2016-08-07
返 回 值：
备    注：如果参数为空 则是向服务器发送请求消息
*********************************************************/
int CMFCChatDlg::UpdateFriendInfo(MSG_FRND_INFO* msg_info)
{
	if(msg_info == NULL)
	{
		msg_info = new MSG_FRND_INFO;
		msg_info->nType = GET_ALL_FRIEND_INFO;
		memset(msg_info->ListID, 0, ID_MAX * FRIEND_MAX);
		memset(msg_info->ListName, 0, NAME_MAX * FRIEND_MAX);
		SendMsg(msg_info, sizeof(*msg_info));
		return FALSE;
	}

	m_Friend.UpdateBasicInfo(msg_info);

	m_Friend.ShowFriendInfo(&m_lstctlFriend);

	UpdateData(FALSE);

	delete msg_info; // 释放内存
	return 0;
}

/*********************************************************
函数名称：RefreshListCtrlData
功能描述：刷新好友列表控件的数据
创建日期：2016-08-17
备    注：
*********************************************************/
int CMFCChatDlg::RefreshListCtrlData(void)
{	
	m_Friend.ShowFriendInfo(&m_lstctlFriend);

	return 0;
}


/*********************************************************
函数名称：SystemMessage
功能描述：提示来自服务器的系统消息
参数说明：
返 回 值：
备    注：
*********************************************************/
int CMFCChatDlg::SystemMessage(MSG_SYS* msg_sys)
{
	if (msg_sys->nIDPrompt == IDS_DEL_FRIEND_SUCCESS)
	{
		//UpdateFriendInfo();
	}

	// 首先弹一个框 提示内容
	AfxMessageBox(msg_sys->nIDPrompt);

	// 然后根据nIDPrompt来执行一些其他操作
	switch(msg_sys->nIDPrompt)
	{
	case IDS_ADD_FRIEND_SUCCESS:
		if(m_pAddFriendDlg)
		{
			m_pAddFriendDlg->OnCancel();
			m_pAddFriendDlg = NULL;
		}
		UpdateFriendInfo();
		break;
	default:
		break;
	}

	delete msg_sys;

	return 0;
}


// 添加/删除好友
//
/*********************************************************
函数名称：RecvAddFriendRequest
功能描述：收到好友请求
参数说明：msg_add-请求消息
返 回 值：
备    注：
*********************************************************/
int CMFCChatDlg::RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add)
{
	msg_add->nType = ADD_FRIEND_ANSWER;
	CString csFromID;
	csFromID = msg_add->FromID;
	int nRes;
	nRes = MessageBox(L"账号：" + csFromID + L" 请求添加您为好友，是否同意请求？", L"好友添加请求", MB_YESNO);
	if(nRes == IDYES)
	{
		msg_add->nReturn = ADD_FRIEND_AGREE;
	}
	else
	{
		msg_add->nReturn = ADD_FRIEND_REFUSE;
	}
	SendMsg(msg_add, sizeof(*msg_add));
	
	return 0;
}

//按钮：打开添加好友对话框
void CMFCChatDlg::OnBnClickedButtonAddFriendDlg()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_pAddFriendDlg == NULL)
	{
		m_pAddFriendDlg = new CAddFriendDlg(this);
		m_pAddFriendDlg->Create(IDD_ADD_FRIEND_DLG, GetDesktopWindow());
		m_pAddFriendDlg->ShowWindow(SW_SHOW);
	}

}


// 获得当前选中的ID 用来删除好友
void CMFCChatDlg::OnNMClickListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if(-1 != pNMListView->iItem) // 如果iItem不是-1，就说明有列表项被选择 
	{		
		char cID[ID_MAX];
		m_Friend.GetItemID(cID, pNMListView->iItem);
		CString strID;
		strID = cID;
		SetDlgItemText(IDC_EDIT_FRIEND, strID);
	}
}

// 删除好友
/*********************************************************
函数名称：OnBnClickedBtDelFriend（未完成）
功能描述：删除好友
参数说明：
返 回 值：
备    注：
*********************************************************/
void CMFCChatDlg::OnBnClickedBtDelFriend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strID;
	GetDlgItemText(IDC_EDIT_FRIEND, strID);
	if (strID == "")
	{// 没有选中
		MessageBox(L"请选择要删除的好友！", L"系统提示");
	}
	char cID[ID_MAX];
	WideCharToMultiByte(CP_ACP, 0, strID, strID.GetLength() + 1, cID, ID_MAX, NULL, NULL);
	char name[NAME_MAX];
	m_Friend.GetFriendName(name, cID);
	CString strName;
	strName.Format(_T("%s"), name);

	int nRes = MessageBox(L"你确定要删除好友：" + strName + L"吗？", L"删除好友", MB_YESNO);

	if (nRes == IDYES)
	{
		MSG_MAKE_FRIEND msg_del;
		memset(&msg_del, 0, sizeof(MSG_MAKE_FRIEND));
		msg_del.nType = DELETE_FRIEND;
		strcpy_s(msg_del.ToID, cID);
		char myID[ID_MAX];
		WideCharToMultiByte(CP_ACP, 0, m_csMyID, m_csMyID.GetLength() + 1, myID, ID_MAX, NULL, NULL);
		memcpy(msg_del.FromID, myID, ID_MAX);
		m_pSocketChat->Send(&msg_del, sizeof(MSG_MAKE_FRIEND));
	}
}

// 一对一聊天
//
/*********************************************************
函数名称：OnNMDblclkListFriend
功能描述：双击好友列表时打开聊天窗口
创建日期：2016-08-17
备    注：
*********************************************************/
void CMFCChatDlg::OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 将指针转换为容易操作的结构类型
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if(-1 != pNMListView->iItem) // 如果iItem不是-1，就说明有列表项被选择 
	{
		OpenChatDlg(pNMListView->iItem);
	}

}
/*********************************************************
函数名称：OpenChatDlg
功能描述：打开聊天对话框
参数说明：nItem-序号
创建日期：2016-08-15
返 回 值：
*********************************************************/
int CMFCChatDlg::OpenChatDlg(int nItem)
{
	char nID[ID_MAX];
	m_Friend.GetItemID(nID, nItem);
	char name[NAME_MAX];
	m_Friend.GetFriendName(name, nID);
	
	// 发送消息 请求获取好友的详细信息
	MSG_USERINFO msg_info;
	memset(&msg_info, 0, sizeof(MSG_USERINFO));
	msg_info.nType = GET_FRIEND_INFO;
	strcpy_s(msg_info.nID, nID);
	SendMsg(&msg_info, sizeof(MSG_USERINFO));

	bool Flag = FALSE;
	POSITION pos = m_listChatDlg.GetHeadPosition();
	while(pos != NULL)
	{
		CChatDlg *p = m_listChatDlg.GetNext(pos);
		if(!strcmp(nID, p->m_nID))
		{
			Flag = TRUE; // 聊天框已经存在
			p->ShowWindow(SW_SHOWNORMAL);
			break;
		}
	}

	if(!Flag) // 聊天框不存在就创建一个
	{
		CChatDlg *pChatDlg = new CChatDlg(this);
		strcpy_s(pChatDlg->m_nID, ID_MAX, nID); // 复制要在create之前执行 不然窗口标题会乱码
		memcpy_s(pChatDlg->m_Name, NAME_MAX, name, NAME_MAX);	
		pChatDlg->Create(IDD_CHAT_DLG, GetDesktopWindow());
		pChatDlg->ShowWindow(SW_SHOW);		
		m_listChatDlg.AddTail(pChatDlg);
		pChatDlg->SetTimer(123, 1000, NULL);
		RefreshChatDlgMsg();
	}

	return 0;
}

/*********************************************************
函数名称：UpdateOneFriendInfo（未完成）
功能描述：更新本地的好友信息
作    者：余志荣
参数说明：msg_file -- 请求结构体
创建日期：2016-08-29
返 回 值：
*********************************************************/
int CMFCChatDlg::UpdateOneFriendInfo(MSG_USERINFO *msg_info)
{
	m_Friend.UpdateDetailInfo(msg_info);
	POSITION pos = m_listChatDlg.GetHeadPosition();
	while (pos != NULL)
	{
		CChatDlg *pDlg = m_listChatDlg.GetAt(pos);
		m_listChatDlg.GetNext(pos);
		if(!strcmp(pDlg->m_nID, msg_info->nID))
		{
			pDlg->RefreshStatic(msg_info);
		}
	}

	return 0;
}

/*********************************************************
函数名称：CloseChatDlg
功能描述：关闭聊天对话框
参数说明：nID-好友ID
创建日期：2016-08-15
返 回 值：
*********************************************************/
int CMFCChatDlg::CloseChatDlg(char* nID)
{
	POSITION pos = m_listChatDlg.GetHeadPosition();
	while(pos != NULL)
	{
		CChatDlg *p = m_listChatDlg.GetNext(pos);
		if(!strcmp(nID, p->m_nID))
		{
			if(pos == NULL)
			{// 要删除的元素是最后一个
				m_listChatDlg.RemoveTail();
			}
			else
			{
				m_listChatDlg.GetPrev(pos);
				m_listChatDlg.RemoveAt(pos);	
			}
			break;
		}
	}



	return 0;
}

/*********************************************************
函数名称：RefreshChatDlgMsg
功能描述：刷新未查看的消息
创建时间：2016-08-03
备    注：如果聊天框打开了就显示消息 没有打开就在主窗口提示
*********************************************************/
int CMFCChatDlg::RefreshChatDlgMsg(void)
{
	// 遍历第一次
	//   如果聊天窗口已打开 则将消息输出到聊天窗口

	int nFlag = 0;	
	POSITION posMsg = m_listChatMsg.GetHeadPosition();
	while(posMsg != NULL)
	{
		struct MSG_TRANSPOND *pChatMsg = m_listChatMsg.GetNext(posMsg); // 返回当前位置的元素 并将pos指向下一个元素			
		CString csFromID;
		csFromID = pChatMsg->FromID;
		POSITION posDlg = m_listChatDlg.GetHeadPosition();
		while(posDlg != NULL)
		{
			CChatDlg *pChatDlg = m_listChatDlg.GetNext(posDlg);
			if(!strcmp(pChatDlg->m_nID, pChatMsg->FromID))
			{
				nFlag = 1;
				CString csMsg, csDate, csTime;			
				csMsg.Format(_T("%s"), pChatMsg->Data);
				csDate = pChatMsg->Date;
				csTime = pChatMsg->Time;
				CTime tm = CTime::GetCurrentTime();
				if(tm.Format(_T("%Y-%m-%d")) == csDate)
				{	
					// 当天接收
					pChatDlg->AddMessage(csFromID, csTime, csMsg);
				}
				else
				{
					// 隔天接收
					pChatDlg->AddMessage(csFromID, csDate + " " + csTime, csMsg);
				}

				if(posMsg != NULL)
				{
					POSITION posMsgTemp = posMsg;
					m_listChatMsg.GetPrev(posMsgTemp); //将pos1指向当前元素
					m_listChatMsg.RemoveAt(posMsgTemp); //从消息中移除
				}
				else
				{
					m_listChatMsg.RemoveTail();
				}

				//delete pChatMsg; // 释放结构体占用的内存 *出错*	
			}
		}				
	}

	m_Friend.InitIsHaveMsg();
	posMsg = m_listChatMsg.GetHeadPosition();
	while(posMsg != NULL)
	{
		struct MSG_TRANSPOND *pChatMsg = m_listChatMsg.GetNext(posMsg);
		m_Friend.SetIsHaveMsg(pChatMsg->FromID, TRUE);
	}

	// 刷新列表控件里面的信息
	RefreshListCtrlData();

	return 0;
}


// 收发心跳包
//
/*********************************************************
函数名称：HeartBeat
功能描述：收发心跳包
作    者：余志荣
参数说明：msg_sys -- 此参数为NULL时是发送 不为NULL时是接收
创建日期：2016-08-15
返 回 值：
*********************************************************/
int CMFCChatDlg::HeartBeat(MSG_SYS* msg_sys)
{
	if(msg_sys == NULL)
	{// 发送心跳包
		msg_sys = new MSG_SYS;
		msg_sys->nType = HEARTBEAT;
		SendMsg(msg_sys, sizeof(MSG_SYS));
		return 0;
	}
	
	delete msg_sys;
	return 1;
}

/*********************************************************
函数名称：OnTimer
功能描述：定时检测与服务器的连接 若超过一定时间则关闭程序
作    者：余志荣
参数说明：
创建日期：2016-08-15
返 回 值：
*********************************************************/
void CMFCChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_HEARTBEAT)
	{
		if(m_pSocketChat != NULL)
		{
			HeartBeat();
			int nMaxSec = 3000;
			CTimeSpan tmsp;
			tmsp = CTime::GetTickCount() - m_pSocketChat->m_tmLastMsg;
			if(tmsp.GetTotalSeconds() > nMaxSec)
			{
				KillTimer(TIMER_HEARTBEAT);
				MessageBox(L"连接已断开，请重新登陆");
				OnCancel();
				return;
			}
		}
	}
	if(nIDEvent == TIMER_UPDATE_FRIEND_INFO)
	{
		UpdateFriendInfo();
	}

	CDialogEx::OnTimer(nIDEvent);
}


// 文件传输
//
/*********************************************************
函数名称：OnBnClickedBtFileDlg
功能描述：打开文件传输记录窗口
作    者：余志荣
参数说明：
创建日期：2016-08-29
返 回 值：
*********************************************************/
void CMFCChatDlg::OnBnClickedBtFileDlg()
{
	// 如果窗口不存在就创建一个 如果有的话直接显示
	if(m_pFileRcrdDlg == NULL)
	{
		m_pFileRcrdDlg = new CFileRecordDlg();
		m_pFileRcrdDlg->Create(IDD_FILE_DLG,this->GetDesktopWindow());
		m_pFileRcrdDlg->ShowWindow(SW_SHOW);
		m_pFileRcrdDlg->SetTimer(TIMER_UPDATE_FILE_DLG, 1000, NULL);
	}
	else
	{
		m_pFileRcrdDlg->ShowWindow(SW_SHOW);
	}
	m_pFileRcrdDlg->RefreshListCtrlData();
}

/*********************************************************
函数名称：GetAvailFileID
功能描述：获得未被占用的文件ID号
作    者：余志荣
参数说明：
创建日期：2016-08-29
返 回 值：可用的文件ID
*********************************************************/
int CMFCChatDlg::GetAvailFileID(void)
{
	int nID = 0;
	POSITION pos = m_FileRecordList.GetHeadPosition();
	while(pos != NULL)
	{
		FILE_RECORD *pFile_record = m_FileRecordList.GetAt(pos);
		if (nID <= pFile_record->nFileID)
		{
			nID = pFile_record->nFileID + 1;
		}
		m_FileRecordList.GetNext(pos);
	}
	return nID;
}

/*********************************************************
函数名称：AnswerFileRequest
功能描述：回应文件传输请求
作    者：余志荣
参数说明：msg_file -- 请求结构体
创建日期：2016-08-29
返 回 值：
*********************************************************/
int CMFCChatDlg::AnswerFileRequest(MSG_FILE_REQUEST* msg_req)
{
	ShowWindow(SW_SHOW);
	CString strFromID;
	strFromID = msg_req->FromID;
	CString strFileName;
	strFileName = msg_req->FileName;
	CString strFileSize;
	if(msg_req->nFileSize >= 1073741824) // 大于1GB
	{
		strFileSize.Format(L" %.2lf GB", msg_req->nFileSize / 1073741824.0);
	}
	else if(msg_req->nFileSize >= 1048576) // 小于1GB大于1MB
	{
		strFileSize.Format(L" %.2lf MB", msg_req->nFileSize / 1048576.0);
	}
	else
	{
		strFileSize.Format(L" %.2lf KB", msg_req->nFileSize / 1024.0);
	}
	
	// 弹出信息框 询问是否接受文件
	int nRes; // 是否接收
	//如果聊天框已经打开 就在聊天框弹窗 否则在主界面弹窗
	bool bIsOpen = FALSE; 
	POSITION pos = m_listChatDlg.GetHeadPosition();
	while(pos != NULL)
	{
		CChatDlg *p = m_listChatDlg.GetNext(pos);
		if(!strcmp(msg_req->FromID, p->m_nID))
		{// 聊天框已经打开
			bIsOpen = TRUE; 
			nRes = p->MessageBox(L"账号：" + strFromID + L" 请求发送文件:\r\n" + strFileName + strFileSize +
				L"\r\n是否同意请求？", L"文件传输请求", MB_YESNO);
			break;
		}
	}
	if(!bIsOpen)
	{// 聊天框没有打开
		nRes = MessageBox(L"账号：" + strFromID + L" 请求发送文件:\r\n" + strFileName + strFileSize +
			L"\r\n是否同意请求？", L"文件传输请求", MB_YESNO);
	}

	if(nRes == IDYES)
	{// 同意接收
		if(!InitFileTransSocket())
		{
			//传输Socket连接失败
			//GetDesktopWindow()->MessageBox(L"传输初始化失败");
			return FALSE;
		}

		CFileDialog fileDlg(FALSE, NULL, strFileName, OFN_OVERWRITEPROMPT, NULL, GetDesktopWindow());		
		
		if (IDOK == fileDlg.DoModal())
		{
			strFileName = fileDlg.GetPathName();
			CString strFileFolder = strFileName.Left(strFileName.GetLength() - fileDlg.GetFileName().GetLength() - 1);//得到文件夹	

			CFile file;
			// 测试一下文件是否能打开
			if(!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
			{
				MessageBox(L"保存路径有误！");			
			}
			else
			{
				file.Close();
				// 构建本地记录
				FILE_RECORD *pRecord = new FILE_RECORD;
				memset(pRecord, 0, sizeof(FILE_RECORD)); // 结构体置零
				pRecord->bType = false; // 传输类型为接收
				strcpy_s(pRecord->FromID, msg_req->FromID); // 发送者
				strcpy_s(pRecord->ToID, msg_req->ToID); // 接受者 就是自己
				WideCharToMultiByte(CP_ACP, 0, strFileFolder, strFileFolder.GetLength() + 1, pRecord->FilePath, 512, NULL, NULL); // 文件路径
				strcpy_s(pRecord->FileName, msg_req->FileName); // 文件名
				pRecord->nFileID = msg_req->nFileID; // 文件ID
				pRecord->nFileSize = msg_req->nFileSize; // 文件大小
				m_FileRecordList.AddTail(pRecord); // 添加到本地记录
				
				if(m_pSocketFileTrans->InitFileTrans(pRecord))
				{	
					// 客户端已经准备好 告诉另一端可以开始传
					pRecord->nTransState = 1; // 进行中
					msg_req->nType = MESSAGE_FILE_AGREE;
					SendMsg(msg_req, sizeof(MSG_FILE_REQUEST));
					return TRUE;
				}
			}			
			// 传输初始化失败			
				msg_req->nType = MESSAGE_FILE_ERROR;
				SendMsg(msg_req, sizeof(MSG_FILE_REQUEST));					
				GetDesktopWindow()->MessageBox(L"文件创建失败");							
				return FALSE;		
		}
	}
	// 拒绝接收
	msg_req->nType = MESSAGE_FILE_REFUSE;
	SendMsg(msg_req, sizeof(MSG_FILE_REQUEST));
	return FALSE;
}

/*********************************************************
函数名称：AnswerFileRequest
功能描述：处理收到的文件传输回应
作    者：余志荣
参数说明：msg_file -- 请求结构体
创建日期：2016-08-29
返 回 值：
*********************************************************/
int CMFCChatDlg::RecvFileRequestAnswer(MSG_FILE_REQUEST * msg_ans)
{	
	POSITION pos = m_FileRecordList.GetHeadPosition();
	while (pos != NULL)
	{
		FILE_RECORD * pRecord = m_FileRecordList.GetAt(pos);
		m_FileRecordList.GetNext(pos);
		if (!strcmp(pRecord->ToID, msg_ans->ToID))
		{// 匹配接收者
			if (!strcmp(pRecord->FileName, msg_ans->FileName))
			{// 匹配文件
				if (msg_ans->nType == MESSAGE_FILE_REFUSE)
				{// 拒绝接收
					pRecord->nTransState = 6;
					MessageBox(L"对方拒绝了你的文件发送请求。", L"文件传输");
					break;
				}
				if (msg_ans->nType == MESSAGE_FILE_ERROR)
				{
					pRecord->nTransState = 5;
					MessageBox(L"对方遇到外星人，无法同意发送请求。", L"文件传输");
					break;
				}

			}
		}
	}
	return TRUE;
}

/*********************************************************
函数名称：InitFileTransSocket
功能描述：初始化文件传输用的Socket
作    者：余志荣
参数说明：
创建日期：2016-08-30
返 回 值：
*********************************************************/
BOOL CMFCChatDlg::InitFileTransSocket(void)
{
	if(m_pSocketFileTrans != NULL)
	{
		return TRUE;
	}

	m_pSocketFileTrans = new CFileSocket();

	if(m_pSocketFileTrans == NULL)
	{
		//套接字初始化失败
		delete m_pSocketFileTrans;
		m_pSocketFileTrans = NULL;
		MessageBox(_T("套接字初始化失败！"));
		return FALSE;
	}

	if(!m_pSocketFileTrans->Create())
	{
		//创建失败
		delete m_pSocketFileTrans;
		m_pSocketFileTrans = NULL;
		MessageBox(_T("套接字创建失败！"));
		return FALSE;
	}

	if(m_pSocketFileTrans->Connect(_T(SERVER_IP), SERVER_PORT) == 0)
	{
		//连接服务器失败
		delete m_pSocketFileTrans;
		m_pSocketFileTrans = NULL;
		MessageBox(_T("连接服务器失败！"));
		return FALSE;
	}
	MSG_SYS msg_sys;
	msg_sys.nType = FILE_TRANS_SOCKET;
	WideCharToMultiByte(CP_ACP, 0, m_csMyID, m_csMyID.GetLength() + 1, msg_sys.nID, ID_MAX, NULL, NULL);
	m_pSocketFileTrans->Send(&msg_sys, sizeof(MSG_SYS));

	return TRUE;
}

/*********************************************************
函数名称：CloseFileTransScoket
功能描述：删除文件传输用的Socket
作    者：余志荣
参数说明：
创建日期：2016-09-01
返 回 值：
*********************************************************/
void CMFCChatDlg::CloseFileTransScoket(void)
{
	//delete m_pSocketFileTrans;
	//if (theApp.m_pMainWnd != NULL)
	//{	
	//	m_pSocketFileTrans = NULL;
	//}
	m_pSocketFileTrans = NULL;
	//return TRUE;
}

/*********************************************************
函数名称：StartFileTrans
功能描述：回应文件传输请求
作    者：余志荣
参数说明：msg_file -- 请求结构体
创建日期：2016-08-29
返 回 值：
*********************************************************/
int CMFCChatDlg::StartFileTrans(MSG_FILE_REQUEST *msg_ans)
{
	//MessageBox(L"文件开始传啦！");
	//m_TestThread = AfxBeginThread(FileCopy, this);
	//FileCopy(this);
	//IsRun = TRUE;
	FILE_RECORD *pFileRecord = NULL;
	POSITION pos = m_FileRecordList.GetHeadPosition();
	while (pos != NULL)
	{
		pFileRecord = m_FileRecordList.GetAt(pos);
		m_FileRecordList.GetNext(pos);
		if (pFileRecord->bType == false)
		{// 不是发送记录就跳到下一条记录
			continue;
		}
		if(strcmp(msg_ans->ToID, pFileRecord->ToID))
		{// 接收者ID不一样就跳到下一条记录
			continue;
		}
		if (!strcmp(msg_ans->FileName, pFileRecord->FileName))
		{
			if(!InitFileTransSocket())
			{
				return FALSE;
			}
			IsRun = TRUE;
			m_FileTransThread = AfxBeginThread(SendFile, pFileRecord);
			break;
		}
	}


	//CString strFileName;
	//strFileName = pFileRecord->FileName;
	//CString strFilePath;
	//strFilePath = pFileRecord->FilePath;

	//CFile fileSend;

	//if(!fileSend.Open(strFilePath, CFile::modeRead))
	//{
	//	GetDesktopWindow()->MessageBox(L"文件打开失败!");
	//	return 0;
	//}
	//if(pFileRecord->nFileSize != fileSend.GetLength())
	//{
	//	GetDesktopWindow()->MessageBox(L"文件信息发生变更!");
	//	return 0;
	//}
	//ULONGLONG nRemain = pFileRecord->nFileSize; // 剩余大小
	//if(!InitFileTransSocket())
	//{
	//	return FALSE;
	//}

	//// 告诉对方开始传输
	////msg_ans->nType = MESSAGE_FILE_TRANS_BEGIN;
	////SendMsg(msg_ans, sizeof(MSG_FILE_REQUEST));
	////m_pSocketFileTrans->Send(msg_ans, sizeof(MSG_FILE_REQUEST));
	////return FALSE;
	//MSG_FILE_CONTENT msg_file;
	//memset(&msg_file, 0, sizeof(MSG_FILE_CONTENT)); // 清空结构体
	//strcpy_s(msg_file.FromID, pFileRecord->FromID); // 发送方
	//strcpy_s(msg_file.ToID, pFileRecord->ToID); // 接收方
	//msg_file.nFileID = pFileRecord->nFileID; // 文件ID
	//msg_file.nType = MESSAGE_FILE_CONTENT; // 消息类型

	//int nPacket = 0; // 数据包编号
	//while (TRUE)
	//{
	//	/*
	//	if (nRemain <= 2000) // 此处2000需要跟着MSG_FILE_CONTENT结构体改变
	//	{
	//		memset(msg_file.FileData, 0, 2000);
	//		fileSend.Read(msg_file.FileData, 2000);
	//		msg_file.nPacketID = nPacket;
	//		m_pSocketFileTrans->Send(&msg_file, sizeof(MSG_FILE_CONTENT));
	//		break;
	//	}
	//	else
	//	{
	//		fileSend.Read(msg_file.FileData, 2000);
	//		nRemain -= 2000;
	//	}*/

	//	int nLen = fileSend.Read(msg_file.FileData, FILE_PACKET_SIZE);
	//	if(nLen == 0)
	//	{
	//		break;
	//	}
	//	nRemain -= nLen;
	//	msg_file.nPacketID = nPacket;
	//	//Sleep(10);
	//	//TRACE("nPacket = %d, nRemain = %d\n", nPacket, nRemain);
	//	m_pSocketFileTrans->Send(&msg_file, sizeof(MSG_FILE_CONTENT));
	//	++nPacket;
	//}
	//fileSend.Close();
	//// 结束传输
	////msg_ans->nType = MESSAGE_FILE_TRANS_END;
	////SendMsg(msg_ans, sizeof(MSG_FILE_REQUEST));
	////m_pSocketFileTrans->Send(msg_ans, sizeof(MSG_FILE_REQUEST));

	return 0;
}

/*********************************************************
函数名称：SendFile
功能描述：发送文件的线程
作    者：余志荣
参数说明：lpParam -- 指针类型是FILE_RECORD*
创建日期：2016-08-29
返 回 值：
*********************************************************/
UINT AFX_CDECL SendFile(LPVOID lpParam)
{		
	FILE_RECORD *pRecord = (FILE_RECORD *)lpParam;
	CFileSocket *pSocket = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_pSocketFileTrans;
	if(pSocket == NULL)
	{
		return FALSE;
	}
	CString strFileName;
	strFileName = pRecord->FileName;
	CString strFilePath;
	strFilePath = pRecord->FilePath;

	CFile fileSend;

	if(!fileSend.Open(strFilePath + "\\" + strFileName, CFile::modeRead))
	{
		//GetDesktopWindow()->MessageBox(L"文件打开失败!");
		pRecord->nTransState = 5;
		AfxMessageBox(L"文件打开失败!");
		return FALSE;
	}
	if(pRecord->nFileSize != fileSend.GetLength())
	{
		//GetDesktopWindow()->MessageBox(L"文件信息发生变更!");
		pRecord->nTransState = 5;
		AfxMessageBox(L"文件信息发生变更!");
		return FALSE;
	}
	ULONGLONG nRemain = pRecord->nFileSize; // 剩余大小

	// 告诉对方开始传输
	//msg_ans->nType = MESSAGE_FILE_TRANS_BEGIN;
	//SendMsg(msg_ans, sizeof(MSG_FILE_REQUEST));
	//m_pSocketFileTrans->Send(msg_ans, sizeof(MSG_FILE_REQUEST));
	//return FALSE;

	pRecord->nTransState = 1; // 传输进行中

	MSG_FILE_CONTENT msg_file;
	memset(&msg_file, 0, sizeof(MSG_FILE_CONTENT)); // 清空结构体
	strcpy_s(msg_file.FromID, pRecord->FromID); // 发送方
	strcpy_s(msg_file.ToID, pRecord->ToID); // 接收方
	msg_file.nFileID = pRecord->nFileID; // 文件ID
	msg_file.nType = MESSAGE_FILE_CONTENT; // 消息类型

	int nPacket = 0; // 数据包编号
	ULONGLONG ullCount = 0; // 已发送的字节数
	bool bIsFinish = false; // 记录是否完成传输
	while (IsRun)
	{
		/*
		// 2016年8月31日10:59:52修改
		if (nRemain <= 2000) // 此处2000需要跟着MSG_FILE_CONTENT结构体改变
		{
			memset(msg_file.FileData, 0, 2000);
			fileSend.Read(msg_file.FileData, 2000);
			msg_file.nPacketID = nPacket;
			m_pSocketFileTrans->Send(&msg_file, sizeof(MSG_FILE_CONTENT));
			break;
		}
		else
		{
			fileSend.Read(msg_file.FileData, 2000);
			nRemain -= 2000;
		}*/

		int nLen = fileSend.Read(msg_file.FileData, FILE_PACKET_SIZE);
		if(nLen == 0)
		{
			bIsFinish =true;
			break;
		}
		nRemain -= nLen;
		msg_file.nPacketSize = nLen;
		ullCount += nLen;
		msg_file.nPacketID = nPacket;
		++nPacket;
		Sleep(20); // 避免发送堵塞
		//TRACE("nPacket = %d, nRemain = %d\n", nPacket, nRemain);	
		pSocket->Send(&msg_file, sizeof(MSG_FILE_CONTENT));
		
		pRecord->nRecvPacket = nPacket; // 这里指的是已发送的包号
		pRecord->nRecvSize = ullCount; // 这里指的是已发送的字节数

	}
	fileSend.Close();
	
	if(bIsFinish)
	{// 已完成发送
		pRecord->nTransState = 3; // 状态：已完成
	}

	IsRun = false;


	//pSocket->Close(); // 这里close会出错 原因应该是socket不是在这个线程创建的
	//delete pSocket;
	//((CMFCChatDlg *)theApp.m_pMainWnd)->m_pSocketFileTrans = NULL;

// 结束传输
	//msg_ans->nType = MESSAGE_FILE_TRANS_END;
	//SendMsg(msg_ans, sizeof(MSG_FILE_REQUEST));
	//m_pSocketFileTrans->Send(msg_ans, sizeof(MSG_FILE_REQUEST));
	Sleep(100);
	MSG_FILE_OPERA msg_opera;
	memset(&msg_opera, 0, sizeof(MSG_FILE_OPERA));
	msg_opera.nType = MESSAGE_FILE_TRANS_END;
	strcpy_s(msg_opera.ToID, msg_file.ToID);
	strcpy_s(msg_opera.FromID, msg_file.FromID);
	pSocket->Send(&msg_opera, sizeof(MSG_FILE_OPERA));

	return TRUE;
}

// （弃用）
UINT AFX_CDECL RecvFile(LPVOID lpParam)
{	/*
	CFile fileSrc;
	// 参数一 文件的路径
	// 参数二 打开文件的模式
	//  CFile::modeCreate  创建方式打开文件，如文件已存在则将其长度设置为0
	//  CFile::modeRead 只读方式打开文件
	//  CFile::modeWrite 写入方式打开文件
	fileSrc.Open(strFilePath, CFile::modeRead);	
	//fileDest.Open(csDesPath + fileSrc.GetFileName(), CFile::modeCreate|CFile::modeWrite);

	// 文件总长度
	ULONGLONG fileSize = fileSrc.GetLength();
	// 已拷贝的长度
	ULONGLONG hadCopy = 0;

	//int nBufSize = 1024;
	char Buf[4096];
	while(IsRun)
	{
		// 读取文件数据
		memset(Buf, '\0', sizeof(Buf));
		int ret = fileSrc.Read(Buf, sizeof(Buf));

		// 写入文件
		fileDest.Write(Buf, ret);
		
		hadCopy += ret;
		
		if(fileSize != 0)
			((CFileCopyDlg*)lpParam)->m_fRate = hadCopy / (float)fileSize; // 进度

		((CFileCopyDlg*)lpParam)->m_csMsg.Format(_T("进度：%.2fMB/%.2fMB"), hadCopy / 1024.0 / 1024.0, fileSize / 1024.0 / 1024.0);
		// 如果到达文件结尾则中止循环
		if(ret < sizeof(Buf))
		{
			((CFileCopyDlg*)lpParam)->m_csMsg = "进度：复制完成！";
			break;
		}
	}

	// 关闭文件 
	fileDest.Close();
	fileSrc.Close();

	if(!IsRun)
	{
		((CFileCopyDlg*)lpParam)->m_csMsg = "进度：复制被中断！";		
	}
	IsRun = FALSE;
	*/
	return 0;
}

/*********************************************************
函数名称：RecvFile（弃用）
功能描述：接收FileSocket传来的消息
作    者：余志荣
参数说明：
创建日期：2016-08-30
返 回 值：
*********************************************************/
int CMFCChatDlg::RecvFile(void)
{
	// 2016年8月31日08:42:16注释
	// 此方法会造成堵塞
	/*
	MSG_FILE_REQUEST msg_opera;
	m_pSocketFileTrans->Receive(&msg_opera, sizeof(MSG_FILE_REQUEST));
	//return FALSE;
	if(msg_opera.nType == MESSAGE_FILE_TRANS_BEGIN)
	{
		CString strFileName;
		strFileName = msg_opera.FileName;
		CFile fileRecv;
		
		if(!fileRecv.Open(_T("C:\\Users\\95446\\Desktop\\源码\\ChatClient\\") + strFileName, CFile::modeCreate | CFile::modeWrite))
		{
			GetDesktopWindow()->MessageBox(L"文件创建失败！");
			return FALSE;
		}
		int nPacket = 0;
		ULONGLONG nRemain = msg_opera.nFileSize;
		while(true)
		{
			TRACE(L"nRemain = %d\n", nRemain);
			TRACE(L"nPacket = %d\n", nPacket);
			MSG_FILE_CONTENT msg_content;
			m_pSocketFileTrans->Receive(&msg_content, sizeof(MSG_FILE_CONTENT));
					
			if(msg_content.nType == MESSAGE_FILE_TRANS_END)
			{		
				//break;
			}
			if(msg_content.nType == MESSAGE_FILE_CONTENT)
			{
				//if(msg_content.nPacketID == nPacket)
				{
					if(nRemain <= 2000)
					{
						fileRecv.Write(msg_content.FileData, nRemain);
						
						break;
					}
					else
					{
						fileRecv.Write(msg_content.FileData, 2000);
						nRemain -= 2000;
						++nPacket;
					}
				}
			}
		}
		fileRecv.Close();
	}
	*/

	return 0;
}



/*********************************************************
函数名称：LoadFileRecord
功能描述：加载文件传输记录
作    者：余志荣
创建日期：2016-09-02
返 回 值：
*********************************************************/
BOOL CMFCChatDlg::LoadFileRecord(void)
{
	CFile file;
	if(!file.Open(L".//" + m_csMyID + L".txt", CFile::modeReadWrite))
	{
		return FALSE;
	}

	// 删除所有记录
	POSITION pos = m_FileRecordList.GetHeadPosition();
	while(pos != NULL)
	{
		FILE_RECORD *p = m_FileRecordList.GetNext(pos);
		//delete p;
		if(pos == NULL)
		{// 已经到了链表尾
			m_FileRecordList.RemoveTail();
		}
		else
		{
			m_FileRecordList.GetPrev(pos);
			m_FileRecordList.RemoveAt(pos);
			pos = m_FileRecordList.GetHeadPosition();
		}
	}

	while(TRUE)
	{
		FILE_RECORD *p = new FILE_RECORD;
		int nRet = file.Read(p, sizeof(FILE_RECORD));

		// 如果读不到字节则退出
		if(nRet == 0)
		{
			delete p;
			break;
		}
		else
		{
			m_FileRecordList.AddTail(p);		
		}
	}
	file.Close();

	return TRUE;
}

/*********************************************************
函数名称：SaveFileRecord
功能描述：保存文件传输记录
作    者：余志荣
创建日期：2016-09-02
返 回 值：
*********************************************************/
BOOL CMFCChatDlg::SaveFileRecord(void)
{
	CFile file;	
	//int nReadBytes = 0; //存储文件中读取的总字节数

	if(!file.Open(L".//" + m_csMyID + L".txt", CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}
	POSITION pos = m_FileRecordList.GetHeadPosition();
	while(pos != NULL)
	{
		FILE_RECORD *p = m_FileRecordList.GetNext(pos);		
		file.Write(p, sizeof(FILE_RECORD));
	}
	file.Close();

	return TRUE;
}
