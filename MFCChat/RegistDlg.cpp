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
函数名称：OnBnClickedButtonRegist
功能描述：给服务器发送注册消息
*********************************************************/
void CRegistDlg::OnBnClickedButtonRegist()
{
	// TODO: 在此添加控件通知处理程序代码

	//将数据更新到里面
	UpdateData(TRUE);

	if(m_csName == "" || m_csKey == "")
	{
		MessageBox(_T("账号或密码不能为空！"));
		return;
	}
	if(m_csKey != m_csKey2)
	{
		MessageBox(L"两次输入的密码不一样！");
		return;
	}
	if(m_cSex == '\0')
	{
		MessageBox(L"请选择性别！");
		return;	
	}

	struct MSG_REGISTER msg_reg;
	msg_reg.nType = REGISTER;

	memset(msg_reg.nID, '\0', ID_MAX);
	memcpy(msg_reg.Name, m_csName, m_csName.GetLength() * 2 + 2);
	size_t i; // 已转换的字符数
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
函数名称：RegisterSuccess
功能描述：注册成功
参数说明：msg_reg-注册成功时服务器发来的消息
*********************************************************/
void CRegistDlg::RegisterSuccess(struct MSG_REGISTER *msg_reg)
{
	CString csMsg;
	CString csID;
	csID = msg_reg->nID;
	csMsg.Format(L"注册成功！您的账号是：%s。请妥善保管好自己的账号。", csID);
	MessageBox(csMsg);


	((CLoginDlg *)m_pParentWnd)->m_csID = msg_reg->nID;
	((CLoginDlg *)m_pParentWnd)->m_csKey = msg_reg->PassWord;
	((CLoginDlg *)m_pParentWnd)->UpdateData(FALSE);
	//EndDialog(2);
	OnCancel();
}


/*********************************************************
函数名称：OnBnClickedRadioSexMale
功能描述：性别单选框
*********************************************************/
void CRegistDlg::OnBnClickedRadioSexMale()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cSex = 'm';
}
void CRegistDlg::OnBnClickedRadioFemale()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cSex = 'f';
}


void CRegistDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	((CLoginDlg*)m_pParentWnd)->m_pRegDlg = NULL;

	DestroyWindow();
	
	//CDialogEx::OnCancel();
}
void CRegistDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialogEx::PostNcDestroy();
}
