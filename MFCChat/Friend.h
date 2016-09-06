/************************************************
File name��Friend.h
Create by����־��
CreateDate��2016-08-07
Use������������¼���ѵ���Ϣ
Change by: 2016-08-07 ��־�� ����
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


/************************************************
Class name: CFriend
CreateDate��2016-08-07
Create by����־��
Explain�������������¼���ѵ���Ϣ�������Ի���
Change by: 
************************************************/

class CFriend
{
public:
	CFriend(void);
	~CFriend(void);

	int UpdateBasicInfo(MSG_FRND_INFO* msg_info); // ���±�������
	//int OpenChatDlg(int nItem); // �����촰��
	int InitIsHaveMsg(); // ���������˵�m_bIsHaveMsgΪfalse
	int SetIsHaveMsg(char* nID, bool status); // ����nID��m_bIsHaveMsgΪstatus
	int ShowFriendInfo(CListCtrl * pListCtrl)const; // ��ʾ�����б�
	int GetItemID(char* nID, int nItem)const; // ������ΪnItem��ID
	int GetFriendName(char* name, char* nID)const; // ���nID��name
	int UpdateDetailInfo(MSG_USERINFO* msg_info); // ���º��ѵ���ϸ��Ϣ
	int GetDetailInfo(MSG_USERINFO* msg_info); // ��ú��ѵ���ϸ��Ϣ�����֡��˺š����䡢�Ա����䣩

protected:
	BOOL IsExist(CFriendInfo** p, char* nID)const; // ���nID�Ƿ����

	CList<CFriendInfo*, CFriendInfo*> m_friendlist;

	//CImageList m_ImgList; // ͷ���б�

};

