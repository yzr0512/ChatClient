

/************************************************
�� �� ����ChatSocket.cpp
��    �ߣ���־��
�������ڣ�2016-08-03
��    ;��CChatSocket���ʵ���ļ�
�޸ļ�¼��2016-08-03 ��־�� ����
************************************************/ 

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

// ����
void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_tmLastMsg = CTime::GetTickCount();
	((CMFCChatDlg*)m_pParentWnd)->RecvMsg();

	CSocket::OnReceive(nErrorCode);
}

// �ر�
void CChatSocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	((CMFCChatDlg*)m_pParentWnd)->CloseChatSocket();
	CSocket::OnClose(nErrorCode);
}
