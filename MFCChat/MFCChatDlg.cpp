
// MFCChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCChat.h"
#include "MFCChatDlg.h"
#include "afxdialogex.h"
#include "RegistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCChatDlg �Ի���

CMFCChatDlg::CMFCChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCChatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_csMyName = _T("");

	m_pAddFriendDlg = NULL;
	m_pLoginDlg = NULL;
	m_csMyID = _T("");
	m_pSocketChat = NULL;

}

void CMFCChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_lstctlFriend);
	DDX_Control(pDX, IDC_COMBO_STATE, m_cbState);
	DDX_Text(pDX, IDC_STATIC_USERNAME, m_csMyName);
	DDV_MaxChars(pDX, m_csMyName, 20);
}

BEGIN_MESSAGE_MAP(CMFCChatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIEND, &CMFCChatDlg::OnNMDblclkListFriend)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FRIEND_DLG, &CMFCChatDlg::OnBnClickedButtonAddFriendDlg)
	ON_BN_CLICKED(IDC_BT_DEL_FRIEND, &CMFCChatDlg::OnBnClickedBtDelFriend)
END_MESSAGE_MAP()


// CMFCChatDlg ��Ϣ��������

BOOL CMFCChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	
	//SocketInit(); // ��ʼ��Socket
	
	// �򿪵�¼����
	// �����½���ڷ���2����ʾ��½����ֻ�Ǳ��ر� ������Ҳ�ر�	
	if(m_pLoginDlg == NULL)
	{
		m_pLoginDlg = new CLoginDlg(this);
		m_pLoginDlg->Create(IDD_LOGIN_DLG);
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
	ShowWindow(SW_SHOW);
	/*
	int nRes;	
	nRes = m_dlgLogin.DoModal();
	if (nRes != LOGIN_SUCCESS)
	{	
		EndDialog(2);
	}
	
	UpdateData(TRUE);
	// ��ʾ�û���
	m_csMyName = m_dlgLogin.m_csID;
	*/

	// �����б�
	//  �����б��ĸ�ʽ
	InitListCtrlStyle();
	//  �����б�������
	RefreshListCtrlData();

	// ��������״̬
	m_cbState.SelectString(0, _T("����"));
		
	UpdateData(FALSE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCChatDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//˫������ʱ�����촰��
void CMFCChatDlg::OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	*pResult = 0;

	// ��ָ��ת��Ϊ���ײ����Ľṹ����
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	// ������촰�ڵ����
	if(-1 != pNMListView->iItem) // ���iItem����-1����˵�����б��ѡ�� 
	{
		CString csFrndName; // ��ѡ�к��ѵ�����
		//CFriendInfo *pFriendInfo;
		//POSITION pos = m_listFriendInfo.GetHeadPosition();
		for(int i = 0; i != pNMListView->iItem; ++i)
		{
			m_listFriendInfo.GetNext(pos);
		}
		pFriendInfo = m_listFriendInfo.GetAt(pos);
		csFriend = pFriendInfo->csID;
		//csFriend = m_lstctlFriend.GetItemText(pNMListView->iItem, 0);

		// Ѱ���Ƿ��Ѵ�����ú��ѵ�����Ի���
		//  nFlag = 1��ʾ���ڣ�nFlag = 0��ʾ������
		//  ��������ִ���κβ�������������������һ���Ի���
		int nFlag = 0;
		for each(auto p in m_vecpChatDlg) 
		{
			if(p->m_csID == csFriend)
			{
				nFlag = 1;
			}
		}
		if(nFlag == 0) // �������뱻ѡ�к��ѵĶԻ����½�һ��
		{
			CChatDlg* pChatDlg;
			pChatDlg = new CChatDlg();
			
			// ��ѡ���ĺ��Ѵ��ݸ�����Ի���
			pChatDlg->m_csID = csFriend;
			
			m_vecpChatDlg.push_back(pChatDlg);
			// ������ģ̬�Ի���
			pChatDlg->Create(IDD_CHAT_DLG, this);
			// ��ʾ�Ի���
			pChatDlg->ShowWindow(SW_SHOW);

			RefreshChatDlgMsg(); // ��ʾδ���յ���Ϣ
			RefreshListCtrlData(); // �������Ի�����б��ؼ�
		}
	}
	*/
}

//���Ӻ���
void CMFCChatDlg::OnBnClickedButtonAddFriendDlg()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	
	if(m_pAddFriendDlg == NULL)
	{
		m_pAddFriendDlg = new CAddFriendDlg(this);
		m_pAddFriendDlg->Create(IDD_ADD_FRIEND_DLG);
		m_pAddFriendDlg->ShowWindow(SW_SHOW);
	}

}

// ��ʼ�������б��ı�ͷ
int CMFCChatDlg::InitListCtrlStyle(void)
{
	// ��ȡ�б��ؼ��Ŀ���
	CRect rect;
	m_lstctlFriend.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// ����ListCtrl����ʽ
	//  LVS_EX_GRIDLINES ������
	//  LVS_EX_FULLROWSELECT ����ѡ��
	m_lstctlFriend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// �����ͷ
	// �����ֱ�Ϊ�����������ڼ��У����������е����ָ�ʽ���п�����������������������
	//  LVCFMT_CENTER ����
	//  LVCFMT_LEFT   �����
	//  LVCFMT_RIGHT  �Ҷ���
	m_lstctlFriend.InsertColumn(0, _T("������"), LVCFMT_CENTER, int(nColInterval  * 0.75));   
    m_lstctlFriend.InsertColumn(1, _T("״̬"), LVCFMT_CENTER, int(nColInterval * 0.25));   

	return 0;
}

// ���º����б�����	
int CMFCChatDlg::RefreshListCtrlData(void)
{	/*
	// ���б���ͼ�ؼ��в����б���������б������ı�   
	POSITION pos = m_listFriendInfo.GetHeadPosition();
	int i = 0;
	while(pos != NULL)
	{
		CString csAdd; // ������Ϣ
		CFriendInfo *pFriendInfo;
		pFriendInfo = m_listFriendInfo.GetNext(pos);

		// ����Ƿ�������Ϣδ����
		POSITION posMsg = m_listChatMsg.GetHeadPosition();
		while(posMsg != NULL)
		{
			struct MSG_TRANSPOND *p;
			p = m_listChatMsg.GetNext(posMsg);
			CString csFormID;
			csFormID.Format(_T("%s"), p->FormID);
			if(csFormID == pFriendInfo->csID)
			{
				csAdd = "������Ϣ��";
				break;
			}			
		}
		m_lstctlFriend.InsertItem(i, pFriendInfo->csID + csAdd); //��Ӧ������ʾname��
		switch(pFriendInfo->nStatus)
		{
		case STATUS_ONLINE:
			m_lstctlFriend.SetItemText(i, 1, _T("����"));
			break;
		case STATUS_BUSY:
			m_lstctlFriend.SetItemText(i, 1, _T("æµ"));
			break;
		case STATUS_OFFLINE:
			m_lstctlFriend.SetItemText(i, 1, _T("����"));
			break;
		default:
			m_lstctlFriend.SetItemText(i, 1, _T("�������˹�����"));
			break;
		}

		i++;
	
	}
*/
	return 0;
}


/*********************************************************
�������ƣ�SocketInit
������������ʼ��Socket
�� �� ֵ���ɹ�����TRUE
��    ע��
*********************************************************/
bool CMFCChatDlg::SocketInit(void)
{
	if(m_pSocketChat != NULL)
	{
		return TRUE;
	}

	m_pSocketChat = new CChatSocket(this);

	if(m_pSocketChat == NULL)
	{
		//�׽��ֳ�ʼ��ʧ��
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("�׽��ֳ�ʼ��ʧ�ܣ�"));
		return FALSE;
	}

	if(!m_pSocketChat->Create())
	{
		//����ʧ��
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("�׽��ִ���ʧ�ܣ�"));
		return FALSE;
	}
	
	if(m_pSocketChat->Connect(_T("127.0.0.1"), 8080) == 0)
	{
		//���ӷ�����ʧ��
		delete m_pSocketChat;
		m_pSocketChat = NULL;
		MessageBox(_T("���ӷ�����ʧ�ܣ�"));
		return FALSE;
	}
	return TRUE;
}

// ������Ϣ
int CMFCChatDlg::RecvMsg(void)
{
	int nBugLen = 512;
	struct MSG_T msg_recv; // ���յ�������
	int nRes; // ���յ����ֽ���
	nRes = m_pSocketChat->Receive((char*)&msg_recv, nBugLen);

	if(nRes == 0) // û�н��յ�����
	{
		return 0;
	}
	
	switch(msg_recv.nType)
	{
	case LOGIN_SUCCESS: // ��¼�ɹ�
		LoginSuccess((struct MSG_USERINFO*)&msg_recv);
		break;	
	case CHATING_TEXT_MSG:
		struct MSG_TRANSPOND *chatmsg;
		chatmsg = (MSG_TRANSPOND *)&msg_recv;
		m_listChatMsg.AddTail(chatmsg);		
		RefreshChatDlgMsg();
		RefreshListCtrlData();
		break;
	case REGISTER_SUCCESS:
		//m_dlgLogin.m_dlgRegist.RegisterSuccess((struct MSG_REGISTER*)&msg_recv);
		m_pLoginDlg->m_pRegDlg->RegisterSuccess((struct MSG_REGISTER*)&msg_recv);
		break;
	case REGISTER_FAILED: // ע��ʧ��
		/*CString csMsg;
		csMsg.Format(_T("%s"),msg_recv.Data);		
		MessageBox(csMsg);*/
		break;
	case IDS_SYSTEM_MESSAGE: // ϵͳ��Ϣ
		SystemMessage((struct MSG_SYS*)&msg_recv);
		break;
	case GET_STRANGER_INFO:
		m_pAddFriendDlg->ShowStrangerInfo((struct MSG_USERINFO *)&msg_recv);
		break;
	case ADD_FRIEND_REQUEST:
		RecvAddFriendRequest((struct MSG_TRANSPOND*)&msg_recv);
		break;
	case GET_FRIEND_INFO_ALL:
		UpdateFriendInfo((MSG_FRND_INFO*)&msg_recv);
		break;
	default:
		break;
	}

/*
	if(msg_recv.nType == LOGIN_SUCCESS) // ��¼�ɹ�
	{
		UpdateData(TRUE);
		m_csMyID = m_dlgLogin.m_csID;		
		m_dlgLogin.EndDialog(LOGIN_SUCCESS);
		return 0;
	}

	if(msg_recv.nType == LOGIN_FAILED) // ��¼ʧ��
	{
		CString strMsg;
		strMsg.Format(_T("%s"), msg_recv.Data);
		MessageBox(strMsg);
		return 0;
	}

	if(msg_recv.nType == CHATING_TEXT_MSG) // ������Ϣ
	{
		struct MSG_TRANSPOND *chatmsg;
		chatmsg = (MSG_TRANSPOND *)&msg_recv;

		m_listChatMsg.AddTail(chatmsg);
		
		RefreshChatDlgMsg();
		RefreshListCtrlData();
		
		return 0;
	}
*/
/*
	if(msg_recv.nType == GET_FRIEND_NUM) // ����ȫ��������Ϣ
	{
		// �������ȴ�һ����Ϣ ���������ж��ٸ�����
		int total = ((struct msg_friend_num*)&msg_recv)->nTotal;
		for(int i = 0; i != total; ++i)
		{
			nRes = m_pSocketChat->Receive((char*)&msg_recv, nBugLen);
			// ����������д

			InitFriendInfo((struct msg_friend*)&msg_recv);
		}
		return 0;
	
	}
*/
	/*
	if(msg_recv.nType == GET_STRANGER_INFO) // İ������Ϣ ���Ӻ��ѶԻ���
	{
		m_pAddFriendDlg->RefreshListCtrlData((struct msg_friend*)&msg_recv);
		return 0;
	}
	*/
/*
	if(msg_recv.nType == ADD_FRIEND_SUCCESS) // �������ӳɹ�
	{
		struct msg_friend * pMsgFriend = (struct msg_friend*)&msg_recv;		
		class CFriendInfo friendinfo;
		friendinfo.csID.Format(_T("%s"), pMsgFriend->ID); // ID
		friendinfo.csName.Format(_T("%s"), pMsgFriend->Name);// ����
		friendinfo.nStatus = pMsgFriend->nStatus; // ����״̬
		friendinfo.num = m_listFriendInfo.GetCount(); // ���
		m_listFriendInfo.AddTail(&friendinfo); // ���ӵ�����
		
		RefreshListCtrlData(); // ���µ��ؼ�

		MessageBox(_T("���ӳɹ���"));

		m_pAddFriendDlg->EndDialog(2);

		return 0;
	}
*/
	/*
	if(msg_recv.nType == ADD_FRIEND_FAILED) // ��������ʧ��
	{
		CString csMsg;
		csMsg.Format(_T("%s"),msg_recv.Data);
		
		MessageBox(csMsg);
		return 0;
	}
	*/
	return 0;
}

// ������Ϣ
int CMFCChatDlg::SendMsg(void *msg, int nBufLen)
{
	if(m_pSocketChat == NULL)
	{
		SocketInit();
	}

	int nRes;
	nRes = m_pSocketChat->Send(msg, nBufLen);
	if(nRes == 0)
	{
		MessageBox(_T("��Ϣ����ʧ�ܣ�"));
	}
	return nRes;
}

// ˢ�����촰�ڵ���Ϣ
int CMFCChatDlg::RefreshChatDlgMsg(void)
{
	// ������һ��
	//   ������촰���Ѵ� ����Ϣ��������촰��
	POSITION pos = m_listChatMsg.GetHeadPosition();
	struct MSG_TRANSPOND *pChatMsg;

	int nFlag = 0;
	while(pos != NULL)
	{
		pChatMsg = m_listChatMsg.GetNext(pos); // ���ص�ǰλ�õ�Ԫ�� ����posָ����һ��Ԫ��
		CString csFormID;
		csFormID.Format(_T("%s"), pChatMsg->FromID);
		for each(auto p in m_vecpChatDlg)
		{
			if(csFormID == p->m_csID) // ���ID������Ի����Ѿ���
			{
				nFlag = 1;
				CString csMsg, csDate, csTime;
			
				csMsg.Format(_T("%s"), pChatMsg->Data);
				csDate.Format(_T("%s"), pChatMsg->Date);
				csTime.Format(_T("%s"), pChatMsg->Time);
				CTime tm = CTime::GetCurrentTime();
				if(tm.Format(_T("%Y-%m-%d")) == csDate)
				{	
					// �������
					p->AddMessage(csFormID, csTime, csMsg);
				}
				else
				{
					// �������
					p->AddMessage(csFormID, csDate + " " + csTime, csMsg);
				}
				
				POSITION pos1 = pos;

				delete pChatMsg; // �ͷŽṹ��ռ�õ��ڴ�
				m_listChatMsg.GetPrev(pos1); //��pos1ָ��ǰԪ��
				m_listChatMsg.RemoveAt(pos1); //����Ϣ���Ƴ�
				// ɾ��Ԫ��֮�� Ҫ���±���һ�� ����break ���� ����ʱ����pos1���
				//pos = m_listChatMsg.GetHeadPosition();

			}
		}		
	}
	// ˢ���б��ؼ��������Ϣ
	RefreshListCtrlData();

	return 0;
}

// �ӷ�������ȡ������Ϣ
int CMFCChatDlg::InitFriendInfo(struct msg_friend *pMsgFriend)
{
	/*
	// �������ΪNULL �����m_listFriendInfo
	// Ȼ�������������һ����ȡ������Ϣ����Ϣ
	if(pMsgFriend == NULL)
	{
		m_listFriendInfo.RemoveAll(); // ɾ�����к���
		struct msg_friend_num msg;
		msg.nType = GET_FRIEND_INFO_ALL;
		SendMsg(&msg);
		return 0;
	}

	// ���������ΪNULL ����m_listFriendInfo��������Ԫ��
	class CFriendInfo friendinfo;
	friendinfo.csID.Format(_T("%s"), pMsgFriend->ID); // ID
	friendinfo.csName.Format(_T("%s"), pMsgFriend->Name);// ����
	friendinfo.nStatus = pMsgFriend->nStatus; // ����״̬
	friendinfo.num = m_listFriendInfo.GetCount(); // ���
	m_listFriendInfo.AddTail(&friendinfo);
	*/
	return 0;
}

void CMFCChatDlg::OnBnClickedBtDelFriend()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	
}


// �Ի���ر�ʱ����������������
void CMFCChatDlg::OnCancel()
{
	// TODO: �ڴ�����ר�ô����/����û���
	if(m_csMyID != "")
	{
		LoginOut();
	}
	DestroyWindow();
	//CDialogEx::OnCancel();
}
void CMFCChatDlg::PostNcDestroy()
{
	// TODO: �ڴ�����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


/*********************************************************
�������ƣ�LoginSuccess
������������¼�ɹ�
����˵����msg_info-��½�ɹ�ʱ���������ص��û���Ϣ
�� �� ֵ��
��    ע��
*********************************************************/
int CMFCChatDlg::LoginSuccess(MSG_USERINFO* msg_info)
{
	UpdateData(TRUE);
	m_csMyID = m_pLoginDlg->m_csID;

	//m_dlgLogin.EndDialog(LOGIN_SUCCESS);
	m_pLoginDlg->OnCancel(); // �رյ�¼����
	m_pLoginDlg = NULL;
	m_csMyName = (LPCTSTR)msg_info->Name; // �û��ǳ�
	//MessageBox(L"��½�ɹ���");
	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	return 0;
}


/*********************************************************
�������ƣ�LoginOut
�����������˳���¼
����˵����
�� �� ֵ��
��    ע��
**************** *****************************************/
int CMFCChatDlg::LoginOut(void)
{
	struct MSG_LOGIN msg_lg;
	msg_lg.nType = LOGIN_OUT;
	msg_lg.nStatus = STATUS_OFFLINE;
	size_t i;
	wcstombs_s(&i, msg_lg.nID, m_csMyID, m_csMyID.GetLength() + 1);
	SendMsg(&msg_lg, sizeof(msg_lg));
	m_csMyID = "";
	return 0;
}


/*********************************************************
�������ƣ�SystemMessage
������������ʾ���Է�������ϵͳ��Ϣ
����˵����
�� �� ֵ��
��    ע��
*********************************************************/
int CMFCChatDlg::SystemMessage(MSG_SYS* msg_sys)
{
	// ���ȵ�һ���� ��ʾ����
	AfxMessageBox(msg_sys->nIDPrompt);

	// Ȼ�����nIDPrompt��ִ��һЩ��������
	switch(msg_sys->nIDPrompt)
	{
	case IDS_ADD_FRIEND_SUCCESS:
		m_pAddFriendDlg->OnCancel();
		m_pAddFriendDlg = NULL;
		break;
	default:
		break;
	}


	return 0;
}


/*********************************************************
�������ƣ�RecvAddFriendRequest
�����������յ���������
����˵����msg_add-������Ϣ
�� �� ֵ��
��    ע��
*********************************************************/
int CMFCChatDlg::RecvAddFriendRequest(struct MSG_TRANSPOND* msg_add)
{
	msg_add->nType = ADD_FRIEND_ANSWER;
	CString csFromID;
	csFromID = msg_add->FromID;
	int nRes;
	nRes = MessageBox(L"�˺ţ�" + csFromID + L" ����������Ϊ���ѣ��Ƿ�ͬ������", L"������������", MB_YESNO);
	if(nRes == IDYES)
	{
		msg_add->nReturn = ADD_FRIEND_AGREE;
	}
	else
	{
		msg_add->nReturn = ADD_FRIEND_REFUSE;
	}
	SendMsg(msg_add, sizeof(*msg_add));
	
	return 0;
}


/*********************************************************
�������ƣ�UpdateFriendInfo
��������������ȫ��������Ϣ
����˵����msg_info-ȫ�����ѵ�ID�����ֺ�״̬
�� �� ֵ��
��    ע���������Ϊ�� ���������������������Ϣ
*********************************************************/
int CMFCChatDlg::UpdateFriendInfo(MSG_FRND_INFO* msg_info)
{
	if(msg_info == NULL)
	{
		
	}
	



	return 0;
}