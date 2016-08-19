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


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tmLastMsg = CTime::GetTickCount();
	((CMFCChatDlg*)m_pParentWnd)->RecvMsg();

	CSocket::OnReceive(nErrorCode);
}


void CChatSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CSocket::OnClose(nErrorCode);
}
