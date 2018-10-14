#include "stdafx.h"
#include "GeoPolygon.h"
#include"GeoLayer.h"
#include "MyPoint.h"

#include <algorithm>
#define _USE_MATH_DEFINES   //����math.h�еĶ���  
#include <math.h>

using namespace std;


CGeoPolygon::CGeoPolygon()
{
	times = 4;  //�������
	areaT = 100000;  //�����ֵ
	isNULL = false;
	TopoArea = 0;
	isTopoed = false;
	circleNum = 0;
	shapeArea = 0;
	hasFindMABR = false;
	hasFindRectangle = false;
	hasGetBuildings = false;
}

CGeoPolygon::CGeoPolygon(CGeoPolygon & polygon)
{
	int size = polygon.circles.size();
	for (int i = 0; i < size; i++) {
		CGeoPolyline* polyline = new CGeoPolyline(*(polygon.circles[i]));
		circles.push_back(polyline);		
	}
}

CGeoPolygon::~CGeoPolygon()
{
	
	for (vector<CGeoPolyline*>::iterator itb = circles.begin();
		itb != circles.end(); ++itb) {
		CGeoPolyline* circle = *itb;
		if (circle) {
			delete circle;
			circle = NULL;
		}
	}
	circles.clear();

	for (vector<CGeoPolyline*>::iterator itc = commonEdge.begin();
		itc != commonEdge.end(); ++itc) {
		CGeoPolyline* commonedge = *itc;
		if (commonedge) {
			delete commonedge;
			commonedge = NULL;
		}
	}
	commonEdge.clear();

	int size = commoncircles.size();
	for (int i = 0; i < size; i++) {
		vector<CGeoPolyline*> circle = commoncircles[i];
		for (vector<CGeoPolyline*>::iterator itd = circle.begin();
			itd != circle.end(); ++itd) {
			CGeoPolyline* commonedge = *itd;
			if (commonedge) {
				delete commonedge;
				commonedge = NULL;
			}
		}
		circle.clear();
	}
	vector<vector<CGeoPolyline*>>().swap(commoncircles);

	vector<double>().swap(area);
	vector<double>().swap(circleLength);
	vector<int>().swap(circlePointNum);
	
}


void CGeoPolygon::Draw(CDC * pDC)
{
	int sizeofcircle = circles.size();
	if (sizeofcircle == 0) {
		int sizeofCommonEdge = commonEdge.size();
		for (int i = 0; i < sizeofCommonEdge; i++) {
			commonEdge[i]->Draw(pDC);
		}
	}
	CPoint pt;
	for (int i = 0; i < sizeofcircle; i++)//���ӻ�����
	{
		CGeoPolyline* polyline=this->circles[i];
		int sizeofline = polyline->GetSize();
		
		for (int j = 0; j < sizeofline; j++) //�ӻ�����
		{
			circles[i]->Draw(pDC);
			//CMyPoint* point =polyline->GetPointAt(j);
			//pt =point->GetCPoint();
			//if (j == 0) {
			//	pDC->MoveTo(pt);
			//	//pDC->SetPixel(pt, RGB(255, 0, 0));
			//}
			//else
			//{
			//	pDC->LineTo(pt);
			//	pDC->MoveTo(pt);
			//	pDC->SetPixel(pt, RGB(255, 0, 0));
			//}
		}
		//delete polyline;
		//polyline = NULL;
	}
}

void CGeoPolygon::Draw(CDC * pDC, CMyDCConfig myDCcfg)
{
	
	if (myDCcfg.style == 2) {
		int num = commonEdge.size();
		if (myDCcfg.fillstyle==0)
		{
			for (int i = 0; i < num; i++) {
				if (!commonEdge[i]->isNULL) {
					commonEdge[i]->Draw(pDC, myDCcfg);
				}		    
			}
		}else if (myDCcfg.fillstyle==1)
		{
			//fillColor
			CBrush brush(RGB(247,238,214));
			CBrush *pOldBrush = pDC->SelectObject(&brush);  //��ˢ
			pDC->BeginPath();
			CArray<CPoint,CPoint> pts;  //���������
			commonEdge[0]->MyPointsToCPoints(circlePoints[0], pts);  //���⻷�ĵ�浽pts��
			CPoint *pta = pts.GetData();   
			pDC->Polygon(pta,pts.GetSize());   //��������
			pDC->EndPath();
			pDC->FillPath();
			pDC->SelectObject(pOldBrush);
			for (int i = 0; i < num; i++) {  //����������
				if (!commonEdge[i]->isNULL) {
					commonEdge[i]->Draw(pDC, myDCcfg);
				}		    
			}
		}

	}
	else if (myDCcfg.style == 3) {
		for (int i = 0; i < circles.size(); i++) {
			//if(i ==0) myDCcfg.lineColor = RGB(255, 0,  0);
			circles[i]->Draw(pDC, myDCcfg);
			//if(circles[i]->areaProportion == 0)    //δ����Ӿ���֮ǰ
			//{
			//	circles[i]->Draw(pDC, myDCcfg);
			//}
			//if(circles[i]->areaProportion >= 0)   //�ҵ���Ӿ��Σ�����������֮��
			//{
			//	circles[i]->Draw(pDC, myDCcfg);
			//	/*CString str;
			//	str.Format("%lf",circles[i]->areaProportion);
			//	pDC->TextOutA(circles[i]->center.x,circles[i]->center.y,str);*/
			//}
		}
		//zf,0705,������Ӿ���
		if(rectangleArray.size()!= 0&&hasFindRectangle == true)
		{
			int xunhuan = rectangleArray.size()/4;
			int count = 0;
			for(int n = 0;n<xunhuan;n++)  //һ������ο����ж������0706
			{
				pDC->MoveTo(*rectangleArray[0+n*4]);
				pDC->LineTo(*rectangleArray[1+n*4]);
				pDC->LineTo(*rectangleArray[2+n*4]);
				pDC->LineTo(*rectangleArray[3+n*4]);
				pDC->LineTo(*rectangleArray[0+n*4]);
			}
		}
		//zf,0706,����С��Ӿ���
		if(rectangleRotate.size()!= 0&&hasFindMABR==true)  //�Ż�֮ǰ
		{
			int xunhuan = rectangleRotate.size()/4;
			int count = 0;
			for(int m = 0;m<xunhuan;m++)  //һ������ο����ж������0706
			{
				pDC->MoveTo(*rectangleRotate[0+m*4]);
				pDC->LineTo(*rectangleRotate[1+m*4]);
				pDC->LineTo(*rectangleRotate[2+m*4]);
				pDC->LineTo(*rectangleRotate[3+m*4]);
				pDC->LineTo(*rectangleRotate[0+m*4]);
				/*CString str1;
				str1.Format("%d",count);
				pDC->TextOutA(rectangleRotate[0+m*4]->x+100,rectangleRotate[0+m*4]->y+100,str1);
				count++;
				CString str2;
				str2.Format("%d",count);
				pDC->TextOutA(rectangleRotate[1+m*4]->x+100,rectangleRotate[1+m*4]->y+100,str2);
				count++;
				CString str3;
				str3.Format("%d",count);
				pDC->TextOutA(rectangleRotate[2+m*4]->x+100,rectangleRotate[2+m*4]->y+100,str3);
				count++;
				CString str4;
				str4.Format("%d",count);
				pDC->TextOutA(rectangleRotate[3+m*4]->x+100,rectangleRotate[3+m*4]->y+100,str4);
				count++;*/
			}
		}
		//zf,0720,����Ͻ�����
		if(buildings.size()!= 0&&hasGetBuildings == true)
		{
			int count = 0;
			pDC->MoveTo(*buildings[0]);
			for(int m = 1;m<buildings.size();m++){
				pDC->LineTo(*buildings[m]);
				/*CString str;
				str.Format("%d",count);
				pDC->TextOutA(buildings[m]->x+100,buildings[m]->y+100,str);
				count++;*/

			}
		}
	}
	else if (myDCcfg.style == 5) {
		if (this->circlePoints.size() == 0) {
			AfxMessageBox("Ӧ����û�н������ˣ�����Ϊ��");
		}
		else {
			CPen pen(PS_SOLID, 1, myDCcfg.lineColor);
			CPen* pOldPen = pDC->SelectObject(&pen);
			int size = this->circlePoints.size();
			for (int i = 0; i < size; i++) {
				vector<CMyPoint*> pts = circlePoints[i];
				int sizeofPts = pts.size();
				for (int i = 0; i < sizeofPts; i++) {
					if (i == 0)
						pDC->MoveTo(pts[i]->GetCPoint());
					else
					{
						pDC->LineTo(pts[i]->GetCPoint());
						pDC->MoveTo(pts[i]->GetCPoint());
						//pDC->SetPixel(pt,myDCcfg.lineColor);
					}
				}
			}
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();
		}
	}
}

int CGeoPolygon::GetCircleNum()
{
	return circles.size();
}


//CGeoPolyline* CGeoPolygon::GetCircleAt(int index)
//{
//	CGeoPolyline* circle = circles[index];
//	return circle;
//}

void CGeoPolygon::AddCircle(CGeoPolyline* line)
{
	circles.push_back(line);
}

CRect* CGeoPolygon::GetObjectMABR()
{
	CRect* rect;
	rect = new CRect();
	long left, right, top, bottom;
	for (int i = 0; i < circles.size(); i++) {
		int size = circles[i]->pts.size();
		for (int j = 0; j < circles[i]->pts.size(); j++) {
			CMyPoint* point = circles[i]->pts[j];
			if (i == 0&&j==0) {
				left = point->Getx();
				right = point->Getx();
				top = point->Gety();
				bottom = point->Gety();
			}
			else {
				if (left > point->Getx()) {
					left = point->Getx();
				}
				if (right < point->Getx()) {
					right = point->Getx();
				}
				if (top < point->Gety()) {
					top = point->Gety();
				}
				if (bottom > point->Gety()) {
					bottom = point->Gety();
				}
			}
		}

	}
	rect->left = left;
	rect->bottom = bottom;
	rect->right = right;
	rect->top = top;
	
	this->objectMABR = rect;
	return objectMABR;
}

void CGeoPolygon::deleteLine(int lineID)
{
}

void CGeoPolygon::rebuildCircle()//circleID��δ��ֵ
{
	int size = commonEdge.size();
	for (int i = 0; i < size; i++) {
		commonEdge[i]->isInCommonCircle = 0;
	}
	if (commoncircles.size() != 0) {
		commoncircles.resize(0);
	}
	if (size == 1) {
		vector<CGeoPolyline*> line;
		CMyPoint* oriPt = commonEdge[0]->pts[0];
		CMyPoint* endPt = commonEdge[0]->pts[commonEdge[0]->pts.size()-1];
		if (oriPt->GetCPoint() == endPt->GetCPoint()) {
			line.push_back(commonEdge[0]);
			commonEdge[0]->isInCommonCircle = 1;
			commoncircles.push_back(line);
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			if (commonEdge[i]->isInCommonCircle == 0) {
				vector<CGeoPolyline*> pline = rebuildCircle(commonEdge, i);
				commoncircles.push_back(pline);
			}		
		}
	}
	size = commoncircles.size();
	area.resize(size);
	circleLength.resize(size);
	circlePoints.resize(size);
	circlePointNum.resize(size);
	this->circleNum = size;
	this->getCirclePoint();
	this->getArea();
	this->getTopoPointNum();
	this->getCircleLength();
}

vector<CGeoPolyline*> CGeoPolygon::rebuildCircle(vector<CGeoPolyline*> lines, int i)//Ҫע���޸�line��circle��Ϣ  
{
	vector<CGeoPolyline*> circle;
	int temp = i;

	do {
		circle.push_back(lines[temp]);
		CPoint pt = findNextLine(lines, temp);
		temp = pt.x;
		int joinStyle = pt.y;
		lines[temp]->isInCommonCircle = joinStyle;		
	} while (temp != i);
	return circle;
}

// ������lines��Ѱ����i��������һ���������������±�,�Լ������������ӷ�ʽ
CPoint CGeoPolygon::findNextLine(vector<CGeoPolyline*> lines, int i)
{
	CPoint pt(-1, -1);
	int size = lines.size();
	CGeoPolyline* pline = lines[i];
	CMyPoint* joinPoint;
	int count = 0;
	int temp = -1;
	int flag = pline->isInCommonCircle;
	if (flag == 0) {
		if (lines[i]->rightAttribute == this->objectAttribute) {
			flag = 1;
			
		}
		else {
			flag = 2;
		}
		pline->isInCommonCircle = flag;
	}
	
	if ((*((lines[i]->pts).end() - 1))->GetCPoint() == lines[i]->pts[0]->GetCPoint()) {
		pt.SetPoint(i, 1);
		return pt;
	}
	if (flag == 1) {
		joinPoint = *((lines[i]->pts).end() - 1);
	}
	else {
		joinPoint = lines[i]->pts[0];
	}
	CPoint joinCPoint = joinPoint->GetCPoint();
	for (int j = 0; j < size; j++) {
		CGeoPolyline* pline1 = lines[j];
		if (!pline1->isNULL) {
			CMyPoint* endPoint = *((pline1->pts).end() - 1);
			CMyPoint* oriPoint = pline1->pts[0];
			if (endPoint->GetCPoint() == joinCPoint) {
				count++;
				if (j != i) {
					temp = j;
					pt.y = 2;
					pt.x = temp;
				}
			}
			if (oriPoint->GetCPoint() == joinCPoint) {
				count++;
				if (j != i) {
					temp = j;
					pt.y = 1;
					pt.x = temp;
				}
			}
		}
		
	}
	if (this->PosinCommonLayer == 250) {
		int sssss = 1;
	}
	int s = count;
	
	if (s != 2) {
		AfxMessageBox("polygon�������ɻ�����:δ�ҵ�������һ��");
		if (this->PosinCommonLayer != 227) {
			int ssss = 1;
		}
		
	}
	if (s == 2 && pt.y == -1) {
		pt.y = 1;
	}
	if (s == 2 && temp == -1) {
		pt.x = i;
	}
	return pt;
}


void CGeoPolygon::mergeCommonEdge(CGeoPolyline * line1,CGeoPolyline* line2,int style)//�ϲ���������line1��line2������Ҫ�޸�line��circleID
	//line1�����=line2����㷵��1��line1���յ�=line2���յ㷵��2��line1���յ�=line2����㷵��3��line1�����=line2���յ㷵��4���޷���������0
{
	if ((line1->isNULL&&!line2->isNULL)||(line2->isNULL&&!line1->isNULL))
		return;
	if (line1->isNULL&&!line2->isNULL){
		AfxMessageBox("�����߾�Ϊ�գ��޷��ϲ�");
	}

	CMyPoint* line1OriPt = line1->pts[0];
	CMyPoint* line1EndPt = line1->pts[line1->pts.size()-1];
	CMyPoint* line2OriPt = line2->pts[0];
	CMyPoint* line2EndPt = line2->pts[line2->pts.size()-1];
	if (line1->PosinCommonLayer==line2->PosinCommonLayer)
	{
		AfxMessageBox("�������ص�");
	}
	if (style==1)
	{
		line1OriPt->isJointPt = false;
		for(int i = 1;i<line2->pts.size();i++){
			line1->pts.insert(line1->pts.begin(),line2->pts[i]);
		}
		line2->isNULL = true;
	} 
	else if (style==2)
	{
		line1EndPt->isJointPt = false;
		for(int i = line2->pts.size()-2;i>=0;i--){
			line1->pts.push_back(line2->pts[i]);
		}
		line2->isNULL = true;
	} else if (style==3)
	{
		line1EndPt->isJointPt = false;
		for(int i = 1;i<line2->pts.size();i++){
			line1->pts.push_back(line2->pts[i]);
		}
		line2->isNULL = true;
	} else if (style==4)
	{
		line2EndPt->isJointPt = false;
		for(int i = 1;i<line1->pts.size();i++){
			line2->pts.push_back(line1->pts[i]);
		}
		line1->isNULL = true;
	} else if(style==0){
		AfxMessageBox("�������޷��ϲ�");
		return;
	}
}

void CGeoPolygon::ReoganizeCommonCircle()
{
	
	this->eraseNullLine();//������һ�����ڲ�ֻ����������������������һ�µ����
	this->rebuildCircle();
	//this->adjustCommonCircle();
}

void CGeoPolygon::eraseNullLine()
{
	for (int i = 0; i < commonEdge.size();) {
		if (commonEdge[i]->isNULL)
			commonEdge.erase(commonEdge.begin() + i);
		else
			i++;
	}
}

void CGeoPolygon::adjustCommonCircle()
{
	int size = commoncircles.size();
	for (int i = 0; i < size; i++) {
		this->adjustCommonCircle(commoncircles[i]);
	}	
}

void CGeoPolygon::adjustCommonCircle(vector<CGeoPolyline*> lines)
{
	int size = lines.size();
	int temp = -1;
	for (int i = 0; i < size; i++) {
		CMyPoint* EndPoint = *((lines[i]->pts).end() - 1);
		CPoint ptEnd = EndPoint->GetCPoint();
		int count = 0;
		for (int j = 0; j < size; j++) {
			CMyPoint* OriPoint = lines[j]->pts[0];
			CPoint ptOri = OriPoint->GetCPoint();
			if (ptEnd == ptOri) {
				count++;
				temp = j;
			}
		}//���һ�����ĵ�β��һ�������������һ��
		for (int j = 0; j < size&&j!=i; j++) {
			CMyPoint* OriPoint = *((lines[j]->pts).end() - 1);
			CPoint ptOri1 = OriPoint->GetCPoint();
			if (ptEnd == ptOri1) {
				count++;
				temp = j;
			}
		}//���һ�����ĵ�β��һ�������������һ��
		if ((temp != 0 && i == size - 1)||count!=1) {
			CString str;
			str.Format("��û�бպ�");
			AfxMessageBox(str);
		}
		if (temp != i + 1) {
			if (i == size - 1 && temp == 0)
				break;
			else {
				swap(lines[temp], lines[i + 1]);
			}	
		}
	}
	
	for (int i = 0; i < lines.size(); i++) {
		int j = i + 1;
		if (j >= lines.size()) {
			j = 0;
		}
		if (lines[i]->leftAttribute == lines[j]->leftAttribute&&lines[i]->rightAttribute == lines[j]->rightAttribute) {
			int ptSize = lines[j]->pts.size();
			for (int m = 1; m < ptSize; m++) {
				lines[i]->pts.push_back(lines[j]->pts[m]);
			}
			lines.erase(lines.begin() + j);
		}
	}
}

//�õ����˺�����
void CGeoPolygon::getArea()
{
	int size = commoncircles.size();
	double areaTemp;
	
	for (int i = 0; i < size; i++) {
		areaTemp =getArea(i);//���circle�ǲ���Ҫ�ж�isNULL
		area[i]=areaTemp;
	}
	if (area.size() > 0) {
		double tempArea ;
		int ll = 0;
		for (int i = 0; i < size; i++) {
			if (i == 0) {
				tempArea = area[0];
			}
			else {
				if (tempArea < area[i]) {
					tempArea = area[i];
					ll = i;
				}
			}
		}
		if (ll != 0) {
			swap(area[0], area[ll]);
			swap(commoncircles[0], commoncircles[ll]);//�˴���Ҫ��֤�Ƿ񽻻��ɹ�
		}
	}
	
	double finalArea = area[0];
	for (int i = 1; i < area.size(); i++) {
		finalArea = finalArea - area[i];
	}
	if (finalArea <= 0) {
		AfxMessageBox("ĳ����������ȻС��0");
	}
	this->TopoArea = finalArea;
}

double CGeoPolygon::getArea(int index)
{
	double area = 0;
	vector<CMyPoint*> cpts = circlePoints[index];
	//ͬʱ���circlePoint���飬shp�ļ��У�ÿ��������һ�������һ������ͬ
	//circlePoints[index] = cpts;
	area = getArea(cpts);
	
	return area;//��ֻ��һ�������
}
//double CGeoPolygon::getArea(int index)
//{
//	vector<CGeoPolyline*> lines = commoncircles[index];
//	vector<CPoint> cpts = this->getCirclePoint(lines);
//	//ͬʱ���circlePoint���飬shp�ļ��У�ÿ��������һ�������һ������ͬ
//	circlePoints[index] = cpts;
//	vector<CPoint> a;
//	a = this->circlePointReorder(cpts);//����֪����һ�����ϵ������˵�ֱ��ڶ˵������е�λ�ã�����һ�������˷ֱ���19,26����ô����Ҫ֪��19��26֮��ĵ��������
//	//y����ֵ�������ڴ˴��޸���������һ�µ�����������Ǵ���
//
//	int sizeOfCpts = cpts.size();
//	vector<vector<Segment>> tixingLine;
//	tixingLine.resize(sizeOfCpts - 1);
//	for (int i = 0; i < sizeOfCpts; i++) {
//		this->tixingSplit(cpts, i, a, tixingLine);//������������������һ��
//	}
//	double s = 0;
//	double area = 0;
//	int sizeOfTixing = tixingLine.size();
//	for (int i = 0; i < sizeOfTixing; i++) {
//		s = getArea(tixingLine[i],i);//���������С��0�����
//		area = s + area;
//	}
//	CPoint pt1 = circlePoints[index][0];
//	circlePoints[index].push_back(pt1);
//	int ptNum = circlePoints[index].size();
//	circlePointNum[index] = ptNum;
//	return area;//��ֻ��һ�������
//}

// ɾ������circle�������
void CGeoPolygon::deleteCircle()
{
	int size = circles.size();
	for (int i = 0; i < size; i++) {
	circles[i]->pts.clear();
	circles[i]->pts.swap(vector<CMyPoint*>());
	}
	circles.clear();
	circles.swap(vector<CGeoPolyline*>());
}

// ��һ��commonCircles�ĵ�˳��ŵ�һ�������ͬʱ�������ݼ��
vector<CMyPoint*> CGeoPolygon::getCirclePoint(vector<CGeoPolyline*> lines)
{
	int size = lines.size();
	for (int i = 0; i < size; i++) {
		lines[i]->isInCommonCircle = 0;
	}
	if (size == 1) {
		CMyPoint* pt1Ori = lines[0]->pts[0];
		CMyPoint* pt1End = *(lines[0]->pts.end() - 1);
		lines[0]->isInCommonCircle = 1;
		if (pt1End->GetCPoint() != pt1Ori->GetCPoint()) {
			AfxMessageBox("commonCircles��δ�պ�");
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			if (i == 0) {
				if (lines[i]->rightAttribute == this->objectAttribute) {
					lines[i]->isInCommonCircle = 1;
				}
				else {
					lines[i]->isInCommonCircle = 2;
				}				
			}
				
			if (lines[i]->isNULL) {
				AfxMessageBox("commonCircles��ȥ������Ȼ���п���");
			}
			int j = i + 1;
			if (j == size)
				j = 0;
			CMyPoint* pt1;
			if (lines[i]->isInCommonCircle == 1) {
				pt1 = *(lines[i]->pts.end() - 1);
			}
			else {
				pt1 = lines[i]->pts[0];
			}

			CMyPoint* pt2Ori = lines[j]->pts[0];
			CMyPoint* pt2End = *(lines[j]->pts.end() - 1);
			if (pt1->GetCPoint() == pt2Ori->GetCPoint()) {
				lines[j]->isInCommonCircle = 1;
			}
			else if (pt1->GetCPoint() == pt2End->GetCPoint()) {
				lines[j]->isInCommonCircle = 2;
			}
			else {
				AfxMessageBox("commonCircles��ȥ������Ȼ������һ������β�㲻������һ�������׵�");//�����˵�һ�������׵�����һ��������
			}
			if (i == size - 1) {
				if (lines[0]->isInCommonCircle == 1) {
					if (pt1->GetCPoint() != lines[0]->pts[0]->GetCPoint()) {
						AfxMessageBox("commonCircles��δ�պ�");
					}
				}
				else {
					if (pt1->GetCPoint() != (*(lines[0]->pts.end()-1))->GetCPoint()) {
						AfxMessageBox("commonCircles��δ�պ�");
					}
				}
				
			}
		}
	}

	//��ʼ
	vector<CMyPoint*> cpts;
	for (int i = 0; i < size; i++) {
		CGeoPolyline* line = lines[i];
		int sizePt = line->pts.size();
		if (line->isInCommonCircle == 1) {//line->rightAttribute == this->objectAttribute
			for (int j = 0; j < sizePt - 1; j++) {
				CMyPoint* myPoint = line->pts[j];
				if (!myPoint->isNULL) {
					
					cpts.push_back(myPoint);
				}
			}
		}
		else if (line->isInCommonCircle == 2) {
			for (int j = sizePt - 1; j >0; j--) {
				CMyPoint* myPoint = line->pts[j];
				if (!myPoint->isNULL) {
					
					cpts.push_back(myPoint);
				}
			}
		}
		else {
			AfxMessageBox("lines���б��Ϊ0�Ĵ���");
		}

	}//pts�еĵ�Ӧ���ǲ��ظ��ĵ�
	cpts.push_back(cpts[0]);
	//��������Ƿ����ظ���
	//for (int i = 1; i < cpts.size() - 1; i++) {
	//	if(cpts[i]== cpts[i+1])
	//		//AfxMessageBox("���������е��ظ�");
	//}
	return cpts;
}

// �Ի��ڵ�����������򣬷��ظõ��������������������λ��
vector<CPoint> CGeoPolygon::circlePointReorder(vector<CPoint>& pts)
{
	int size = pts.size();
	vector<squarePoint> aPoint;
	aPoint.resize(size);
	for (int i = 0; i < size; i++) {
		aPoint[i].pt = pts[i];
		aPoint[i].oldNum = i;
		aPoint[i].newNum = 0;
	}
	sort(aPoint.begin(), aPoint.end());
	//vector<int> repeatNum;
	int sizeAPoint = aPoint.size();
	
	//���������û��ˮƽ�߶�
	for (int i = 0; i < sizeAPoint-1; i++) {
		int j = i + 1;
		if (aPoint[i].pt.y == aPoint[j].pt.y) {
			int m = this->findSamePoint(aPoint, i);
			this->modifySquarePoint(aPoint, i, m);
		}	
	}
	int ddd = 1;
	/*if (repeatNum.size() > 0) {
		for (int i = 0; i < repeatNum.size(); i++) {
			int d = repeatNum[i];
			pts[d].y= pts[d].y-1;
			aPoint[d].pt.y = aPoint[d].pt.y - 1;
		}	
	}*/
	for (int i = 0; i < size; i++) {
		aPoint[i].newNum = i;
	}
	for (int i = 0; i < size; i++) {
		pts[i] = aPoint[i].pt;
	}
	vector<CPoint> a;
	a.resize(size);
	int temp = -1;
	for (int i = 0; i < size; i++) {
		temp = aPoint[i].oldNum;
		a[temp].x = aPoint[i].newNum;
		a[i].y=aPoint[i].pt.y;
	}
	return a;
}
//cpts�ǻ�˳������飬i��ִ���ʷֵĵ�i���㣬����һ��������߶Σ�a��x��ʾ��i�����������Сλ�ã���������һ��������
//�ֱ�Ϊ16,29����ô����a�ĵ�17��28��yֱ�ӽ��зָ�õ���ÿһ���߼ӵ�����line��������
void CGeoPolygon::tixingSplit(vector<CPoint> cpts, int i, vector<CPoint> a, vector<vector<SegMent>>& tixingLine)
{
	
	int size = cpts.size();
	int j = i + 1;
	if (j > size - 1)
		j = 0;
	CPoint pt1 = cpts[i];
	CPoint pt2 = cpts[j];
	int pos1 = a[i].x;//��i�������������ڵ�λ�ã�����16
	int pos2 = a[j].x;//��j�������������ڵ�λ�ã�����20
	int temp;
	if (pos1 > pos2) {
		temp = pos1;
		pos1 = pos2;
		pos2 = temp;
	}
	vector<double> high;//�ŵ���17��19��������ֵ
	for (int m = pos1; m <= pos2; m++) {
		double  h = a[m].y;
		high.push_back(h);
	}
	int highSize = high.size();
	SegMent segment(cpts[i], cpts[j]);
	//segment.setSegment(cpts[i], cpts[j]);
	vector<double> horizonte;//������
	for (int m = 0; m < highSize; m++) {
	
		double x = this->getSegementLeft(segment, high[m],i);
		horizonte.push_back(x);
	}
	for (int m = 0; m < highSize-1; m++) {
	
		CPoint pt1(horizonte[m], high[m]);
		CPoint pt2(horizonte[m+1], high[m+1]);
		SegMent asegment(pt1, pt2);
		//asegment.setSegment(pt1, pt2);
		tixingLine[pos1].push_back(asegment);
		
		pos1++;

	}
	int llll = 1;
}

// �߶�line�ϣ���������Ϊy�ĵ��Ӧ�ĺ�����
double CGeoPolygon::getSegementLeft(SegMent line, double y,int i)
{
	double x = 0;
	if (line.ptEnd.y == line.ptOri.y) {
		int ssss = 1;//������һ��
	}
	
	double lgaocha = line.ptEnd.y - line.ptOri.y;
	double sgaocha = y - line.ptOri.y;
	double part = sgaocha / lgaocha;
	double xcha = line.ptEnd.x - line.ptOri.x;
	x = xcha*part + line.ptOri.x;
	return x;
}

// ����һ�������ڵ�����б����ɵ����
double CGeoPolygon::getArea(vector<SegMent> segments,int m)
{
	int size = segments.size();
	
	double s = 0;
	double area = 0;
	sort(segments.begin(), segments.end());
	for (int i = 0; i < size;i=i+2) {
		CPoint ptOri1 = segments[i].ptOri;
		CPoint ptOri2 = segments[i + 1].ptOri;
		CPoint ptEnd1 = segments[i].ptEnd;
		CPoint ptEnd2 = segments[i + 1].ptEnd;
		double xTop = fabs((double)(ptOri1.x - ptOri2.x));
		double xBottom = fabs((double)(ptEnd1.x - ptEnd2.x));
		double h = fabs((double)(ptOri1.y - ptEnd1.y));
		s = (xTop + xBottom)*h / 2;
		if (s < 0) {
			int llll = 1;
		}
		area = area + s;
	}
	return area;
}


// ������m��������һ�µĵ�����һ������±�
int CGeoPolygon::findSamePoint(vector<squarePoint> aPoint, int m)
{
	double y = aPoint[m].pt.y;
	int size = aPoint.size();
	while (aPoint[m].pt.y == y) {
		m++;
		if (m == size)
			break;
	}
	m = m - 1;
	return m;
}


// �������д�i��j������������޸ģ�����i-1�����j������֮�Ȼ��ƽ�ֵ�ÿ���㣬Ҫ����ƽ��
void CGeoPolygon::modifySquarePoint(vector<squarePoint>& aPoint, int i, int j)
{
	int size = aPoint.size();
	int m = i - 1;
	double y;
	if (i ==0) {
		y = 10;
	}
	else {
		y = aPoint[i].pt.y - aPoint[m].pt.y;
	}
	int num;
	if (i < j)
		num = j - i + 1;
	else
		num = size - i + j + 1;
	double increase = y / num;
	for (int x = i; x != j; x++) {
		if (x == size) {
			x = 0;
		}
		aPoint[x].pt.y = aPoint[x].pt.y - increase*(num - 1);
		num--;
	}
}


// �õ�һ������Ϊpt1,pt2,pt3��������������
//double CGeoPolygon::getTriangleArea(CPoint pt1, CPoint pt2, CPoint pt3)
//{
//	double area = 0;
//	int l1 = getTwoPointDistance(pt1, pt2);
//	int l2 = getTwoPointDistance(pt1, pt3);
//	int l3 = getTwoPointDistance(pt3, pt2);
//	if (l1 == 0 || l2 == 0 || l3 == 0)
//		return 0;
//	int s = (l1 + l2 + l3) / 2;
//	area = sqrt((long double)(s*(s - l1)*(s - l2)*(s - l3)));
//	return area;
//}


// �õ�����֮��ľ���
//double CGeoPolygon::getTwoPointDistance(CPoint pt1, CPoint pt2)
//{
//	int distance = 0;
//	int x = pt1.x - pt2.x;
//	int y = pt1.y - pt2.y;
//	distance = sqrt((long double)(x*x + y*y));
//	return distance;
//}



// �õ����˽�����Ķ���Χ
void CGeoPolygon::getObjectTopoRect()
{
	CRect newRcet;
	double nLeft, nRight, nTop, nBottom;
	CGeoPolyline* line0 = commoncircles[0][0];
	CMyPoint* myPoint0 = line0->pts[0];
	nLeft = myPoint0->Getx();
	nRight = myPoint0->Getx();
	nTop = myPoint0->Gety();
	nBottom = myPoint0->Gety();

	if (!isNULL) {
		int sizeLine = this->commonEdge.size();
		for (int j = 0; j < sizeLine; j++) {
			CGeoPolyline* line = this->commonEdge[j];
			if (!line->isNULL) {
				int sizePoint = line->pts.size();
				for (int z = 0; z < sizePoint; z++) {
					CMyPoint* myPoint = line->pts[z];
					if (!myPoint->isNULL) {
						if (nLeft > myPoint->Getx())
							nLeft = myPoint->Getx();
						if (nRight <myPoint->Getx())
							nRight = myPoint->Getx();
						if (nTop > myPoint->Gety())
							nTop = myPoint->Gety();
						if (nBottom < myPoint->Gety())
							nBottom = myPoint->Gety();
					}
				}
			}
		}
	}
	newRcet.left = nLeft;
	newRcet.right = nRight;
	newRcet.top = nTop;
	newRcet.bottom = nBottom;
	this->objectTopoRect = newRcet;
}


// ����ĳ�������ܳ�
double CGeoPolygon::getCircleLength(int i)
{
	double d = 0;
	vector<CGeoPolyline*> plines = commoncircles[i];
	int sizeofPlines = plines.size();
	for (int j = 0; j < sizeofPlines; j++) {
		if (!plines[j]->isNULL) {
			double l = plines[j]->getLineLength();
			d = d + l;
		}
	}
	return d;
}


// ��CPoint����Χ�ɵĻ������
double CGeoPolygon::getArea(vector<CMyPoint*> pts)
{
	double minY = this->getMinY(pts);
	double tempArea = 0;
	double area = 0;
	int size = pts.size();
	for (int i = 0; i < size; i++) {
		int j = i + 1;
		if (i == size - 1) j = 0;
		double h = pts[j]->Getx() - pts[i]->Getx();
		double y1 = pts[i]->Gety() - minY;
		double y2 = pts[j]->Gety() - minY;
		tempArea = (y1 + y2)*h/2;
		area = area + tempArea;
	}
	if (area < 0)
		area = -area;
	return area;
}


// ��һ�����ϵ㣬��x����С�����Լ�yֵ����С���꣬����100
double CGeoPolygon::getMinY(vector<CMyPoint*> pts)
{
	double y = pts[0]->Gety();
	int size = pts.size();
	for (int i = 0; i < size; i++) {
		
		if (y > pts[i]->Gety()) {
			y = pts[i]->Gety();
		}
	}
	return y;
}


// �õ����˽�����ĵ�������circlePoints�л�ã�
void CGeoPolygon::getTopoPointNum()
{
	int temp = 0;
	if (circlePoints.size() < 1)
		AfxMessageBox("���ȼ������");
	for (int i = 0; i < circlePoints.size(); i++) {
		int ptNum = circlePoints[i].size();
		circlePointNum[i] = ptNum;
		temp = temp + circlePointNum[i];
	}
	TopoPointNum = temp;
}


// �õ�ÿ�����ĳ���
void CGeoPolygon::getCircleLength()
{
	int size = commoncircles.size();
	if (size == 0)
		AfxMessageBox("commonCircle��С��ȻΪ0");
	double circlethresholdTemp;

	for (int i = 0; i < size; i++) {//isNULL��Ϊfalse
		circlethresholdTemp = this->getCircleLength(i);
		circleLength[i]=circlethresholdTemp;
	}
	TopoLength = 0;
	for (int i = 0; i < size; i++) {//isNULL��Ϊfalse
		TopoLength = TopoLength + circleLength[i];
	}
}


// ͨ���ܳ��������ɾ���������������ڻ�
void CGeoPolygon::delErrorCircle()
{
	int size = area.size();
	vector<double> bili;
	for (int i = 0; i < size; i++)
	{
		double part = area[i] / circleLength[i];//ͨ������ܳ�����ɾ���������������ڻ�
		bili.push_back(part);
	}
	for (int i = 0; i < commoncircles.size(); ) {
		CGeoPolyline* line = commoncircles[i][0];
		if (!line->isNULL) {
			if (line->leftPolygonID == -1 && i != 0) {//����������һ���ڲ��п�϶�Ļ�
													//ͨ�����/�ܳ�����ɸѡ

				if (bili[i] < 5000) {
					int size1 = commoncircles[i].size();
					for (int j = 0; j < size1; j++) {
						commoncircles[i][j]->isNULL = true;
					}
					bili.erase(bili.begin() + 1);
					commoncircles.erase(commoncircles.begin() + i);
					TopoArea = TopoArea + area[i];
					area.erase(area.begin() + i);
					circleLength.erase(circleLength.begin() + i);
					circlePoints.erase(circlePoints.begin() + i);
					TopoPointNum = TopoPointNum - circlePointNum[i];
					circlePointNum.erase(circlePointNum.begin() + i);
					circleNum--;
				}
				else
					i++;
			}
			else {
				i++;
			}
		}
		else {//�û�Ϊ��
			i++;
		}
	}
	bili.clear();
}


// ��commonCircle�л�ȡ�㣬����һ��˳����뵽circlePoints��
void CGeoPolygon::getCirclePoint()
{
	int size = commoncircles.size();
	circlePoints.resize(size);
	for(int i = 0; i < size; i++) {
		vector<CGeoPolyline*> lines = commoncircles[i];
		vector<CMyPoint*> cpts = this->getCirclePoint(lines);
		
		circlePoints[i] = cpts;
	}
	
}


// �ж�һ�����Ƿ����������
bool CGeoPolygon::isPtIn(CPoint pt)
{
	int threshold =commoncircles.size();
	double angleSum = 0.00;
	double angleTemp1 = 0.00;
	double angleTemp2 = 0.00;
	if(threshold>0){//˵������rebuildcircle������Դ�CirclePoint�н��ж�ȡ
		for(int i = 0;i<threshold;i++){
			vector<CGeoPolyline*> lines = commoncircles[i];
			int size1 = lines.size();
			angleTemp2 = 0.0;
			for(int j = 0;j<size1;j++){
				CGeoPolyline* pline = lines[j];
				angleTemp2+=pline->getAnglePtToLine(pt);
			}
			double cha1 = abs(1000*(getMod(angleTemp2,2*M_PI)));
			if(cha1>1){
				AfxMessageBox("һ��commoncircle��Ȼ����360��");
			}
			angleTemp1+=angleTemp2;
		}
		double cha2 =abs(abs(angleTemp1)-2*M_PI)*1000;
		if(cha2>1)
			return false;
		else 
			return true;
	}else{//ֻ�ܴ�Circle�����ж�ȡ
		int size2 = circles.size();
		
		for(int i = 0;i<size2;i++){
			angleTemp1 = 0;
			CGeoPolyline* line = circles[i];
			angleTemp1 = line->getAnglePtToLine(pt);
			double cha1 =getMod(angleTemp1,2*M_PI);
			cha1 = abs(1000*(cha1));
			if(cha1>1){
				AfxMessageBox("һ��circle��Ȼ����360��");
			}
			angleSum+=angleTemp1;
		}
		double cha2 = abs(abs(angleSum)-2*M_PI)*1000;
		if(cha2>1)
			return false;
		else 
			return true;
	}
}


// ����a��b������������С��ľ���ֵ
//double CGeoPolygon::getMod(double a, double b)
//{
//	double a1 = abs(a);
//	double b1 = abs(b);
//	int n = a1/b1;
//	double c = a1-b1*n;
//	c=abs(c);
//	double c1 = b-c;
//	if(c1<c)
//		c = c1;
//	return c;
//}


// �ж�һ�����Ƿ����������
bool CGeoPolygon::isPtInNew(CPoint pt)
{
	int threshold =commoncircles.size();
	if(threshold>0){//˵������rebuildcircle������Դ�CirclePoint�н��ж�ȡ
		int sum1 = 0;
		int temp = 0;
		for(int i = 0;i<threshold;i++){
			vector<CMyPoint*> pts = circlePoints[i];
			int ptNum = pts.size();
			for(int j = 0;j<ptNum-1;j++){
				CPoint pt1 = pts[j]->GetCPoint();
				CPoint pt2 = pts[j+1]->GetCPoint();
				temp+=getLinkofPtToSeg(pt1,pt2,pt);
			}
			sum1+=temp;
		}	
		if(sum1==0)
			return false;
		else if(sum1==2||sum1==-2)
			return true;
		else
			AfxMessageBox("�������");
	}else{//ֻ�ܴ�Circle�����ж�ȡ
		int size2 = circles.size();
		int sum1 = 0;
		int temp = 0;
		for(int i = 0;i<size2;i++){			
			CGeoPolyline* line = circles[i];
			for(int j = 0;j<line->pts.size()-1;j++){
				CPoint pt1 = line->pts[j]->GetCPoint();
				CPoint pt2 = line->pts[j+1]->GetCPoint();
				temp = getLinkofPtToSeg(pt1,pt2,pt);
				sum1+=temp;
			}
		}
		if(sum1==0)
			return false;
		else if(sum1==2||sum1==-2)
			return true;
		else 
			AfxMessageBox("�������");
	}
	
}


// �жϵ�pt�����Ϊpt1�յ�Ϊpt2���߶εĹ�ϵ����Դ���ж������Ƿ��ڶ�����ڵ��·������޷��жϵ�����ı߽���

int CGeoPolygon::getLinkofPtToSeg(CPoint pt1, CPoint pt2, CPoint pt)
{
	int i=0;
	CPoint pt12;//pt1��pt2������
	pt12.SetPoint((pt2.x-pt1.x)/100.0,(pt2.y-pt1.y)/100.0);
	double y = pt.y;
	double minY = min(pt1.y,pt2.y);
	double maxY = max(pt1.y,pt2.y);
	
	if (pt2.y>pt1.y)
	{
		i = 1;
	} 
	else if(pt2.y<pt1.y)
	{
		i = -1;
	}else
		i = 0;

	if(y<minY||y>maxY)
		return 0; 
	else if (y>minY&&y<maxY)
	{
		int j = ptPostoLine(pt1,pt2,pt);
		return 2*i*j;
	}else{
		int j = ptPostoLine(pt1,pt2,pt);
		return i*j;
	}
}


// pt1��pt2�ֱ���һ���߶ε������յ㣬�ж�pt���߶ε����Ҳ࣬1������࣬-1�����ϣ�0�����Ҳ�
//int CGeoPolygon::getPtLRtoLine(CPoint pt1, CPoint pt2, CPoint pt)
//{
//	int i=0;
//	//CPoint pt12;//pt1��pt2������
//	//pt12.SetPoint((pt2.x-pt1.x)/100.0,(pt2.y-pt1.y)/100.0);
//	//CPoint pt10;//p1��pt0������
//	//pt10.SetPoint((pt1.x-pt.x)/100.0,(pt1.y-pt.y)/100.0);
//
//	//double chaji =pt12.x*pt10.y - pt12.y*pt10.x;
//	int x = (pt.y-pt1.y)*(pt2.x - pt1.x)/(pt2.y - pt1.y)+pt1.x;
//	if(pt.x<x)
//		i=1;
//	else if(pt.x>x)
//		i=0;
//	else i = -1;
//	return i;
//}




// �ж�һ���߶�����Ĺ�ϵ��0�߶������⣻1����ӣ�2�ཻ��3����ӣ�4���ڲ�;5�ص�
int CGeoPolygon::isSegmentInPolygon(SegMent line1)
{
	CPoint pt1Ori = line1.ptOri;
	CPoint pt1End = line1.ptEnd;
	CPoint pt(0,0);
	struct PointAtt{
		int pos;//-1�������⣻0������߽磻1��������
		CPoint pt;
		PointAtt(int p,CPoint pt2){
			pos = p;
			pt = pt2;
		}
	};   
	PointAtt ptOri(0,pt1Ori),ptEnd(0,pt1End);
	if(isPtOnEdge(pt1Ori)){
		ptOri.pos = 0;
	}else if(isPtInNew(pt1Ori)){
		ptOri.pos = 1;
	}else{
		ptOri.pos = -1;
	}

	if(isPtOnEdge(pt1End)){
		ptEnd.pos = 0;
	}else if(isPtInNew(pt1End)){
		ptEnd.pos = 1;
	}else{
		ptEnd.pos = -1;
	}

	if (ptOri.pos*ptEnd.pos==-1)
	{
		return 2;
	}//�ж��߶���β��������Ĺ�ϵ
	//�����ж��߶�����Ľ��㣬����п�Խʽ���㣬�򷵻�2�����û�����е㣬�򷵻�0,4������ж�����е㣬�ж��Ƿ��ص��������е��ж��߶�����������
	vector<CPoint> joinPoints;
	int threshold =commoncircles.size();
	if(threshold>0){//˵������rebuildcircle������Դ�CirclePoint�н��ж�ȡ
		int sum1 = 0;
		int temp = 0;
		for(int i = 0;i<threshold;i++){
			vector<CMyPoint*> pts = circlePoints[i];
			int ptNum = pts.size();
			for (int j=0;j<ptNum-1;j++)
			{
				SegMent line2(pts[j]->GetCPoint(),pts[j+1]->GetCPoint());
				if (intersect(line1,line2))
				{
					return 2;
				} 
				else
				{
					CPoint tempPt1 = getJointPt(line2,line1);
					if (tempPt1==CPoint(1,1))//˵�������߶���ȫ������ı߽��ص�
					{ 
						return 5;
					}else if (tempPt1!=CPoint(0,0))
					{
						joinPoints.push_back(tempPt1);
					} 

					CPoint tempPt2 = getJointPt(line1,line2);
					if (tempPt2==CPoint(1,1))//˵�����ĳ���߽���ȫ�ڴ����߶��ڲ��ص�
					{ 
						joinPoints.push_back(line2.ptOri);
						joinPoints.push_back(line2.ptEnd);
					}else if (tempPt2!=CPoint(0,0))
					{
						joinPoints.push_back(tempPt2);
					} 
				}
			}
			if(joinPoints.size()==0){//��������߶�����û�н��㣬ֻ���ж��߶�ĳ���˵������ڻ������⣬�Ӷ�����0,4
				CPoint apt = line1.ptOri;
				if (isPtInNew(apt))
				{
					return 4;
				} 
				else
				{
					return 0;
				}
			}else{//�����߶������н��㣬�����㰴��xy�������е������ڻ������⣬�ο��Լ��㷽��
				vector<CPoint> paiXuPoints;
				paiXuPoints = pointsSort(joinPoints);
			}
		}	
		
	}else{//ֻ�ܴ�Circle�����ж�ȡ
		int size2 = circles.size();
		int sum1 = 0;
		int temp = 0;
		for(int i = 0;i<size2-1;i++){			
			CGeoPolyline* line = circles[i];
			
		}
		
	}

	return 0;
}


// �жϵ��Ƿ�����ı߽���
bool CGeoPolygon::isPtOnEdge(CPoint pt)
{
	CMyPoint myPt(pt);
	if (!myPt.isInRect(objectMABR))
	{
		return false;
	}else{
		int threshold =commoncircles.size();
		if(threshold>0){//˵������rebuildcircle������Դ�CirclePoint�н��ж�ȡ
			int sum1 = 0;
			int temp = 0;
			for(int i = 0;i<threshold;i++){
				vector<CMyPoint*> pts = circlePoints[i];
				int ptNum = pts.size();
				for(int j = 0;j<ptNum-1;j++){
					CPoint pt1 = pts[j]->GetCPoint();
					CPoint pt2 = pts[j+1]->GetCPoint();
					SegMent line(pt1,pt2);
					if (isPtOnSegment(pt,line))
					{
						return true;
					}
				}
			}	
			return false;
		}else{//ֻ�ܴ�Circle�����ж�ȡ
			int size2 = circles.size();
			int sum1 = 0;
			int temp = 0;
			for(int i = 0;i<size2;i++){			
				CGeoPolyline* line = circles[i];
				for(int j = 0;j<line->pts.size()-1;j++){
					CPoint pt1 = line->pts[j]->GetCPoint();
					CPoint pt2 = line->pts[j+1]->GetCPoint();
					SegMent line(pt1,pt2);
					if (isPtOnSegment(pt,line))
					{
						return true;
					}
				}
			}
			return false;
		}
	}	
}


// �жϵ��Ƿ����߶���
bool CGeoPolygon::isPtOnSegment(CPoint pt, SegMent line1)
{
	if (pt==line1.ptEnd||pt==line1.ptOri)
	{
		return true;
	}
	CPoint ptOri = line1.ptOri;
	CPoint ptEnd = line1.ptEnd;
	CRect rect(0,0,0,0);
	rect.bottom = ptEnd.y>ptOri.y?ptEnd.y:ptOri.y;
	rect.top = ptEnd.y<ptOri.y?ptEnd.y:ptOri.y;
	rect.left = ptEnd.x<ptOri.x?ptEnd.x:ptOri.x;
	rect.right = ptEnd.x>ptOri.x?ptEnd.x:ptOri.x;
	CMyPoint myPt(pt);
	if (myPt.isInRect(rect))
	{
		double x1 = ptEnd.x-ptOri.x;
		double y1 = ptEnd.y - ptOri.y;
		double x2 = pt.x-ptOri.x;
		double y2 = pt.y - ptOri.y;
		if (y1/x1==y2/x2)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}else{
		return false;
	}
	
}

// �õ�line2����line1�ϵĶ˵㣬���ֻ��һ���˵㣬���ظö˵㣬���û�ж˵㷵�أ�0,0��������������˵㣬���أ�1,1����line2�������˵�ͬʱ���
CPoint CGeoPolygon::getJointPt(SegMent line1, SegMent line2)
{
	CPoint pt(0,0);
	CPoint pt2Ori = line2.ptOri;
	CPoint pt2End = line2.ptEnd;
	int count = 0;
	if (isPtOnSegment(pt2Ori,line1))
	{
		pt=pt2Ori;
		count++;
	}
	if (isPtOnSegment(pt2End,line1))
	{
		pt =pt2End;
		count++;
	}
	if (count==0||count==1)
	{
		return pt;
	}else{
		return CPoint(1,1);
	}
}
//�ж��߶�line1���߶�line2�Ƿ��ཻ �ཻ����true, ���ཻ����false  
bool CGeoPolygon::intersect(SegMent line1,SegMent line2)  
{  
	CPoint aa=line1.ptOri;
	CPoint bb=line1.ptEnd;
	CPoint cc=line2.ptOri;
	CPoint dd=line2.ptEnd;
	double delta = determinant(bb.x-aa.x, cc.x-dd.x, bb.y-aa.y, cc.y-dd.y);  
	if ( delta<=(1e-6) && delta>=-(1e-6) )  // delta=0����ʾ���߶��غϻ�ƽ��  
	{  
		return false;  
	}  
	double namenda = determinant(cc.x-aa.x, cc.x-dd.x, cc.y-aa.y, cc.y-dd.y) / delta;  
	if ( namenda>1 || namenda<0 )  
	{  
		return false;  
	}  
	double miu = determinant(bb.x-aa.x, cc.x-aa.x, bb.y-aa.y, cc.y-aa.y) / delta;  
	if ( miu>1 || miu<0 )  
	{  
		return false;  
	}  
	return true;  
}  
///------------alg 3------------ 
///------------alg 3------------ Դ��//blog.csdn.net/rickliuxiao/article/details/6259322
double CGeoPolygon::determinant(double v1, double v2, double v3, double v4)  // ����ʽ  
{  
	return (v1*v3-v2*v4);  
}  

// �Ե�����oriPts����ȥ������xС����ǰ��x��ͬyС��ǰ
vector<CPoint> CGeoPolygon::pointsSort(vector<CPoint> oriPts)
{
	vector<CPoint> paiXuPoints;
	if (oriPts.size()==0)
	{
		return vector<CPoint>();
	}
	paiXuPoints.push_back(oriPts[0]);
	int oriPtNum = oriPts.size();
	for (int i = 1;i<oriPts.size();i++)
	{
		CPoint tempPt = oriPts[i];
		int paiXuptSize = paiXuPoints.size();
		if (tempPt.x>paiXuPoints[paiXuptSize-1].x||(tempPt.x==paiXuPoints[paiXuptSize-1].x&&tempPt.y>paiXuPoints[paiXuptSize-1].y))
		{
			paiXuPoints.push_back(tempPt);
			continue;
		}
		for (int j = 0;j<paiXuptSize;j++)//����
		{
			CPoint paiXuPt = paiXuPoints[j];
			if (tempPt.x>paiXuPt.x)
			{
				continue;
			} 
			else if (tempPt.x<paiXuPt.x)
			{
				paiXuPoints.insert(paiXuPoints.begin()+j,tempPt);
			} 
			else
			{
				if (tempPt.y>paiXuPt.y)
				{
					continue;
				} 
				else if (tempPt.y==paiXuPt.y)
				{
					break;
					
				} 
				else
				{
					paiXuPoints.insert(paiXuPoints.begin()+j,tempPt);
				}
			}
			
		}
	}
	return paiXuPoints;
}


// ����pts����һ���߶������ཻ�Ķ�����������ж����������ڵ���е��Ƿ����������⣬��������⣬����-1�������ڣ�����1��������У�����0���е�ȫ�ڱ߽��ϣ�����2
int CGeoPolygon::getSegmentPos(vector<CPoint> pts)
{
	int inNum = 0;
	int outNum = 0;
	int onNum = 0;
	int size = pts.size();
	for (int i = 0;i<size -1;i++)
	{
		double x =(pts[i].x+pts[i+1].x)/2;
		double y =(pts[i].y+pts[i+1].y)/2;
		CPoint pt;
		pt.SetPoint(x,y);
		if (isPtOnEdge(pt))
		{
			onNum++;
		}else if (isPtInNew(pt))
		{
			inNum++;
		} 
		else
		{
			outNum++;
		}
	}
	if(outNum==0&&inNum==0&&onNum!=0){
		return 2;
	}else if (outNum==0&&inNum!=0)
	{
		return 1;
	}else if (outNum!=0&&inNum==0)
	{
		return -1;
	}else if (outNum!=0&&inNum!=0)
	{
		return 0;
	}
	return 0;
}




// �ж��������Ƿ���Ժϲ����������������ͬһ�˵㣬��������һ�£�����Ժϲ�
bool CGeoPolygon::judgeLineMerge(CGeoPolyline* line1,CGeoPolyline* line2)
{
	bool isJointPt = false;
	if (line1->pts[0]->GetCPoint()==line2->pts[line2->pts.size()-1]->GetCPoint()||line1->pts[0]->GetCPoint()==line2->pts[0]->GetCPoint()
		||line1->pts[line1->pts.size()-1]->GetCPoint()==line2->pts[line2->pts.size()-1]->GetCPoint()||line1->pts[line1->pts.size()-1]->GetCPoint()==line2->pts[0]->GetCPoint())
	{
		isJointPt = true;
	}
	bool isAttSame = false;
	if ((line1->leftAttribute==line2->leftAttribute&&line1->rightAttribute==line2->rightAttribute)
		||(line1->leftAttribute==line2->rightAttribute&&line1->rightAttribute==line2->leftAttribute))
	{
		isAttSame = true;
	}
	if (isJointPt&&isAttSame)
	{
		return true;
	} 
	else
	{
		return false;
	}
	
}


// �ж��������ںϵ����ͣ�line1�����=line2����㷵��1��line1���յ�=line2���յ㷵��2��line1���յ�=line2����㷵��3��line1�����=line2���յ㷵��4���޷���������0
int CGeoPolygon::styleLineMerge(CGeoPolyline* line1, CGeoPolyline* line2)
{
	if (line1->pts[0]->GetCPoint()==line2->pts[0]->GetCPoint())
	{
		return 1;
	}else if (line1->pts[line1->pts.size()-1]->GetCPoint()==line2->pts[line2->pts.size()-1]->GetCPoint())
	{
		return 2;
	}else if (line1->pts[line1->pts.size()-1]->GetCPoint()==line2->pts[0]->GetCPoint())
	{
		return 3;
	}else if (line1->pts[0]->GetCPoint()==line2->pts[line2->pts.size()-1]->GetCPoint())
	{
		return 4;
	}else{
		return 0;
	}
}


// zf,0623����Ĺ���,��
void CGeoPolygon::ObjectFilter(void)
{
	if(circlePointList.size() != 0)
		vector<int>().swap(circlePointList);
	//����������
	for(int i = 0;i<circles.size();i++)
	{
		int number = circles[i]->pts.size();
		circlePointList.push_back(number);
		if(number>159&&number<=1001)  //����Ҫ�Լ�ȷ����ֵ
		{
			//circles.pop_back();
			circles.erase(circles.begin() + i);
		}
	}
}

// zf,��������ˣ�0701
void CGeoPolygon::ObjectFilterByArea(void)
{
	//�����
	vector<int> temp;  //�������飬����㻷
	if(circleAreaList.size()!=0) vector<double>().swap(circleAreaList);  //�����趨Ϊ��
	for(int i = 0;i<circles.size();i++)   //����������
	{
		double circlearea = circles[i]->getArea(circles[i]->pts);  //��ȡ�������
		circleAreaList.push_back(circlearea);
		if(circlearea<=areaT) temp.push_back(i); //*****�趨��ֵ*****�������С����ֵ����㻷������Ӹ���ֵ
	}
	vector<CGeoPolyline*> tempPolyline;     //�����µ�����㻷�Ļ�����
	if(tempPolyline.size()!=0) vector<CGeoPolyline*>().swap(tempPolyline); //��ֹ�����鲻Ϊ��
	for(int j = 0;j<circles.size();j++)     //��ӷ���㻷��������
	{
		bool answer = isContained(temp,j);    //�ж�j�Ƿ����temp�У�������˵��Ϊ���໷����circles�в�Ӧ�д˻�
		if(answer == false)                   //��������˵����Ϊ���໷����circlesӦ��˻�
			tempPolyline.push_back(circles[j]);   
	}
	tempPolyline.swap(circles);         //�������õ�ȥ�����໷֮��Ļ�circles
}

// ��ȡ��������л��ĵ���,0701
vector<int> CGeoPolygon::getPointNum(void)
{
	return circlePointList;
}

// ��ȡ��������л������,0701
vector<double> CGeoPolygon::getCircleArea(void)
{
	return circleAreaList;
}

// Ѱ��һ�����Ӿ��Σ�zf��0704
void CGeoPolygon::FindRectangle(void)
{
	if(rectangleArray.size() != 0)
		vector<CPoint*>().swap(rectangleArray);
	for(int i = 0;i<circles.size();i++)
	{
		FindRectangle(circles[i]->pts);
	}
	hasFindRectangle = true;
	hasFindMABR = false;
	hasGetBuildings = false;
	//rotateAll();  //������ת
}

// Ѱ��ĳһ��circle�İ���Ӿ���,zf,0704
void CGeoPolygon::FindRectangle(vector<CMyPoint*> pts)
{
	//AfxMessageBox("һ�����Ӿ���!");
	int size = pts.size();
	if(size == 0)
		AfxMessageBox("�û�Ϊ��");
	else
	{
		double Xmax = 0;
		double Ymax = 0;
		double Xmin = 60000000;  //��Сֵ���ܳ�ʼΪ0��
		double Ymin = 1000000000;  //��Сֵ���ܳ�ʼΪ0��
		for(int i = 0;i<size;i++)
		{
			double tempx = pts[i]->Getx();
			double tempy = pts[i]->Gety();
			if(tempx>=Xmax) Xmax = tempx;  //���x��
			if(tempy>=Ymax) Ymax = tempy;  //���y��
			if(tempx<=Xmin) Xmin = tempx;  //��Сx
			if(tempy<=Ymin) Ymin = tempy;  //��Сy
		}
		CPoint *pt1 = new CPoint(Xmax,Ymin);  //����
		CPoint *pt2 = new CPoint(Xmax,Ymax);
		CPoint *pt3 = new CPoint(Xmin,Ymax);
		CPoint *pt4 = new CPoint(Xmin,Ymin);

		rectangleArray.push_back(pt1);
		rectangleArray.push_back(pt2);
		rectangleArray.push_back(pt3);
		rectangleArray.push_back(pt4);
	}
}

// ĳһ��pt��center��תtheta�Ƕȣ�zf,0706
CPoint* CGeoPolygon::rotate(CPoint* pt, CPoint* center, double theta)
{
	double x1 = pt->x;
	double y1 = pt->y;
	double x0 = center->x;
	double y0 = center->y;

	double Q = theta / 180 * 3.1415926;  //�Ƕ�
	double A = cos(Q);
	double B = sin(Q);
	double x2,y2;

	x2 = (x1-x0)*A-(y1-y0)*B+x0;   //��ת��ʽ
	y2 = (x1-x0)*B+(y1-y0)*A+y0;

	CPoint* rotatePoint = new CPoint(x2,y2);
	//CPoint* rotatePoint = new CPoint((x1+10,y1+10));
	return rotatePoint;
}

// ��ת���о��Σ�zf��0706��������
void CGeoPolygon::rotateAll(void)
{
	if(rectangleRotate.size() != 0)
		vector<CPoint*>().swap(rectangleRotate);
	if(rectangleArray.size()!= 0)
	{
		int xunhuan = rectangleArray.size()/4;
		for(int n = 0;n<xunhuan;n++)  //һ������ο����ж������0706
		{
			double Cx = ((*(rectangleArray[1+n*4])).x + (*(rectangleArray[2+n*4])).x)/2;
			double Cy = ((*(rectangleArray[0+n*4])).y + (*(rectangleArray[1+n*4])).y)/2;
			CPoint* center = new CPoint(Cx,Cy);

			rectangleRotate.push_back(rotate(rectangleArray[0+n*4],center,45));
			rectangleRotate.push_back(rotate(rectangleArray[1+n*4],center,45));
			rectangleRotate.push_back(rotate(rectangleArray[2+n*4],center,45));
			rectangleRotate.push_back(rotate(rectangleArray[3+n*4],center,45));
		}
	}
}

// zf,0708��Ѱ����С��Ӿ���
void CGeoPolygon::FindMABR(void)
{
	if(rectangleRotate.size() != 0)    //*******
			vector<CPoint*>().swap(rectangleRotate);
	for(int i = 0;i<circles.size();i++)
	{
		circles[i]->MyPoint2Point(circles[i]->pts);       //ptsת��odinPts
		CPoint* center = FindCenter(circles[i]->odinPts);  //Ѱ������
		circles[i]->setCenter(center);

		if(circles[i]->MABR.size()!=0)     //*******
			vector<MABR_ZF*>().swap(circles[i]->MABR);

		for(int angle = 0;angle <= 90;angle++)   // -----1--------ԭʼͼ����ת0-90��
		{
			vector<CPoint*> tempArray;  //�洢��ת��Ļ�
			if(tempArray.size() != 0)  // //*******
				vector<CPoint*>().swap(tempArray);
			for(int j = 0;j<circles[i]->pts.size();j++)    //��תĳ������ÿһ��
				tempArray.push_back(rotate(circles[i]->odinPts[j],center,angle));  //��ת���ͼ�δ���tempArray��
			vector<CPoint*> temp = FindRectangle(tempArray); //-----------2---------����ת�����Ӿ���
			if(temp.size() == 0)
				AfxMessageBox("����Ӿ��λ�ȡʧ�ܣ�");
			else
			{
				MABR_ZF *tempRect = new MABR_ZF();   //ĳ����ת�Ƕ�ʱ����Ӿ���ָ��
				if(tempRect->vertices.size()!=0)     //*******
					vector<CPoint*>().swap(tempRect->vertices);
				for(int count = 0;count<temp.size();count++)    //����Ӿ��ζ���ת�Ƶ�circles�ı�����
					tempRect->vertices.push_back(temp[count]);
				//��ÿ����Ӿ��ε����
				double deltaX = tempRect->vertices[2]->x - tempRect->vertices[0]->x;
				double deltaY = tempRect->vertices[2]->y - tempRect->vertices[0]->y;
				tempRect->area = deltaY * deltaX;
				tempRect->ID = angle;
				circles[i]->MABR.push_back(tempRect);  //MABR�����ڴ洢��ת������ÿ����Ӿ��ε����飬�����Ԫ��ÿ�������ָ��
			}
		}

		//------3-----------�Ƚ�ÿ����Ӿ��Σ�ȷ��ÿ���������С����Ӿ���
		int finalID;
		double compare = 60000000000;
		for(int num = 0;num<circles[i]->MABR.size();num++)
		{
			if(compare > circles[i]->MABR[num]->area)
			{
				finalID = circles[i]->MABR[num]->ID;
				compare = circles[i]->MABR[num]->area;
			}
		}
		for(int num = 0;num<circles[i]->MABR.size();num++)
		{
			if(circles[i]->MABR[num]->ID == finalID)
			{
				circles[i]->finalMABR.area = circles[i]->MABR[num]->area;
				circles[i]->finalMABR.ID = circles[i]->MABR[num]->ID;
				if(circles[i]->finalMABR.vertices.size()!=0)     //*******
					vector<CPoint*>().swap(circles[i]->finalMABR.vertices);
				for(int pointNum = 0; pointNum<circles[i]->MABR[num]->vertices.size();pointNum++)
					circles[i]->finalMABR.vertices.push_back(circles[i]->MABR[num]->vertices[pointNum]);
				break;
			}
		}

		//----------4-------����Ӿ��γ��෴�ķ�����ת��ͬ����
		int finalAngle = circles[i]->finalMABR.ID;
		for(int final = 0;final<circles[i]->finalMABR.vertices.size();final++)
			rectangleRotate.push_back(rotate(circles[i]->finalMABR.vertices[final],center,-finalAngle));  //�������˴��Ƕ��෴
	}
	hasFindMABR = true;
	hasFindRectangle = false;
	hasGetBuildings = false;
}

// Ѱ�һ������ģ����ģ���zf��0708
CPoint* CGeoPolygon::FindCenter(vector<CPoint*> ptsArray)
{
	double tempX,tempY;
	double sumX = 0;
	double sumY = 0;
	int size = ptsArray.size();
	for(int i = 0;i<size;i++)
	{
		sumX = sumX + ptsArray[i]->x;
		sumY = sumY + ptsArray[i]->y;
	}
	tempX = sumX/size;
	tempY = sumY/size;
	CPoint* pt = new CPoint(tempX,tempY);
	return pt;
}

// ĳ����ε�һ����Ӿ��Σ�zf��0708
vector<CPoint*> CGeoPolygon::FindRectangle(vector<CPoint*> pts)
{
	vector<CPoint*> temp;
	//int size = pts.size();
	if(pts.size() == 0)
		AfxMessageBox("�û�Ϊ��");
	else
	{
		double Xmax = 0;
		double Ymax = 0;
		double Xmin = 6000000000;  //��Сֵ���ܳ�ʼΪ0��
		double Ymin = 1000000000;  //��Сֵ���ܳ�ʼΪ0��
		for(int i = 0;i<pts.size();i++)
		{
			double tempx = pts[i]->x;
			double tempy = pts[i]->y;
			if(tempx>=Xmax) Xmax = tempx;  //���x��
			if(tempy>=Ymax) Ymax = tempy;  //���y��
			if(tempx<=Xmin) Xmin = tempx;  //��Сx
			if(tempy<=Ymin) Ymin = tempy;  //��Сy
		}
		
		CPoint *pt0 = new CPoint(Xmin,Ymin);  //����
		CPoint *pt1 = new CPoint(Xmax,Ymin);  //����
		CPoint *pt2 = new CPoint(Xmax,Ymax);  //����
		CPoint *pt3 = new CPoint(Xmin,Ymax);  //����

		temp.push_back(pt0);
		temp.push_back(pt1);
		temp.push_back(pt2);
		temp.push_back(pt3);
		
	}
	return temp;
}

// ȥ������㣬zf��0717
void CGeoPolygon::RemoveRedundant(void)
{
	for(int count = 0; count<times;count++) GetHalfPoints();

	for(int i = 0;i<circles.size();i++)  //����ÿ��circles
	{
		vector<int> temp;                //���ڴ���������±�
		bool odinary = true;             //�趨״̬����ȷ�����㵥Ԫ���׵�
		int count = 0;                   //��¼ɾ���ĵ�����һ������������ָ�Ϊ0
		int ptsNum = circles[i]->pts.size();
		for(int n = 0;n<ptsNum-2;n++)
		{
			CMyPoint*first,*middle,*last;  //��������
			if(odinary)   //������û�����������
			{
				first = circles[i]->pts[n];
				middle = circles[i]->pts[n+1];
				last = circles[i]->pts[n+2];
			}
			else         //���������������������
			{
				first = circles[i]->pts[n-count];   //��������㣬ɾ�����ж������֮��ĵ��Ƿ�Ϊ����㻹��Ҫ֮ǰ���㵥Ԫ���׵�
				middle = circles[i]->pts[n+1];
				last = circles[i]->pts[n+2];
			}
			double angle = calAngle(first,middle,last);   //����Ƕ�
			if(angle >= 20&&angle<=160)         //�Ƕ���ֵ��Ϊ10��,���Ƕȴ�����ֵ���������������Ҫȥ����
			{
				odinary = true;
				count = 0;                 //��¼ɾ���ĵ�����һ������������ָ�Ϊ0
			}
			else   // ���������
			{
				temp.push_back(n+1);     //��¼�������circles[i]->pts���±�
				odinary = false;         //���������
				count++;                 //����������ʱ��Ҫ��¼
			}
		}
		vector<CMyPoint*> tempPoints;    //������ʱ�㼯
		if(tempPoints.size()!=0) vector<CMyPoint*>().swap(tempPoints); //��ֹ�㼯��Ϊ��
		for(int j = 0;j<ptsNum;j++)
		{
			bool answer = isContained(temp,j);    //�ж�j�Ƿ����temp�У�������˵��Ϊ����㣬�������в�Ӧ�д˵�
			if(answer == false)                   //��������˵����Ϊ����㣬������Ӧ��˵�
				tempPoints.push_back(circles[i]->pts[j]);   
		}
		tempPoints.swap(circles[i]->pts);         //�������õ�ȥ�������֮��Ļ�circles[i]->pts
	}
}

// 0904,zf,����һ��ĵ�
void CGeoPolygon::GetHalfPoints(void)
{
	for(int i = 0;i<circles.size();i++){
		vector<CMyPoint*> tempPoints;    //������ʱ�㼯
		if(tempPoints.size()!=0) vector<CMyPoint*>().swap(tempPoints); //��ֹ�㼯��Ϊ��
		for(int j = 0;j<circles[i]->pts.size();j++){
			if(j%2==0) tempPoints.push_back(circles[i]->pts[j]);
		}
		tempPoints.swap(circles[i]->pts);
	}
}


// ��������֮��ĽǶȣ�zf��0717
double CGeoPolygon::calAngle(CMyPoint* p1, CMyPoint* p2, CMyPoint* p3)
{
	double x1 = p1->Getx();
	double y1 = p1->Gety();
	double x2 = p2->Getx();
	double y2 = p2->Gety();
	double x3 = p3->Getx();
	double y3 = p3->Gety();

	double angle,p1p2,p2p3,p1p3;
	p1p2 = sqrt(((x1 - x2) * (x1 - x2) + (y1 -y2) * (y1 -y2)));
	p2p3 = sqrt(((x3 - x2) * (x3 - x2) + (y3 -y2) * (y3 -y2)));
	p1p3 = sqrt(((x1 - x3) * (x1 - x3) + (y1 -y3) * (y1 -y3)));
	angle = acos((p1p2 * p1p2 + p2p3 * p2p3 - p1p3 * p1p3) / (2 * p1p2 * p2p3)) * 180.0 / 3.1415926;
	if(angle<0)
		angle = angle*(-1);
    return angle;
}

// �ж�ĳ�����ǲ����������У�zf��0717
bool CGeoPolygon::isContained(vector<int> temp, int i)
{
	bool answer = false;
	for(int j = 0;j<temp.size();j++)
	{
		if(i == temp[j]) answer = true;
		if(answer) break;
	}
	return answer;
}

// ���� ������ռ��С��Ӿ��εı�����������/��С��Ӿ��� ��zf��0712
bool CGeoPolygon::areaProportion(void)
{
	bool hasgetMABR = true;
	for(int i = 0;i<circles.size();i++)
	{
		double circleArea = circles[i]->getArea(circles[i]->pts);
		if(circles[i]->finalMABR.vertices.size() == 0) hasgetMABR = false;  //��С��Ӿ��β�����ʱ
		else
		{
			double deltaX = circles[i]->finalMABR.vertices[0]->x - circles[i]->finalMABR.vertices[2]->x;
			double deltaY = circles[i]->finalMABR.vertices[0]->y - circles[i]->finalMABR.vertices[2]->y;
			//double MABRArea = deltaX * deltaY;
			circles[i]->areaProportion = circleArea/(deltaX * deltaY);
		}
	}
	return hasgetMABR;
}

// ��С��Ӿ����Ż���0712��zf
bool CGeoPolygon::optimize(void)
{
	hasFindMABR==false;
	//------1------- ������ж���ε����ռ��
	bool hasgetMABR = areaProportion();
    //------2------  ���ݲ�ͬ������ʵʩ��ͬ����
	if(hasgetMABR)
	{
		for(int i = 0 ;i<circles.size();i++)
		{
			if(circles[i]->areaProportion<0.9) fitting(circles[i]); //������90%������С��Ӿ�������ϴ�������90%������С��Ӿ��ξ��������
		}
	}
	return hasgetMABR;
}

// MABR��Ͻ��������Σ�zf��0719
void CGeoPolygon::fitting(CGeoPolyline* line)
{
	vector<CPoint*> tempArray;  //�洢�õ�MABRʱ�Ļ�������
	for(int i = 0;i<line->odinPts.size();i++)
		tempArray.push_back(rotate(line->odinPts[i],&(line->center),line->finalMABR.ID)); //��ת����ÿһ��
	tempArray.swap(line->odinPts);  //����0919

	//------����ƽ����xy���line->finalMABR�����д�����µĽ��������εĵ� line->odinPts
	//------�ȼ������е��ĸ����㣬�ó�Xmax��Xmin��Ymax��Ymin,�ж�����������������
	vector<CPoint*> temp = FindRectangle(line->odinPts); 
	double Ymin = temp[0]->y, Xmin = temp[0]->x, Xmax = temp[2]->x, Ymax = temp[2]->y;

	double y1,x2,y3,x4;
	int i1,i2,i3,i4;
	for(int i = 0;i<line->odinPts.size();i++){
		if(line->odinPts[i]->x == Xmin)  i1 = i;  //��
		if(line->odinPts[i]->y == Ymax)  i2 = i;  //��
		if(line->odinPts[i]->x == Xmax)  i3 = i;  //��
		if(line->odinPts[i]->y == Ymin)  i4 = i;  //��
	}
	y1 = line->odinPts[i1]->y;x2 = line->odinPts[i2]->x;y3 = line->odinPts[i3]->y;x4 = line->odinPts[i4]->x;
	vector<CPoint*> line1,line2,line3,line4;
	for(int j = 0;j<line->odinPts.size();j++){
		double compareX = line->odinPts[j]->x,compareY = line->odinPts[j]->y;  //���ֺ���Ҫ������
		if(i2<i3){     
			if( (compareX<=x2&&compareY>y1) && (j<=i2||j>=i1) ) line1.push_back(line->odinPts[j]);
			if( (compareX>x2&&compareY>=y3) && (j>=i2&&j<=i3) ) line2.push_back(line->odinPts[j]);
			if( (compareX>=x4&&compareY<y3) && (j>=i3&&j<=i4) ) line3.push_back(line->odinPts[j]);
			if( (compareX<x4&&compareY<=y1) && (j<=i1&&j>=i4) ) line4.push_back(line->odinPts[j]);
		}
		else{
			if( (compareX<=x2&&compareY>y1) && (j>=i1&&j<=i2) ) line1.push_back(line->odinPts[j]);
			if( (compareX>x2&&compareY>=y3) && (j<=i2||j>=i3) ) line2.push_back(line->odinPts[j]);
			if( (compareX>=x4&&compareY<y3) && (j>=i3&&j<=i4) ) line3.push_back(line->odinPts[j]);
			if( (compareX<x4&&compareY<=y1) && (j<=i1&&j>=i4) ) line4.push_back(line->odinPts[j]);
		}
	}
	int p1 = 0,p2 = 0 ,p3 = 0 ,p4 = 0 ;
	double deltaX,deltaY,tempArea;
	double compareArea1 = 0;
	for(int i = 0;i<line1.size();i++){
		deltaX = line1[i]->x - Xmin;
		deltaY = Ymax - line1[i]->y;
		tempArea = deltaY * deltaX;
		if(tempArea >= compareArea1){
			compareArea1 = tempArea;
			p1 = i;
		}
	}
	if(line1.size()==0) line1.push_back(&CPoint(Xmin,Ymax));
	double compareArea2 = 0;
	for(int i = 0;i<line2.size();i++){
		deltaX = Xmax - line2[i]->x;
		deltaY = Ymax - line2[i]->y;
		tempArea = deltaY * deltaX;
		if(tempArea >= compareArea2){
			compareArea2 = tempArea;
			p2 = i;
		}
	}
	if(line2.size()==0) line2.push_back(&CPoint(Xmin,Ymax));
	double compareArea3 = 0;
	for(int i = 0;i<line3.size();i++){
		double tempArea = 0;
		deltaX = Xmax - line3[i]->x;
		deltaY = line3[i]->y - Ymin;
		tempArea = deltaY * deltaX;
		if(tempArea >= compareArea3){
			compareArea3 = tempArea;
			p3 = i;
		}
	}
	if(line3.size()==0) line3.push_back(&CPoint(Xmin,Ymax));
	double compareArea4 = 0;
	for(int i = 0;i<line4.size();i++){
		double tempArea = 0;
		deltaX = line4[i]->x - Xmin;
		deltaY = line4[i]->y - Ymin;
		tempArea = deltaY * deltaX;
		if(tempArea >= compareArea4){
			compareArea4 = tempArea;
			p4 = i;
		}
	}
	if(line4.size()==0) line4.push_back(&CPoint(Xmin,Ymax));

	vector<CPoint*> outcome;
	double minArea = areaT;
	
	bool proper1 = !(line1[p1]->x>=line3[p3]->x||line1[p1]->y<=line3[p3]->y);   //������λ ��Ϊ��
	bool proper2 = !(line2[p2]->x<=line4[p4]->x||line2[p2]->y<=line3[p3]->y);
	if(proper1&&proper2){
		if(line1.size()!=0){
			if(compareArea1>=minArea){
				CPoint *pt1 = new CPoint(Xmin,line1[p1]->y);
				outcome.push_back(pt1);
				outcome.push_back(line1[p1]);
				CPoint *pt2 = new CPoint(line1[p1]->x,Ymax);
				outcome.push_back(pt2);
			}
			else outcome.push_back(temp[3]);
		}
		if(line2.size()!=0){
			if(compareArea2>=minArea){
				CPoint *pt3 = new CPoint(line2[p2]->x,Ymax);
				outcome.push_back(pt3);
				outcome.push_back(line2[p2]);
				CPoint *pt4 = new CPoint(Xmax,line2[p2]->y);
				outcome.push_back(pt4);
			}
			else outcome.push_back(temp[2]);
		}
		if(line3.size()!=0){
			if(compareArea3>=minArea){
				CPoint *pt5 = new CPoint(Xmax,line3[p3]->y);
				outcome.push_back(pt5);
				outcome.push_back(line3[p3]);
				CPoint *pt6 = new CPoint(line3[p3]->x,Ymin);
				outcome.push_back(pt6);
			}
			else outcome.push_back(temp[1]);
		}
		if(line4.size()!=0){
			if(compareArea4>=minArea){
				CPoint *pt7 = new CPoint(line4[p4]->x,Ymin);
				outcome.push_back(pt7);
				outcome.push_back(line4[p4]);
				CPoint *pt8 = new CPoint(Xmin,line4[p4]->y);
				outcome.push_back(pt8);
			}
			else outcome.push_back(temp[0]);
		}

		CPoint* pt = outcome[0];
		outcome.push_back(pt);
	}
	if(!proper1){
		if(line1.size()!=0){
			if(compareArea1>=compareArea3){
				if(compareArea1>=minArea){
					CPoint *pt1 = new CPoint(Xmin,line1[p1]->y);
					outcome.push_back(pt1);
					outcome.push_back(line1[p1]);
					CPoint *pt2 = new CPoint(line1[p1]->x,Ymax);
					outcome.push_back(pt2);
				}
				else outcome.push_back(temp[3]);
			}
			else outcome.push_back(temp[3]);
		}

		if(line2.size()!=0){
			if(compareArea2>=minArea){
				CPoint *pt3 = new CPoint(line2[p2]->x,Ymax);
				outcome.push_back(pt3);
				outcome.push_back(line2[p2]);
				CPoint *pt4 = new CPoint(Xmax,line2[p2]->y);
				outcome.push_back(pt4);
			}
			else outcome.push_back(temp[2]);
		}
		if(line3.size()!=0){
			if(compareArea1<compareArea3){
				if(compareArea3>=minArea){
					CPoint *pt5 = new CPoint(Xmax,line3[p3]->y);
					outcome.push_back(pt5);
					outcome.push_back(line3[p3]);
					CPoint *pt6 = new CPoint(line3[p3]->x,Ymin);
					outcome.push_back(pt6);
				}
				else outcome.push_back(temp[1]);
			}
			else outcome.push_back(temp[1]);
		}
		if(line4.size()!=0){
			if(compareArea4>=minArea){
				CPoint *pt7 = new CPoint(line4[p4]->x,Ymin);
				outcome.push_back(pt7);
				outcome.push_back(line4[p4]);
				CPoint *pt8 = new CPoint(Xmin,line4[p4]->y);
				outcome.push_back(pt8);
			}
			else outcome.push_back(temp[0]);
		}

		CPoint* pt = outcome[0];
		outcome.push_back(pt);
	}

	if(!proper2){
		if(line1.size()!=0){
			if(compareArea1>=minArea){
				CPoint *pt1 = new CPoint(Xmin,line1[p1]->y);
				outcome.push_back(pt1);
				outcome.push_back(line1[p1]);
				CPoint *pt2 = new CPoint(line1[p1]->x,Ymax);
				outcome.push_back(pt2);
			}
			else outcome.push_back(temp[3]);
		}
		if(line2.size()!=0){
			if(compareArea2>compareArea4){
				if(compareArea2>=minArea){
					CPoint *pt3 = new CPoint(line2[p2]->x,Ymax);
					outcome.push_back(pt3);
					outcome.push_back(line2[p2]);
					CPoint *pt4 = new CPoint(Xmax,line2[p2]->y);
					outcome.push_back(pt4);
				}
				else outcome.push_back(temp[2]);
			}
			else outcome.push_back(temp[2]);
		}
		if(line3.size()!=0){
			if(compareArea3>=minArea){
				CPoint *pt5 = new CPoint(Xmax,line3[p3]->y);
				outcome.push_back(pt5);
				outcome.push_back(line3[p3]);
				CPoint *pt6 = new CPoint(line3[p3]->x,Ymin);
				outcome.push_back(pt6);
			}
			else outcome.push_back(temp[1]);
		}
		if(line4.size()!=0){
			if(compareArea2<=compareArea4){
				if(compareArea4>=minArea){
					CPoint *pt7 = new CPoint(line4[p4]->x,Ymin);
					outcome.push_back(pt7);
					outcome.push_back(line4[p4]);
					CPoint *pt8 = new CPoint(Xmin,line4[p4]->y);
					outcome.push_back(pt8);
				}
				else outcome.push_back(temp[0]);
			}
			else outcome.push_back(temp[0]);
		}

		CPoint* pt = outcome[0];
		outcome.push_back(pt);
	}
	
    int finalAngle = line->finalMABR.ID;
	if(buildings.size() != 0) vector<CPoint*>().swap(buildings);
	for(int final = 0;final<outcome.size();final++)
		buildings.push_back(rotate(outcome[final],&(line->center),-finalAngle));  //�������˴��Ƕ��෴

	hasFindRectangle = false;
	hasFindMABR = false;
	hasGetBuildings = true;
}

// ȥ��β�� 1003 zf
void CGeoPolygon::DeleteTail(void)
{
	for(int i = 0 ; i < circles.size();i++){

		int firstNum = circles[i]->pts.size();
		int lastNum = 0;
		int t = circles[i]->pointDisThreshold; //�±����ֵ
		double length = circles[i]->calAvgDis(circles[i]->pts);  //������ֵ
		int count = 0;
		while( count<1 && firstNum!=lastNum )
		{
			firstNum = circles[i]->pts.size();
			int num = circles[i]->pts.size()-1;  //���ĵ���(ȥ��β��)
			int finalJ = 0, finalK = 0, FinalDis = 0, finalType = 0;  //���J,K,����
			if(num > t) { //�����ܵ������ڵ�������ֵʱ

				//�ҵ����л����±����������
				for(int j = 0 ; j < (num - t); j++){ //��������ÿ���㣬
					int tempDis = 0, tempk = 0, temptype = 0;
					for(int k = (j + t) ; k<num && (j+num-k)>=t ; k++) {  //���ÿһ���㣬����õ������Ҫ��ĵ�ľ��룬�ҵ��±��������һ����,
						int dis = (j+num-k)>=(k-j)?(k-j):(j+num-k);   //ȡС��
						int type = (j+num-k)>=(k-j)?1:2;   // 1����˳��2�����˳��
						if( (calDis(circles[i]->pts[j],circles[i]->pts[k])<(length*3)) && (tempDis<dis) ){  //Ѱ���±�����
							tempDis = dis;
							tempk = k;
							temptype = type;
						}
					}
					if( FinalDis<=tempDis ){
						FinalDis = tempDis;
						finalJ = j;
						finalK = tempk;
						finalType = temptype;
					}
				}	
			}
			//���� J K �����±꣬�ж������±�ֱ�ӵĵ���ɵĻ�����Ƿ񳬹���ֵ,
			switch(finalType){
			case 1:
				{
					CGeoPolyline* line = new CGeoPolyline();
					for(int ii = finalJ; ii<finalK+1;ii++)   line->pts.push_back(circles[i]->pts[ii]);
					line->pts.push_back(circles[i]->pts[finalJ]);
					if(line->getArea(line->pts)<= (1*areaT)){
						CGeoPolyline *tmp = new CGeoPolyline();
						for(int iii = 0 ; iii<circles[i]->pts.size();iii++)
							if(iii<finalJ||iii>finalK) tmp->pts.push_back(circles[i]->pts[iii]);
						tmp->pts.swap(circles[i]->pts);
					}
					else{
						/*CGeoPolyline *tmp = new CGeoPolyline();
						CGeoPolyline *othertmp = new CGeoPolyline();
						for(int iii = 0 ; iii<circles[i]->pts.size();iii++)
						{
							if(iii<finalJ||iii>finalK) tmp->pts.push_back(circles[i]->pts[iii]);
							else othertmp->pts.push_back(circles[i]->pts[iii]);
						}
						tmp->pts.swap(circles[i]->pts);
						if(othertmp->pts.size()!=0){
							othertmp->pts.push_back(othertmp->pts[0]);
							circles.push_back(othertmp);
						}*/
					}
					break;
				}
			case 2:
				{
					CGeoPolyline* line = new CGeoPolyline();
					for(int ii = 0; ii<finalJ+1;ii++)   line->pts.push_back(circles[i]->pts[ii]);
					for(int jj = finalK; jj<circles[i]->pts.size();jj++)  line->pts.push_back(circles[i]->pts[jj]);
					line->pts.push_back(circles[i]->pts[0]);
					if(line->getArea(line->pts)<= (1*areaT)){   //�ж����
						CGeoPolyline *tmp = new CGeoPolyline();  //��С�ڣ���tmpΪ��ȥ��β�͵��
						for(int iii = 0 ; iii<circles[i]->pts.size();iii++)
							if(iii>finalJ&&iii<finalK) tmp->pts.push_back(circles[i]->pts[iii]);
						if(tmp->pts.size()!=0) tmp->pts.push_back(tmp->pts[0]);
						tmp->pts.swap(circles[i]->pts);   //����
					}
					else{
						/*CGeoPolyline *tmp = new CGeoPolyline();
						CGeoPolyline *othertmp = new CGeoPolyline();
						for(int iii = 0 ; iii<circles[i]->pts.size();iii++)
						{
							if(iii>finalJ&&iii<finalK) tmp->pts.push_back(circles[i]->pts[iii]);
							else othertmp->pts.push_back(circles[i]->pts[iii]);
						}
						tmp->pts.swap(circles[i]->pts);
						if(othertmp->pts.size()!=0){
							othertmp->pts.push_back(othertmp->pts[0]);
							circles.push_back(othertmp);
						}*/
					}
					break;
				}
			case 0:
					break;
			}
			lastNum = circles[i]->pts.size();
			count++;
		}
		
	}
}


// ��������ľ���  1003 zf
double CGeoPolygon::calDis(CMyPoint* pt1, CMyPoint* pt2)
{
	double deltaX = pt1->Getx() - pt2->Getx();
	double deltaY = pt1->Gety() - pt2->Gety();
	return sqrt(deltaX*deltaX+deltaY*deltaY);
}
