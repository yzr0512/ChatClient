#pragma once

// CChatSocket ÃüÁîÄ¿±ê

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
protected:
	CWnd *m_pParentWnd;

};


