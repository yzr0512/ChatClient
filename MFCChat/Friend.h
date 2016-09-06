/************************************************
File name：Friend.h
Create by：余志荣
CreateDate：2016-08-07
Use：本地用来记录好友的信息
Change by: 2016-08-07 余志荣 创建
************************************************/ 

#pragma once
#include "MFCChat.h"

// 记录单个好友的信息
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
CreateDate：2016-08-07
Create by：余志荣
Explain：这个类用来记录好友的信息（包括对话框）
Change by: 
************************************************/

class CFriend
{
public:
	CFriend(void);
	~CFriend(void);

	int UpdateBasicInfo(MSG_FRND_INFO* msg_info); // 更新本地数据
	//int OpenChatDlg(int nItem); // 打开聊天窗口
	int InitIsHaveMsg(); // 重置所有人的m_bIsHaveMsg为false
	int SetIsHaveMsg(char* nID, bool status); // 设置nID的m_bIsHaveMsg为status
	int ShowFriendInfo(CListCtrl * pListCtrl)const; // 显示好友列表
	int GetItemID(char* nID, int nItem)const; // 获得序号为nItem的ID
	int GetFriendName(char* name, char* nID)const; // 获得nID的name
	int UpdateDetailInfo(MSG_USERINFO* msg_info); // 更新好友的详细信息
	int GetDetailInfo(MSG_USERINFO* msg_info); // 获得好友的详细信息（名字、账号、邮箱、性别、年龄）

protected:
	BOOL IsExist(CFriendInfo** p, char* nID)const; // 检查nID是否存在

	CList<CFriendInfo*, CFriendInfo*> m_friendlist;

	//CImageList m_ImgList; // 头像列表

};

