#pragma once
#include "afxcmn.h"


// CPolygonSimplifyDialog �Ի���

class CPolygonSimplifyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPolygonSimplifyDialog)

public:
	CPolygonSimplifyDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPolygonSimplifyDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double MinBuildingArea;
	double MinGardenArea;
	double m_MinParkArea;
	afx_msg void OnEnChangeEditgardenarea();
	afx_msg void OnBnClicked2();
	double m_MinQitayuandiArea;
	// ��������
	CSliderCtrl m_Slider_building;
	// ��԰����
	CSliderCtrl m_Slider_Garden;
};
