#pragma once
#include "afxwin.h"


// CAttFindDialog �Ի���

class CAttFindDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAttFindDialog)

public:
	CAttFindDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttFindDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGFINDATTRIBUTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comboBox;
	CString attribute;

	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedButtoncolor();
	COLORREF lineColor;
	CEdit m_showColor;
};
