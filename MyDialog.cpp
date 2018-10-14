// MyDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewTopo.h"
#include "MyDialog.h"
#include "afxdialogex.h"


// CMyDialog �Ի���

IMPLEMENT_DYNAMIC(CMyDialog, CDialogEx)

CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_lineTelorance(0)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITLINE, m_lineTelorance);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC1, &CMyDialog::OnStnClickedStatic1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMyDialog::OnCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDITLINE, &CMyDialog::OnChangeEditline)
END_MESSAGE_MAP()


// CMyDialog ��Ϣ�������


void CMyDialog::OnStnClickedStatic1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL CMyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Slider.SetRange(0, 10);

	m_Slider.SetTicFreq(0.5);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMyDialog::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_lineTelorance = m_Slider.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CMyDialog::OnChangeEditline()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_Slider.SetPos(m_lineTelorance);
}
