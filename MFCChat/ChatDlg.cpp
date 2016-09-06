
/************************************************
文 件 名：ChatDlg.cpp
作    者：余志荣
创建日期：2016-08-17
用    途：“聊天”对话框的实现文件
修改记录：2016-08-17 余志荣 创建
************************************************/ 

// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MFCChatDlg.h"

#include <afxsock.h>
#include <afxwin.h>

// CChatDlg 对话框

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


// CChatDlg 消息处理程序


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString csName;
	csName.Format(L"%s", m_Name);
	//SetWindowText(_T("Chatting with ") + csName);	
	SetWindowText(csName); // 设置窗口标题

	this->SetBackgroundColor(RGB(255, 255, 255)); // 设置背景颜色

	// 设置表头
	CRect rect;
	m_ListFileRecord.GetClientRect(&rect);
	int nColInterval = rect.Width();
	m_ListFileRecord.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
 
	m_ListFileRecord.InsertColumn(0, _T(""), LVCFMT_CENTER,		int(nColInterval * 0.1));	
	m_ListFileRecord.InsertColumn(1, _T("文件名"), LVCFMT_CENTER,		int(nColInterval * 0.6));   
	//m_ListFileRecord.InsertColumn(1, _T("日期"), LVCFMT_CENTER,		int(nColInterval * 0.3));   
	m_ListFileRecord.InsertColumn(2, _T("状态"), LVCFMT_CENTER,		int(nColInterval * 0.3));   
	

	CFont *font = new CFont;
	//CString strFontName;    // 字体名称   
	//LOGFONT lf;             // LOGFONT变量   

	// 将lf所有字节清零   
	//memset(&lf, 0, sizeof(LOGFONT));   

	// 将lf中的元素字体名设为“宋体”   
	//_tcscpy_s(lf.lfFaceName, 25, _T("微软雅黑"));   

	font->CreateFont(20, //以逻辑单位方式指定字体的高度
		0, //以逻辑单位方式指定字体中字符的平均宽度
		0, //指定偏离垂线和X轴在显示面上的夹角（单位：0.1度）
		0, //指定字符串基线和X轴之间的夹角（单位：0.1度）
		FW_NORMAL, //指定字体磅数
		FALSE, //是不是斜体
		FALSE, //加不加下划线
		0, //指定是否是字体字符突出
		ANSI_CHARSET, //指定字体的字符集
		OUT_DEFAULT_PRECIS, //指定所需的输出精度
		CLIP_DEFAULT_PRECIS, //指定所需的剪贴精度
		DEFAULT_QUALITY, //指示字体的输出质量
		DEFAULT_PITCH | FF_SWISS, //指定字体的间距和家族
		_T("微软雅黑") //指定字体的字样名称
		);

	// 构造字体对话框，初始选择字体名为“宋体”   
	//CFontDialog fontDlg(&lf);   

	// 如果m_font已经关联了一个字体资源对象，则释放它   
	//if (font.m_hObject)   
	//{   
	//	font.DeleteObject();   
	//}   
	// 使用选定字体的LOGFONT创建新的字体   
	//font.CreateFontIndirect(fontDlg.m_cf.lpLogFont);   
	// 获取编辑框IDC_FONT_EDIT的CWnd指针，并设置其字体   
	GetDlgItem(IDC_EDIT_OUTPUT_MSG)->SetFont(font);   
	GetDlgItem(IDC_EDIT_INPUT_MSG)->SetFont(font);  
	m_StaticInfo.SetFont(font);
	
	//RefreshStatic();

	// 如果用户选择了字体对话框的OK按钮，则获取被选择字体的名称并显示到编辑框里   
	//strFontName = fontDlg.m_cf.lpLogFont->lfFaceName;   
	//SetDlgItemText(IDC_FONT_EDIT, strFontName);  
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 关闭聊天窗口时同时删除主窗口中的元素
// 需要重载OnCancel()和PostNcDestroy()
void CChatDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	((CMFCChatDlg*)m_pParentWnd)->CloseChatDlg(m_nID);

	// 删除窗口
	DestroyWindow();
}
void CChatDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 删除当前对象
	delete this;

	CDialogEx::PostNcDestroy();
}


// 此处可设置快捷键
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) // 按下回车键发送消息
		{
			OnBnClickedButtonSendMsg();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


/*********************************************************
函数名称：OnBnClickedButtonSendMsg
功能描述：发送信息
创建时间：2016-08-17
备    注：
*********************************************************/
void CChatDlg::OnBnClickedButtonSendMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_csInputMsg == "") // 编辑框中未输入内容
	{
		// 气泡提示：编辑框的内容不能为空
		// 
		return;
	}
	
	struct MSG_TRANSPOND msg_chat;
	msg_chat.nType = CHATING_TEXT_MSG;

	CString csMyID; // 我的ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // 现在时间
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
	
	CString csMyID; // 我的ID
	csMyID = ((CMFCChatDlg*)m_pParentWnd)->m_csMyID;

	CTime tm = CTime::GetCurrentTime(); // 现在时间
	CString csDate = tm.Format("%Y-%m-%d");
	CString csTime = tm.Format("%H:%M:%S");	

	memcpy(msg_chat.ToID, m_csID, m_csID.GetLength() * 2);
	memcpy(msg_chat.FromID, csMyID, csMyID.GetLength() * 2);
	memcpy(msg_chat.Date, csDate, csDate.GetLength() * 2);
	memcpy(msg_chat.Time, csTime, csTime.GetLength() * 2);
	memcpy(msg_chat.Data, m_csInputMsg, m_csInputMsg.GetLength() * 2);
	*/
	// 发送消息
	
	if(((CMFCChatDlg*)m_pParentWnd)->SendMsg(&msg_chat, sizeof(msg_chat)))
	{
		// 发送成功后 显示我发的内容
		AddMessage(_T("我"), csTime, m_csInputMsg);
		// 清空输入框的内容
		m_csInputMsg = "";
	}

	// 将信息更新到屏幕
	UpdateData(FALSE);
	
}


/*********************************************************
函数名称：AddMessage
功能描述：在编辑框添加信息
创建时间：2016-08-15
备    注：
*********************************************************/
int CChatDlg::AddMessage(const CString& csName, const CString& csTime, const CString& csMsg)
{	
	// 如果传进来的csName是好友的ID 则将ID换成名字
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
函数名称：RefreshListCtrlData
功能描述：刷新列表控件的数据
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
*********************************************************/
int CChatDlg::RefreshListCtrlData(void)
{
	// 删除所有行
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
		{// 发送记录
			if(!strcmp(m_nID, pRecord->ToID))
			{
				m_ListFileRecord.InsertItem(i, L"发");
				CString strFileName;
				strFileName = pRecord->FileName;
				m_ListFileRecord.SetItemText(i, 1, strFileName);
				CString strState;
				double dbCent; // 百分数
				switch(pRecord->nTransState)
				{
				case 0:
					strState = "等待接收";
					break;
				case 1:
					dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // 发送的百分比
					strState.Format(L"%.2lf%%", dbCent);
					break;
				case 2:
					strState = "暂停";
					break;
				case 3:
					strState = "已完成";
					break;
				case 4:
					strState = "取消";
					break;
				case 5:
					strState = "出错";
					break;
				case 6:
					strState = "拒绝";
					break;
				default:
					strState = "";
					break;
				}
				m_ListFileRecord.SetItemText(i, 2, strState);
			}
		}
		else
		{// 接受记录
			if(!strcmp(m_nID, pRecord->FromID))
			{
				m_ListFileRecord.InsertItem(i, L"收");
				CString strFileName;
				strFileName = pRecord->FileName;
				m_ListFileRecord.SetItemText(i, 1, strFileName);
				CString strState;
				double dbCent;
				switch(pRecord->nTransState)
				{
				case 0:
					strState = "等待接收";
					break;
				case 1:
					dbCent = ((double)pRecord->nRecvSize / pRecord->nFileSize) * 100; // 接受的百分比
					strState.Format(L"%.2lf%%", dbCent);
					break;
				case 2:
					strState = "暂停";
					break;
				case 3:
					strState = "已完成";
					break;
				case 4:
					strState = "取消";
					break;
				case 5:
					strState = "出错";
					break;
				case 6:
					strState = "拒绝";
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
函数名称：RefreshStatic(未完成)
功能描述：刷新静态标签的内容
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
*********************************************************/
int CChatDlg::RefreshStatic(MSG_USERINFO * msg_info)
{
	/*
	// 2016年9月1日16:31:01修改
	MSG_USERINFO msg_info;
	memset(&msg_info, 0, sizeof(MSG_USERINFO));
	strcpy_s(msg_info.nID, m_nID);
	//((CMFCChatDlg*)GetParent())->m_Friend.GetDetailInfo(&msg_info);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_INFO);
	CString strTemp1, strTemp2;
	strTemp1 = "账号：";
	strTemp2 = msg_info.nID;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n姓名：";
	strTemp2 = msg_info.Name;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n邮箱：";
	strTemp2 = msg_info.Email;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n性别：";
	strTemp2 = msg_info.Sex;
	strTemp1 += strTemp2;
	pWnd->SetWindowTextW(strTemp1);
	*/

	CWnd* pWnd = GetDlgItem(IDC_STATIC_INFO);
	CString strTemp1, strTemp2;
	strTemp1 = "账号：";
	strTemp2 = msg_info->nID;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n姓名：";
	strTemp2.Format(L"%s", msg_info->Name);
	//strTemp2 = msg_info->Name;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n邮箱：";
	strTemp2 = msg_info->Email;
	strTemp1 += strTemp2;
	strTemp1 += "\r\n性别：";
	if (msg_info->Sex == 'm')
	{
		strTemp2 = "男";
	} 
	else if (msg_info->Sex == 'f')
	{
		strTemp2 = "女";
	}
	else
	{
		strTemp2 = "未知";
	}
	strTemp1 += strTemp2;
	pWnd->SetWindowTextW(strTemp1);

	pWnd->Invalidate();

	return 0;
}

/*********************************************************
函数名称：OnBnClickedButtonSendFile
功能描述：发送传送文件请求
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
*********************************************************/
void CChatDlg::OnBnClickedButtonSendFile()
{
	// 构造打开文件对话框
	CFileDialog fileDlg(TRUE); 
	
	CString strFilePath;
	// 显示打开文件对话框
    if (IDOK == fileDlg.DoModal())   
    {   
		strFilePath = fileDlg.GetPathName();// 包含文件路径和文件名
	}
	else
	{
		return;
	}

	CFile file;

	if(!file.Open(strFilePath, CFile::modeRead))
	{
		MessageBox(L"选择文件有误！");
		return;
	}
	CString strFileName;
	strFileName = file.GetFileName(); // 文件名
	//strFilePath = file.GetFilePath(); // 文件路径
	strFilePath = strFilePath.Left(strFilePath.GetLength() - strFileName.GetLength() - 1);

	MSG_FILE_REQUEST msg_file;
	memset(&msg_file, 0, sizeof(MSG_FILE_REQUEST)); // 结构体清空
	// 构造文件发送请求结构体
	msg_file.nType = MESSAGE_FILE_REQUEST;
	strcpy_s(msg_file.ToID, m_nID);	// 对方ID
	CString strMyID = ((CMFCChatDlg*)theApp.m_pMainWnd)->m_csMyID; // 我的ID
	WideCharToMultiByte(CP_ACP, 0, strMyID, strMyID.GetLength() + 1, msg_file.FromID, ID_MAX, NULL, NULL);
	msg_file.nFileID = ((CMFCChatDlg*)theApp.m_pMainWnd)->GetAvailFileID(); // 获得可用的文件ID
	//WideCharToMultiByte(CP_ACP, 0, strFilePath, strFilePath.GetLength() + 1, msg_file.FileName, 512, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strFileName, strFileName.GetLength() + 1, msg_file.FileName, 512, NULL, NULL); // 文件名
	msg_file.nFileSize = file.GetLength(); // 文件大小

	((CMFCChatDlg*)theApp.m_pMainWnd)->SendMsg(&msg_file, sizeof(MSG_FILE_REQUEST)); // 发送消息

	// 本地记录构建
	FILE_RECORD *pFile_record = new FILE_RECORD;
	memset(pFile_record, 0, sizeof(FILE_RECORD));// 结构体清空
	pFile_record->bType = true; // 记录类型 设为 发送
	strcpy_s(pFile_record->ToID, m_nID);	// 对方ID
	strcpy_s(pFile_record->FromID, msg_file.FromID); // 我的ID
	WideCharToMultiByte(CP_ACP, 0, strFilePath, strFilePath.GetLength() + 1, pFile_record->FilePath, 512, NULL, NULL);// 文件路径
	WideCharToMultiByte(CP_ACP, 0, strFileName, strFileName.GetLength() + 1, pFile_record->FileName, 512, NULL, NULL);// 文件名
	pFile_record->nFileID = msg_file.nFileID; // 文件ID
	pFile_record->nFileSize = file.GetLength(); // 文件大小
	
	((CMFCChatDlg*)theApp.m_pMainWnd)->m_FileRecordList.AddTail(pFile_record); // 将记录添加到链表中
	
	file.Close(); // 关闭文件
}

/*********************************************************
函数名称：OnCtlColor
功能描述：更改只读编辑框的颜色
作者：	  余志荣
创建时间：2016-08-29
参数说明：
返 回 值：
*********************************************************/
HBRUSH CChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{// 改变只读编辑框的颜色
		// 一定要两个一起用 改RGB没用 好迷。。。。
		pDC->SetBkMode(TRANSPARENT);
		CBrush brush;
		brush.CreateSolidBrush(RGB(255,255,255));
		hbr = (HBRUSH)brush;
	}
/*
	// TODO: Change any attributes of the DC here
	switch (nCtlColor) //对所有同一类型的控件进行判断
	{ 
	// process my edit controls by ID.
		case CTLCOLOR_EDIT:
		case CTLCOLOR_STATIC:
	switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
	{     
		// first CEdit control ID
	case IDC_EDIT1:         // 第一个文本框
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
	case IDC_EDIT2:         // 第二个文本框
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

// 定时刷新文件记录列表控件的内容
void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	RefreshListCtrlData();

	CDialogEx::OnTimer(nIDEvent);
}
