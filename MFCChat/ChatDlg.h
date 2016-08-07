#pragma once


// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
	enum { IDD = IDD_CHAT_DLG };


	CString m_csID;	// 对方的ID

	// 在Output编辑框中添加消息
	int AddMessage(const CString& csName, const CString& csTime, const CString& csMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg();


	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:	
	CString m_csInputMsg; // 输入的内容
	CString m_csOutputMsg; // 输出的内容
	CWnd* m_pParentWnd; // 父窗口
};
