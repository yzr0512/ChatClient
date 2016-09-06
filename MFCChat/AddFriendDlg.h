
/************************************************
文 件 名：AddFriendDlg.h
作    者：余志荣
创建日期：2016-08-27
用    途：“添加好友”对话框的头文件
修改记录：2016-08-27 by 余志荣 创建
************************************************/ 


#pragma once

#include "MyButton.h"
#include "afxwin.h"

// CAddFriendDlg dialog

class CAddFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddFriendDlg)

public:
	CAddFriendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddFriendDlg();
	virtual BOOL OnInitDialog(); // 初始化窗口

	virtual void OnCancel(); // 关闭窗口
	virtual void PostNcDestroy(); // 关闭窗口

	int InitListCtrlStyle(void); // 初始化列表控件的样式

	// 按钮事件
	afx_msg void OnBnClickedButtonFind(); // 查找
	afx_msg void OnBnClickedButtonAddFriend(); // 添加

	int ShowStrangerInfo(MSG_USERINFO* msg_info); // 显示查到到的信息

// Dialog Data
	enum { IDD = IDD_ADD_FRIEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	CListCtrl m_listctrlFriend;	// 列表控件
	CString m_csFriendID; // 对方ID
	CWnd* m_pParentWnd; // 父窗口

	CBtnRndRct m_BtnAddFrnd; // 按钮控件变量 用来重绘样式
	CBtnRndRct m_BtnFind;	 // 按钮控件变量 用来重绘样式
};
