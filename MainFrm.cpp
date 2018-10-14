
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "NewTopo.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	//ID_INDICATOR_MOUSE_POS,  //���λ������ֵ����
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_TIME,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_strText = "      �人��ѧ�ʻ�Ժ  ";
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	/*m_wndStatusBar.SetPaneText(1, "111111");
	m_wndStatusBar.SetPaneText(2, "222222");
	m_wndStatusBar.SetPaneText(3, "333333");*/
	
	//SetTimer(1, 1000, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTime;
	// ��ȡϵͳ��ǰʱ�䣬�����浽curTime   
	CTime curTime = CTime::GetCurrentTime();

	// ��ʽ��curTime�����ַ������浽strTime   
	strTime = curTime.Format(_T("%H:%M:%S"));
	// ��״̬����ʱ�䴰������ʾϵͳʱ���ַ���   
	m_wndStatusBar.SetPaneText(4, strTime);

	static int strID = 0;   //�ַ�����
							//�ﵽ������Ļ��ĩβ
	if (strID >= (m_strText.GetLength() - 1))
	{
		strID = 0;
	}
	//m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_SEPARATOR), ((LPCSTR)m_strText) + strID);
	//������ʾ����Ļ�����ں���ռ��λ������+2
	strID = strID + 2;
	CFrameWnd::OnTimer(nIDEvent);
}
