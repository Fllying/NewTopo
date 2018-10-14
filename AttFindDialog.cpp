// AttFindDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewTopo.h"
#include "AttFindDialog.h"
#include "afxdialogex.h"


// CAttFindDialog �Ի���

IMPLEMENT_DYNAMIC(CAttFindDialog, CDialogEx)

CAttFindDialog::CAttFindDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGFINDATTRIBUTE, pParent)
{

}

CAttFindDialog::~CAttFindDialog()
{
}

void CAttFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Control(pDX, IDC_EDIT1, m_showColor);
}


BEGIN_MESSAGE_MAP(CAttFindDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAttFindDialog::OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTONCOLOR, &CAttFindDialog::OnBnClickedButtoncolor)
END_MESSAGE_MAP()


// CAttFindDialog ��Ϣ�������


BOOL CAttFindDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	lineColor = RGB(32, 190, 190);
	m_comboBox.AddString(_T("0110"));

	m_comboBox.AddString(_T("0210"));

	m_comboBox.AddString(_T("0212"));
	m_comboBox.AddString(_T("0213"));

	m_comboBox.AddString(_T("0230"));

	m_comboBox.AddString(_T("0290"));
	m_comboBox.AddString(_T("0300"));

	m_comboBox.AddString(_T("0310"));

	m_comboBox.AddString(_T("0322"));
	m_comboBox.AddString(_T("0323"));

	m_comboBox.AddString(_T("0330"));

	m_comboBox.AddString(_T("0400"));
	m_comboBox.AddString(_T("0411"));

	m_comboBox.AddString(_T("0412"));

	m_comboBox.AddString(_T("0500"));
	m_comboBox.AddString(_T("0520"));
	m_comboBox.AddString(_T("0550"));
	m_comboBox.AddString(_T("0600"));
	m_comboBox.AddString(_T("0640"));
	m_comboBox.AddString(_T("0700"));
	m_comboBox.AddString(_T("0710"));
	m_comboBox.AddString(_T("0712"));
	m_comboBox.AddString(_T("0713"));
	m_comboBox.AddString(_T("0719"));
	m_comboBox.AddString(_T("0720"));
	m_comboBox.AddString(_T("0820"));
	m_comboBox.AddString(_T("0950"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAttFindDialog::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_comboBox.GetCurSel();

	CString strCBText;

	m_comboBox.GetLBText(nIndex, strCBText);
	attribute = strCBText;
}


void CAttFindDialog::OnBnClickedButtoncolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	lineColor = RGB(32, 190, 190);      // ��ɫ�Ի���ĳ�ʼ��ɫΪ��ɫ  
	CColorDialog colorDlg(lineColor);         // ������ɫ�Ի��򣬴����ʼ��ɫֵ   

	if (IDOK == colorDlg.DoModal())       // ��ʾ��ɫ�Ի��򣬲��ж��Ƿ����ˡ�ȷ����   
	{
		lineColor = colorDlg.GetColor();      // ��ȡ��ɫ�Ի�����ѡ�����ɫֵ   
		
	}
	BYTE Red = GetRValue(lineColor); ///�õ�����ɫ
	BYTE Green = GetGValue(lineColor); ///�õ�����ɫ
	BYTE Blue = GetBValue(lineColor); ///�õ�����ɫ
	char chR[4];
	itoa(Red, chR, 10);

	char chG[4];
	itoa(Green, chG, 10);

	char chB[4];
	itoa(Blue, chB, 10);

	CString strR, strG, strB;
	strR = chR;
	strG = chG;
	strB = chB;

	CString strRGBText = strR + "," + strG + "," + strB;
	m_showColor.SetWindowText(strRGBText);
	
}
