

/************************************************
文 件 名：MFCChatDlg.h
作    者：余志荣
创建日期：2016-08-29
用    途：主窗口的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

// MFCChatDlg.h : 头文件
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "ChatDlg.h"
#include "LoginDlg.h"
#include "AddFriendDlg.h"
#include "ChatSocket.h"
#include "FileSocket.h"
#include "Friend.h"
#include "FileRecordDlg.h"

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
	virtual BOOL OnInitDialog();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedBtFileDlg();
	afx_msg void OnPaint();
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedButtonAddFriendDlg();
	afx_msg void OnBnClickedBtDelFriend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	virtual void PostNcDestroy();

	bool SocketInit(void); // 初始化Socket
	void CloseChatSocket(void); // 关闭socket

	int SendMsg(void *msg, int nBufLen); // 发送消息	
	int RecvMsg(void); // 接收消息	
	int SystemMessage(MSG_SYS* msg_sys); // 系统消息
	// 登陆模块
	int LoginSuccess(MSG_USERINFO* msg_info); // 上线
	int LoginOut(void); // 下线
	int UpdateFriendInfo(struct MSG_FRND_INFO* msg_info = NULL); // 更新好友基本信息
	// 添加好友
	int RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add); // 收到好友请求
	// 一对一聊天
	int OpenChatDlg(int nItem);
	int CloseChatDlg(char* nID);
	int UpdateOneFriendInfo(MSG_USERINFO *msg_info);
	// 文件传输
	BOOL InitFileTransSocket(void);
	void CloseFileTransScoket(void);
	int GetAvailFileID(void);	
	int AnswerFileRequest(MSG_FILE_REQUEST* msg_file); // 回应请求
	int RecvFileRequestAnswer(MSG_FILE_REQUEST * msg_ans); // 收到回应
	int StartFileTrans(MSG_FILE_REQUEST *msg_ans);
	int RecvFile(void); // 弃用
	// 刷存在感
	int HeartBeat(MSG_SYS* msg_sys = NULL);
	
	// 重绘按钮控件用的变量
	CBtnRndRct m_BtnAddFriend;
	CBtnRndRct m_BtnDelFriend;
	CBtnRndRct m_BtnExit;
	CBtnRndRct m_BtnFileDlg;
	
	CWinThread* m_FileTransThread; // 文件传输线程
	
	// 对话框
	CLoginDlg* m_pLoginDlg; // 登录窗口指针
	//CLoginDlg m_dlgLogin; // 登录窗口
	CAddFriendDlg* m_pAddFriendDlg; // “添加好友”对话框
	CFileRecordDlg* m_pFileRcrdDlg; // 文件传输记录对话框
	

	// 本人信息
	CString m_csMyID; // 我的ID
	CString m_csMyName; // 我的昵称
	
	// 好友
	CFriend m_Friend;
	

	CList<FILE_RECORD*, FILE_RECORD*> m_FileRecordList; // 文件收发记录
	
	// Socket指针
	CChatSocket* m_pSocketChat; // 与服务器通信的socket指针	
	CFileSocket* m_pSocketFileTrans; // 用来传文件的socket

protected:	
	int RefreshListCtrlData(void); // 更新好友列表数据	
	int RefreshChatDlgMsg(void); // 刷新聊天窗口的消息	
	int InitFriendInfo(struct msg_friend *pMsgFriend = NULL);
	
	CComboBox m_cbState; // 在线状态控件
	CListCtrl m_lstctlFriend; // 好友列表视图控件
	
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // 待阅读的聊天消息列表
	CList<CChatDlg*, CChatDlg*> m_listChatDlg; // 聊天窗口链表

public:
	afx_msg void OnNMClickListFriend(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL LoadFileRecord(void);
	BOOL SaveFileRecord(void);
};

