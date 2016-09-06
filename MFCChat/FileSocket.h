
/************************************************
文 件 名：FileSocket.h
作    者：余志荣
创建日期：2016-08-31
用    途：CFileSocket类的头文件
修改记录：2016-08-31 由 余志荣 创建
************************************************/ 

#pragma once


/************************************************
类    名：CFileSocket
作    者：余志荣
创建日期：2016-08-31
功能描述：这个类专门用来接收文件
修改记录：2016-08-31 by 余志荣 创建
************************************************/

// CFileSocket 命令目标

class CFileSocket : public CSocket
{
public:
	CFileSocket();
	virtual ~CFileSocket();
	BOOL InitFileTrans(FILE_RECORD* pRecord); // 初始化传输 （打开文件等等）
	virtual void OnReceive(int nErrorCode); // 接收信息
	virtual void OnClose(int nErrorCode);

private:
	CFile* m_pFile; // 操作文件用的指针
	FILE_RECORD* m_pFileRecord; // 本地的记录

	ULONGLONG m_ullCount; // 已经接收的字节数
	ULONGLONG m_ullFileSize; // 文件的总大小
	int m_nLastPacket; // 文件最后一个包的ID  不是目前接收最后一个包！！	
};


