#pragma once
#include<vector>
#include"GeoObject.h"
#include"stdafx.h"
#include"GeoPolygon.h"
#include"GeoPolyline.h"
#include"GeoPoint.h"
#include"MyDCConfig.h"
#include "GeoBuilding.h"

struct LinePointPosition {//�������Ƚ���ͬʱ�����ص�һ���������յ㣬�Լ��ڶ����������յ㣬��Ϊ˳ʱ�뷽��
	int line1Ori;
	int line1End;
	int line2Ori;
	int line2End;
	int posInCommonLayer;
	
};
using namespace std;
class CGeoLayer
{
public:
	CGeoLayer();
	~CGeoLayer();
	bool isTopLayer;
private:
	CRect rectLWC;

public:

	vector<CMyPoint*> commonPoint;
	vector<CRect*> objectMABRs;
	vector<CGeoObject*> objects;
	vector<CGeoPolyline*> commonEdge;
	vector<CGeoObject*> objectsTemp;
	unsigned int i;
	bool isPolygonSimply;
	int IsLineSimply;
	void Draw(CDC *pdc, CMyDCConfig myCDcfg);//���˽�����ͨ������������

	CGeoObject* GetObjectAt(int);//��ȡ��n������
	void LayerGeneral();//ͼ���ۺ�
	void TopoBuild();//���˽���
	void AddObject(CGeoObject *obj,vector<CGeoObject*>* objs);//���Ҫ��
	int GetObjectNum();//��ȡҪ�ظ���
	void BuildMABR();//������MABR���뵽����
	void GetObjectIntersectMABR();//��ÿ�������ཻ����С��Ӿ��Σ�������Ӧ������objectMABRs�е�λ�ü��뵽��object��������,ͬ��ͬһ��ͼ��
	void setRect(CRect rect);
	CRect getRect();
	LinePointPosition findCommonPointInLine(int polygon1, int circle1ID, int pt1ID, int polygon2, int circle2ID, int pt2ID);//���������ϵĵ����ʱ�����д���
	void pointCompared(CGeoPolygon* polygonObject1, CGeoPolygon* polygonObject2);//Ѱ����ͬ�㣬ͬʱ�������ˣ�����
	void pointCompared(CGeoPolyline* circle1, CGeoPolyline* circle2);//Ѱ����ͬ�㣬ͬʱ�������ˣ�����
	void pointCompared();
	void pointCompared(CGeoObject* object);
	void AddAllBoundary();//���ڱ߽��ϵĵ������������
	void AddBoundary(CGeoPolygon *polygon);//��ĳ�����ϵĵ�������빫������
	void AddBoundary(CGeoPolyline *circle);//��ĳ�����ϵĵ�������빫������
	void EndPointGetValue();//��ÿ���������һ����ָ�����
	void EndPointGetValue(CGeoPolygon* polygon);
	void EndPointGetValue(CGeoPolyline* polyline);
	void simplify(double telorance);
	void PolylineDeleted(vector<double> areaParameter);
	void PolylineDeleted(CGeoPolyline* polyline,int num);//ɾ��ĳ����
	void PolylineDelete(CGeoPolyline* line,bool isRemainEnd);//ɾ��һ����ʱ�������ڸ������ɾ������
	void MergeTwoPolygon(CGeoPolygon* leftPolygon, CGeoPolygon* rightPolygon,int num);//��ͬ�����Ժϲ�
	void MergeTwoPolygon(CGeoPolygon* leftPolygon, CGeoPolygon* rightPolygon);//��ͬ�����Ժϲ�
	void deleteCircle();
	bool PolygonCanBeDelete(CGeoPolygon* polygon, vector<double> areaParameter);
	void ReoganizeCommonCircle();
	// ������ͼ����·�Χ
	CRect updateTopoRect();
	// �õ�objects��ÿһ��object�����
	void getObjectArea();
	// ��TOPO�����е�����ڲ�������ɾ����ͬʱ������������ܳ�������	void deleteErrorCircle();
	void CGeoLayer::delErrorCircle();
	// ���������ݽ��и��£��������߻������滯��������
	void updateTopoLayer();
	// ��ͬ������ϲ�
	void PolylineDeleted();
	// �õ��ڵ���ͼ�����������IDֵ���������ڣ�����-1
	int getSelectPolygonID(CPoint pt);
	// �жϵ��Ƿ��ھ�����
	//bool isPtInRect(CPoint pt, CRect rect);
	
	void buildSimple(double angle, double area, double length);
	// ����ÿ�����ϻ������˽ṹ�Ĺ�����
	void buildCircle(void);
	// �õ���·������
	void getRoadCentreLine(void);
	// zf,0623,
	void ObjectFilter(void);
	// zf�������
	void ObjectFilterByArea(void);
	// ��¼��ͼ�������л��ĵ���������
	vector<vector<int>> circlePointList;
	// ��¼��ͼ�������л������������
	vector<vector<double>> circleAreaList;
	// ����һά����,zf,0701
	vector<int> zf_point;
	//���һά����,zf,0701
	vector<double> zf_area;
	// //��ά����ת����һά����
	void getNewArray_Point(void);
	// ��ά����ת��Ϊһά���飬���
	void getNewArray_Area(void);
	// Ѱ��һ�����Ӿ��Σ�zf��0704
	void FindRectangle(void);
	// zf,0708,��С��ȡ��Ӿ���
	void FindMABR(void);
	// ��С��Ӿ����Ż���0712��zf
	void optimize(void);
	// ȥ������㣬zf��0717
	void RemoveRedundant(void);
	// 1003 zf
	void DeleteTail(void);
};

