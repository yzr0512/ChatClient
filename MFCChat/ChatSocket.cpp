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
}

CChatSocket::~CChatSocket()
{
}


// CChatSocket ��Ա����


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	((CMFCChatDlg*)m_pParentWnd)->RecvMsg();

	CSocket::OnReceive(nErrorCode);
}


void CChatSocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnClose(nErrorCode);
}
