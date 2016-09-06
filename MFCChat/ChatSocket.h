
/************************************************
文 件 名：ChatSocket.h
作    者：余志荣
创建日期：2016-08-03
用    途：CChatSocket类的头文件
修改记录：2016-08-03 余志荣 创建
************************************************/ 

#pragma once


/************************************************
类    名：CChatSocket
作    者：余志荣
创建日期：2016-08-03
功能描述：这个类用来与服务器通信
修改记录：2016-08-03 by 余志荣 创建
************************************************/
// CChatSocket 命令目标

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	virtual void OnReceive(int nErrorCode); // 接收
	virtual void OnClose(int nErrorCode); // 关闭

	CTime m_tmLastMsg; // 最后一次通信时间
protected:
	CWnd *m_pParentWnd; // 主窗口指针

};


