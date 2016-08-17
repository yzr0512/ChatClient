
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

// CMFCChatDlg 对话框

CMFCChatDlg::CMFCChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCChatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_csMyName = _T("");

	m_pAddFriendDlg = NULL;
	m_pLoginDlg = NULL;
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
}

BEGIN_MESSAGE_MAP(CMFCChatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIEND, &CMFCChatDlg::OnNMDblclkListFriend)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FRIEND_DLG, &CMFCChatDlg::OnBnClickedButtonAddFriendDlg)
	ON_BN_CLICKED(IDC_BT_DEL_FRIEND, &CMFCChatDlg::OnBnClickedBtDelFriend)
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
	
	//SocketInit(); // 初始化Socket
	
	// 打开登录窗口
	// 如果登陆窗口返回2，表示登陆窗口只是被关闭 主窗口也关闭	
	if(m_pLoginDlg == NULL)
	{
		m_pLoginDlg = new CLoginDlg(this);
		m_pLoginDlg->Create(IDD_LOGIN_DLG);
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
	ShowWindow(SW_SHOW);
	/*
	int nRes;	
	nRes = m_dlgLogin.DoModal();
	if (nRes != LOGIN_SUCCESS)
	{	
		EndDialog(2);
	}
	
	UpdateData(TRUE);
	// 显示用户名
	m_csMyName = m_dlgLogin.m_csID;
	*/

	// 好友列表
	//  设置列表的格式
	InitListCtrlStyle();
	//  更新列表的数据
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


//双击好友列表事件
void CMFCChatDlg::OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 将指针转换为容易操作的结构类型
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	// 多个聊天窗口的情况
	if(-1 != pNMListView->iItem) // 如果iItem不是-1，就说明有列表项被选择 
	{
		CString csFrndName; // 被选中好友的名字
		//CFriendInfo *pFriendInfo;
		//POSITION pos = m_listFriendInfo.GetHeadPosition();
		for(int i = 0; i != pNMListView->iItem; ++i)
		{
			m_listFriendInfo.GetNext(pos);
		}
		pFriendInfo = m_listFriendInfo.GetAt(pos);
		csFriend = pFriendInfo->csID;
		//csFriend = m_lstctlFriend.GetItemText(pNMListView->iItem, 0);

		// 寻找是否已存在与该好友的聊天对话框
		//  nFlag = 1表示存在，nFlag = 0表示不存在
		//  若存在则不执行任何操作，若不存在则新增一个对话框
		int nFlag = 0;
		for each(auto p in m_vecpChatDlg) 
		{
			if(p->m_csID == csFriend)
			{
				nFlag = 1;
			}
		}
		if(nFlag == 0) // 不存在与被选中好友的对话框，新建一个
		{
			CChatDlg* pChatDlg;
			pChatDlg = new CChatDlg();
			
			// 将选定的好友传递给聊天对话框
			pChatDlg->m_csID = csFriend;
			
			m_vecpChatDlg.push_back(pChatDlg);
			// 创建非模态对话框
			pChatDlg->Create(IDD_CHAT_DLG, this);
			// 显示对话框
			pChatDlg->ShowWindow(SW_SHOW);

			RefreshChatDlgMsg(); // 显示未接收的消息
			RefreshListCtrlData(); // 更新主对话框的列表控件
		}
	}
	*/

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

// 初始化好友列表的表头
int CMFCChatDlg::InitListCtrlStyle(void)
{
	// 获取列表控件的宽度
	CRect rect;
	m_lstctlFriend.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// 设置ListCtrl的样式
	//  LVS_EX_GRIDLINES 网格线
	//  LVS_EX_FULLROWSELECT 整行选中
	m_lstctlFriend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// 插入表头
	// 参数分别为：列索引（第几列）、列名、列的文字格式、列宽、与列相关联的子项的索引
	//  LVCFMT_CENTER 居中
	//  LVCFMT_LEFT   左对齐
	//  LVCFMT_RIGHT  右对齐
	m_lstctlFriend.InsertColumn(0, _T("好友名"), LVCFMT_CENTER, int(nColInterval  * 0.75));   
    m_lstctlFriend.InsertColumn(1, _T("状态"), LVCFMT_CENTER, int(nColInterval * 0.25));   

	return 0;
}


// 更新好友列表数据	
int CMFCChatDlg::RefreshListCtrlData(void)
{	/*
	// 在列表视图控件中插入列表项，并设置列表子项文本   
	POSITION pos = m_listFriendInfo.GetHeadPosition();
	int i = 0;
	while(pos != NULL)
	{
		CString csAdd; // 附加消息
		CFriendInfo *pFriendInfo;
		pFriendInfo = m_listFriendInfo.GetNext(pos);

		// 检查是否有新消息未接收
		POSITION posMsg = m_listChatMsg.GetHeadPosition();
		while(posMsg != NULL)
		{
			struct MSG_TRANSPOND *p;
			p = m_listChatMsg.GetNext(posMsg);
			CString csFormID;
			csFormID.Format(_T("%s"), p->FormID);
			if(csFormID == pFriendInfo->csID)
			{
				csAdd = "有新消息！";
				break;
			}			
		}
		m_lstctlFriend.InsertItem(i, pFriendInfo->csID + csAdd); //这应该是显示name的
		switch(pFriendInfo->nStatus)
		{
		case STATUS_ONLINE:
			m_lstctlFriend.SetItemText(i, 1, _T("在线"));
			break;
		case STATUS_BUSY:
			m_lstctlFriend.SetItemText(i, 1, _T("忙碌"));
			break;
		case STATUS_OFFLINE:
			m_lstctlFriend.SetItemText(i, 1, _T("离线"));
			break;
		default:
			m_lstctlFriend.SetItemText(i, 1, _T("被外星人拐跑了"));
			break;
		}

		i++;
	
	}
*/
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

// 接收信息
int CMFCChatDlg::RecvMsg(void)
{
	int nBugLen = 4096;
	struct MSG_T msg_recv; // 接收到的数据
	int nRes; // 接收到的字节数
	nRes = m_pSocketChat->Receive((char*)&msg_recv, nBugLen);

	if(nRes == 0) // 没有接收到数据
	{
		return 0;
	}
	
	switch(msg_recv.nType)
	{
	case LOGIN_SUCCESS: // 登录成功
		LoginSuccess((struct MSG_USERINFO*)&msg_recv);
		break;	
	case CHATING_TEXT_MSG:
		struct MSG_TRANSPOND *chatmsg;
		chatmsg = (MSG_TRANSPOND *)&msg_recv;
		m_listChatMsg.AddTail(chatmsg);		
		RefreshChatDlgMsg();
		RefreshListCtrlData();
		break;
	case REGISTER_SUCCESS:
		//m_dlgLogin.m_dlgRegist.RegisterSuccess((struct MSG_REGISTER*)&msg_recv);
		m_pLoginDlg->m_pRegDlg->RegisterSuccess((struct MSG_REGISTER*)&msg_recv);
		break;
	case REGISTER_FAILED: // 注册失败
		/*CString csMsg;
		csMsg.Format(_T("%s"),msg_recv.Data);		
		MessageBox(csMsg);*/
		break;
	case IDS_SYSTEM_MESSAGE: // 系统消息
		SystemMessage((struct MSG_SYS*)&msg_recv);
		break;
	case GET_STRANGER_INFO:
		m_pAddFriendDlg->ShowStrangerInfo((struct MSG_USERINFO *)&msg_recv);
		break;
	case ADD_FRIEND_REQUEST:
		RecvAddFriendRequest((struct MSG_TRANSPOND*)&msg_recv);
		break;
	case GET_ALL_FRIEND_INFO:
		UpdateFriendInfo((MSG_FRND_INFO*)&msg_recv);
		break;
	default:
		break;
	}

	return 0;
}

// 发送消息
int CMFCChatDlg::SendMsg(void *msg, int nBufLen)
{
	if(m_pSocketChat == NULL)
	{
		if(!SocketInit())
		{// Socket初始化失败
			return 0;
		}
	}

	int nRes;
	nRes = m_pSocketChat->Send(msg, nBufLen);
	if(nRes == 0)
	{
		MessageBox(_T("消息发送失败！"));
	}
	return nRes;
}

// 刷新聊天窗口的消息
int CMFCChatDlg::RefreshChatDlgMsg(void)
{
	/*
	// 遍历第一次
	//   如果聊天窗口已打开 则将消息输出到聊天窗口
	POSITION pos = m_listChatMsg.GetHeadPosition();
	struct MSG_TRANSPOND *pChatMsg;

	int nFlag = 0;
	while(pos != NULL)
	{
		pChatMsg = m_listChatMsg.GetNext(pos); // 返回当前位置的元素 并将pos指向下一个元素
		CString csFormID;
		csFormID.Format(_T("%s"), pChatMsg->FromID);
		for each(auto p in m_vecpChatDlg)
		{
			if(csFormID == p->m_csID) // 与该ID的聊天对话框已经打开			
			{
				nFlag = 1;
				CString csMsg, csDate, csTime;
			
				csMsg.Format(_T("%s"), pChatMsg->Data);
				csDate.Format(_T("%s"), pChatMsg->Date);
				csTime.Format(_T("%s"), pChatMsg->Time);
				CTime tm = CTime::GetCurrentTime();
				if(tm.Format(_T("%Y-%m-%d")) == csDate)
				{	
					// 当天接收
					p->AddMessage(csFormID, csTime, csMsg);
				}
				else
				{
					// 隔天接收
					p->AddMessage(csFormID, csDate + " " + csTime, csMsg);
				}
				
				POSITION pos1 = pos;

				delete pChatMsg; // 释放结构体占用的内存
				m_listChatMsg.GetPrev(pos1); //将pos1指向当前元素
				m_listChatMsg.RemoveAt(pos1); //从消息中移除
				// 删除元素之后 要重新遍历一遍 或者break 或者 用临时变量pos1解决
				//pos = m_listChatMsg.GetHeadPosition();

			}
		}		
	}
	// 刷新列表控件里面的信息
	RefreshListCtrlData();
	*/

	// 遍历第一次
	//   如果聊天窗口已打开 则将消息输出到聊天窗口
	
	int nFlag = 0;	
	POSITION posMsg = m_listChatMsg.GetHeadPosition();
	while(posMsg != NULL)
	{
		struct MSG_TRANSPOND *pChatMsg = m_listChatMsg.GetNext(posMsg); // 返回当前位置的元素 并将pos指向下一个元素			
		CString csFormID;
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
	//MessageBox(L"登陆成功！");
	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	UpdateFriendInfo();
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