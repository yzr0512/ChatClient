#pragma once

#include "MyButton.h"

// CLoginDlg 对话框
class CRegistDlg; // 类的前向声明 不能创建对象 但可以设指针

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegDlg();

	CString m_csID; // 账号
	CString m_csKey; // 密码	
	
	CRegistDlg* m_pRegDlg; // 注册窗口
	CWnd* m_pParentWnd;	
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void OnCancel();
	virtual void PostNcDestroy();

	CBtnRndRct m_BtnLogin;
	CBtnRndRct m_BtnRegist;
	CBtnImage  m_BtnClose;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
