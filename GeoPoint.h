#pragma once
#include "GeoObject.h"
class CGeoPoint :
	public CGeoObject
{//����
public:
	CGeoPoint();
	void Draw(CDC *pDC);
	CGeoPoint(CPoint pt1);
	~CGeoPoint();

	void setPoint(CPoint pt1);
	double getx();
	double gety();
	int pointStyle;//0�������ڹ����ߣ�1�������ڹ����ߵ������ڶ˵㣬2�������ߵĶ˵�
private:
	CPoint pt;
};

