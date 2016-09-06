/************************************************
File name：Friend.cpp
Create by：余志荣
CreateDate：2016-08-07
Use：CFriend类的实现文件
Change by: 2016-08-07 余志荣 创建
************************************************/ 

#include "StdAfx.h"
#include "Friend.h"
#include "ChatDlg.h"

CFriendInfo::CFriendInfo()
{
	memset(m_nID, 0, ID_MAX);
	memset(m_Name, 0, NAME_MAX);
	memset(m_Email, 0, EMAIL_MAX);
	m_Sex = '\0';
	m_nStatus = IDS_ERR_USER_NOT_EXIST;
	m_nAge = 0;
	m_bIsHaveMsg = FALSE;
}

CFriendInfo::~CFriendInfo()
{
}



CFriend::CFriend(void)
{
	//m_ImgList = NULL;
}


CFriend::~CFriend(void)
{
}


/*********************************************************
函数名称：UpdateBasicInfo
功能描述：更新好友的基本信息（ID，姓名，状态）
创建时间：2016-08-07
参数说明：
返 回 值：
*********************************************************/
int CFriend::UpdateBasicInfo(MSG_FRND_INFO* msg_info)
{
	if(msg_info == NULL)
	{
		return FALSE;
	}

	/*
	// 2016年9月2日23:17:23 余志荣 修改 这个无法删除好友
	for(int i = 0; i != msg_info->nNum; ++i)
	{
		CFriendInfo* p = NULL;
		if(IsExist(&p, msg_info->ListID[i]))
		{		
			memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
			p->m_nStatus = msg_info->nStatus[i];
		}
		else
		{
			p = new CFriendInfo();
			strcpy_s(p->m_nID, msg_info->ListID[i]);
			memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
			p->m_nStatus = msg_info->nStatus[i];
			m_friendlist.AddTail(p);
		}
	}
	*/

	if (msg_info->nNum == m_friendlist.GetCount())
	{// 好友数目不变
		for(int i = 0; i != msg_info->nNum; ++i)
		{
			CFriendInfo* p = NULL;
			if(IsExist(&p, msg_info->ListID[i]))
			{		
				memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
				p->m_nStatus = msg_info->nStatus[i];
			}
			else
			{
				p = new CFriendInfo();
				strcpy_s(p->m_nID, msg_info->ListID[i]);
				memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
				p->m_nStatus = msg_info->nStatus[i];
				m_friendlist.AddTail(p);
			}
		}
	}
	else
	{
		POSITION pos = m_friendlist.GetHeadPosition();
		POSITION pos1;
		while (pos != NULL)
		{
			CFriendInfo *p = m_friendlist.GetNext(pos);
			delete p;
			pos1 = pos;
			if (pos1 == NULL)
			{// 删除列表的末尾
				m_friendlist.RemoveTail();
			}
			else
			{// 删除链表中间的元素
				m_friendlist.GetPrev(pos1);
				m_friendlist.RemoveAt(pos1);
			}
		}

		// 添加好友到链表
		for(int i = 0; i != msg_info->nNum; ++i)
		{
			CFriendInfo* p = new CFriendInfo();

			strcpy_s(p->m_nID, msg_info->ListID[i]);
			memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
			p->m_nStatus = msg_info->nStatus[i];
			m_friendlist.AddTail(p);
		}
	}

	// 2016年9月2日23:24:39 余志荣 修改结束 
	return TRUE;
}


/*********************************************************
函数名称：ShowFriendInfo
功能描述：显示好友的基本信息（ID，姓名，状态）
创建时间：2016-08-07
参数说明：pListCtrl-列表控件指针
返 回 值：
*********************************************************/
int CFriend::ShowFriendInfo(CListCtrl * pListCtrl)const
{
	// 删除所有行
	pListCtrl->DeleteAllItems();
	// 删除所有列
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pListCtrl->DeleteColumn(0);
		}
	}


	// 构建图标列表
	CImageList *ImageList = new CImageList;
	//ImageList->Create(40, 40, ILC_COLOR, 0, 20);
	ImageList->Create(40, 40, ILC_COLOR, 0, 20);

	CBitmap * pBmp = NULL;  
	pBmp = new CBitmap();  
	pBmp->LoadBitmapW(IDB_BITMAP_OFFLINE);  
	ImageList->Add(pBmp,RGB(0,0,0));  
	delete pBmp;
	/*	
	pBmp = new CBitmap();  
	pBmp->LoadBitmapW(IDB_BITMAP_OFFLINE);  
	ImageList->Add(pBmp,RGB(0,0,0));  
	delete pBmp;  
	*/
	/*
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIconW();  
	ImageList->Add(hIcon);  
	*/

	// 给m_lstctlData添加表头
	CRect rect;
	pListCtrl->GetClientRect(&rect);
	int nColInterval = rect.Width();
	pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); // 设置风格
	
	pListCtrl->SetImageList(ImageList, LVSIL_SMALL ); // 设置图标列表
	pListCtrl->InsertColumn(0, _T(""), LVCFMT_CENTER,		int(nColInterval * 0.17));
    pListCtrl->InsertColumn(1, _T("名称"), LVCFMT_CENTER,	int(nColInterval * 0.63));	
	pListCtrl->InsertColumn(2, _T("状态"), LVCFMT_CENTER,	int(nColInterval * 0.2));   
	
	// 添加数据
	int i = 0; // 行号

	LVITEM lvItem={0}; // 用来插入图片的
	//第一行数据   
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;  //文字、图片、状态   
	lvItem.iItem = i;       //行号
	lvItem.iImage = 0;		//图片索引号
	lvItem.iSubItem = 0;    //子列号

	POSITION pos = m_friendlist.GetHeadPosition();
	CFriendInfo *pinfo = NULL;
	while(pos != NULL)
	{
		pinfo = m_friendlist.GetNext(pos);
		CString strTemp; // 用于转换多字符

		strTemp = (LPCTSTR)pinfo->m_Name; // 昵称是以宽字符来储存
		if(pinfo->m_bIsHaveMsg)
		{
			strTemp += " 有新的消息！";
		}
		lvItem.iItem = i;
		pListCtrl->InsertItem(&lvItem);
		pListCtrl->SetItemText(i, 1, strTemp);
		strTemp.LoadStringW(pinfo->m_nStatus);// 状态是数字
		pListCtrl->SetItemText(i, 2, strTemp);		
		++i;
	}

	return TRUE;
}


/*********************************************************
函数名称：IsExist
功能描述：检查ID是否存在 如果存在就返回该ID的指针
创建时间：2016-08-07
参数说明：p-指向好友信息指针的指针 nID-账号ID
返 回 值：
*********************************************************/
BOOL CFriend::IsExist(CFriendInfo** p,char* nID)const
{
	POSITION pos = m_friendlist.GetHeadPosition();
	while(pos != NULL)
	{
		*p = m_friendlist.GetNext(pos);
		if(!strcmp((*p)->m_nID, nID))
		{
			return TRUE;
		}
	}
	return FALSE;
}


/*********************************************************
函数名称：GetItemID
功能描述：获得第nItem个好友的ID
创建时间：2016-08-15
参数说明：nID-账号ID nItem-序号
返 回 值：
*********************************************************/
int CFriend::GetItemID(char* nID, int nItem)const
{
	ASSERT(nID);
	CFriendInfo *p = NULL;
	POSITION pos = m_friendlist.GetHeadPosition();
	for(int i = -1; i != nItem; ++i)
	{
		p = m_friendlist.GetNext(pos);
		if(p == NULL)
		{
			return -1;
		}
	}
	strcpy_s(nID, ID_MAX, p->m_nID);
	return 0;
}


/*********************************************************
函数名称：GetFriendName
功能描述：获得nID的Name
创建时间：2016-08-15
参数说明：name-名字 nID-账号
返 回 值：
*********************************************************/
int CFriend::GetFriendName(char* name, char* nID)const
{
	ASSERT(name && nID);
	CFriendInfo *p = NULL;
	if(IsExist(&p, nID))
	{
		memcpy(name, p->m_Name, NAME_MAX);
	}
	else
	{
		memset(name, 0, NAME_MAX);	
	}
	return 0;
}


/*********************************************************
函数名称：InitIsHaveMsg
功能描述：初始化成员变量m_bIsHaveMsg 全部设置为FALSE
创建时间：2016-08-15
参数说明：
返 回 值：
*********************************************************/
int CFriend::InitIsHaveMsg()
{
	POSITION pos = m_friendlist.GetHeadPosition();
	while(pos != NULL)
	{
		CFriendInfo *p = m_friendlist.GetNext(pos);
		p->m_bIsHaveMsg = FALSE;
	}
	return 0;
}


/*********************************************************
函数名称：SetIsHaveMsg
功能描述：设置某个ID的m_bIsHaveMsg
创建时间：2016-08-15
参数说明：
返 回 值：
*********************************************************/
int CFriend::SetIsHaveMsg(char* nID, bool status)
{
	CFriendInfo *p = NULL;
	if(IsExist(&p, nID))
	{
		p->m_bIsHaveMsg = status;
	}
	return 0;
}


/*********************************************************
函数名称：UpdateDetailInfo
功能描述：更新某个ID的详细信息
创建时间：2016-08-29
参数说明：msg_info -- 某个ID的详细信息
返 回 值：
*********************************************************/
int CFriend::UpdateDetailInfo(MSG_USERINFO* msg_info)
{
	CFriendInfo *p = NULL;
	if(IsExist(&p, msg_info->nID))
	{
		memcpy(p->m_Name, msg_info->Name, NAME_MAX);
		//strcpy_s(p->m_Name, msg_info->Name);
		strcpy_s(p->m_Email, msg_info->Email);
		p->m_nAge = msg_info->nAge;
		p->m_Sex = msg_info->Sex;
		p->m_nStatus = msg_info->nStatus;
	}
	else
	{

	}	
	return 0;
}

/*********************************************************
函数名称：GetDetailInfo
功能描述：获取某个ID的详细信息
创建时间：2016-08-29
参数说明：msg_info -- 某个ID的详细信息
返 回 值：
*********************************************************/
int CFriend::GetDetailInfo(MSG_USERINFO* msg_info)
{	
	CFriendInfo *p = NULL;
	if(IsExist(&p, msg_info->nID))
	{
		strcpy_s(msg_info->Name, p->m_Name);
		strcpy_s(msg_info->Email, p->m_Email);
		msg_info->nAge = p->m_nAge;
		msg_info->Sex = p->m_Sex;
		msg_info->nStatus = p->m_nStatus;
	}
	else
	{

	}
	
	return 0;
}
