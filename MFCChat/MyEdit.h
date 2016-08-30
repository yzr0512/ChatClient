#pragma once


// CEditLogID

enum				//ö�ٽ�����
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
	CRect m_rctEdit;		//�༭��rct��screen���ݶ���client����
	CFont m_Font;			//����

	BOOL m_bHover;		//is��ͣ
	BOOL m_bTracking;		//is���
	BOOL m_bUser;		//is���û��޸ģ�setWindowText���ᴥ��change��Ϣ
	BOOL m_bEmpty;		//is�ı�Ϊ��
	int m_nCtrlState;

	COLORREF m_clrShadow;				//��Ӱ	
	COLORREF m_clrBkGndNormal;		//Ĭ�����뽹���ɫ
	COLORREF m_clrBkGndFocus;		//���뽹���ɫ
	COLORREF m_clrBorderNormal;		//Ĭ�ϱ߿�����ɫ
	COLORREF m_clrBorderHorver;		//��ͣ�߿�����ɫ
	COLORREF m_clrBorderEdit;			//�༭�������ɫ

	int m_nFocusStyle;						//������
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
	bool m_bIsEmpty; // ���ֻ����ʾ��Ϣ��Ϊtrue

private:
	bool m_bTracking;
};

// CEditLoginID ��½���ڵ�ID�༭��
