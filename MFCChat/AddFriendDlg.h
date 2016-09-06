
/************************************************
�� �� ����AddFriendDlg.h
��    �ߣ���־��
�������ڣ�2016-08-27
��    ;������Ӻ��ѡ��Ի����ͷ�ļ�
�޸ļ�¼��2016-08-27 by ��־�� ����
************************************************/ 


#pragma once

#include "MyButton.h"
#include "afxwin.h"

// CAddFriendDlg dialog

class CAddFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddFriendDlg)

public:
	CAddFriendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddFriendDlg();
	virtual BOOL OnInitDialog(); // ��ʼ������

	virtual void OnCancel(); // �رմ���
	virtual void PostNcDestroy(); // �رմ���

	int InitListCtrlStyle(void); // ��ʼ���б�ؼ�����ʽ

	// ��ť�¼�
	afx_msg void OnBnClickedButtonFind(); // ����
	afx_msg void OnBnClickedButtonAddFriend(); // ���

	int ShowStrangerInfo(MSG_USERINFO* msg_info); // ��ʾ�鵽������Ϣ

// Dialog Data
	enum { IDD = IDD_ADD_FRIEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	CListCtrl m_listctrlFriend;	// �б�ؼ�
	CString m_csFriendID; // �Է�ID
	CWnd* m_pParentWnd; // ������

	CBtnRndRct m_BtnAddFrnd; // ��ť�ؼ����� �����ػ���ʽ
	CBtnRndRct m_BtnFind;	 // ��ť�ؼ����� �����ػ���ʽ
};
