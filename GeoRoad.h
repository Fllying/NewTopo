#pragma once
#include "geopolygon.h"
class CGeoRoad :
	public CGeoPolygon
{
public:
	vector<CGeoPolyline*> roadCentreLine;
	CGeoRoad(void);
	~CGeoRoad(void);

	// ���ɵ�·������
	void generateRoadCentreLine(void);
	// �õ���·������
	vector<CGeoPolyline*> getRoadCentreLine(void);
	// �õ������ĵ�·��
	CGeoRoad* refineRoad(void);
	// �õ��ɵ�·��������������չwidth��ĵ�·��
	CGeoRoad* refineRoad(double width);
	// �õ���ĳ��polyline��������չwidth��ĵ�·��
	CGeoRoad* refineRoad(double width,CGeoPolyline* line);
};

