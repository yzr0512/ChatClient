

/************************************************
文 件 名：ChatSocket.cpp
作    者：余志荣
创建日期：2016-08-03
用    途：CChatSocket类的实现文件
修改记录：2016-08-03 余志荣 创建
************************************************/ 

// ChatSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "ChatSocket.h"
#include "MFCChatDlg.h"

// CChatSocket

CChatSocket::CChatSocket(CWnd *pWnd)
{
	m_pParentWnd = pWnd;
	m_tmLastMsg = CTime::GetTickCount();
}

CChatSocket::~CChatSocket()
{
}


// CChatSocket 成员函数

// 接收
void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tmLastMsg = CTime::GetTickCount();
	((CMFCChatDlg*)m_pParentWnd)->RecvMsg();

	CSocket::OnReceive(nErrorCode);
}

// 关闭
void CChatSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CMFCChatDlg*)m_pParentWnd)->CloseChatSocket();
	CSocket::OnClose(nErrorCode);
}
