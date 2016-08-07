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
	m_pChatDlg = NULL;
}
CFriendInfo::~CFriendInfo()
{
	
}



CFriend::CFriend(void)
{
}


CFriend::~CFriend(void)
{
}


int CFriend::UpdateBasicInfo(MSG_FRND_INFO* msg_info)
{
	if(msg_info == NULL)
	{
		return FALSE;
	}
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
	return 0;
}


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

	// 给m_lstctlData添加表头
	CRect rect;
	pListCtrl->GetClientRect(&rect);
	int nColInterval = rect.Width();
	pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//pListCtrl->InsertColumn(0, _T(""), LVCFMT_CENTER,			int(nColInterval / 7));   
    pListCtrl->InsertColumn(0, _T("Name"), LVCFMT_CENTER,		int(nColInterval * 0.8));	
	pListCtrl->InsertColumn(1, _T("Status"), LVCFMT_CENTER,		int(nColInterval * 0.2));   
	// 添加数据
	int i = 0;
	POSITION pos = m_friendlist.GetHeadPosition();
	CFriendInfo *pinfo = NULL;
	while(pos != NULL)
	{
		pinfo = m_friendlist.GetNext(pos);
		CString strTemp; // 用于转换多字符

		//strTemp = pinfo->nID;	// nID是多字符
		//pListCtrl->InsertItem(i, strTemp);
		strTemp = (LPCTSTR)pinfo->m_Name; // 昵称是以宽字符来储存
		pListCtrl->InsertItem(i, strTemp);		
		strTemp.LoadStringW(pinfo->m_nStatus);// 状态是数字
		pListCtrl->SetItemText(i, 1, strTemp);
		++i;
	}

	return TRUE;
}


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

