
/************************************************
文 件 名：ChatDlg.h
作    者：余志荣
创建日期：2016-08-17
用    途：“聊天”对话框的头文件
修改记录：2016-08-17 余志荣 创建
************************************************/ 

#pragma once
#include "afxwin.h"

// CChatDlg 对话框


class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
	enum { IDD = IDD_CHAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog(); // 初始化窗口
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg(); // 发送文本消息
	afx_msg void OnBnClickedButtonSendFile(); // 发送文件消息
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int RefreshListCtrlData(void); // 刷新文件记录
	int RefreshStatic(MSG_USERINFO * msg_info); // 刷新静态标签

	char m_nID[ID_MAX]; // 对方的账号
	char m_Name[NAME_MAX]; // 对方的名字

	// 按钮控件变量 用于重绘样式
	CBtnRndRct m_BtnSendMsg;
	CBtnRndRct m_BtnSendFile;
	CBtnRndRct m_BtnRecvFile;
	CBtnRndRct m_BtnClose;
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	
	// 在Output编辑框中添加消息
	int AddMessage(const CString& csName, const CString& csTime, const CString& csMsg);

protected:
	CString m_csInputMsg; // 输入的内容
	CString m_csOutputMsg; // 输出的内容
	CWnd* m_pParentWnd; // 父窗口指针
	
	CListCtrl m_ListFileRecord; // 文件记录列表
	CEdit m_EditCtrlOut; // 输出信息 编辑框
	CEdit m_EditInput; // 输入信息 编辑框
	CStatic m_StaticInfo; // 好友信息 静态标签
};
