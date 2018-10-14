#pragma once
#include "GeoObject.h"
#include"GeoPoint.h"
#include<vector>
#include"MyPoint.h"
#include"MyDCConfig.h"
#include"MABR_ZF.h"
#include "atltypes.h"
using namespace std;
typedef struct tagCOLINFO
{
	USHORT from;
	USHORT to;
	BYTE   nItem;//����դ����ʱΪ������ֵ
}COLINFO;
typedef CArray<COLINFO, COLINFO> ColInfoArray;
typedef CArray<CPoint, CPoint> CPointArray;
class CGeoPolyline :
	public CGeoObject
{
public:
	CGeoPolyline();
	CGeoPolyline(CGeoPolyline & polyline);
	//CGeoPolyline(CGeoPolyline ** polyline);
	~CGeoPolyline();

	int leftPolygonID;//����ID
	int rightPolygonID;//����ID
	CString leftAttribute;//��������
	CString rightAttribute;//��������
	int polygonID;//δ����ǰ�������������ID
	int circleID;//δ����ǰ���û���circle�����е��±�
	vector<CMyPoint*> pts;
	//vector<int>endPointPos;//�˵��±�
	bool canDeleted;
	

	void AddPoint(CMyPoint*);
	void setPoint(int index, CMyPoint* mypoint);
	void Draw(CDC* pDC);
	void Draw(CDC* pDC, CMyDCConfig myDCcfg);
	int GetSize();
	CMyPoint* GetPointAt(int index);
	void removePoints(int index1, int index2, int  posInCommonLayer);
	//�㵽�߶ε���̾���
	double MinsPointToSeg(CMyPoint* pt, CMyPoint* ptOri,CMyPoint* ptEnd);
	//double distanceBetweenTwoPoints(CMyPoint* pt1, CMyPoint* pt2);
	int getMaxArcLength(vector<CMyPoint*> pts,int start,int end, double& maxLength);
	int getFarestPointNum(vector<CMyPoint*> pts);
	void DouglasPeuckerDataCompress(double tolerance);
	void DouglasPeuckerDataCompress(vector<CMyPoint*> pts,CUIntArray& aPtNum, double tolerance);
	bool JudgeCanDeleted();
	// �Ƿ���빫����
	int isInCommonCircle;//0����δ�뻷��1�������׵�����һ��������2������β������һ������
	// �õ�ĳ�����ĳ���
	double getLineLength();
	// �õ�һ������һ�����ļн�
	double getAnglePtToLine(CPoint pt);
	// pt1��ʾ�߶���㣬pt2��ʾ�߶��յ㣬pt��ʾ�����
	//double getAngle(CPoint pt1, CPoint pt2, CPoint pt);
	// pt1��ʾ�߶���㣬pt2��ʾ�߶��յ㣬pt��ʾ����㣬�ж�pt1��pt��pt2���ߵ����Ҳ࣬��෵��-1���Ҳ෵��1�����Ϸ���0
	int getPtLRtoLine(CPoint pt1, CPoint pt2, CPoint pt);

	// // �Ƴ�����ε�����㣬������С��angleͬʱ�����߼�������С��area������ɾ���ö��㣬ͬʱ��ʾ�������ݣ�Ҫע��˵㲻��ɾ
	void buildLineSimple(double angle, double area,double length);
	// ����a��b������������С��ľ���ֵ
	//double getMod(double a, double b);
	//int getStyleFourptsUnit(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4);
	// �������ڽ��ĵ�ȥ�̱ߴ���
	void buildFourptDelshortline(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4);
	// �������ڽ��ĵ�Z��ȥ�̱ߴ���
	void buildFourptDelShortlineZ(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4);
	// �������ڽ��ĵ�U��ȥ�̱ߴ���
	void buildFourptDelShortlineU(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4);
	// �ж��ĵ��Ƿ���ƿ����
	bool isBottleNeck(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4);
	// �õ����е�i��ǰ��i�������Ч���±�
	int getPrevalidPtNum(int x);
	// �õ����е�x����һ����Ч����±�
	int getNextvalidPtNum(int x);
	// ��MyPoint��������תΪCPoint����
	bool MyPointsToCPoints(vector<CMyPoint*> pts, CPointArray &points);
	// zf,0623,������
	void ObjectFilter(void);
	// //zf,����ĳ���������
	double getArea(vector<CMyPoint*> pts);
	// zf.��ȡ��СY
	double getMinY(vector<CMyPoint*> pts);
	// zf������
	void ObjectFilterByArea(void);
	// Ѱ��һ�����Ӿ��Σ�zf��0704
	void FindRectangle(void);

	//������Ӷ�������飬zf��0708
	vector<MABR_ZF*> MABR;
	//������С��Ӿ��Σ�zf��0708,δ��ת����
	MABR_ZF finalMABR;
	// ordinary Points �����ĵ㼯��zf��0708
	vector<CPoint*> odinPts;
	// ��CMyPoint����ת��ΪCPoint��
	void MyPoint2Point(vector<CMyPoint*> pts);

	// �������ռ��С��Ӿ��ε����������zf��0712
	double areaProportion;
	// �������ģ����ģ���zf��0712
	CPoint center;
	// �趨�������ģ�zf��0712
	void setCenter(CPoint* pt);

	// ���������ƽ�����룬zf��1003
	//double avgDis;
	// ���������ƽ�����룬zf,1003
	double calAvgDis(vector<CMyPoint*> pts);
	// �����������룬zf��1003
	double calDis(CMyPoint* pt1, CMyPoint* pt2);
	// �±����ֵ
	int pointDisThreshold;
};

