#include "StdAfx.h"
#include "GeoRoad.h"


CGeoRoad::CGeoRoad(void)
{
}


CGeoRoad::~CGeoRoad(void)
{
}

// ���ɵ�·������
void CGeoRoad::generateRoadCentreLine(void)
{
	
}

// �õ���·������
vector<CGeoPolyline*> CGeoRoad::getRoadCentreLine(void)
{
	return roadCentreLine;
}



// �õ������ĵ�·��(�ǲ���Ҫ����������������չ�������⣩
CGeoRoad* CGeoRoad::refineRoad(void)
{
	return NULL;
}


// �õ��ɵ�·��������������չwidth��ĵ�·��
CGeoRoad* CGeoRoad::refineRoad(double width)
{
	return NULL;
}
// �õ���ĳ��polyline��������չwidth��ĵ�·��
CGeoRoad* CGeoRoad::refineRoad(double width,CGeoPolyline* line)
{
	return NULL;
}
