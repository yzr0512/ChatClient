
/************************************************
�� �� ����ChatDlg.cpp
��    �ߣ���־��
�������ڣ�2016-08-17
��    ;�������족�Ի����ʵ���ļ�
�޸ļ�¼��2016-08-17 ��־�� ����
************************************************/ 

// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

#include <afxsock.h>
#include <afxwin.h>

// CChatDlg �Ի���

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_pParentWnd(pParent)
{
	m_csInputMsg = _T("");
	m_csOutputMsg = _T("");
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_MSG, m_csInputMsg);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_MSG, m_csOutputMsg);
	DDX_Control(pDX, IDC_BUTTON_SEND_MSG, m_BtnSendMsg);
	DDX_Control(pDX, IDC_BUTTON_SEND_FILE, m_BtnSendFile);
	DDX_Control(pDX, IDC_BT_RECV_FILE, m_BtnRecvFile);
	DDX_Control(pDX, IDCANCEL, m_BtnClose);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFileRecord);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_MSG, m_EditCtrlOut);
	DDX_Control(pDX, IDC_EDIT_INPUT_MSG, m_EditInput);
	DDX_Control(pDX, IDC_STATIC_INFO, m_StaticInfo);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND_MSG, &CChatDlg::OnBnClickedButtonSendMsg)
	ON_BN_CLICKED(IDC_BUTTON_SEND_FILE, &CChatDlg::OnBnClickedButtonSendFile)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChatDlg ��Ϣ�������


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString csName;
	csName.Format(L"%s", m_Name);
	//SetWindowText(_T("Chatting with ") + csName);	
	SetWindowText(csName); // ���ô��ڱ���

	this->SetBackgroundColor(RGB(255, 255, 255)); // ���ñ�����ɫ

	// ���ñ�ͷ
	CRect rect;
	m_ListFileRecord.GetClientRect(&rect);
	int nColInterval = rect.Width();
	m_ListFileRecord.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
 
	m_ListFileRecord.InsertColumn(0, _T(""), LVCFMT_CENTER,		int(nColInterval * 0.1));	
	m_ListFileRecord.InsertColumn(1, _T("�ļ���"), LVCFMT_CENTER,		int(nColInterval * 0.6));   
	//m_ListFileRecord.InsertColumn(1, _T("����"), LVCFMT_CENTER,		int(nColInterval * 0.3));   
	m_ListFileRecord.InsertColumn(2, _T("״̬"), LVCFMT_CENTER,		int(nColInterval * 0.3));   
	

	CFont *font = new CFont;
	//CString strFontName;    // ��������   
	//LOGFONT lf;             // LOGFONT����   

	// ��lf�����ֽ�����   
	//memset(&lf, 0, sizeof(LOGFONT));   

	// ��lf�е�Ԫ����������Ϊ�����塱   
	//_tcscpy_s(lf.lfFaceName, 25, _T("΢���ź�"));   

	font->CreateFont(20, //���߼���λ��ʽָ������ĸ߶�
		0, //���߼���λ��ʽָ���������ַ���ƽ�����
		0, //ָ��ƫ�봹�ߺ�X������ʾ���ϵļнǣ���λ��0.1�ȣ�
		0, //ָ���ַ������ߺ�X��֮��ļнǣ���λ��0.1�ȣ�
		FW_NORMAL, //ָ���������
		FALSE, //�ǲ���б��
		FALSE, //�Ӳ����»���
		0, //ָ���Ƿ��������ַ�ͻ��
		ANSI_CHARSET, //ָ��������ַ���
		OUT_DEFAULT_PRECIS, //ָ��������������
		CLIP_DEFAULT_PRECIS, //ָ������ļ�������
		DEFAULT_QUALITY, //ָʾ������������
		DEFAULT_PITCH | FF_SWISS, //ָ������ļ��ͼ���
		_T("΢���ź�") //ָ���������������
		);

	// ��������Ի��򣬳�ʼѡ��������Ϊ�����塱   
	//CFontDialog fontDlg(&lf);   

	// ���m_font�Ѿ�������һ��������Դ�������ͷ���   
	//if (font.m_hObject)   
	//{   
	//	font.DeleteObject();   
	//}   
	// ʹ��ѡ�������LOGFONT�����µ�����   
	//font.CreateFontIndirect(fontDlg.m_cf.lpLogFont);   
	// ��ȡ�༭��IDC_FONT_EDIT��CWndָ�룬������������   
	GetDlgItem(IDC_EDIT_OUTPUT_MSG)->SetFont(font);   
	GetDlgItem(IDC_EDIT_INPUT_MSG)->SetFont(font);  
	m_StaticInfo.SetFont(font);
	
	//RefreshStatic();

	// ����û�ѡ��������Ի����OK��ť�����ȡ��ѡ����������Ʋ���ʾ���༭����   
	//strFontName = fontDlg.m_cf.lpLogFont->lfFaceName;   
	//SetDlgItemText(IDC_FONT_EDIT, strFontName);  
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// �ر����촰��ʱͬʱɾ���������е�Ԫ��
// ��Ҫ����OnCancel()��PostNcDestroy()
void CChatDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	((CMFCChatDlg*)m_pParentWnd)->CloseChatDlg(m_nID);

	// ɾ������
	DestroyWindow();
}
void CChatDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ɾ����ǰ����
	delete this;

	CDialogEx::PostNcDestroy();
}


// �˴������ÿ�ݼ�
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) // ���»س���������Ϣ
		{
			OnBnClickedButtonSendMsg();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


/*********************************************************
�������ƣ�OnBnClickedButtonSendMsg
����������������Ϣ
����ʱ�䣺2016-08-17
��    ע��
*********************************************************/
void CChatDlg::OnBnClickedButtonSendMsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_csInputMsg == "") // �༭����δ��������
	{
		// ������ʾ���༭������ݲ���Ϊ��
		// 
		return;
	}
	
	struct MSG_TRANSPOND msg_chat;
	msg_chat.nType = CHATING_TEXT_MSG;

	CString csMyID; // �ҵ�ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // ����ʱ��
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	
	
	memcpy(msg_chat.ToID, m_nID, ID_MAX);
	memcpy(msg_chat.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2 + 2);
	wcstombs(msg_chat.FromID, csMyID, csMyID.GetLength() + 1);
	wcstombs(msg_chat.Date, csDate, csDate.GetLength() + 1);
	wcstombs(msg_chat.Time, csTime, csTime.GetLength() + 1);

	/*
	memset(msg_chat.ToID, '\0', sizeof(msg_chat.ToID));
	memset(msg_chat.FromID, '\0', sizeof(msg_chat.FromID));
	memset(msg_chat.Date, '\0', sizeof(msg_chat.Date));
	memset(msg_chat.Time, '\0', sizeof(msg_chat.Time));
	memset(msg_chat.Data, '\0', sizeof(msg_chat.Data));
	
	CString csMyID; // �ҵ�ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // ����ʱ��
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	

	memcpy(msg_chat.ToID, m_csID, m_csID.GetLength() * 2);
	memcpy(msg_chat.FromID, csMyID, csMyID.GetLength() * 2);
	memcpy(msg_chat.Date, csDate, csDate.GetLength() * 2);
	memcpy(msg_chat.Time, csTime, csTime.GetLength() * 2);
	memcpy(msg_chat.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2);
	*/
	// ������Ϣ
	
	if(((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_chat, sizeof(msg_chat)))
	{
		// ���ͳɹ��� ��ʾ�ҷ�������
		AddMessage(_T("��"), csTime, m_csInputMsg);
		// �������������
		m_csInputMsg = "";
	}

	// ����Ϣ���µ���Ļ
	UpdateData(FALSE);
	
}


/*********************************************************
�������ƣ�AddMessage
�����������ڱ༭�������Ϣ
����ʱ�䣺2016-08-15
��    ע��
*********************************************************/
int CChatDlg::AddMessage(const CString& csName, const CString& csTime, const CString& csMsg)
{	
	// �����������csName�Ǻ��ѵ�ID ��ID��������
	CString csTemp;
	csTemp = m_nID;
	if(csTemp == csName)	
		csTemp.Format(L"%s", m_Name);			
	else
		csTemp = csName;
	m_csOutputMsg += csTemp;
	m_csOutputMsg += ": ";
	m_csOutputMsg += csTime;
	m_csOutputMsg += "\r\n";
	m_csOutputMsg += csMsg;
	m_csOutputMsg += "\r\n";
	UpdateData(FALSE);
	m_EditCtrlOut.LineScroll(m_EditCtrlOut.GetLineCount());
	return 0;
}


/*********************************************************
�������ƣ�RefreshListCtrlData
����������ˢ���б�ؼ�������
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
int CChatDlg::RefreshListCtrlData(void)
{
	// ɾ��������
	m_ListFileRecord.DeleteAllItems();
	//CList<FILE_RECORD*, FILE_RECORD*> *FileRecordList = &((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList;
	int i = 0;
	//POSITION pos = FileRecordList.GetHeadPosition();
	POSITION pos = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetHeadPosition();
	while (pos != NULL)
	{
		//FILE_RECORD *pRecord = FileRecordList.GetAt(pos);
		FILE_RECORD *pRecord = ((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetAt(pos);
		if(pRecord->bType == true)
		{// ���ͼ�¼
			if(!strcmp(m_nID, pRecord->ToID))
			{
				m_ListFileRecord.InsertItem(i, L"��");
				CString strFileName;
				strFileName = pRecord->FileName;
				m_ListFileRecord.SetItemText(i, 1, strFileName);
				CString strState;
				double dbCent; // �ٷ���
				switch(pRecord->nTransState)
				{
				case 0:
					strState = "�ȴ�����";
					break;
				case 1:
					dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // ���͵İٷֱ�
					strState.Format(L"%.2lf%%", dbCent);
					break;
				case 2:
					strState = "��ͣ";
					break;
				case 3:
					strState = "�����";
					break;
				case 4:
					strState = "ȡ��";
					break;
				case 5:
					strState = "����";
					break;
				case 6:
					strState = "�ܾ�";
					break;
				default:
					strState = "";
					break;
				}
				m_ListFileRecord.SetItemText(i, 2, strState);
			}
		}
		else
		{// ���ܼ�¼
			if(!strcmp(m_nID, pRecord->FromID))
			{
				m_ListFileRecord.InsertItem(i, L"��");
				CString strFileName;
				strFileName = pRecord->FileName;
				m_ListFileRecord.SetItemText(i, 1, strFileName);
				CString strState;
				double dbCent;
				switch(pRecord->nTransState)
				{
				case 0:
					strState = "�ȴ�����";
					break;
				case 1:
					dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // ���ܵİٷֱ�
					strState.Format(L"%.2lf%%", dbCent);
					break;
				case 2:
					strState = "��ͣ";
					break;
				case 3:
					strState = "�����";
					break;
				case 4:
					strState = "ȡ��";
					break;
				case 5:
					strState = "����";
					break;
				case 6:
					strState = "�ܾ�";
				default:
					strState = "";
					break;
				}
				m_ListFileRecord.SetItemText(i, 2, strState);
			}
		}
		((CMFCChatDlg *)theApp.m_pMainWnd)->m_FileRecordList.GetNext(pos);
	}



	return 0;
}

/*********************************************************
�������ƣ�RefreshStatic(δ���)
����������ˢ�¾�̬��ǩ������
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
int CChatDlg::RefreshStatic(MSG_USERINFO * msg_info)
{
	/*
	// 2016��9��1��16:31:01�޸�
	MSG_USERINFO msg_info;
	memset(&msg_info, 0, sizeof(MSG_USERINFO));
	strcpy_s(msg_info.nID, m_nID);
	//((CMFCChatDlg*)GetParent())->m_Friend.GetDetailInfo(&msg_info);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_INFO);
	CString strTemp1, strTemp2;
	strTemp1 = "�˺ţ�";
	strTemp2 = msg_info.nID;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n������";
	strTemp2 = msg_info.Name;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n���䣺";
	strTemp2 = msg_info.Email;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n�Ա�";
	strTemp2 = msg_info.Sex;
	strTemp1 += strTemp2;
	pWnd->SetWindowTextW(strTemp1);
	*/

	CWnd* pWnd = GetDlgItem(IDC_STATIC_INFO);
	CString strTemp1, strTemp2;
	strTemp1 = "�˺ţ�";
	strTemp2 = msg_info->nID;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n������";
	strTemp2.Format(L"%s", msg_info->Name);
	//strTemp2 = msg_info->Name;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n���䣺";
	strTemp2 = msg_info->Email;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n�Ա�";
	if (msg_info->Sex == 'm')
	{
		strTemp2 = "��";
	} 
	else if (msg_info->Sex == 'f')
	{
		strTemp2 = "Ů";
	}
	else
	{
		strTemp2 = "δ֪";
	}
	strTemp1 += strTemp2;
	pWnd->SetWindowTextW(strTemp1);

	pWnd->Invalidate();

	return 0;
}

/*********************************************************
�������ƣ�OnBnClickedButtonSendFile
�������������ʹ����ļ�����
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
void CChatDlg::OnBnClickedButtonSendFile()
{
	// ������ļ��Ի���
	CFileDialog fileDlg(TRUE); 
	
	CString strFilePath;
	// ��ʾ���ļ��Ի���
    if (IDOK == fileDlg.DoModal())   
    {   
		strFilePath = fileDlg.GetPathName();// �����ļ�·�����ļ���
	}
	else
	{
		return;
	}

	CFile file;

	if(!file.Open(strFilePath, CFile::modeRead))
	{
		MessageBox(L"ѡ���ļ�����");
		return;
	}
	CString strFileName;
	strFileName = file.GetFileName(); // �ļ���
	//strFilePath = file.GetFilePath(); // �ļ�·��
	strFilePath = strFilePath.Left(strFilePath.GetLength() - strFileName.GetLength() - 1);

	MSG_FILE_REQUEST msg_file;
	memset(&msg_file, 0, sizeof(MSG_FILE_REQUEST)); // �ṹ�����
	// �����ļ���������ṹ��
	msg_file.nType = MESSAGE_FILE_REQUEST;
	strcpy_s(msg_file.ToID, m_nID);	// �Է�ID
	CString strMyID = ((CMFCChatDlg*)theApp.m_pMainWnd)->m_csMyID; // �ҵ�ID
	WideCharToMultiByte(CP_ACP, 0, strMyID, strMyID.GetLength() + 1, msg_file.FromID, ID_MAX, NULL, NULL);
	msg_file.nFileID = ((CMFCChatDlg*)theApp.m_pMainWnd)->GetAvailFileID(); // ��ÿ��õ��ļ�ID
	//WideCharToMultiByte(CP_ACP, 0, strFilePath, strFilePath.GetLength() + 1, msg_file.FileName, 512, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strFileName, strFileName.GetLength() + 1, msg_file.FileName, 512, NULL, NULL); // �ļ���
	msg_file.nFileSize = file.GetLength(); // �ļ���С

	((CMFCChatDlg*)theApp.m_pMainWnd)->SendMsg(&msg_file, sizeof(MSG_FILE_REQUEST)); // ������Ϣ

	// ���ؼ�¼����
	FILE_RECORD *pFile_record = new FILE_RECORD;
	memset(pFile_record, 0, sizeof(FILE_RECORD));// �ṹ�����
	pFile_record->bType = true; // ��¼���� ��Ϊ ����
	strcpy_s(pFile_record->ToID, m_nID);	// �Է�ID
	strcpy_s(pFile_record->FromID, msg_file.FromID); // �ҵ�ID
	WideCharToMultiByte(CP_ACP, 0, strFilePath, strFilePath.GetLength() + 1, pFile_record->FilePath, 512, NULL, NULL);// �ļ�·��
	WideCharToMultiByte(CP_ACP, 0, strFileName, strFileName.GetLength() + 1, pFile_record->FileName, 512, NULL, NULL);// �ļ���
	pFile_record->nFileID = msg_file.nFileID; // �ļ�ID
	pFile_record->nFileSize = file.GetLength(); // �ļ���С
	
	((CMFCChatDlg*)theApp.m_pMainWnd)->m_FileRecordList.AddTail(pFile_record); // ����¼��ӵ�������
	
	file.Close(); // �ر��ļ�
}

/*********************************************************
�������ƣ�OnCtlColor
��������������ֻ���༭�����ɫ
���ߣ�	  ��־��
����ʱ�䣺2016-08-29
����˵����
�� �� ֵ��
*********************************************************/
HBRUSH CChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{// �ı�ֻ���༭�����ɫ
		// һ��Ҫ����һ���� ��RGBû�� ���ԡ�������
		pDC->SetBkMode(TRANSPARENT);
		CBrush brush;
		brush.CreateSolidBrush(RGB(255,255,255));
		hbr = (HBRUSH)brush;
	}
/*
	// TODO: Change any attributes of the DC here
	switch (nCtlColor) //������ͬһ���͵Ŀؼ������ж�
	{ 
	// process my edit controls by ID.
		case CTLCOLOR_EDIT:
		case CTLCOLOR_STATIC:
	switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�
	{     
		// first CEdit control ID
	case IDC_EDIT1:         // ��һ���ı���
		// here
		pDC->SetBkColor(m_bluecolor);    // change the background
		// color [background colour
		// of the text ONLY]
		pDC->SetTextColor(m_textcolor); // change the text color
		hbr = (HBRUSH) m_bluebrush;    // apply the blue brush
		// [this fills the control
		// rectangle]
		break;   
		// second CEdit control ID
	case IDC_EDIT2:         // �ڶ����ı���
		// but control is still
		// filled with the brush
		// color!
		pDC->SetBkMode(TRANSPARENT);   // make background
		// transparent [only affects
		// the TEXT itself]
		pDC->SetTextColor(m_textcolor); // change the text color
		hbr = (HBRUSH) m_redbrush;     // apply the red brush
		// [this fills the control
		// rectangle]
		break;
	default: 
		hbr=CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
		break;
	} 
	break;
}
// TODO: Return a different brush if the default is not desired
return hbr;

*/
	return hbr;
}

// ��ʱˢ���ļ���¼�б�ؼ�������
void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	RefreshListCtrlData();

	CDialogEx::OnTimer(nIDEvent);
}
