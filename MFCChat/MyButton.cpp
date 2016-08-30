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



// CBtnBase 消息处理程序



// 鼠标左键按下
void CBtnBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nCtrlState == CTRL_FOCUS)
	{
		m_nCtrlState = CTRL_SELECTED;
		Invalidate();
	}

	CButton::OnLButtonDown(nFlags, point);
}

// 鼠标左键松开
void CBtnBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_nCtrlState == CTRL_SELECTED)
	{
		m_nCtrlState = CTRL_FOCUS;
		Invalidate();
	}
	CButton::OnLButtonUp(nFlags, point);
}

// 鼠标停留在按钮上
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

// 鼠标离开按钮时
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

// 鼠标移动时
void CBtnBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking) // 若当前处于非跟踪状态则开启新的跟踪
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
	CButton::OnMouseMove(nFlags, point);
}

// 擦除背景
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
	m_pFont->CreateFont(15, //以逻辑单位方式指定字体的高度
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
		_T("宋体") //指定字体的字样名称
		);

	// QQ的按钮颜色
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
	dc.Attach(lpDrawItemStruct->hDC);       //按钮控件DC    

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
	
	//绘制背景
	CRect rect(lpDrawItemStruct->rcItem);
	DrawRoundRect(dc,5,rect,clrBK);
	
	//绘制文字  
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
		rc.OffsetRect(1, 1); // 文字向右下方偏移 给人一种按下按钮的感觉
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
	{// 重写
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
	UINT       CtlType; // 表示控件类型，当为常量ODT_BUTTON表示为自绘制按钮。
	UINT       CtlID; // 表示控件的ID。
	UINT       itemID; 
	UINT       itemAction; // 绘制动作。
	UINT       itemState; // 在当前绘制动作完成后，确定控件所处状态。 
	HWND       hwndItem; // 控件窗口的句柄。 
	HDC         hDC; // 设备环境句柄。 
	RECT       rcItem; // 确定自绘制控件大小的矩形。
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
				TRACE( "必须载入一副位图 "); 
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
	{// 按钮被按下
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
				TRACE( "必须载入一副位图 "); 
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
			TRACE( "必须载入一副位图   U "); 
		} 
	}
	*/
	if(m_BKImage == NULL)
	{
		TRACE("请加载图片！\n");
		return;
	}

	CRect rcButton(lpDrawItemStruct->rcItem);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC dcMem;
	dcMem.Attach(lpDrawItemStruct->hDC);	
	// 将按钮尺寸改为图片尺寸
	SetWindowPos(NULL, 
		rcButton.left, rcButton.top, // 按钮的新位置
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3, // 按钮的新尺寸
		SWP_NOMOVE | SWP_NOZORDER); // 不改变位置 | 不改变层次关系
	
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
		rcButton.left, rcButton.top, // 目标矩形的左上角坐标
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3, // 目标矩形的尺寸
		0, m_BKImage.GetHeight() / 3 * nImgNum, // 图片的左上角坐标
		m_BKImage.GetWidth(), m_BKImage.GetHeight() / 3 // 绘图的尺寸
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
				TRACE("加载图片失败！\n");
			}
		}
	}
}




IMPLEMENT_DYNAMIC(CBtnRadio, CButton)


CBtnRadio::CBtnRadio()
{
	m_pFont = new CFont;
	m_pFont->CreateFont(12, //以逻辑单位方式指定字体的高度
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
		_T("宋体") //指定字体的字样名称
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
	dc.Attach(lpDrawItemStruct->hDC);       //按钮控件DC    

	COLORREF clrBK = RGB(255,255,0);  
	//int n = this->GetCheck();
	if(m_bCheck == TRUE)
	{// 已选中
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
	
	//绘制背景
	CRect rect(lpDrawItemStruct->rcItem);
	//DrawRoundRect(dc,10,rect,clrBK);
	dc.FillRect(&rect, &CBrush(clrBK));
	//绘制文字  
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
	//	rc.OffsetRect(1, 1); // 文字向右下方偏移 给人一种按下按钮的感觉
	//}
	dc.DrawText(strText, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	// 恢复原设置
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
