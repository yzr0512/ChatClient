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
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	int InitListCtrlStyle(void);
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonAddFriend();
	int ShowStrangerInfo(MSG_USERINFO* msg_info);

// Dialog Data
	enum { IDD = IDD_ADD_FRIEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	CListCtrl m_listctrlFriend;	// 列表控件
	CString m_csFriendID; // 对方ID
	CWnd* m_pParentWnd; // 父窗口

	CBtnRndRct m_BtnAddFrnd;
	CBtnRndRct m_BtnFind;
};
