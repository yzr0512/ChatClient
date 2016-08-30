#pragma once

/************************************************
Class name: CBtnBase
Create by：余志荣
CreateDate：2016-08-27
Explain：这个类用来继承系统自带按钮，并封装了重绘按钮需要的参数
	可在此类的基础上重绘按钮的外观以及交互效果
Change by: 2016-08-27 by 余志荣 新增此类
************************************************/ 
// CBtnBase 按钮基类

class CBtnBase : public CButton
{
	DECLARE_DYNAMIC(CBtnBase)

public:
	CBtnBase();
	virtual ~CBtnBase();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/) = NULL;

protected:
	DECLARE_MESSAGE_MAP()

	int m_nCtrlState;

private:
	bool m_bTracking;
	
	virtual void PreSubclassWindow();
};


/************************************************
Class name: CBtnRndRct
Create by：余志荣
CreateDate：2016-08-27
Explain：这个类用来绘制圆角矩形按钮 背景纯色 没有图片
Change by: 2016-08-27 by 余志荣 新增此类
************************************************/ 
// CBtnRndRct 圆角矩形按钮

class CBtnRndRct : public CBtnBase
{
public:
	CBtnRndRct();
	~CBtnRndRct();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/); // 绘制按钮
	void SetColor(
		COLORREF clrFont, 
		COLORREF clrBKFocus, 
		COLORREF clrBKUnfocus,
		COLORREF clrBKSelected,
		COLORREF clrBKDisable); // 设置按钮的各种颜色（字体 高亮 正常 选中 不可用）
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE); // 设置字体

private:
	void DrawRoundRect(CDC& dc, int radius, CRect rect, COLORREF clrBK); // 绘制圆角矩形

	COLORREF m_clrFont;			// 字体颜色
	COLORREF m_clrBKFocus;		// 高亮颜色
	COLORREF m_clrBKUnfocus;	// 正常颜色
	COLORREF m_clrBKSelected;	// 选中颜色
	COLORREF m_clrBKDisable;	// 禁用颜色
	CFont* m_pFont;				// 字体

};

/************************************************
Class name: CBtnImage
Create by：余志荣
CreateDate：2016-08-27
Explain：这个类用来绘制图片按钮 可以透明 按钮大小会根据图片调整
	图片包含按钮三个状态（正常 高亮 按下） 竖放
Change by: 2016-08-27 by 余志荣 新增此类
************************************************/ 
// CBtnImg 图片按钮

class CBtnImage : public CBtnBase
{
public:
	CBtnImage();
	~CBtnImage();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetImage(int nResID, const CString &strImgType); // 设置按钮的图片

private:
	CImage m_BKImage; // 图片
	
};


/************************************************
Class name: CBtnRadio
Create by：余志荣
CreateDate：2016-08-27
Explain：这个类用来绘制单选框 背景纯色
Change by: 2016-08-27 by 余志荣 新增此类
************************************************/ 
// CBtnRadio 单选框

class CBtnRadio : public CButton
{
	DECLARE_DYNAMIC(CBtnRadio)

public:
	CBtnRadio();
	virtual ~CBtnRadio();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/); // 绘制按钮
	void SetCheck(BOOL bCheck = TRUE); // 设置按钮的选中状态

protected:
	BOOL m_bCheck; // 记录按钮的选中状态
	BOOL m_bFocus; // 鼠标是否悬停在按钮上方
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	BOOL m_bTracking; // 是否正在跟踪鼠标

	CFont* m_pFont; // 字体

	COLORREF m_clrFont;			//字体颜色 
	COLORREF m_clrBKFocus;		//高亮颜色
	COLORREF m_clrBKUnfocus;	//正常颜色
	COLORREF m_clrBKSelected;	//选中颜色
	COLORREF m_clrBKDisable;	//禁用颜色

};