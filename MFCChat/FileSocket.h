
/************************************************
�� �� ����FileSocket.h
��    �ߣ���־��
�������ڣ�2016-08-31
��    ;��CFileSocket���ͷ�ļ�
�޸ļ�¼��2016-08-31 �� ��־�� ����
************************************************/ 

#pragma once


/************************************************
��    ����CFileSocket
��    �ߣ���־��
�������ڣ�2016-08-31
���������������ר�����������ļ�
�޸ļ�¼��2016-08-31 by ��־�� ����
************************************************/

// CFileSocket ����Ŀ��

class CFileSocket : public CSocket
{
public:
	CFileSocket();
	virtual ~CFileSocket();
	BOOL InitFileTrans(FILE_RECORD* pRecord); // ��ʼ������ �����ļ��ȵȣ�
	virtual void OnReceive(int nErrorCode); // ������Ϣ
	virtual void OnClose(int nErrorCode);

private:
	CFile* m_pFile; // �����ļ��õ�ָ��
	FILE_RECORD* m_pFileRecord; // ���صļ�¼

	ULONGLONG m_ullCount; // �Ѿ����յ��ֽ���
	ULONGLONG m_ullFileSize; // �ļ����ܴ�С
	int m_nLastPacket; // �ļ����һ������ID  ����Ŀǰ�������һ��������	
};


