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
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg();
	
	//CString m_csID;	// 对方的ID
	char m_nID[ID_MAX]; // 对方的ID
	char m_Name[NAME_MAX];
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
	CWnd* m_pParentWnd; // 父窗口

public:
	int RefreshListCtrlData(void);
	afx_msg void OnBnClickedButtonSendFile();
	
	int RefreshStatic(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
