#pragma once


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

	CString m_csName;
	CString m_csKey;
	CString m_csEmail;
	CString m_csKey2;
	UINT m_nAge;
	char m_cSex;	
	CWnd* m_pParentWnd;
public:
	virtual BOOL OnInitDialog();
};

