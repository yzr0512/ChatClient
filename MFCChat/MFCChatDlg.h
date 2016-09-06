

/************************************************
�� �� ����MFCChatDlg.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;�������ڵ�ͷ�ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

// MFCChatDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "ChatDlg.h"
#include "LoginDlg.h"
#include "AddFriendDlg.h"
#include "ChatSocket.h"
#include "FileSocket.h"
#include "Friend.h"
#include "FileRecordDlg.h"

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
	virtual BOOL OnInitDialog();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedBtFileDlg();
	afx_msg void OnPaint();
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedButtonAddFriendDlg();
	afx_msg void OnBnClickedBtDelFriend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	virtual void PostNcDestroy();

	bool SocketInit(void); // ��ʼ��Socket
	void CloseChatSocket(void); // �ر�socket

	int SendMsg(void *msg, int nBufLen); // ������Ϣ	
	int RecvMsg(void); // ������Ϣ	
	int SystemMessage(MSG_SYS* msg_sys); // ϵͳ��Ϣ
	// ��½ģ��
	int LoginSuccess(MSG_USERINFO* msg_info); // ����
	int LoginOut(void); // ����
	int UpdateFriendInfo(struct MSG_FRND_INFO* msg_info = NULL); // ���º��ѻ�����Ϣ
	// ��Ӻ���
	int RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add); // �յ���������
	// һ��һ����
	int OpenChatDlg(int nItem);
	int CloseChatDlg(char* nID);
	int UpdateOneFriendInfo(MSG_USERINFO *msg_info);
	// �ļ�����
	BOOL InitFileTransSocket(void);
	void CloseFileTransScoket(void);
	int GetAvailFileID(void);	
	int AnswerFileRequest(MSG_FILE_REQUEST* msg_file); // ��Ӧ����
	int RecvFileRequestAnswer(MSG_FILE_REQUEST * msg_ans); // �յ���Ӧ
	int StartFileTrans(MSG_FILE_REQUEST *msg_ans);
	int RecvFile(void); // ����
	// ˢ���ڸ�
	int HeartBeat(MSG_SYS* msg_sys = NULL);
	
	// �ػ水ť�ؼ��õı���
	CBtnRndRct m_BtnAddFriend;
	CBtnRndRct m_BtnDelFriend;
	CBtnRndRct m_BtnExit;
	CBtnRndRct m_BtnFileDlg;
	
	CWinThread* m_FileTransThread; // �ļ������߳�
	
	// �Ի���
	CLoginDlg* m_pLoginDlg; // ��¼����ָ��
	//CLoginDlg m_dlgLogin; // ��¼����
	CAddFriendDlg* m_pAddFriendDlg; // ����Ӻ��ѡ��Ի���
	CFileRecordDlg* m_pFileRcrdDlg; // �ļ������¼�Ի���
	

	// ������Ϣ
	CString m_csMyID; // �ҵ�ID
	CString m_csMyName; // �ҵ��ǳ�
	
	// ����
	CFriend m_Friend;
	

	CList<FILE_RECORD*, FILE_RECORD*> m_FileRecordList; // �ļ��շ���¼
	
	// Socketָ��
	CChatSocket* m_pSocketChat; // �������ͨ�ŵ�socketָ��	
	CFileSocket* m_pSocketFileTrans; // �������ļ���socket

protected:	
	int RefreshListCtrlData(void); // ���º����б�����	
	int RefreshChatDlgMsg(void); // ˢ�����촰�ڵ���Ϣ	
	int InitFriendInfo(struct msg_friend *pMsgFriend = NULL);
	
	CComboBox m_cbState; // ����״̬�ؼ�
	CListCtrl m_lstctlFriend; // �����б���ͼ�ؼ�
	
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // ���Ķ���������Ϣ�б�
	CList<CChatDlg*, CChatDlg*> m_listChatDlg; // ���촰������

public:
	afx_msg void OnNMClickListFriend(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL LoadFileRecord(void);
	BOOL SaveFileRecord(void);
};

