
// MFCChat.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Struct.h"
#include "MyButton.h"

// CMFCChatApp:
// �йش����ʵ�֣������ MFCChat.cpp
//

class CMFCChatApp : public CWinApp
{
public:
	CMFCChatApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CMFCChatApp theApp;
