
// MFCChatDlg.h : 头文件
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include "ChatDlg.h"
#include "LoginDlg.h"
#include "AddFriendDlg.h"
#include "ChatSocket.h"



// CMFCChatDlg 对话框
class CMFCChatDlg : public CDialogEx
{
// 构造
public:
	CMFCChatDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCCHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedButtonAddFriendDlg();

public:	
	CAddFriendDlg* m_pAddFriendDlg; // “添加好友”对话框

	std::vector<CChatDlg*> m_vecpChatDlg; // 多个聊天窗口

	CString m_csMyID; // 我的ID
	CString m_csMyName; // 我的昵称

	CLoginDlg* m_pLoginDlg; // 登录窗口指针

	bool SocketInit(void); // 初始化Socket	
	int RecvMsg(void); // 接收消息
	int SendMsg(void *msg, int nBufLen); // 发送消息

protected:	
	CComboBox m_cbState; // 在线状态控件
	CListCtrl m_lstctlFriend; // 好友列表视图控件
	CLoginDlg m_dlgLogin; // 登录窗口	
	CChatSocket* m_pSocketChat; // 与服务器通信的socket指针	
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // 待阅读的聊天消息列表
	
	//CList<class CFriendInfo*, class CFriendInfo*> m_listFriendInfo; // 好友信息列表

protected:
	int InitListCtrlStyle(void); // 初始化好友列表	
	int RefreshListCtrlData(void); // 更新好友列表数据	
	int RefreshChatDlgMsg(void); // 刷新聊天窗口的消息	
	int InitFriendInfo(struct msg_friend *pMsgFriend = NULL);

public:
	afx_msg void OnBnClickedBtDelFriend();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	int LoginSuccess(MSG_USERINFO* msg_info);

	int SystemMessage(MSG_SYS* msg_sys);
	// 离线
	int LoginOut(void);
	// 收到好友请求
	int RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add);
	int UpdateFriendInfo(struct MSG_FRND_INFO* msg_info = NULL);
};

