// MyEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "MyEdit.h"


// CEditLogID

IMPLEMENT_DYNAMIC(CEditLogID, CEdit)

CEditLogID::CEditLogID()
{
	m_clrBkGndNormal = RGB(255,255,255);			//�༭�򱳾�ɫ
	m_clrBkGndFocus = RGB(255,255,255);				//�༭�򽹵㱳��ɫ	
	m_clrBorderEdit = RGB(0xEA,0xEA,0xEA);			//�༭������ɫ
	m_clrBorderNormal = RGB(51, 139, 192);			//Ĭ�ϱ߿���ɫ
	m_clrBorderHorver = RGB(96, 200, 253);			//��ͣ�߿���ɫ
	m_clrShadow = RGB(0xB8,0xB8,0xB8);

	m_strFaceName = L"����";
	m_nPointSize = 120;

	m_Font.CreatePointFont(									//Ĭ��������ɫ��С
		m_nPointSize,
		m_strFaceName);

	m_strGrayString = L"�������ı�";
	m_nFocusStyle = FOCUS_DEFAULT;					//����������ΪĬ��
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



// CEditLogID ��Ϣ�������




BOOL CEditLogID::OnEraseBkgnd(CDC* pDC)
{
	CPen BorderEditPen;					//�༭���������
	BorderEditPen.CreatePen(PS_SOLID, 1, m_clrBorderEdit); // ��ɫ��

	CPen BorderNormalPen;			//Ĭ�ϱ߿򻭱�
	BorderNormalPen.CreatePen(PS_SOLID, 1, m_clrBorderNormal); // ��ɫ��

	CPen BorderHoverPen;				//��ͣ�߿򻭱�

	CRect rctBorder;						//�༭��rct 
	GetWindowRect(&rctBorder);
	m_rctEdit = rctBorder;
	ScreenToClient(&rctBorder);

	CPen * OldPen;

//���Գɹ�������н��㣬����Ӱ
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

//����ͣ�߿�
	if(m_bHover)			//��������ͣ
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

//���༭��Ĭ�ϱ߿�
	rctBorder.InflateRect(3,3);
	OldPen = pDC->SelectObject(&BorderNormalPen);
	pDC->RoundRect(&rctBorder,CPoint(0,0));
	pDC->SelectObject(OldPen);
	rctBorder.InflateRect(-3,-3);

//���༭�����
	rctBorder.InflateRect(1,1);
	OldPen = pDC->SelectObject(&BorderEditPen);
	pDC->RoundRect(&rctBorder,CPoint(0,0));
	pDC->SelectObject(OldPen);
	rctBorder.InflateRect(-1,-1);

//��������
	SetFont(m_nPointSize,m_strFaceName);
	
	return CEdit::OnEraseBkgnd(pDC);
}

// ��ý���
void CEditLogID::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	// TODO: �ڴ˴������Ϣ����������
	if(m_bEmpty)
	{
		m_bUser = false;
		SetWindowTextW(L"");
	}
}

// ʧȥ����
void CEditLogID::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	m_bUser = false;
	if(m_bEmpty)
		SetWindowTextW(m_strGrayString);

	if(m_nFocusStyle & FOCUS_FONTBOLD)
		Invalidate();

//�ػ���Ӱ���֣��Իָ�ԭ��
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
	// TODO: �ڴ˴������Ϣ����������
}

void CEditLogID::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bHover = true;
	Invalidate();
	CEdit::OnMouseHover(nFlags, point);
}

void CEditLogID::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bHover = false;
	m_bTracking = FALSE;

//�����������ػ���ͣ�߿�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bUser = true;
	CString str;
	GetWindowText(str);
	if(str.GetLength()<=2)
		this->Invalidate();

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CEditLogID::OnEnChange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(m_bUser)
	{
		CString str;
		GetWindowText(str);
		m_bEmpty = str.IsEmpty();
	}
}

// OnCtlColor����Ӧ�ӿؼ�������WM_CTLCOLOR��Ϣ��CtlColor�Ƿ���ؼ��Լ�������WM_CTLCOLOR��Ϣ��
HBRUSH CEditLogID::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����
	HBRUSH hbr;
	COLORREF clr = RGB(255,255,255);
	if(m_nFocusStyle & FOCUS_BKCOLOR)
	{
		if(::GetFocus() == m_hWnd)
			clr = m_clrBkGndFocus;		//ʵ�ʿɱ༭�����ɫ�޸ģ����޸�Ĭ�ϰ�ɫ
		else
			clr = m_clrBkGndNormal;
	}

	//pDC->SelectObject(&m_Font);			������������Ҳ���Ե�

	pDC->SetBkColor(clr);
	hbr = ::CreateSolidBrush(clr);

	if(m_bEmpty)
		pDC->SetTextColor(RGB(192, 192, 192));
	else
		pDC->SetTextColor(RGB(0,0,0));

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
	return hbr;	//�༭���ɫ��ˢ
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
	{// �����ʾ��Ϣ
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
		// ����_TrackMouseEvent���������
		// �����ͣ���ڰ�ť�ϻᴥ��OnMouseHover
		//   �뿪��ť�ᴥ��OnMouseLeave
		// ����һ�κ����ʧЧ
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme); // �ṹ��Ĵ�С
		tme.hwndTrack = this->m_hWnd; // ��ť�ľ�� 
		tme.dwFlags = TME_LEAVE|TME_HOVER; // ���ٵ�����
		tme.dwHoverTime = 1; // ����Hover�¼���Ҫ���ͣ����ʱ��   ���ⴥ������Ҫ��Hover�¼�		
		if(_TrackMouseEvent(&tme) == TRUE)
			m_bTracking = true;
		else
			m_bTracking = false;
	}

	CEdit::OnMouseMove(nFlags, point);
}

