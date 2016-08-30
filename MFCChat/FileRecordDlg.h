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
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlRecv;
	CListCtrl m_ListCtrlSend;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioSend();
	afx_msg void OnBnClickedRadioRecv();
};
