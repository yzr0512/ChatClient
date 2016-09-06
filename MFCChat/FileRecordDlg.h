
/************************************************
文 件 名：FileRecordDlg.h
作    者：余志荣
创建日期：2016-08-29
用    途：文件记录窗口的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

#pragma once
//#include "MyButton.h"

// CFileRecordDlg 对话框

class CFileRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRecordDlg)

public:
	CFileRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileRecordDlg();

// 对话框数据
	enum { IDD = IDD_FILE_DLG };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSend(); // 改变显示的列表
	afx_msg void OnBnClickedRadioRecv(); // 改变显示的列表
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int RefreshListCtrlData(void); // 刷新列表的数据

	// 按钮控件变量 用来重绘按钮
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	
	CListCtrl m_ListCtrlRecv;
	CListCtrl m_ListCtrlSend;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	bool m_bIsSendList; // 用来记录当前显示的列表控件

};
