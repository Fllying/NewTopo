
// NewTopoView.h : CNewTopoView ��Ľӿ�
//

#include <stdlib.h>
#include <crtdbg.h>
#include<string>
#include<sstream>
#include<iostream>
#include"GeoLayer.h"
#include"GeoMap.h"
#include"SelectDialog.h"
#include"AttFindDialog.h"
#include"MainFrm.h"
using namespace std;

#pragma once


//#define MAPSCALE  1000//wmj

#define  ZOOMIN    10001
#define  ZOOMOUT   10002
#define  PAN       10003
#define  TRANSLATE 10004
#define  ONWHOLE   10005
#define  TOPOBUILD 10006
#define  SELECT 10007
#define  POLYGONSELECT 10008
#define  ADDLAYER   10009
#define  ADJOINPOLYGONQUERY 10010
//#define SIMPLIFY   10007


#include "myDCConfig.h"
struct RecordItem {
	BYTE name[11];
	BYTE fieldType;
	int Reserved3;
	BYTE fieldLength;
	BYTE decimalCount;
	short Reserved4;
	BYTE workID;
	short Reserved5[5];
	BYTE mDXFlag1;
};
class CNewTopoView : public CView
{
protected: // �������л�����
	CNewTopoView();


	DECLARE_DYNCREATE(CNewTopoView)

// ����
public:
	CNewTopoDoc* GetDocument() const;
	CMyDCConfig dcCFG;
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	int MAPSCALE;
	int currrentOperateStatus;
	BOOL isMouseDown;

	CDC* dcMem;
	CGeoMap *map;	
	CBitmap bmp;
	CRect rectWC;//��Ļ��Χ�ڵ��߼�Rect;top<bottom,left<right
	CPoint pt1,pt2 ;
	CGeoPolyline* pline11;
	CGeoPolyline* pline22;
	CRect oldRect;//����
	int selectID;
	vector<RecordItem> recordItems;
	bool isTopo;//�Ƿ��������
	int OnChangeByteOrder(int indata);
	void readData(CString& filename,int& layerIndex);//������
	void OnReadDbf(CString filename,int layerIndex);
	void DBFintoSHP(vector<CString>cc, vector<double>shapeArea,int layerIndex);//����ȡ����dbf���ݼӵ�shp�ļ��ж�Ӧ��ͼ����
	virtual ~CNewTopoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnZoomin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateZoomin(CCmdUI *pCmdUI);
	afx_msg void OnWhole();
	afx_msg void OnTranslation();
	afx_msg void OnUpdateTranslation(CCmdUI *pCmdUI);
	afx_msg void OnTopobuild();
	
	afx_msg void OnSimplify();
	afx_msg void OnPolygonsimplify();
	afx_msg void OnAddorimap();
	afx_msg void OnToposave();
	void WriteShp(CString& filename );
	// ��ʮ����ת����ʮ�����ƣ����ܱ������ȡ
	int OnChangeByteOrderTenToSixteen(int indate);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	// ˫�����ͼ
	//bool OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ��ɶ�dbf�����
	void WriteDbf(CString filename);
	afx_msg void OnSelectpolygonid();
	afx_msg void OnFindattribute();
	afx_msg void OnMergegonsarea();
	afx_msg void OnMergepolygonsameatt();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	bool IsSizeChanged;
	afx_msg void OnPolygonSelect();
	// ͨ�������ѡ��ĳ���棬ͬʱ������ʾ
	void MouseSelectPolygon(CPoint pt);
	afx_msg void OnAdddata();
	afx_msg void OnPartTopo();
//	afx_msg void OnBuildSimple();
	afx_msg void OnBuildsimple();
	afx_msg void OnRefineroad();
	afx_msg void OnAdjoinpolygonquery();
	afx_msg void OnFilter();
	afx_msg void OnFilterByArea();
	afx_msg void OnFindRectangle();
	afx_msg void OnMABR();
	afx_msg void OnOptimize();
	afx_msg void OnRemoveredundant();
	afx_msg void OnFinal();
	afx_msg void OnDeletetail();
};

#ifndef _DEBUG  // NewTopoView.cpp �еĵ��԰汾
inline CNewTopoDoc* CNewTopoView::GetDocument() const
   { return reinterpret_cast<CNewTopoDoc*>(m_pDocument); }
#endif

