#pragma once
#include<vector>
using namespace std;
struct PointTopo//���ڴ洢һ��������Ӧ�ĵ��桢�������е�λ����Ϣ
{
	int x;//����������±�
	int y;//�����ڵĻ������е��±�
	int z;//�����ڻ��е��±�
	void setTopo(int x1, int y1, int z1) {
		x = x1;
		y = y1;
		z = z1;
	}
};
class CMyPoint
{
public:
	CMyPoint();
	CMyPoint(CPoint pt1);
	CMyPoint(CMyPoint &point);
	//CMyPoint(CMyPoint** point);
	CPoint GetCPoint();
	double Getx();
	double Gety();
	BOOL isCommon;//�Ƿ���뵽����������
	unsigned long int PosInCommonPoint;
	~CMyPoint();
	//vector<PointTopo> PointGonTopuPos;//һ��������������Map�����е�λ�ã������pt1ͬ����polygon1��polygon2��������������
	                                  //λ�÷ֱ���35��1086,�򽫸õ����������ͨ��CPoint��ʽ��¼���˵���ж��CPoint�����Բ�������
	bool isNULL;
	void setPoint(CPoint pt1);
	int polygonID;//��������layerͼ���е��±�
	int circleID;//��������circles�����е��±�
	int ptID;//����circle��pts�����е��±�
	bool isJointPt;//�Ƿ��Ƕ˵�
	bool isUsed;

	CPoint pt;
public:
	// �ж�һ�����Ƿ���rect��
	bool isInRect(CRect rect);
	// CMyPointת��ΪCPoint�࣬zf��0708
	CPoint* MyPoint2Point(CMyPoint* pt);
};

