
/************************************************
文 件 名：FileSocket.cpp
作    者：余志荣
创建日期：2016-08-31
用    途：CFileSocket类的实现文件
修改记录：2016-08-31 由 余志荣 创建
************************************************/ 

// FileSocket.cpp : 实现文件
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


// CFileSocket 成员函数

/*********************************************************
函数名称：OnReceive
功能描述：接收文件
作    者：余志荣
创建时间：2016-08-31
参数说明：nErrorCode -- 错误代码
*********************************************************/
void CFileSocket::OnReceive(int nErrorCode)
{
	// 2016年8月31日09:19:21修改
	//((CMFCChatDlg*)theApp.m_pMainWnd)->RecvFile();
	// 新方法
	MSG_FILE_CONTENT msg_content;	
	int nSize = Receive(&msg_content, sizeof(MSG_FILE_CONTENT));

	//将接收到的文件写到新建的文件中去
	if (m_pFile == NULL)
	{
		AfxMessageBox(_T("文件创建发生错误!"));
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
	{// 这是文件的最后一个包 但不一定是最后收的那个包
		unWriteSize = m_ullFileSize - (m_nLastPacket - 1) * FILE_PACKET_SIZE;
	} 
	else
	{
		unWriteSize = FILE_PACKET_SIZE;
	}

	m_pFileRecord->nRecvPacket = msg_content.nPacketID;

	LONGLONG lOff = msg_content.nPacketID * FILE_PACKET_SIZE;
	m_pFile->Seek(lOff , CFile::begin); // 指针偏移 解决包的乱序问题
	//m_pFile->Write(msg_content.FileData, unWriteSize);
	m_pFile->Write(msg_content.FileData, msg_content.nPacketSize);

	m_ullCount += unWriteSize;
	m_pFileRecord->nRecvSize = m_ullCount;

	if (m_ullCount >= m_ullFileSize)
	{// 接收完成		
		//Close(); // 关闭socket
		MSG_FILE_OPERA msg_opera;
		memset(&msg_opera, 0, sizeof(MSG_FILE_OPERA));
		msg_opera.nType = MESSAGE_FILE_TRANS_END;
		strcpy_s(msg_opera.ToID, msg_content.FromID); // 接收和发送方反转
		strcpy_s(msg_opera.FromID, msg_content.ToID);
		Send(&msg_opera, sizeof(MSG_FILE_OPERA));

		m_pFile->Close();//关闭文件
		
		// 设置本地记录
		m_pFileRecord->nTransState = 3; // 状态：已完成
			
		//复原
		m_ullCount = 0;
		m_ullFileSize = 0;
		m_nLastPacket = 0;
		m_pFileRecord = NULL;

		AfxMessageBox(_T("成功接收文件"));
	}

	CSocket::OnReceive(nErrorCode);
}

/*********************************************************
函数名称：InitFileTrans
功能描述：初始化文件传输（打开文件等）
作    者：余志荣
创建时间：2016-08-31
参数说明：pRecord -- 本地记录
返 回 值：TRUE -- 成功
		  FALSE -- 文件创建失败
*********************************************************/
BOOL CFileSocket::InitFileTrans(FILE_RECORD* pRecord)
{
	CString strFileName; // 文件名
	strFileName = pRecord->FileName;
	CString strFilePath;// 文件路径
	strFilePath = pRecord->FilePath;
	if(m_pFile == NULL)
	{
		m_pFile = new CFile();
	}
	if(!m_pFile->Open(strFilePath + "\\" + strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("文件创建失败！"));
		return FALSE;
	}
	m_pFileRecord = pRecord;
	m_ullFileSize = pRecord->nFileSize;
	
	m_nLastPacket = ceil(m_ullFileSize / (double)FILE_PACKET_SIZE);
	return TRUE;
}

/*********************************************************
函数名称：OnClose
功能描述：关闭socket时执行的一些操作
作    者：余志荣
创建时间：2016-09-01
参数说明：nErrorCode -- 错误代码
*********************************************************/
void CFileSocket::OnClose(int nErrorCode)
{
	((CMFCChatDlg *)theApp.m_pMainWnd)->CloseFileTransScoket();
	CSocket::OnClose(nErrorCode);
}
