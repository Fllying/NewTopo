#pragma once
#include "GeoPolygon.h"
//#include "geos.h"
class CGeoBuilding :
	public CGeoPolygon
{
public:
	CGeoBuilding();
	~CGeoBuilding();
	
	// �Ƴ�����ε������
	void buildPolygonSimple(double angle, double area, double length);
	// �õ����������ε���С��Ӿ���
	CGeoPolygon getMinMABR(void);
};

