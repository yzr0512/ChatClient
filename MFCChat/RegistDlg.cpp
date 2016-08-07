// RegistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCChat.h"
#include "RegistDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

// CRegistDlg dialog

IMPLEMENT_DYNAMIC(CRegistDlg, CDialogEx)

CRegistDlg::CRegistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegistDlg::IDD, pParent)	
{
	//m_pParentWnd = (CLoginDlg *)pParent;
	m_pParentWnd = pParent;
	m_csName = _T("");
	m_csKey = _T("");
	m_csEmail = _T("");
	m_csKey2 = _T("");
	m_nAge = 0;
	m_cSex = '\0';
}

CRegistDlg::~CRegistDlg()
{
}

void CRegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME_REG, m_csName);
	DDV_MaxChars(pDX, m_csName, NAME_MAX);
	
	DDX_Text(pDX, IDC_EDIT_KEY_REGIST, m_csKey);
	DDV_MaxChars(pDX, m_csKey, KEY_MAX);
	DDX_Text(pDX, IDC_EDIT_KEY_AGAIN_REGIST, m_csKey2);
	DDV_MaxChars(pDX, m_csKey2, KEY_MAX);

	DDX_Text(pDX, IDC_EDIT_EMAIL_REG, m_csEmail);

	DDX_Text(pDX, IDC_EDIT_AGE_REG, m_nAge);
	DDV_MinMaxUInt(pDX, m_nAge, 0, 150);
}


BEGIN_MESSAGE_MAP(CRegistDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REGIST, &CRegistDlg::OnBnClickedButtonRegist)
	ON_BN_CLICKED(IDC_RADIO_SEX_MALE, &CRegistDlg::OnBnClickedRadioSexMale)
	ON_BN_CLICKED(IDC_RADIO_FEMALE, &CRegistDlg::OnBnClickedRadioFemale)
END_MESSAGE_MAP()


// CRegistDlg message handlers

/*********************************************************
�������ƣ�OnBnClickedButtonRegist
����������������������ע����Ϣ
*********************************************************/
void CRegistDlg::OnBnClickedButtonRegist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�����ݸ��µ�����
	UpdateData(TRUE);

	if(m_csName == "" || m_csKey == "")
	{
		MessageBox(_T("�˺Ż����벻��Ϊ�գ�"));
		return;
	}
	if(m_csKey != m_csKey2)
	{
		MessageBox(L"������������벻һ����");
		return;
	}
	if(m_cSex == '\0')
	{
		MessageBox(L"��ѡ���Ա�");
		return;	
	}

	struct MSG_REGISTER msg_reg;
	msg_reg.nType = REGISTER;

	memset(msg_reg.nID, '\0', ID_MAX);
	memcpy(msg_reg.Name, m_csName, m_csName.GetLength() * 2 + 2);
	size_t i; // ��ת�����ַ���
	wcstombs_s(&i, msg_reg.PassWord, m_csKey, m_csKey.GetLength() + 1);
	wcstombs_s(&i, msg_reg.Email, m_csEmail, m_csEmail.GetLength() + 1);	

	msg_reg.nAge = m_nAge;
	msg_reg.Sex = m_cSex;

	//((CLoginDlg *)m_pParentWnd)->SendMsg(&msg_reg);
	CMFCChatDlg *pMainDlg = NULL;
	pMainDlg = (CMFCChatDlg*)((CLoginDlg *)m_pParentWnd)->m_pParentWnd;
	pMainDlg->SendMsg(&msg_reg, sizeof(msg_reg));

}


/*********************************************************
�������ƣ�RegisterSuccess
����������ע��ɹ�
����˵����msg_reg-ע��ɹ�ʱ��������������Ϣ
*********************************************************/
void CRegistDlg::RegisterSuccess(struct MSG_REGISTER *msg_reg)
{
	CString csMsg;
	CString csID;
	csID = msg_reg->nID;
	csMsg.Format(L"ע��ɹ��������˺��ǣ�%s�������Ʊ��ܺ��Լ����˺š�", csID);
	MessageBox(csMsg);


	((CLoginDlg *)m_pParentWnd)->m_csID = msg_reg->nID;
	((CLoginDlg *)m_pParentWnd)->m_csKey = msg_reg->PassWord;
	((CLoginDlg *)m_pParentWnd)->UpdateData(FALSE);
	//EndDialog(2);
	OnCancel();
}


/*********************************************************
�������ƣ�OnBnClickedRadioSexMale
�����������Ա�ѡ��
*********************************************************/
void CRegistDlg::OnBnClickedRadioSexMale()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cSex = 'm';
}
void CRegistDlg::OnBnClickedRadioFemale()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cSex = 'f';
}


void CRegistDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	((CLoginDlg*)m_pParentWnd)->m_pRegDlg = NULL;

	DestroyWindow();
	
	//CDialogEx::OnCancel();
}
void CRegistDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;

	CDialogEx::PostNcDestroy();
}
