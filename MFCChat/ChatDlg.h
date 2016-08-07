#pragma once


// CChatDlg �Ի���

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
	enum { IDD = IDD_CHAT_DLG };


	CString m_csID;	// �Է���ID

	// ��Output�༭���������Ϣ
	int AddMessage(const CString& csName, const CString& csTime, const CString& csMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonSendMsg();


	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:	
	CString m_csInputMsg; // ���������
	CString m_csOutputMsg; // ���������
	CWnd* m_pParentWnd; // ������
};
