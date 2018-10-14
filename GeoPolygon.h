#pragma once
#include "GeoObject.h"
#include"GeoPoint.h"
#include"GeoPolyline.h"
#include"MyDCConfig.h"
#include"MyPoint.h"
#include"MABR_ZF.h"
#include<vector>
using namespace std;

struct squarePoint {
	CPoint pt;
	int oldNum;
	int newNum;
	bool operator < (const squarePoint &m)const {
		return pt.y < m.pt.y;
	}
};

class CGeoPolygon :
	public CGeoObject
{
public:
	CGeoPolygon();
	CGeoPolygon(CGeoPolygon &polygon);
	~CGeoPolygon();
	
	void Draw(CDC *pDC);//Ҫ�ػ���
	void Draw(CDC *pDC, CMyDCConfig myDCcfg);//ͨ��������������л���
	int GetCircleNum();//�õ��ӻ���Ŀ

    //CGeoPolyline* GetCircleAt(int index);//�õ����еĵ�index�ӻ�
	void AddCircle(CGeoPolyline*);//����ӻ�
	CRect* GetObjectMABR();
	void deleteLine(int lineID);//Ŀǰû�õ�
	void rebuildCircle();//��commonEdge����������������������ӵ�commonCircle��
	vector<CGeoPolyline*> rebuildCircle(vector<CGeoPolyline*> lines, int i);//��lines[i]�γ�һ���������������
	void mergeCommonEdge(CGeoPolyline* line1,CGeoPolyline* line2,int style);//�ϲ���������line1��line2������Ҫ�޸�line��circleID
	//line1�����=line2����㷵��1��line1���յ�=line2���յ㷵��2��line1���յ�=line2����㷵��3��line1�����=line2���յ㷵��4���޷���������0
	void ReoganizeCommonCircle();//�õ�������commonCircles,ͬʱcirclePoints��circlePointNum���ٴ洢�ռ䡢TopoCircleNum����ֵ

	void eraseNullLine();//�ؽ�commonCircleʱɾ��isNULL=true����
	void adjustCommonCircle();//�����������ڲ�����˳�����ú���
	void adjustCommonCircle(vector<CGeoPolyline*> lines);//���ú���
	// �������
	double TopoArea; 
	double shapeArea;//�ʼ��������
	int circleNum;//����
	int TopoPointNum;//�������
	double TopoLength;//���л��ĳ���
	bool isTopoed;

	vector<CGeoPolyline*>circles;//�ӻ�
	vector<CGeoPolyline*>commonEdge;//�����ù���������
	vector<vector<CGeoPolyline*>> commoncircles;//���ڴ�Ź����ߵĻ������ڼ���������Լ�����ļ�
	vector<vector<CMyPoint*>> circlePoints;   //ÿ�����ĵ㣬zf
	vector<int> circlePointNum;//ÿ���ӻ��ϵ����Ŀ
	vector<vector<int>> nodePos;//ÿ���ӻ��Ͻ����λ��

	void getArea();//�õ����˺�����
	double getArea(int index);//��������linesΧ�ɵĻ��������

	 // �õ�һ������Ϊpt1,pt2,pt3��������������
	//double getTriangleArea(CPoint pt1, CPoint pt2, CPoint pt3);
	// �õ�����֮��ľ���
	//double getTwoPointDistance(CPoint pt1, CPoint pt2);
	// ɾ������circle�������
	void deleteCircle();
	// ������lines��Ѱ����i��������һ���������������±�
	CPoint findNextLine(vector<CGeoPolyline*> lines, int i);
	// ��һ��commonCircles�ĵ�˳��ŵ�һ�������ͬʱ�������ݼ��
	vector<CMyPoint*> getCirclePoint(vector<CGeoPolyline*> lines);
	// �Ի��ڵ�����������򣬷��ظõ��������������������λ��.pt.x�����˸õ�������������λ�ã�pt.y����
	//��������i����С��������ֵ
	vector<CPoint> circlePointReorder(vector<CPoint>& pts);
	//�����ʷ�
	void tixingSplit(vector<CPoint> cpts, int i, vector<CPoint> a, vector<vector<SegMent>>& tixingLine);
	// �߶�line�ϣ���������Ϊy�ĵ��Ӧ�ĺ�����
	double getSegementLeft(SegMent line, double y,int i);
	// ����һ�������ڵ�����б����ɵ����
	double getArea(vector<SegMent> segments,int i);
	// ������m��������һ�µĵ�����һ������±�
	int findSamePoint(vector<squarePoint> aPoint, int m);
	// �������д�i��j������������޸ģ�����i-1�����j������֮�Ȼ��ƽ�ֵ�ÿ���㣬Ҫ����ƽ��
	void modifySquarePoint(vector<squarePoint>& aPoint, int i, int j);
	// �õ����˽�����Ķ���Χ
	void getObjectTopoRect();
	// ����ĳ�������ܳ�
	double getCircleLength(int i);
	// ��CPoint����Χ�ɵĻ������
	double getArea(vector<CMyPoint*> pts);
	// ��һ�����ϵ㣬��x����С�����Լ�yֵ����С���꣬����100
	double getMinY(vector<CMyPoint*> pts);
	// �õ����˽�����ĵ�������circlePoints�л�ã�
	void getTopoPointNum();
	// ÿ�������ܳ�
	vector<double> circleLength;
	// �õ�ÿ�����ĳ���
	void getCircleLength();
	// ÿ���������
	vector<double> area;
	// ͨ���ܳ��������ɾ���������������ڻ�
	void delErrorCircle();
	// ��commonCircle�л�ȡ�㣬����һ��˳����뵽circlePoints��
	void getCirclePoint();
	// �ж�һ�����Ƿ����������
	bool isPtIn(CPoint pt);
	// ����a��b������������С��ľ���ֵ
	//double getMod(double a, double b);
	// �ж�һ�����Ƿ����������
	bool isPtInNew(CPoint pt);
	// �жϵ�pt�����Ϊpt1�յ�Ϊpt2���߶εĹ�ϵ
	int getLinkofPtToSeg(CPoint pt1, CPoint pt2, CPoint pt);
	// pt1��pt2�ֱ���һ���߶ε������յ㣬�ж�pt���߶ε�����,��1������࣬-1�����ϣ�0�����Ҳ�
	//int getPtLRtoLine(CPoint pt1, CPoint pt2, CPoint pt);
	// �ж�һ���߶�����Ĺ�ϵ��0�߶������⣻1����ӣ�2�ཻ��3����ӣ�4���ڲ�
	int isSegmentInPolygon(SegMent line);
	// �жϵ��Ƿ�����ı߽���
	bool isPtOnEdge(CPoint pt);
	// �жϵ��Ƿ�������
	bool isPtOnSegment(CPoint pt, SegMent line1);
	// �õ�line2����line1�ϵĶ˵㣬���ֻ��һ���˵㣬���ظö˵㣬���û�ж˵㷵�أ�0,0��������������˵㣬���أ�1,1����line2�������˵�ͬʱ���
	CPoint getJointPt(SegMent line1, SegMent line2);
	//aa, bbΪһ���߶����˵� cc, ddΪ��һ���߶ε����˵� �ཻ����true, ���ཻ����false  
	bool intersect(SegMent line1,SegMent line2);  
	double determinant(double v1, double v2, double v3, double v4);
	// �Ե�����oriPts����ȥ������xС����ǰ��x��ͬyС��ǰ
	vector<CPoint> pointsSort(vector<CPoint> oriPts);
	// ����pts����һ���߶������ཻ�Ķ�����������ж����������ڵ���е��Ƿ����������⣬��������⣬����-1�������ڣ�����1��������У�����0
	int getSegmentPos(vector<CPoint> pts);
	
	// �ж��������Ƿ���Ժϲ����������������ͬһ�˵㣬��������һ�£�����Ժϲ�
	bool judgeLineMerge(CGeoPolyline* line1,CGeoPolyline* line2);
	// �ж��������ںϵ����ͣ�line1�����=line2����㷵��1��line1���յ�=line2���յ㷵��2��line1���յ�=line2����㷵��3��line1�����=line2���յ㷵��4���޷���������0
	int styleLineMerge(CGeoPolyline* line1, CGeoPolyline* line2);

	// zf,0623����Ĺ���
	void ObjectFilter(void);
	// zf,���������
	void ObjectFilterByArea(void);
	//zf��0701,���ĵ�����ɵ�����

	vector<int> circlePointList;
	//zf��0701,���������ɵ�����
	vector<double> circleAreaList;
	// ��ȡ��������л��ĵ���

	vector<int> getPointNum(void);
	// ��ȡ��������л������
	vector<double> getCircleArea(void);
	// Ѱ��һ�����Ӿ��Σ�zf��0704
	void FindRectangle(void);

	// �洢һ����Ӿ��ε�һά���飬zf��0704
	vector<CPoint*> rectangleArray;

	// Ѱ��ĳһ��circle����Ӿ��Σ�zf��0704
	void FindRectangle(vector<CMyPoint*> pts);

	// �洢��ת����Ӿ��ε����飬zf��0706
	vector<CPoint*> rectangleRotate;

	// ĳһ��pt��center��תtheta���ȣ�zf,0706
	CPoint* rotate(CPoint* pt, CPoint* center, double theta);
	// ��ת���о��Σ�zf��0706
	void rotateAll(void);
	// zf,0708��Ѱ����С��Ӿ���
	void FindMABR(void);
	// Ѱ�һ������ģ����ģ���zf��0708
	CPoint* FindCenter(vector<CPoint*> ptsArray);
	// ĳ����ε�һ����Ӿ��Σ�zf��0708
	vector<CPoint*> FindRectangle(vector<CPoint*> pts);
	// ���� ������ռ��С��Ӿ��εı�����������/��С��Ӿ��� ��zf��0712
	bool areaProportion(void);
	// ��С��Ӿ����Ż���0712��zf
	bool optimize(void);
	// �Ƿ��ҵ���Ӿ��Σ�zf��0714
	bool hasFindMABR;
	// �Ƿ��ҵ�һ����Ӿ��Σ�����Ӿ��Σ�
	bool hasFindRectangle;
	// ȥ������㣬zf��0717
	void RemoveRedundant(void);
	// ��������֮��ĽǶȣ�zf��0717
	double calAngle(CMyPoint* p1, CMyPoint* p2, CMyPoint* p3);
	// �ж�ĳ�����ǲ����������У�zf��0717
	bool isContained(vector<int> temp, int i);
	// MABR��Ͻ��������Σ�zf��0719
	void fitting(CGeoPolyline* line);
	// ��Ϻ�ɹ���zf,0720
	vector<CPoint*> buildings;
	// 20180720,zf
	bool hasGetBuildings;
	// 0904,zf,����һ��ĵ�
	void GetHalfPoints(void);
	
	// ȥ��β�� 1003 zf
	void DeleteTail(void);
	// ��������ľ���  1003 zf
	double calDis(CMyPoint* pt1, CMyPoint* pt2);
	// �����ֵ��zf��1003
	int areaT;
	// �������  zf  1003
	int times;
};
