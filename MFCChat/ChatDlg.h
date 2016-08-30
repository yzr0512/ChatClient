#pragma once
#include "afxwin.h"


// CChatDlg �Ի���




class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
	enum { IDD = IDD_CHAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg();
	
	//CString m_csID;	// �Է���ID
	char m_nID[ID_MAX]; // �Է���ID
	char m_Name[NAME_MAX];
	CBtnRndRct m_BtnSendMsg;
	CBtnRndRct m_BtnSendFile;
	CBtnRndRct m_BtnRecvFile;
	CBtnRndRct m_BtnClose;
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	
	// ��Output�༭���������Ϣ
	int AddMessage(const CString& csName, const CString& csTime, const CString& csMsg);

protected:	
	CString m_csInputMsg; // ���������
	CString m_csOutputMsg; // ���������
	CWnd* m_pParentWnd; // ������

public:
	int RefreshListCtrlData(void);
	afx_msg void OnBnClickedButtonSendFile();
	
	int RefreshStatic(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
