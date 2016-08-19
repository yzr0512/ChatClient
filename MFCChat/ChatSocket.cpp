// ChatSocket.cpp : ʵ���ļ�
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


// CChatSocket ��Ա����


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_tmLastMsg = CTime::GetTickCount();
	((CMFCChatDlg*)m_pParentWnd)->RecvMsg();

	CSocket::OnReceive(nErrorCode);
}


void CChatSocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CSocket::OnClose(nErrorCode);
}
