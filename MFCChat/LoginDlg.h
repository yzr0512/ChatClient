
/************************************************
�� �� ����LoginDlg.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;����½���ڵ�ͷ�ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

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
	afx_msg void OnBnClickedButtonLogin(); // ��½
	afx_msg void OnBnClickedButtonRegDlg(); // ע��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	CString m_csID; // �˺�
	CString m_csKey; // ����	
	
	CRegistDlg* m_pRegDlg; // ע�ᴰ��
	CWnd* m_pParentWnd;	

	virtual void OnCancel();
	virtual void PostNcDestroy();

	// ��ť�ؼ����� �����ػ水ť
	CBtnRndRct m_BtnLogin;
	CBtnRndRct m_BtnRegist;
	CBtnImage  m_BtnClose;

};
