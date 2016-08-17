/************************************************
Class name: CFriend
CreateDate��2016-08-07
Explain�������������¼���ѵ���Ϣ�������Ի���
Change by: 
************************************************/ 
#pragma once
#include "MFCChat.h"
// ��¼�������ѵ���Ϣ
class CFriendInfo
{
public:
	CFriendInfo();
	~CFriendInfo();

	char m_nID[ID_MAX];
	char m_Name[NAME_MAX];
	char m_Email[EMAIL_MAX];
	char m_Sex;	
	int  m_nStatus;
	int  m_nAge;
	bool m_bIsHaveMsg;
};

class CFriend
{
public:
	CFriend(void);
	~CFriend(void);

	int UpdateBasicInfo(MSG_FRND_INFO* msg_info);	
	int OpenChatDlg(int nItem);
	
	int InitIsHaveMsg();
	int SetIsHaveMsg(char* nID, bool status);


	int ShowFriendInfo(CListCtrl * pListCtrl)const;
	int GetItemID(char* nID, int nItem)const;
	int GetFriendName(char* name, char* nID)const;

protected:
	BOOL IsExist(CFriendInfo** p, char* nID)const;

	CList<CFriendInfo*, CFriendInfo*> m_friendlist;
};

