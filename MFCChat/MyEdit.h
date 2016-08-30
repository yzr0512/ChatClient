#pragma once


// CEditLogID

enum				//枚举焦点风格
{
	FOCUS_DEFAULT,
	FOCUS_BKCOLOR,
	FOCUS_FONTBOLD,
	FOCUS_SHADOW
};

class CEditLogID : public CEdit
{
	DECLARE_DYNAMIC(CEditLogID)

public:
	CEditLogID();
	virtual ~CEditLogID();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	CRect m_rctEdit;		//编辑框rct，screen数据而非client数据
	CFont m_Font;			//字体

	BOOL m_bHover;		//is悬停
	BOOL m_bTracking;		//is检测
	BOOL m_bUser;		//is被用户修改，setWindowText都会触发change消息
	BOOL m_bEmpty;		//is文本为空
	int m_nCtrlState;

	COLORREF m_clrShadow;				//阴影	
	COLORREF m_clrBkGndNormal;		//默认输入焦点底色
	COLORREF m_clrBkGndFocus;		//输入焦点底色
	COLORREF m_clrBorderNormal;		//默认边框条颜色
	COLORREF m_clrBorderHorver;		//悬停边框条颜色
	COLORREF m_clrBorderEdit;			//编辑框框条颜色

	int m_nFocusStyle;						//焦点风格
	CString m_strGrayString;

	CString m_strFaceName;
	int m_nPointSize;

	void SetFont(int nPointSize, LPCTSTR lpszFaceName);
	void SetFocusStyle(int nStyle = FOCUS_DEFAULT);
	void SetBkGndColor(
		COLORREF clrNormal = RGB(255,255,255),
		COLORREF clrFocus = RGB(255,255,255)
		);
	void SetBorderColor(
		COLORREF clrBorderEdit = RGB(0xEA,0xEA,0xEA),
		COLORREF clrBorderNormal = RGB(51, 139, 192),
		COLORREF clrBorderHover = RGB(96, 200, 253));
	void SetShadowColor(COLORREF clrShadow = RGB(0xB8,0xB8,0xB8));
	void SetGrayString(CString str);
};

class CEditBase : public CEdit
{
	DECLARE_DYNAMIC(CEditBase)
public:
	CEditBase();
	virtual ~CEditBase();
	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
	bool m_bIsFocus;
	bool m_bIsHover;
	bool m_bIsEmpty; // 如果只有提示信息则为true

private:
	bool m_bTracking;
};

// CEditLoginID 登陆窗口的ID编辑框
