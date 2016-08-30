#pragma once

#include "MyButton.h"

// CLoginDlg �Ի���
class CRegistDlg; // ���ǰ������ ���ܴ������� ��������ָ��

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegDlg();

	CString m_csID; // �˺�
	CString m_csKey; // ����	
	
	CRegistDlg* m_pRegDlg; // ע�ᴰ��
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
