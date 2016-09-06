
/************************************************
�� �� ����FileSocket.cpp
��    �ߣ���־��
�������ڣ�2016-08-31
��    ;��CFileSocket���ʵ���ļ�
�޸ļ�¼��2016-08-31 �� ��־�� ����
************************************************/ 

// FileSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "FileSocket.h"
#include "MFCChatDlg.h"
#include <math.h>
// CFileSocket

CFileSocket::CFileSocket()
{
	m_pFile = NULL;
	m_pFileRecord = NULL;
	m_ullCount = 0;
	m_ullFileSize = 0;
	m_nLastPacket = 0;
}

CFileSocket::~CFileSocket()
{
}


// CFileSocket ��Ա����

/*********************************************************
�������ƣ�OnReceive
���������������ļ�
��    �ߣ���־��
����ʱ�䣺2016-08-31
����˵����nErrorCode -- �������
*********************************************************/
void CFileSocket::OnReceive(int nErrorCode)
{
	// 2016��8��31��09:19:21�޸�
	//((CMFCChatDlg*)theApp.m_pMainWnd)->RecvFile();
	// �·���
	MSG_FILE_CONTENT msg_content;	
	int nSize = Receive(&msg_content, sizeof(MSG_FILE_CONTENT));

	//�����յ����ļ�д���½����ļ���ȥ
	if (m_pFile == NULL)
	{
		AfxMessageBox(_T("�ļ�������������!"));
		return;
	}

	UINT unWriteSize;
	//if (m_ullFileSize - m_ullCount < FILE_PACKET_SIZE)
	//{
	//	unWriteSize = m_ullFileSize - m_ullCount;
	//} 
	//else
	//{
	//	unWriteSize = FILE_PACKET_SIZE;
	//}
	if (m_nLastPacket == msg_content.nPacketID)
	{// �����ļ������һ���� ����һ��������յ��Ǹ���
		unWriteSize = m_ullFileSize - (m_nLastPacket - 1) * FILE_PACKET_SIZE;
	} 
	else
	{
		unWriteSize = FILE_PACKET_SIZE;
	}

	m_pFileRecord->nRecvPacket = msg_content.nPacketID;

	LONGLONG lOff = msg_content.nPacketID * FILE_PACKET_SIZE;
	m_pFile->Seek(lOff , CFile::begin); // ָ��ƫ�� ���������������
	//m_pFile->Write(msg_content.FileData, unWriteSize);
	m_pFile->Write(msg_content.FileData, msg_content.nPacketSize);

	m_ullCount += unWriteSize;
	m_pFileRecord->nRecvSize = m_ullCount;

	if (m_ullCount >= m_ullFileSize)
	{// �������		
		//Close(); // �ر�socket
		MSG_FILE_OPERA msg_opera;
		memset(&msg_opera, 0, sizeof(MSG_FILE_OPERA));
		msg_opera.nType = MESSAGE_FILE_TRANS_END;
		strcpy_s(msg_opera.ToID, msg_content.FromID); // ���պͷ��ͷ���ת
		strcpy_s(msg_opera.FromID, msg_content.ToID);
		Send(&msg_opera, sizeof(MSG_FILE_OPERA));

		m_pFile->Close();//�ر��ļ�
		
		// ���ñ��ؼ�¼
		m_pFileRecord->nTransState = 3; // ״̬�������
			
		//��ԭ
		m_ullCount = 0;
		m_ullFileSize = 0;
		m_nLastPacket = 0;
		m_pFileRecord = NULL;

		AfxMessageBox(_T("�ɹ������ļ�"));
	}

	CSocket::OnReceive(nErrorCode);
}

/*********************************************************
�������ƣ�InitFileTrans
������������ʼ���ļ����䣨���ļ��ȣ�
��    �ߣ���־��
����ʱ�䣺2016-08-31
����˵����pRecord -- ���ؼ�¼
�� �� ֵ��TRUE -- �ɹ�
		  FALSE -- �ļ�����ʧ��
*********************************************************/
BOOL CFileSocket::InitFileTrans(FILE_RECORD* pRecord)
{
	CString strFileName; // �ļ���
	strFileName = pRecord->FileName;
	CString strFilePath;// �ļ�·��
	strFilePath = pRecord->FilePath;
	if(m_pFile == NULL)
	{
		m_pFile = new CFile();
	}
	if(!m_pFile->Open(strFilePath + "\\" + strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("�ļ�����ʧ�ܣ�"));
		return FALSE;
	}
	m_pFileRecord = pRecord;
	m_ullFileSize = pRecord->nFileSize;
	
	m_nLastPacket = ceil(m_ullFileSize / (double)FILE_PACKET_SIZE);
	return TRUE;
}

/*********************************************************
�������ƣ�OnClose
�����������ر�socketʱִ�е�һЩ����
��    �ߣ���־��
����ʱ�䣺2016-09-01
����˵����nErrorCode -- �������
*********************************************************/
void CFileSocket::OnClose(int nErrorCode)
{
	((CMFCChatDlg *)theApp.m_pMainWnd)->CloseFileTransScoket();
	CSocket::OnClose(nErrorCode);
}
