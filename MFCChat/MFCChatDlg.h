
// MFCChatDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include "ChatDlg.h"
#include "LoginDlg.h"
#include "AddFriendDlg.h"
#include "ChatSocket.h"



// CMFCChatDlg �Ի���
class CMFCChatDlg : public CDialogEx
{
// ����
public:
	CMFCChatDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCCHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedButtonAddFriendDlg();

public:	
	CAddFriendDlg* m_pAddFriendDlg; // ����Ӻ��ѡ��Ի���

	std::vector<CChatDlg*> m_vecpChatDlg; // ������촰��

	CString m_csMyID; // �ҵ�ID
	CString m_csMyName; // �ҵ��ǳ�

	CLoginDlg* m_pLoginDlg; // ��¼����ָ��

	bool SocketInit(void); // ��ʼ��Socket	
	int RecvMsg(void); // ������Ϣ
	int SendMsg(void *msg, int nBufLen); // ������Ϣ

protected:	
	CComboBox m_cbState; // ����״̬�ؼ�
	CListCtrl m_lstctlFriend; // �����б���ͼ�ؼ�
	CLoginDlg m_dlgLogin; // ��¼����	
	CChatSocket* m_pSocketChat; // �������ͨ�ŵ�socketָ��	
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // ���Ķ���������Ϣ�б�
	
	//CList<class CFriendInfo*, class CFriendInfo*> m_listFriendInfo; // ������Ϣ�б�

protected:
	int InitListCtrlStyle(void); // ��ʼ�������б�	
	int RefreshListCtrlData(void); // ���º����б�����	
	int RefreshChatDlgMsg(void); // ˢ�����촰�ڵ���Ϣ	
	int InitFriendInfo(struct msg_friend *pMsgFriend = NULL);

public:
	afx_msg void OnBnClickedBtDelFriend();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	int LoginSuccess(MSG_USERINFO* msg_info);

	int SystemMessage(MSG_SYS* msg_sys);
	// ����
	int LoginOut(void);
	// �յ���������
	int RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add);
	int UpdateFriendInfo(struct MSG_FRND_INFO* msg_info = NULL);
};

