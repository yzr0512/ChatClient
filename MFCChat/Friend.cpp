/************************************************
File name��Friend.cpp
Create by����־��
CreateDate��2016-08-07
Use��CFriend���ʵ���ļ�
Change by: 2016-08-07 ��־�� ����
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
�������ƣ�UpdateBasicInfo
�������������º��ѵĻ�����Ϣ��ID��������״̬��
����ʱ�䣺2016-08-07
����˵����
�� �� ֵ��
*********************************************************/
int CFriend::UpdateBasicInfo(MSG_FRND_INFO* msg_info)
{
	if(msg_info == NULL)
	{
		return FALSE;
	}

	/*
	// 2016��9��2��23:17:23 ��־�� �޸� ����޷�ɾ������
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
	{// ������Ŀ����
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
			{// ɾ���б��ĩβ
				m_friendlist.RemoveTail();
			}
			else
			{// ɾ�������м��Ԫ��
				m_friendlist.GetPrev(pos1);
				m_friendlist.RemoveAt(pos1);
			}
		}

		// ��Ӻ��ѵ�����
		for(int i = 0; i != msg_info->nNum; ++i)
		{
			CFriendInfo* p = new CFriendInfo();

			strcpy_s(p->m_nID, msg_info->ListID[i]);
			memcpy(p->m_Name, msg_info->ListName[i], NAME_MAX);
			p->m_nStatus = msg_info->nStatus[i];
			m_friendlist.AddTail(p);
		}
	}

	// 2016��9��2��23:24:39 ��־�� �޸Ľ��� 
	return TRUE;
}


/*********************************************************
�������ƣ�ShowFriendInfo
������������ʾ���ѵĻ�����Ϣ��ID��������״̬��
����ʱ�䣺2016-08-07
����˵����pListCtrl-�б�ؼ�ָ��
�� �� ֵ��
*********************************************************/
int CFriend::ShowFriendInfo(CListCtrl * pListCtrl)const
{
	// ɾ��������
	pListCtrl->DeleteAllItems();
	// ɾ��������
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pListCtrl->DeleteColumn(0);
		}
	}


	// ����ͼ���б�
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

	// ��m_lstctlData��ӱ�ͷ
	CRect rect;
	pListCtrl->GetClientRect(&rect);
	int nColInterval = rect.Width();
	pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); // ���÷��
	
	pListCtrl->SetImageList(ImageList, LVSIL_SMALL ); // ����ͼ���б�
	pListCtrl->InsertColumn(0, _T(""), LVCFMT_CENTER,		int(nColInterval * 0.17));
    pListCtrl->InsertColumn(1, _T("����"), LVCFMT_CENTER,	int(nColInterval * 0.63));	
	pListCtrl->InsertColumn(2, _T("״̬"), LVCFMT_CENTER,	int(nColInterval * 0.2));   
	
	// �������
	int i = 0; // �к�

	LVITEM lvItem={0}; // ��������ͼƬ��
	//��һ������   
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;  //���֡�ͼƬ��״̬   
	lvItem.iItem = i;       //�к�
	lvItem.iImage = 0;		//ͼƬ������
	lvItem.iSubItem = 0;    //���к�

	POSITION pos = m_friendlist.GetHeadPosition();
	CFriendInfo *pinfo = NULL;
	while(pos != NULL)
	{
		pinfo = m_friendlist.GetNext(pos);
		CString strTemp; // ����ת�����ַ�

		strTemp = (LPCTSTR)pinfo->m_Name; // �ǳ����Կ��ַ�������
		if(pinfo->m_bIsHaveMsg)
		{
			strTemp += " ���µ���Ϣ��";
		}
		lvItem.iItem = i;
		pListCtrl->InsertItem(&lvItem);
		pListCtrl->SetItemText(i, 1, strTemp);
		strTemp.LoadStringW(pinfo->m_nStatus);// ״̬������
		pListCtrl->SetItemText(i, 2, strTemp);		
		++i;
	}

	return TRUE;
}


/*********************************************************
�������ƣ�IsExist
�������������ID�Ƿ���� ������ھͷ��ظ�ID��ָ��
����ʱ�䣺2016-08-07
����˵����p-ָ�������Ϣָ���ָ�� nID-�˺�ID
�� �� ֵ��
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
�������ƣ�GetItemID
������������õ�nItem�����ѵ�ID
����ʱ�䣺2016-08-15
����˵����nID-�˺�ID nItem-���
�� �� ֵ��
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
�������ƣ�GetFriendName
�������������nID��Name
����ʱ�䣺2016-08-15
����˵����name-���� nID-�˺�
�� �� ֵ��
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
�������ƣ�InitIsHaveMsg
������������ʼ����Ա����m_bIsHaveMsg ȫ������ΪFALSE
����ʱ�䣺2016-08-15
����˵����
�� �� ֵ��
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
�������ƣ�SetIsHaveMsg
��������������ĳ��ID��m_bIsHaveMsg
����ʱ�䣺2016-08-15
����˵����
�� �� ֵ��
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
�������ƣ�UpdateDetailInfo
��������������ĳ��ID����ϸ��Ϣ
����ʱ�䣺2016-08-29
����˵����msg_info -- ĳ��ID����ϸ��Ϣ
�� �� ֵ��
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
�������ƣ�GetDetailInfo
������������ȡĳ��ID����ϸ��Ϣ
����ʱ�䣺2016-08-29
����˵����msg_info -- ĳ��ID����ϸ��Ϣ
�� �� ֵ��
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
