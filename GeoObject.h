#pragma once
#include<vector>
#include"MyDCConfig.h"
#include "atltypes.h"
using namespace std;

struct SegMent {

	CPoint ptOri;
	CPoint ptEnd;
	SegMent(CPoint pt1, CPoint pt2){
		ptOri = pt1;
		ptEnd = pt2;
	}
	void setSegMent(CPoint pt1, CPoint pt2) {
		ptOri = pt1;
		ptEnd = pt2;
	}
	bool operator < (const SegMent &m)const {
		return ptOri.x < m.ptOri.x;
	}
};
class CGeoObject
{
public:
	CGeoObject();
	~CGeoObject();
	unsigned long int MABRpos;
	bool isNULL;
	unsigned long int PosinCommonLayer;//������Map������objects�е�λ��
	CString objectAttribute;//����
	
	CRect* objectMABR;//��С��Ӿ���
	vector<long int> intersectMABRpos;//��֮�ཻ��С��Ӿ�����Map��objectMABR�е�λ��
	vector<long int> intersectMABRposCopy;

	BOOL isObjectMABRintersect(CGeoObject* geoObject);//�����������С��Ӿ����Ƿ��ཻ
	void BesidesObjectSearch();//��ΧҪ������
	void TopoBuild();//���˽���
	void ObjectMerge();//Ҫ�غϲ�
	void ObjectDelete();//Ҫ��ɾ��
	void ObjectSimplify();//Ҫ�ػ���
	void ObjectShift();//Ҫ����λ
	
	virtual CRect* GetObjectMABR();
	virtual int GetCircleNum();//�õ��ӻ���Ŀ	
	//void getIntersectMABRposCopy();//������ֵ

	virtual void Draw(CDC * pDC){};//Ҫ�ػ���
	virtual void Draw(CDC* pDC, float width, int lineStyle, COLORREF lineColor, int fillstyle, COLORREF fillColor);//
	virtual void Draw(CDC* pdc, CMyDCConfig myDCcfg);//
													 // һ���潨�����˺�ķ�Χ
	CRect objectTopoRect;
	// // �õ�һ������Ϊpt1,pt2,pt3��������������
	double getTriArea(CPoint pt1, CPoint pt2, CPoint pt3);
	// �õ�����֮�����
	double getTwoPointDis(CPoint pt1, CPoint pt2);
	// ����a��b������������С��ľ���ֵ
	double getMod(double a, double b);
	// pt1��ʾ�߶���㣬pt2��ʾ�߶��յ㣬pt��ʾ�����,���ص��Ǧ���
	double getAngle(CPoint pt1, CPoint pt2, CPoint pt);
	//  pt1��pt2�ֱ���һ���߶ε������յ㣬�ж�pt���߶ε����Ҳ࣬1������࣬-1�����ϣ�0�����Ҳ�
	int ptPostoLine(CPoint pt1, CPoint pt2, CPoint pt);
	// �㵽ֱ�ߵ���̾���
	double MinsPointToLine(CPoint pt, CPoint ptOri, CPoint ptEnd);
	// �õ���ƽ�е�����ֱ�ߵĽ���
	CPoint getLineCrossPt(SegMent segMent1, SegMent segMent2);
	// �ж�pt1��pt4�Ƿ�����pt2-pt3��ͬ�࣬��ͬ�෵��1����෵��-1����һ���������������ϣ�����0
	int getStyleFourptsUnit(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4);

	// zf,0623,����������
	virtual void ObjectFilter(void);
	// zf��0701�����������
	virtual void ObjectFilterByArea(void);
	// ���ص������飬zf��0701
	virtual vector<int> getPointNum(void);
	// ����������飬zf��0701
	virtual vector<double> getCircleArea(void);
	// Ѱ��һ�����Ӿ��Σ�zf��0704
	virtual void FindRectangle(void);
	// zf,0708,��С��ȡ��Ӿ���
	virtual void FindMABR(void);
	// ��С��Ӿ����Ż���0712��zf
	virtual bool optimize(void);
	// ȥ������㣬zf��0717
	virtual void RemoveRedundant(void);
	// zf 1003
	virtual void DeleteTail(void);
};

