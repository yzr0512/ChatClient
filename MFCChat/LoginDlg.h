
/************************************************
文 件 名：LoginDlg.h
作    者：余志荣
创建日期：2016-08-29
用    途：登陆窗口的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

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
	afx_msg void OnBnClickedButtonLogin(); // 登陆
	afx_msg void OnBnClickedButtonRegDlg(); // 注册
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	CString m_csID; // 账号
	CString m_csKey; // 密码	
	
	CRegistDlg* m_pRegDlg; // 注册窗口
	CWnd* m_pParentWnd;	

	virtual void OnCancel();
	virtual void PostNcDestroy();

	// 按钮控件变量 用来重绘按钮
	CBtnRndRct m_BtnLogin;
	CBtnRndRct m_BtnRegist;
	CBtnImage  m_BtnClose;

};
