// PolygonSimplifyDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewTopo.h"
#include "PolygonSimplifyDialog.h"
#include "afxdialogex.h"


// CPolygonSimplifyDialog �Ի���

IMPLEMENT_DYNAMIC(CPolygonSimplifyDialog, CDialogEx)

CPolygonSimplifyDialog::CPolygonSimplifyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, MinBuildingArea(0)
	, MinGardenArea(0)
	, m_MinParkArea(0)
	, m_MinQitayuandiArea(0)
{

}

CPolygonSimplifyDialog::~CPolygonSimplifyDialog()
{
}

void CPolygonSimplifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITBUILDINGAREA, MinBuildingArea);
	DDX_Text(pDX, IDC_EDITGARDENAREA, MinGardenArea);
	DDX_Text(pDX, IDC_EDIT2, m_MinParkArea);
	DDX_Text(pDX, IDC_QITAYUANDI, m_MinQitayuandiArea);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider_building);
	DDX_Control(pDX, IDC_SLIDER2, m_Slider_Garden);
}


BEGIN_MESSAGE_MAP(CPolygonSimplifyDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDITGARDENAREA, &CPolygonSimplifyDialog::OnEnChangeEditgardenarea)
	ON_BN_CLICKED(2, &CPolygonSimplifyDialog::OnBnClicked2)
END_MESSAGE_MAP()


// CPolygonSimplifyDialog ��Ϣ�������


void CPolygonSimplifyDialog::OnEnChangeEditgardenarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPolygonSimplifyDialog::OnBnClicked2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
