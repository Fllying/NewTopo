
// NewTopo.h : NewTopo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CNewTopoApp:
// �йش����ʵ�֣������ NewTopo.cpp
//

class CNewTopoApp : public CWinApp
{
public:
	CNewTopoApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNewTopoApp theApp;
