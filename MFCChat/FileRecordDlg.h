
/************************************************
�� �� ����FileRecordDlg.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;���ļ���¼���ڵ�ͷ�ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

#pragma once
//#include "MyButton.h"

// CFileRecordDlg �Ի���

class CFileRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRecordDlg)

public:
	CFileRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileRecordDlg();

// �Ի�������
	enum { IDD = IDD_FILE_DLG };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSend(); // �ı���ʾ���б�
	afx_msg void OnBnClickedRadioRecv(); // �ı���ʾ���б�
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int RefreshListCtrlData(void); // ˢ���б������

	// ��ť�ؼ����� �����ػ水ť
	CBtnRadio m_BtnRadioSend;
	CBtnRadio m_BtnRadioRecv;
	
	CListCtrl m_ListCtrlRecv;
	CListCtrl m_ListCtrlSend;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	bool m_bIsSendList; // ������¼��ǰ��ʾ���б�ؼ�

};
