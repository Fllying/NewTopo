#include "stdafx.h"
#include "GeoBuilding.h"


CGeoBuilding::CGeoBuilding()
{
}


CGeoBuilding::~CGeoBuilding()
{
}


// �Ƴ�����ε������
void CGeoBuilding::buildPolygonSimple(double angle, double area, double length)
{
	int circleNum1 = circleNum;//�����⣬���ϲ���֮��circleҪ������֯
	for (int i = 0;i<circleNum1;i++)
	{
		vector<CGeoPolyline*> simpleCircle = commoncircles[i];//Ӧ��Ϊ�գ��ϲ����ʱ��Ҫ����circleNum�����˽�����ҪrebuildCircle���ϲ�Ҫrebuild��rebuildǰҪ�ж�
		//rebuildǰҪ�ж������Ƿ��ǿգ�����ǿվ����룬���վ���� 
		int lineSize = simpleCircle.size();
		for (int j = 0;j<lineSize;j++)
		{
			CGeoPolyline* lineTemp = simpleCircle[j];
			lineTemp->buildLineSimple(angle,area,length);
		}
	}
}





// �õ����������ε���С��Ӿ���
CGeoPolygon CGeoBuilding::getMinMABR(void)
{
	return CGeoPolygon();
}
