#pragma once


// CSelectDialog �Ի���

class CSelectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDialog)

public:
	CSelectDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECTDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��ѯĳ�����id��Ȼ�������ʾ
	int m_SelectID;
};
