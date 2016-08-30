#include "stdafx.h"
#include "MyButton.h"


#define CTRL_NOFOCUS  0x101    
#define CTRL_FOCUS    0x102  
#define CTRL_SELECTED 0x103  
// CBtnBase 

IMPLEMENT_DYNAMIC(CBtnBase, CButton)


CBtnBase::CBtnBase()
{
	m_bTracking = false;
	m_nCtrlState = CTRL_NOFOCUS;
}

CBtnBase::~CBtnBase()
{
}


BEGIN_MESSAGE_MAP(CBtnBase, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CBtnBase ��Ϣ�������



// ����������
void CBtnBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nCtrlState == CTRL_FOCUS)
	{
		m_nCtrlState = CTRL_SELECTED;
		Invalidate();
	}

	CButton::OnLButtonDown(nFlags, point);
}

// �������ɿ�
void CBtnBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_nCtrlState == CTRL_SELECTED)
	{
		m_nCtrlState = CTRL_FOCUS;
		Invalidate();
	}
	CButton::OnLButtonUp(nFlags, point);
}

// ���ͣ���ڰ�ť��
void CBtnBase::OnMouseHover(UINT nFlags, CPoint point)
{
	if(m_nCtrlState == CTRL_NOFOCUS)
	{
		m_nCtrlState = CTRL_FOCUS;
		Invalidate();
	}

	m_bTracking = false;

	CButton::OnMouseHover(nFlags, point);
}

// ����뿪��ťʱ
void CBtnBase::OnMouseLeave()
{
	if(m_nCtrlState != CTRL_NOFOCUS)
	{
		m_nCtrlState = CTRL_NOFOCUS;
		Invalidate();
	}

	m_bTracking = false;

	CButton::OnMouseLeave();
}

// ����ƶ�ʱ
void CBtnBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking) // ����ǰ���ڷǸ���״̬�����µĸ���
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
	CButton::OnMouseMove(nFlags, point);
}

// ��������
BOOL CBtnBase::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}


void CBtnBase::PreSubclassWindow()
{
	SetButtonStyle(BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}



// CBtnRndRct


CBtnRndRct::CBtnRndRct()
{
    m_pFont = new CFont;
	m_pFont->CreateFont(15, //���߼���λ��ʽָ������ĸ߶�
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
		_T("����") //ָ���������������
		);

	// QQ�İ�ť��ɫ
	m_clrFont = RGB(255, 255, 255);
	m_clrBKFocus = RGB(80, 164, 249);
	m_clrBKUnfocus = RGB(81, 158, 225);
	m_clrBKSelected = RGB(35, 116, 208);
	
	m_clrBKDisable = RGB(150, 150, 150);
}


CBtnRndRct::~CBtnRndRct()
{
	if(m_pFont != NULL)
	{
		m_pFont->DeleteObject();
		
		delete m_pFont;
		m_pFont = NULL;
	}
}


void CBtnRndRct::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;  
	dc.Attach(lpDrawItemStruct->hDC);       //��ť�ؼ�DC    

	COLORREF clrBK = RGB(255,255,255);  
	switch(m_nCtrlState)    
	{
	case CTRL_NOFOCUS:
		clrBK = m_clrBKUnfocus;
		break;
	case CTRL_FOCUS:
		clrBK = m_clrBKFocus;
		break;
	case CTRL_SELECTED:
		clrBK = m_clrBKSelected;
		break;
	default:
		break;
	}
	
	if(IsWindowEnabled() == FALSE)
	{
		clrBK = m_clrBKDisable;  
	}
	
	//���Ʊ���
	CRect rect(lpDrawItemStruct->rcItem);
	DrawRoundRect(dc,5,rect,clrBK);
	
	//��������  
	CFont* pOldFont;
	if(m_pFont)
	{
		pOldFont = dc.SelectObject(m_pFont);
	}
	COLORREF clrOld = dc.SetTextColor(m_clrFont);
	CString strText;
	GetWindowText(strText);
	int oldMode = dc.SetBkMode(TRANSPARENT);
	CRect rc(lpDrawItemStruct->rcItem);
	if(m_nCtrlState == CTRL_SELECTED)
	{
		rc.OffsetRect(1, 1); // ���������·�ƫ�� ����һ�ְ��°�ť�ĸо�
	}
	dc.DrawText(strText, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc.SetBkMode(oldMode);
	dc.SetTextColor(clrOld);
	if(m_pFont)
	{
		dc.SelectObject(pOldFont);
	}
	
	dc.Detach();
}


void CBtnRndRct::DrawRoundRect(CDC& dc, int radius, CRect rect, COLORREF clrBK)
{
	POINT pt;
	pt.x = radius;
	pt.y = radius;

	int penSize = 1;
	CPen pen;
	pen.CreatePen(PS_SOLID, penSize, clrBK);
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush brush(clrBK);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.RoundRect(&rect, pt);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();

}


void CBtnRndRct::SetColor(COLORREF clrFont, COLORREF clrBKFocus, 
		COLORREF clrBKUnfocus, COLORREF clrBKSelected, COLORREF clrBKDisable)
{
	m_clrFont = clrFont;
	m_clrBKFocus = clrBKFocus;
	m_clrBKUnfocus = clrBKUnfocus;
	m_clrBKSelected = clrBKSelected;
	m_clrBKDisable = clrBKDisable;
}


void CBtnRndRct::SetFont(CFont* pFont, BOOL bRedraw)
{
	if(m_pFont)
	{
		delete m_pFont;
	}
	m_pFont = pFont;
	
	if(bRedraw)
	{// ��д
		Invalidate();
	}
}



// CBtnImage


CBtnImage::CBtnImage()
{}

CBtnImage::~CBtnImage()
{}


void CBtnImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	/*
	typedef   struct   tagDRAWITEMSTRUCT   { 
	UINT       CtlType; // ��ʾ�ؼ����ͣ���Ϊ����ODT_BUTTON��ʾΪ�Ի��ư�ť��
	UINT       CtlID; // ��ʾ�ؼ���ID��
	UINT       itemID; 
	UINT       itemAction; // ���ƶ�����
	UINT       itemState; // �ڵ�ǰ���ƶ�����ɺ�ȷ���ؼ�����״̬�� 
	HWND       hwndItem; // �ؼ����ڵľ���� 
	HDC         hDC; // �豸��������� 
	RECT       rcItem; // ȷ���Ի��ƿؼ���С�ľ��Ρ�
	DWORD     itemData; 
	}   DRAWITEMSTRUCT; 
	*/
	/*
	CRect rcButton(lpDrawItemStruct->rcItem);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT state = lpDrawItemStruct->itemState;
	UINT action = lpDrawItemStruct->itemAction;
	UINT CtlID = lpDrawItemStruct->CtlID;
	CString strCaption; 
	GetWindowText(strCaption);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap   m_Bitmap; 
	CBitmap   *pOldBitmap; 
	BITMAP   s_Bmp; 

	//this->UpdateLayeredWindow(&dcMem, NULL, NULL, NULL, NULL, RGB(0, 0, 0), 

	if(state == ODS_FOCUS)
	{
		if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
		{ 
			pOldBitmap = dcMem.SelectObject(&m_Bitmap); 
			m_Bitmap.GetBitmap(&s_Bmp); 
			pDC->StretchBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), 
				&dcMem, 0, 0, s_Bmp.bmWidth, s_Bmp.bmHeight / 3, SRCCOPY);   
		} 
		else   
		{ 
			if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
			{ 
				pOldBitmap   =   dcMem.SelectObject(&m_Bitmap); 
				m_Bitmap.GetBitmap(&s_Bmp); 
				pDC->StretchBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), 
					&dcMem, 0, 0, s_Bmp.bmWidth, s_Bmp.bmHeight / 3, SRCCOPY);   
			} 
			else 
			{ 
				TRACE( "��������һ��λͼ "); 
			} 
		} 
	}
	if(state   ==   4) 
	{ 
		if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
		{ 
			pOldBitmap   =   dcMem.SelectObject(&m_Bitmap); 
			m_Bitmap.GetBitmap(&s_Bmp); 
			pDC   ->   StretchBlt(rcButton.left,rcButton.top,rcButton.Width(),rcButton.Height(), 
				&dcMem,0,0,s_Bmp.bmWidth,s_Bmp.bmHeight,SRCCOPY);   
		} 
	} 
	if(state   &   ODS_SELECTED) 
	{// ��ť������
		if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
		{
			pOldBitmap   =   dcMem.SelectObject(&m_Bitmap); 
			m_Bitmap.GetBitmap(&s_Bmp); 
			pDC->StretchBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), 
				&dcMem, 0, s_Bmp.bmHeight / 3 * 2, s_Bmp.bmWidth, s_Bmp.bmHeight / 3, SRCCOPY);   
		}
	}
	else
	{
		if(m_bIsMouseOn == TRUE)
		{
			if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
			{
				pOldBitmap = dcMem.SelectObject(&m_Bitmap); 
				m_Bitmap.GetBitmap(&s_Bmp); 
				pDC->StretchBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), 
					&dcMem, 0, s_Bmp.bmHeight / 3, s_Bmp.bmWidth, s_Bmp.bmHeight / 3, SRCCOPY);   
				return;
			}
			else
			{
				TRACE( "��������һ��λͼ "); 
			}
			
		}

		if(m_Bitmap.LoadBitmap(IDB_BITMAP_NORMAL)) 
		{ 
			pOldBitmap   =   dcMem.SelectObject(&m_Bitmap); 
			m_Bitmap.GetBitmap(&s_Bmp); 
			pDC   ->   StretchBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), 
				&dcMem, 0, 0, s_Bmp.bmWidth,s_Bmp.bmHeight / 3, SRCCOPY);   
		} 
		else 
		{ 
			TRACE( "��������һ��λͼ   U "); 
		} 
	}
	*/
	if(m_BKImage == NULL)
	{
		TRACE("�����ͼƬ��\n");
		return;
	}

	CRect rcButton(lpDrawItemStruct->rcItem);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC dcMem;
	dcMem.Attach(lpDrawItemStruct->hDC);	
	// ����ť�ߴ��ΪͼƬ�ߴ�
	SetWindowPos(NULL, 
		rcButton.left, rcButton.top, // ��ť����λ��
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3, // ��ť���³ߴ�
		SWP_NOMOVE | SWP_NOZORDER); // ���ı�λ�� | ���ı��ι�ϵ
	
	int nImgNum = 0;
	switch(m_nCtrlState)    
	{
	case CTRL_NOFOCUS:
		nImgNum = 0;
		break;
	case CTRL_FOCUS:
		nImgNum = 1;
		break;
	case CTRL_SELECTED:
		nImgNum = 2;
		break;
	default:
		break;
	}

	m_BKImage.Draw(dcMem.GetSafeHdc(), 
		rcButton.left, rcButton.top, // Ŀ����ε����Ͻ�����
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3, // Ŀ����εĳߴ�
		0, m_BKImage.GetHeight() / 3 * nImgNum, // ͼƬ�����Ͻ�����
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3 // ��ͼ�ĳߴ�
		);
	dcMem.Detach();

	return;
}

void CBtnImage::SetImage(int nResID, const CString &strImgType)
{
	HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(nResID), strImgType);
	if(hrsrc)
	{
		HGLOBAL hglb = LoadResource(NULL, hrsrc);
		LPVOID lpsz = LockResource(hglb);
		ULARGE_INTEGER srcSize;
		srcSize.QuadPart = SizeofResource(NULL, hrsrc);
		
		IStream * pStream = NULL;
		CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if(pStream)
		{
			pStream->SetSize(srcSize);
			DWORD cbWritten = 0;
			pStream->Write(lpsz, srcSize.LowPart, &cbWritten);
			
			if(m_BKImage.Load(pStream) == S_OK)
			{
				pStream->Release();
			}
			else
			{
				TRACE("����ͼƬʧ�ܣ�\n");
			}
		}
	}
}




IMPLEMENT_DYNAMIC(CBtnRadio, CButton)


CBtnRadio::CBtnRadio()
{
	m_pFont = new CFont;
	m_pFont->CreateFont(12, //���߼���λ��ʽָ������ĸ߶�
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
		_T("����") //ָ���������������
		);
	
	m_clrFont = RGB(0, 0, 0);
	m_clrBKFocus = RGB(207, 207, 207);
	m_clrBKUnfocus = RGB(237, 238, 238);
	m_clrBKSelected = RGB(255, 255, 255);
	
	m_clrBKDisable = RGB(150, 150, 150);
	
	m_bCheck = FALSE;
	m_bFocus = FALSE;
	m_bTracking = FALSE;
}

CBtnRadio::~CBtnRadio()
{}

void CBtnRadio::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;  
	dc.Attach(lpDrawItemStruct->hDC);       //��ť�ؼ�DC    

	COLORREF clrBK = RGB(255,255,0);  
	//int n = this->GetCheck();
	if(m_bCheck == TRUE)
	{// ��ѡ��
		clrBK = m_clrBKSelected;	
	}
	else if(m_bFocus)
	{
		clrBK = m_clrBKFocus;
	}
	else
	{
		clrBK = m_clrBKUnfocus;
	}
	/*
	switch(m_nCtrlState)    
	{
	case CTRL_NOFOCUS:
		clrBK = m_clrBKUnfocus;
		break;
	case CTRL_FOCUS:
		clrBK = m_clrBKFocus;
		break;
	default:
		break;
	}
	*/
	if(IsWindowEnabled() == FALSE)
	{
		clrBK = m_clrBKDisable;  
	}
	
	//���Ʊ���
	CRect rect(lpDrawItemStruct->rcItem);
	//DrawRoundRect(dc,10,rect,clrBK);
	dc.FillRect(&rect, &CBrush(clrBK));
	//��������  
	CFont* pOldFont;
	if(m_pFont)
	{
		pOldFont = dc.SelectObject(m_pFont);
	}
	COLORREF clrOld = dc.SetTextColor(m_clrFont);
	CString strText;
	GetWindowText(strText);
	int oldMode = dc.SetBkMode(TRANSPARENT);
	CRect rc(lpDrawItemStruct->rcItem);
	//if(m_nCtrlState == CTRL_SELECTED)
	//{
	//	rc.OffsetRect(1, 1); // ���������·�ƫ�� ����һ�ְ��°�ť�ĸо�
	//}
	dc.DrawText(strText, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	// �ָ�ԭ����
	dc.SetBkMode(oldMode);
	dc.SetTextColor(clrOld);
	if(m_pFont)
	{
		dc.SelectObject(pOldFont);
	}
	
	dc.Detach();
}
BEGIN_MESSAGE_MAP(CBtnRadio, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CBtnRadio::OnLButtonDown(UINT nFlags, CPoint point)
{
	//this->SetCheck(BST_CHECKED);

	CButton::OnLButtonDown(nFlags, point);
}


void CBtnRadio::OnMouseHover(UINT nFlags, CPoint point)
{
	//int nCheck = this->GetCheck();
	//if(nCheck == BST_UNCHECKED)
	if(m_bCheck == FALSE && m_bFocus == FALSE)
	{
		m_bFocus = TRUE;
		Invalidate();
	}

	m_bTracking = FALSE;
	
	CButton::OnMouseHover(nFlags, point);
}


void CBtnRadio::OnMouseLeave()
{
	//int nCheck = this->GetCheck();
	if(m_bCheck == FALSE && m_bFocus == TRUE)
	{
		m_bFocus = FALSE;
		Invalidate();
	}
	m_bTracking = FALSE;
	
	CButton::OnMouseLeave();
}


void CBtnRadio::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->m_hWnd;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 10;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}


void CBtnRadio::PreSubclassWindow()
{
	SetButtonStyle(BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CBtnRadio::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
	m_bFocus = FALSE;
	Invalidate();
}

BOOL CBtnRadio::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}
