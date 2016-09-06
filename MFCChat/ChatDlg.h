
/************************************************
�� �� ����ChatDlg.h
��    �ߣ���־��
�������ڣ�2016-08-17
��    ;�������족�Ի����ͷ�ļ�
�޸ļ�¼��2016-08-17 ��־�� ����
************************************************/ 

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
	virtual BOOL OnInitDialog(); // ��ʼ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg(); // �����ı���Ϣ
	afx_msg void OnBnClickedButtonSendFile(); // �����ļ���Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int RefreshListCtrlData(void); // ˢ���ļ���¼
	int RefreshStatic(MSG_USERINFO * msg_info); // ˢ�¾�̬��ǩ

	char m_nID[ID_MAX]; // �Է����˺�
	char m_Name[NAME_MAX]; // �Է�������

	// ��ť�ؼ����� �����ػ���ʽ
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
	CWnd* m_pParentWnd; // ������ָ��
	
	CListCtrl m_ListFileRecord; // �ļ���¼�б�
	CEdit m_EditCtrlOut; // �����Ϣ �༭��
	CEdit m_EditInput; // ������Ϣ �༭��
	CStatic m_StaticInfo; // ������Ϣ ��̬��ǩ
};
