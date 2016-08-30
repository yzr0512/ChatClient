#pragma once

/************************************************
Class name: CBtnBase
Create by����־��
CreateDate��2016-08-27
Explain������������̳�ϵͳ�Դ���ť������װ���ػ水ť��Ҫ�Ĳ���
	���ڴ���Ļ������ػ水ť������Լ�����Ч��
Change by: 2016-08-27 by ��־�� ��������
************************************************/ 
// CBtnBase ��ť����

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
Create by����־��
CreateDate��2016-08-27
Explain���������������Բ�Ǿ��ΰ�ť ������ɫ û��ͼƬ
Change by: 2016-08-27 by ��־�� ��������
************************************************/ 
// CBtnRndRct Բ�Ǿ��ΰ�ť

class CBtnRndRct : public CBtnBase
{
public:
	CBtnRndRct();
	~CBtnRndRct();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/); // ���ư�ť
	void SetColor(
		COLORREF clrFont, 
		COLORREF clrBKFocus, 
		COLORREF clrBKUnfocus,
		COLORREF clrBKSelected,
		COLORREF clrBKDisable); // ���ð�ť�ĸ�����ɫ������ ���� ���� ѡ�� �����ã�
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE); // ��������

private:
	void DrawRoundRect(CDC& dc, int radius, CRect rect, COLORREF clrBK); // ����Բ�Ǿ���

	COLORREF m_clrFont;			// ������ɫ
	COLORREF m_clrBKFocus;		// ������ɫ
	COLORREF m_clrBKUnfocus;	// ������ɫ
	COLORREF m_clrBKSelected;	// ѡ����ɫ
	COLORREF m_clrBKDisable;	// ������ɫ
	CFont* m_pFont;				// ����

};

/************************************************
Class name: CBtnImage
Create by����־��
CreateDate��2016-08-27
Explain���������������ͼƬ��ť ����͸�� ��ť��С�����ͼƬ����
	ͼƬ������ť����״̬������ ���� ���£� ����
Change by: 2016-08-27 by ��־�� ��������
************************************************/ 
// CBtnImg ͼƬ��ť

class CBtnImage : public CBtnBase
{
public:
	CBtnImage();
	~CBtnImage();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetImage(int nResID, const CString &strImgType); // ���ð�ť��ͼƬ

private:
	CImage m_BKImage; // ͼƬ
	
};


/************************************************
Class name: CBtnRadio
Create by����־��
CreateDate��2016-08-27
Explain��������������Ƶ�ѡ�� ������ɫ
Change by: 2016-08-27 by ��־�� ��������
************************************************/ 
// CBtnRadio ��ѡ��

class CBtnRadio : public CButton
{
	DECLARE_DYNAMIC(CBtnRadio)

public:
	CBtnRadio();
	virtual ~CBtnRadio();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/); // ���ư�ť
	void SetCheck(BOOL bCheck = TRUE); // ���ð�ť��ѡ��״̬

protected:
	BOOL m_bCheck; // ��¼��ť��ѡ��״̬
	BOOL m_bFocus; // ����Ƿ���ͣ�ڰ�ť�Ϸ�
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	BOOL m_bTracking; // �Ƿ����ڸ������

	CFont* m_pFont; // ����

	COLORREF m_clrFont;			//������ɫ 
	COLORREF m_clrBKFocus;		//������ɫ
	COLORREF m_clrBKUnfocus;	//������ɫ
	COLORREF m_clrBKSelected;	//ѡ����ɫ
	COLORREF m_clrBKDisable;	//������ɫ

};