
/************************************************
�� �� ����ChatSocket.h
��    �ߣ���־��
�������ڣ�2016-08-03
��    ;��CChatSocket���ͷ�ļ�
�޸ļ�¼��2016-08-03 ��־�� ����
************************************************/ 

#pragma once


/************************************************
��    ����CChatSocket
��    �ߣ���־��
�������ڣ�2016-08-03
��������������������������ͨ��
�޸ļ�¼��2016-08-03 by ��־�� ����
************************************************/
// CChatSocket ����Ŀ��

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	virtual void OnReceive(int nErrorCode); // ����
	virtual void OnClose(int nErrorCode); // �ر�

	CTime m_tmLastMsg; // ���һ��ͨ��ʱ��
protected:
	CWnd *m_pParentWnd; // ������ָ��

};


