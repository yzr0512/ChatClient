#pragma once
//#include "MyButton.h"

// CFileRecordDlg �Ի���

class CFileRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRecordDlg)

public:
	CFileRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileRecordDlg();

// �Ի�������
	enum { IDD = IDD_FILE_DLG };
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrlRecv;
	CListCtrl m_ListCtrlSend;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioSend();
	afx_msg void OnBnClickedRadioRecv();
};
