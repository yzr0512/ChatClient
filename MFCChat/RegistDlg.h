#pragma once
/************************************************
文 件 名：RegistDlg.h
作    者：余志荣
创建日期：2016-08-29
用    途：注册对话框的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

#include "LoginDlg.h"
#include "afxwin.h"
// CRegistDlg dialog

class CRegistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegistDlg)

public:
	CRegistDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegistDlg();

// Dialog Data
	enum { IDD = IDD_REGIST_DLG };

	afx_msg void OnBnClickedButtonRegist();
	void RegisterSuccess(struct MSG_REGISTER *msg_reg);
	afx_msg void OnBnClickedRadioSexMale();
	afx_msg void OnBnClickedRadioFemale();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	CBtnRndRct m_BtnRegist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// 用户输入的注册信息
	CString m_csName;
	CString m_csKey;
	CString m_csKey2;
	CString m_csEmail;
	UINT m_nAge;
	char m_cSex;

	CWnd* m_pParentWnd; //父窗口指针
public:
	virtual BOOL OnInitDialog();
};

