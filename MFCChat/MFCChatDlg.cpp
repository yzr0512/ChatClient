
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

//定义一个全局变量，用于控制线程是否退出
volatile BOOL IsRun = FALSE;

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
	m_csMyName = m_dlgLogin.m_csID;
	
	// 更新好友列表的数据
	RefreshListCtrlData();

	// 设置在线状态
	m_cbState.SelectString(0, _T("在线"));
		
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
	if(m_csMyID != "")
	{
		LoginOut();
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


//双击好友列表事件
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

//按钮：添加好友
void CMFCChatDlg::OnBnClickedButtonAddFriendDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_pAddFriendDlg == NULL)
	{
		m_pAddFriendDlg = new CAddFriendDlg(this);
		m_pAddFriendDlg->Create(IDD_ADD_FRIEND_DLG);
		m_pAddFriendDlg->ShowWindow(SW_SHOW);
	}

}


/*********************************************************
函数名称：RefreshListCtrlData
功能描述：刷新列表控件的数据
创建日期：2016-08-17
备    注：
*********************************************************/
int CMFCChatDlg::RefreshListCtrlData(void)
{	
	m_Friend.ShowFriendInfo(&m_lstctlFriend);

	return 0;
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
	
	if(m_pSocketChat->Connect(_T("127.0.0.1"), 8080) == 0)
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

		// 文件传输请求
	case MESSAGE_FILE_REQUEST: // 接收到请求
		AnswerFileRequest((MSG_FILE_REQUEST*)msg_recv);
		break;
	case MESSAGE_FILE_AGREE: // 同意
		StartFileTrans((MSG_FILE_REQUEST*)msg_recv);
		break;
	case MESSAGE_FILE_REFUSE: // 拒绝
		MessageBox(L"对方拒绝了你的文件发送请求。", L"系统消息");
		break;

		// 收到各种信息
	case GET_FRIEND_INFO: // 一个人的详细信息
		UpdateOneFriendInfo((MSG_USERINFO*)msg_recv);
		break;



	default:
		GetDesktopWindow()->MessageBox(L"收到未定义的数据类型！", L"系统提示");
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


// 从服务器获取好友信息
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


// 删除好友
void CMFCChatDlg::OnBnClickedBtDelFriend()
{
	// TODO: 在此添加控件通知处理程序代码
	
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
	m_pLoginDlg = NULL;
	m_csMyName = (LPCTSTR)msg_info->Name; // 用户昵称
	SetWindowTextW(m_csMyName + L" 的主窗口");

	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	UpdateFriendInfo();
	
	SetTimer(TIMER_HEARTBEAT, 2000, NULL);
	SetTimer(TIMER_UPDATE_FRIEND_INFO, 10000, NULL);

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
功能描述：更新全部好友信息
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


	return 0;
}


// 添加好友
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


// 一对一聊天
//
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
	
	//// 发送消息 请求获取好友的详细信息
	//MSG_USERINFO msg_info;
	//memset(&msg_info, 0, sizeof(MSG_USERINFO));
	//msg_info.nType = GET_FRIEND_INFO;
	//strcpy_s(msg_info.nID, nID);
	//SendMsg(&msg_info, sizeof(MSG_USERINFO));

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
		if(!strcmp(pDlg->m_nID, msg_info->nID))
		{
			//pDlg->RefreshStatic();
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
			int nMaxSec = 30000;
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
	}
	else
	{
		m_pFileRcrdDlg->ShowWindow(SW_SHOW);
	}
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
int CMFCChatDlg::AnswerFileRequest(MSG_FILE_REQUEST* msg_file)
{
	CString strFromID;
	strFromID = msg_file->FromID;
	CString strFileName;
	strFileName = msg_file->FileName;
	CString strFileSize;
	if(msg_file->nFileSize >= 1073741824) // 大于1GB
	{
		strFileSize.Format(L" %.2lf GB", msg_file->nFileSize / 1073741824.0);
	}
	else if(msg_file->nFileSize >= 1048576) // 小于1GB大于1MB
	{
		strFileSize.Format(L" %.2lf MB", msg_file->nFileSize / 1048576.0);
	}
	else
	{
		strFileSize.Format(L" %.2lf KB", msg_file->nFileSize / 1024.0);
	}
	int nRes;
	nRes = GetDesktopWindow()->MessageBox(L"收到未定义的数据类型！");MessageBox(L"账号：" + strFromID + L" 请求发送文件:\r\n" + strFileName + strFileSize +
		L"\r\n是否同意请求？", L"文件传输请求", MB_YESNO);
	if(nRes == IDOK)
	{
		msg_file->nType = MESSAGE_FILE_AGREE;
		SendMsg(msg_file, sizeof(MSG_FILE_REQUEST));
	}
	else
	{
		msg_file->nType = MESSAGE_FILE_REFUSE;
		SendMsg(msg_file, sizeof(MSG_FILE_REQUEST));
	}
	return 0;
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
	MessageBox(L"文件开始传啦！");
	//m_TestThread = AfxBeginThread(FileCopy, this);
	//FileCopy(this);
	//IsRun = TRUE;
	return 0;
}

UINT AFX_CDECL SendFile(LPVOID lpParam)
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