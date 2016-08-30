// MyEdit.cpp : 实现文件
//

#include "stdafx.h"

#include "MyEdit.h"


// CEditLogID

IMPLEMENT_DYNAMIC(CEditLogID, CEdit)

CEditLogID::CEditLogID()
{
	m_clrBkGndNormal = RGB(255,255,255);			//编辑框背景色
	m_clrBkGndFocus = RGB(255,255,255);				//编辑框焦点背景色	
	m_clrBorderEdit = RGB(0xEA,0xEA,0xEA);			//编辑框条颜色
	m_clrBorderNormal = RGB(51, 139, 192);			//默认边框颜色
	m_clrBorderHorver = RGB(96, 200, 253);			//悬停边框颜色
	m_clrShadow = RGB(0xB8,0xB8,0xB8);

	m_strFaceName = L"宋体";
	m_nPointSize = 120;

	m_Font.CreatePointFont(									//默认字体颜色大小
		m_nPointSize,
		m_strFaceName);

	m_strGrayString = L"请输入文本";
	m_nFocusStyle = FOCUS_DEFAULT;					//焦点风格设置为默认
	m_bHover = false;
	m_bTracking = true;
	m_bUser = false;
	m_bEmpty = true;
}

CEditLogID::~CEditLogID()
{
}


BEGIN_MESSAGE_MAP(CEditLogID, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	//ON_CONTROL_REFLECT(EN_CHANGE, &CDLEdit::OnEnChange)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CEditLogID 消息处理程序




BOOL CEditLogID::OnEraseBkgnd(CDC* pDC)
{
	CPen BorderEditPen;					//编辑框框条画笔
	BorderEditPen.CreatePen(PS_SOLID, 1, m_clrBorderEdit); // 灰色框

	CPen BorderNormalPen;			//默认边框画笔
	BorderNormalPen.CreatePen(PS_SOLID, 1, m_clrBorderNormal); // 蓝色框

	CPen BorderHoverPen;				//悬停边框画笔

	CRect rctBorder;						//编辑框rct 
	GetWindowRect(&rctBorder);
	m_rctEdit = rctBorder;
	ScreenToClient(&rctBorder);

	CPen * OldPen;

//测试成功，如果有焦点，上阴影
	if(m_nFocusStyle == FOCUS_SHADOW && ::GetFocus() == m_hWnd)
	{
		CBrush * oldBr;
		CBrush br;
		rctBorder.InflateRect(4,4);
		rctBorder.OffsetRect(2,2);
		br.CreateSolidBrush(m_clrShadow);
		oldBr = pDC->SelectObject(&br);
		pDC->Rectangle(rctBorder);
		pDC->SelectObject(oldBr);
		rctBorder.OffsetRect(-2,-2);
		rctBorder.InflateRect(-4,-4);
	}

//画悬停边框
	if(m_bHover)			//如果鼠标悬停
	{	
		rctBorder.InflateRect(4,4);
		BorderHoverPen.CreatePen(
			PS_SOLID,
			1,
			m_clrBorderHorver);
		OldPen = pDC->SelectObject(&BorderHoverPen);
		pDC->Rectangle(rctBorder);
		pDC->SelectObject(OldPen);
		rctBorder.InflateRect(-4,-4);
	}

//画编辑框默认边框
	rctBorder.InflateRect(3,3);
	OldPen = pDC->SelectObject(&BorderNormalPen);
	pDC->RoundRect(&rctBorder,CPoint(0,0));
	pDC->SelectObject(OldPen);
	rctBorder.InflateRect(-3,-3);

//画编辑框框条
	rctBorder.InflateRect(1,1);
	OldPen = pDC->SelectObject(&BorderEditPen);
	pDC->RoundRect(&rctBorder,CPoint(0,0));
	pDC->SelectObject(OldPen);
	rctBorder.InflateRect(-1,-1);

//设置字体
	SetFont(m_nPointSize,m_strFaceName);
	
	return CEdit::OnEraseBkgnd(pDC);
}

// 获得焦点
void CEditLogID::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	// TODO: 在此处添加消息处理程序代码
	if(m_bEmpty)
	{
		m_bUser = false;
		SetWindowTextW(L"");
	}
}

// 失去焦点
void CEditLogID::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	m_bUser = false;
	if(m_bEmpty)
		SetWindowTextW(m_strGrayString);

	if(m_nFocusStyle & FOCUS_FONTBOLD)
		Invalidate();

//重画阴影部分，以恢复原样
	CWnd * parent = this->GetParent();
	if(parent)
	{
		CRect rect = m_rctEdit;
		rect.InflateRect(4,4);
		rect.OffsetRect(2,2);
		parent->ScreenToClient(&rect);
		parent->InvalidateRect(&rect);
		InvalidateRect(&rect);
	}
	// TODO: 在此处添加消息处理程序代码
}

void CEditLogID::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bHover = true;
	Invalidate();
	CEdit::OnMouseHover(nFlags, point);
}

void CEditLogID::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bHover = false;
	m_bTracking = FALSE;

//借助父窗口重画悬停边框
	CWnd * parent = this->GetParent();
	if(parent)
	{
		CRect rect = m_rctEdit;
		rect.InflateRect(4,4);
		parent->ScreenToClient(&rect);
		parent->InvalidateRect(&rect);
	}
	Invalidate();
	CEdit::OnMouseLeave();
}

void CEditLogID::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;
		tme.dwHoverTime = 1;
		if(_TrackMouseEvent(&tme) == TRUE)
			m_bTracking = true;
		else
			m_bTracking = false;
	}
	CEdit::OnMouseMove(nFlags, point);
}

void CEditLogID::SetFont(int nPointSize, LPCTSTR lpszFaceName)
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();

	m_Font.CreatePointFont(
		nPointSize,
		lpszFaceName);

	LOGFONT lf;
	m_Font.GetLogFont(&lf);

	if(m_nFocusStyle == FOCUS_FONTBOLD && ::GetFocus() == m_hWnd)
	{
		lf.lfWeight = FW_BOLD;
		m_Font.DeleteObject();
		m_Font.CreateFontIndirectW(&lf);
	}

	if(-lf.lfHeight < m_rctEdit.Height())
		CWnd::SetFont(&m_Font,false);
}

void CEditLogID::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bUser = true;
	CString str;
	GetWindowText(str);
	if(str.GetLength()<=2)
		this->Invalidate();

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CEditLogID::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(m_bUser)
	{
		CString str;
		GetWindowText(str);
		m_bEmpty = str.IsEmpty();
	}
}

// OnCtlColor是响应子控件发来的WM_CTLCOLOR消息，CtlColor是反射控件自己发出的WM_CTLCOLOR消息。
HBRUSH CEditLogID::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	HBRUSH hbr;
	COLORREF clr = RGB(255,255,255);
	if(m_nFocusStyle & FOCUS_BKCOLOR)
	{
		if(::GetFocus() == m_hWnd)
			clr = m_clrBkGndFocus;		//实际可编辑区域底色修改，不修改默认白色
		else
			clr = m_clrBkGndNormal;
	}

	//pDC->SelectObject(&m_Font);			这里设置字体也可以的

	pDC->SetBkColor(clr);
	hbr = ::CreateSolidBrush(clr);

	if(m_bEmpty)
		pDC->SetTextColor(RGB(192, 192, 192));
	else
		pDC->SetTextColor(RGB(0,0,0));

	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	return hbr;	//编辑框底色画刷
}

void CEditLogID::SetBkGndColor(COLORREF clrNormal,COLORREF clrFocus)
{
	m_clrBkGndNormal = clrNormal;
	m_clrBkGndFocus = clrFocus;
}

void CEditLogID::SetBorderColor(COLORREF clrBorderEdit,COLORREF clrBorderNormal,
							 COLORREF clrBorderHover)
{
	m_clrBorderEdit = clrBorderEdit;
	m_clrBorderNormal = clrBorderNormal;	
	m_clrBorderHorver = clrBorderHover;
}
void CEditLogID::SetFocusStyle(int nStyle)
{
	m_nFocusStyle = nStyle;
}
void CEditLogID::SetShadowColor(COLORREF clrShadow)
{
	m_clrShadow = clrShadow;
}

void CEditLogID::SetGrayString(CString str)
{
	m_strGrayString = str;
}


IMPLEMENT_DYNAMIC(CEditBase, CEdit)

CEditBase::CEditBase()
{
	m_bTracking = false;
	m_bIsFocus = false;
	m_bIsHover = false;
	m_bIsEmpty = true;
}

CEditBase::~CEditBase()
{}


BEGIN_MESSAGE_MAP(CEditBase, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CEditBase::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	
	m_bIsFocus = true;
	if(m_bIsEmpty)
	{// 清空提示信息
		SetWindowTextW(_T(""));
	}
	Invalidate();
}


void CEditBase::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	
	m_bIsFocus = false;
	if(GetWindowTextLengthW() == 0)
	{
		m_bIsEmpty = true;
	}
	else
	{
		m_bIsEmpty = false;
	}
	Invalidate();
}


void CEditBase::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bIsHover = true;
	m_bTracking = false;
	Invalidate();

	CEdit::OnMouseHover(nFlags, point);
}


void CEditBase::OnMouseLeave()
{
	m_bIsHover = false;
	m_bTracking = false;
	Invalidate();

	CEdit::OnMouseLeave();
}


void CEditBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		// 调用_TrackMouseEvent后会跟踪鼠标
		// 若鼠标停留在按钮上会触发OnMouseHover
		//   离开按钮会触发OnMouseLeave
		// 触发一次后跟踪失效
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme); // 结构体的大小
		tme.hwndTrack = this->m_hWnd; // 按钮的句柄 
		tme.dwFlags = TME_LEAVE|TME_HOVER; // 跟踪的类型
		tme.dwHoverTime = 1; // 触发Hover事件需要鼠标停留的时间   避免触发不必要的Hover事件		
		if(_TrackMouseEvent(&tme) == TRUE)
			m_bTracking = true;
		else
			m_bTracking = false;
	}

	CEdit::OnMouseMove(nFlags, point);
}

