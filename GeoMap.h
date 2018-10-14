#pragma once
#include "GeoLayer.h"
#include"MyDCConfig.h"
#include<vector>
#include "atltypes.h"
using std::vector;
class CGeoMap
{
public:
	CGeoMap();
	~CGeoMap();
private:
	CGeoLayer* TopoLayer;//���ڴ�����ж����layer
	CRect rectMWC;//�߼���Χ
	unsigned int i;
	vector<CGeoLayer*> Layer;//���ݿ����
public:	
	//void Draw(CDC *pDC, vector<CGeoLayer*> layer);
	void Draw(CDC *pDC,CMyDCConfig& myDCcfg);
	void MapTopo();//��ͼ�ۺ�
	void TopoLayerAddObject();//���������˵����ж�����뵽layer��objects������
	void AddLayer(CGeoLayer* layer1);///////����ط���һ����ʦ
	CRect GetMapRect();
	CGeoLayer* getTopoLayer();//�õ�˽�б���layer�����ڴ���������˶���
	void setMapRect(CRect rect);
	CRect getMapRect();
	vector<CGeoLayer*> getLayer();//�õ�ԭʼͼ������
	CGeoLayer* getLayerAt(int index);
	int getLayerSize();
	int mapScale;
	void simplify(double telorance);
	
	// �������˺��������ʱ�ķ�Χ
	CRect TopoRect;
	bool isTopoed;
	// �õ����˽��������ݷ�Χ����ֵ��TopoRect
	void getTopoRect();
	// map��TopoLayer��ÿ��������
	void getTopoLayerObjectArea();
	// �õ��ڵ�ͼ��pt���������IDֵ
	int getSelectPolygonID(CPoint pt);
	// ��map������оֲ����ˣ���Layer���������һ��Ԫ�ؽ����жϣ�������һ��Ԫ��layer�л����棬�ͽ��оֲ����£�ͬʱɾ�����ݣ����û�����ݵĻ���������ʾ���޿��������
	void mapPart_Topo(void);
	// ��һ��layer���оֲ����ˣ����˶���ΪTopoLayer������Ӧ�ý���ÿ��object����С��Ӿ��Σ�����ҵ��ཻ�棬Ȼ����������н����ü����Լ����˸���
	void layerPart_Topo(CGeoLayer* layer,CGeoLayer*& topoTayer);
	// ����layer֮���໥������С��Ӿ��ε��ཻ�б����indexΪ0��������layer��ÿ�������������layer��ÿһ��������ཻ�ȶԣ����indexΪ1��������Ҫlayer1��ÿ�������������layer��ÿһ��������ཻ�ȶԣ���ζ��layer2���ڲ��ཻ�Ѿ��б���
	int GetTwoLayerIntersectMABR(CGeoLayer*& layer1, CGeoLayer*& layer2, int index);
	// ��layer2�е�����objects��objectstemp��ӵ�layer1��
	int Layer1AddLayer2(CGeoLayer*& layer1, CGeoLayer* layer2);
	// ��TopoLayer�����һ���棬���������˸���
	void TopoLayerAddObject(CGeoPolygon* polygon);
	// �ж��������Ƿ��ཻ
	int PosTwoPolygon(CGeoPolygon* polygon1, CGeoPolygon* polygon2);
	// �ж�����rect�Ƿ��ཻ
	bool isIntersectTwoRect(CRect rect1, CRect rect2);
	// �������ཻ�������������û�о������˵��棬��return�������һ���������˵��棬����һ���뾭�����˵�����вü�����������涼�������ˣ������¿���
	void twoPolygonIntersect(CGeoPolygon*& polygon1, CGeoPolygon*& polygon2);
	// zf,0623,filter
	void ObjectFilter(void);
	// zf,�����
	void ObjectFilterByArea(void);
	// Ѱ��һ�����Ӿ���
	void FindRectangle(void);
	// zf,0708,��ȡ��С��Ӿ���
	void FindMABR(void);
	// �������Ż� ��zf 0712
	void optimize(void);
	// ȥ������㣬zf��0717
	void RemoveRedundant(void);
	// ȥ��β�� 1003 zf
	void DeleteTail(void);
};

