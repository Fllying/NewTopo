#include "stdafx.h"
#include "GeoMap.h"
#include "GeoLayer.h"
#include"GeoPolygon.h"
using std::vector;

CGeoMap::CGeoMap()
{
	TopoLayer = NULL;
	rectMWC = CRect(0, 0, 0,0);
	TopoRect = CRect(0, 0, 0, 0);
	isTopoed = false;
	mapScale = 0;
}

CGeoMap::~CGeoMap()
{
	for (vector<CGeoLayer*>::iterator itc = Layer.begin();
		itc != Layer.end(); ++itc) {
		CGeoLayer* layer = *itc;
		if (layer) {
			delete layer;
			layer = NULL;
		}
	}
	Layer.clear();
	Layer.swap(vector<CGeoLayer*>());

	delete TopoLayer;
	TopoLayer = NULL;
}

void CGeoMap::Draw(CDC * pDC, CMyDCConfig& myDCcfg)
{
	//bool isAdd_Layer = myDCcfg.isAdd_Layer;
	bool a = myDCcfg.isShowTopo;
	bool c = myDCcfg.isShowExport;
	if (c) {
		myDCcfg.style = 5;
		TopoLayer->Draw(pDC, myDCcfg);
	}
	else {
		if (a) {
			bool b = myDCcfg.isShowOriMap;
			if (b) {
				myDCcfg.style = 4;
				TopoLayer->Draw(pDC, myDCcfg);
			}
			else {
				myDCcfg.style = 2;
				TopoLayer->Draw(pDC, myDCcfg);
			}

		}
		else {
			myDCcfg.style = 3;
			Layer[0]->Draw(pDC, myDCcfg);
		}
	}
	int size = Layer.size();
	if (size>1)
	{
		myDCcfg.style = 3;
		for (int i = 1; i < size; i++) {
			Layer[i]->Draw(pDC, myDCcfg);
		}
	}
	
	
	if (myDCcfg.gaoliangID.size()!=0)
	{
		int selectNum = myDCcfg.gaoliangID.size();
		for (int i = 0;i<selectNum;i++)
		{
			int selectID = myDCcfg.gaoliangID[i];
			myDCcfg.lineColor = RGB(32,190,190);
			if (!isTopoed) {
				myDCcfg.style = 3;
				CGeoPolygon* polygon = (CGeoPolygon*)Layer[0]->objects[selectID];
				polygon->Draw(pDC, myDCcfg);
			}
			else {
				myDCcfg.style = 2;
				myDCcfg.fillstyle = 1;
				CGeoPolygon* polygon = (CGeoPolygon*)TopoLayer->objects[selectID];
				polygon->Draw(pDC, myDCcfg);
				myDCcfg.fillstyle = 0;
			}
		}	
		
	}
	
	
	CString attribute = myDCcfg.Attribute;
	if (attribute != "0000") {
		CGeoLayer* layer;
		if (!isTopoed) {
			layer = Layer[0];
		}
		else {
			layer = TopoLayer;
		}
		myDCcfg.lineColor = myDCcfg.reserveColor;
		int size = layer->objects.size();
		for (int i = 0; i < size; i++) {
			CGeoPolygon* polygon = (CGeoPolygon*)layer->objects[i];
			if (!polygon->isNULL&&polygon->objectAttribute == attribute) {
				polygon->Draw(pDC, myDCcfg);
			}
		}
		myDCcfg.Attribute = "0000";
	}
}

void CGeoMap::AddLayer(CGeoLayer * layer1)
{
	Layer.push_back(layer1);
}

CRect CGeoMap::GetMapRect()
{
	CRect rect;
	int size = Layer.size();
	double left, right,top, bottom;
	for (int i = 0; i < size; i++) {
		if (i == 0) {
			left = Layer[i]->getRect().left;
			right = Layer[i]->getRect().right;
			top = Layer[i]->getRect().top;
			bottom = Layer[i]->getRect().bottom;
		}
		else {
			if (left > Layer[i]->getRect().left) {
				left =Layer[i]->getRect().left;
			}
			if (right < Layer[i]->getRect().right) {
				right = Layer[i]->getRect().right;
			}
			if (top > Layer[i]->getRect().top) {
				top = Layer[i]->getRect().top;
			}
			if (bottom < Layer[i]->getRect().bottom) {
				bottom = Layer[i]->getRect().bottom;
			}
		}
	}
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;
	this->rectMWC = rect;
	return this->rectMWC;
}

CGeoLayer * CGeoMap::getTopoLayer()
{
	return TopoLayer;
}

void CGeoMap::setMapRect(CRect rect)
{
	this->rectMWC = rect;
}

CRect CGeoMap::getMapRect()
{
	return this->rectMWC;
}

vector<CGeoLayer*> CGeoMap::getLayer()
{
	return this->Layer;
}

CGeoLayer * CGeoMap::getLayerAt(int index)
{
	return Layer[index];
}

int CGeoMap::getLayerSize()
{
	return Layer.size();
}

void CGeoMap::simplify(double telorance)
{
	TopoLayer->simplify(telorance);
}

void CGeoMap::MapTopo()
{
	this->TopoLayerAddObject();//ͬʱ������object��ӵ�layer�е�objects�����objectsTemp������	
	TopoLayer->TopoBuild();
}

void CGeoMap::TopoLayerAddObject()
{
	if (TopoLayer==NULL)
	{
		TopoLayer = new CGeoLayer();
	}
	
	for (int i = 0; i < this->getLayerSize(); i++) {
		for (unsigned int j = 0; j < this->getLayerAt(i)->GetObjectNum(); j++) {
			CGeoPolygon* obj =(CGeoPolygon*) this->getLayerAt(i)->objects[j];
			TopoLayer->AddObject(obj, &(TopoLayer->objects));
			CGeoPolygon* obj1 = new CGeoPolygon(*obj);
			TopoLayer->AddObject(obj1, &(TopoLayer->objectsTemp));
		}
	}
}



// �õ����˽��������ݷ�Χ����ֵ��TopoRect
void CGeoMap::getTopoRect()
{
	TopoRect = TopoLayer->updateTopoRect();
}


// map��TopoLayer��ÿ��������
void CGeoMap::getTopoLayerObjectArea()
{
	TopoLayer->getObjectArea();
}


// �õ��ڵ�ͼ��pt���������IDֵ
int CGeoMap::getSelectPolygonID(CPoint pt)
{
	int selectID = -1;
	if(isTopoed){
		selectID = TopoLayer->getSelectPolygonID(pt);
	}else{
		int length = Layer.size();
		for(int i = 0;i<length;i++){
			selectID = Layer[i]->getSelectPolygonID(pt);
			if(selectID!=-1)
				break;
		}
	}
	return selectID;
}


// ��map������оֲ����ˣ���Layer���������һ��Ԫ�ؽ����жϣ�������һ��Ԫ��layer�л����棬�ͽ��оֲ����£�ͬʱɾ�����ݣ����û�����ݵĻ���������ʾ���޿��������
void CGeoMap::mapPart_Topo(void)
{
	int size = Layer.size();
	if(size<=1){
		AfxMessageBox("û�д������");
		return;
	}
	else{
		CGeoLayer* layer = Layer[size-1];
		int objectNum = layer->objects.size();
		if(objectNum==0){
			AfxMessageBox("������ӵ�������ɾֲ�����");
		}else{
			this->layerPart_Topo(layer,TopoLayer);
		}
	}
}


// ��һ��layer���оֲ����ˣ����˶���ΪTopoLayer������Ӧ�ý���ÿ��object����С��Ӿ��Σ�����ҵ��ཻ�棬Ȼ����������н����ü����Լ����˸���,һ����һ�������
void CGeoMap::layerPart_Topo(CGeoLayer* layer,CGeoLayer*& topoLayer)
{
	AfxMessageBox("��ѽ��Ҫ��ʼ�ֲ�������");
	/*layer->BuildMABR();
	int sizeTopo = topoLayer->objects.size();
	GetTwoLayerIntersectMABR(topoLayer,layer,1);
	int sizeNew = topoLayer->objects.size();*/
	int size = layer->objects.size();
	int sizeTopo = topoLayer->objects.size();
	for(int i = 0;i<size;i++){
		CGeoPolygon* polygon = (CGeoPolygon*)layer->objects[i];
		polygon->GetObjectMABR();
		for(int j = 0;j<sizeTopo;j++){
			TopoLayerAddObject(polygon);
		}
	}
}

// ����layer֮���໥������С��Ӿ��ε��ཻ�б����indexΪ0����layer2��������ӵ�layer1�У�����ԭlayer1��layer2�е�������ཻ����,����ʵ��layer2�е����ڲ����ཻ����
int CGeoMap::GetTwoLayerIntersectMABR(CGeoLayer*& layer1, CGeoLayer*& layer2, int index)
{
	int size1 = layer1->objects.size();
	int size2 = layer2->objects.size();
	Layer1AddLayer2(layer1,layer2);
	int size = layer1->objectsTemp.size();
	if (size>0)
	{
		for (i = 0; i < size1; i++) {
			for (int j = size1; j < size; j++) {
				BOOL a = layer1->objectsTemp[i]->isObjectMABRintersect(layer1->objectsTemp[j]);
				if (a) {
					layer1->objectsTemp[i]->intersectMABRpos.push_back(j);
					layer1->objectsTemp[j]->intersectMABRpos.push_back(i);
				}
			}
		}
	}
	for (i = size1; i < size-1; i++) {
		for (int j = i+1; j < size; j++) {
			BOOL a = layer1->objectsTemp[i]->isObjectMABRintersect(layer1->objectsTemp[j]);
			if (a) {
				layer1->objectsTemp[i]->intersectMABRpos.push_back(j);
				layer1->objectsTemp[j]->intersectMABRpos.push_back(i);
			}
		}
	}
	return 0;
}


// ��layer2�е�����objects��objectstemp��ӵ�layer1��
int CGeoMap::Layer1AddLayer2(CGeoLayer*& layer1, CGeoLayer* layer2)
{
	int size2 = layer2->objects.size();
	for(int i = 0;i<size2;i++){
		CGeoPolygon* obj =(CGeoPolygon*) layer2->objects[i];
		layer1->AddObject(obj, &(layer1->objects));
		CGeoPolygon* obj1 = new CGeoPolygon(*obj);
		layer1->AddObject(obj1, &(layer1->objectsTemp));
	}
	return 0;
}


// ��TopoLayer�����һ���棬���������˸���
void CGeoMap::TopoLayerAddObject(CGeoPolygon* polygon)
{
	int sizeTopoObjects = TopoLayer->objects.size();
	polygon->GetObjectMABR();
	for(int i = 0;i<sizeTopoObjects;i++){
		CGeoPolygon* TopoPolygon =(CGeoPolygon*)TopoLayer->objects[i];
		if(PosTwoPolygon(TopoPolygon,polygon)==2){
			twoPolygonIntersect(TopoPolygon,polygon);
		}
	}
}


// �ж��������Ƿ��ཻ,����0���ⲿ����1���ཻ2���ڲ�����3���ں�4
int CGeoMap::PosTwoPolygon(CGeoPolygon* polygon1, CGeoPolygon* polygon2)
{
	int flag = 0;
	bool isIntersectTwoRect = polygon1->isObjectMABRintersect(polygon2);//�ж����������С��Ӿ����Ƿ��ཻ
	if(isIntersectTwoRect){

		return 2;
		//һ����5�֣����룬�ⲿ���ڣ��ཻ���ڲ����ڣ��ں�
	}else{
		return 0;
	}
	return flag;
}

// �������ཻ�������������û�о������˵��棬��return�������һ���������˵��棬����һ���뾭�����˵�����вü�����������涼�������ˣ������¿��ǣ�������
void CGeoMap::twoPolygonIntersect(CGeoPolygon*& polygon1, CGeoPolygon*& polygon2)
{
}


// zf,0623,filter
void CGeoMap::ObjectFilter(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->ObjectFilter();
	}
}


// zf,�����
void CGeoMap::ObjectFilterByArea(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->ObjectFilterByArea();
	}
}


// Ѱ��һ�����Ӿ��Σ�zf,0704
void CGeoMap::FindRectangle(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->FindRectangle();
	}
}


// zf,0708,��ȡ��С��Ӿ���
void CGeoMap::FindMABR(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->FindMABR();
	}
}


// �������Ż� ��zf 0712
void CGeoMap::optimize(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->optimize();
	}
}


// ȥ������㣬zf��0717
void CGeoMap::RemoveRedundant(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->RemoveRedundant();
	}
}


// ȥ��β�� 1003 zf
void CGeoMap::DeleteTail(void)
{
	for(int i = 0;i<Layer.size();i++)
	{
		Layer[i]->DeleteTail();
	}
}
