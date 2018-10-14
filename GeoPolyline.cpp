#include "stdafx.h"
#include "GeoPolyline.h"
#define _USE_MATH_DEFINES   //����math.h�еĶ���  
#include <math.h>

using namespace std;


CGeoPolyline::CGeoPolyline()
{
	isNULL = false;
	isInCommonCircle = false;

	areaProportion = 0;
	center = CPoint(0,0);
	//avgDis = calAvgDis(pts);
	pointDisThreshold = 10;
}

CGeoPolyline::CGeoPolyline(CGeoPolyline& polyline)
{
	
	for (int i = 0; i < polyline.GetSize(); i++)
	{
		CMyPoint* pt=new CMyPoint(*(polyline.pts[i]));
		pts.push_back(pt);
	}
	circleID = polyline.circleID;
	leftPolygonID = polyline.leftPolygonID;
	rightPolygonID = polyline.rightPolygonID;
	polygonID = polyline.polygonID;
	PosinCommonLayer = polyline.PosinCommonLayer;

	/*for (int i = 0; i < polyline.endPointPos.size(); i++)
	{
		int temp = polyline.endPointPos[i];
		endPointPos.push_back(temp);
	}*/
}

CGeoPolyline::~CGeoPolyline()
{
	for (vector<CMyPoint*>::iterator ita = pts.begin();
		ita != pts.end(); ++ita) {
		CMyPoint* pt = *ita;
		if (pt) {
			delete pt;
			pt = NULL;
		}
	}
	pts.clear();
}

void CGeoPolyline::AddPoint(CMyPoint* pt)
{
	pts.push_back(pt);
}

void CGeoPolyline::Draw(CDC* pDC)
{
	int size = pts.size();
	CPoint pt;
	for (int i = 0; i < size; i++)
	{
		CMyPoint* point = pts[i];
		pt = point->GetCPoint();
		if (i == 0)
			pDC->MoveTo(pt);
		else
		{
			pDC->LineTo(pt);
			pDC->MoveTo(pt);
		}
	}
}

void CGeoPolyline::Draw(CDC * pDC, CMyDCConfig myDCcfg)
{
	CPen pen(PS_SOLID, 1, RGB(255,255,255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int size = pts.size();
	CPoint pt;
	CBrush brush(RGB(0,255,0));
	CBrush *pOldBrush = pDC->SelectObject(&brush);  //��ˢ
	
	CArray<CPoint,CPoint> ptss;  //���������
	//commonEdge[0]->MyPointsToCPoints(circlePoints[0], pts);  //���⻷�ĵ�浽pts��
	int count = 0;

	/*char bj = 'aaa';
	int tempxMax = 0,tempyMax = 0;
	for(int i = 0;i<size;i++)
	{
		if(pts[i]->Gety()>tempyMax) tempyMax = pts[i]->Gety();
		if(pts[i]->Getx()>tempxMax) tempxMax = pts[i]->Getx();
	}*/

	char c = 'c';
	for (int i = 0; i < size; i++)
	{
		CMyPoint* point = pts[i];
		pt = point->GetCPoint();
		ptss.Add(pt);
		if (i == 0)
			pDC->MoveTo(pt);
		else
		{
			pDC->LineTo(pt);
			pDC->MoveTo(pt);
			pDC->SetPixel(pt,myDCcfg.lineColor);
		}
		/*if(i==0) 
		{
			CString str;
			str.Format("%c",c);
			pDC->TextOutA(pt.x,pt.y,str);
			count++;
		}*/
		/*if(pts[i]->Gety() == tempyMax )
		{
			CString str;
			str.Format("%c",bj);
			pDC->TextOutA(pt.x+200,pt.y+500,str);
			count++;
		}*/
		/*if(count <= 3)
		{
			CString str;
			str.Format("%d",count);
			pDC->TextOutA(pt.x+100,pt.y+100,str);
			count++;
		}*/
	}

    pDC->BeginPath();
	CPoint *pta = ptss.GetData();
	pDC->Polygon(pta,ptss.GetSize());   //��������
	pDC->EndPath();
	pDC->FillPath();

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}


int CGeoPolyline::GetSize()
{
	return pts.size();
}

CMyPoint* CGeoPolyline::GetPointAt(int index)
{
	return pts[index];
}

void CGeoPolyline::removePoints(int index1, int index2,int posInCommonLayer)
{
	if (index1 < index2) {
		CMyPoint* myPoint2 = new CMyPoint(CPoint(-1, posInCommonLayer));
		/*for (vector<CMyPoint*>::iterator it = pts.begin() + index1 + 1; it != pts.begin() + index2; it++) {
			if (NULL != *it)
			{
				delete *it;
				*it = NULL;
			}
		}*/
		for (vector<CMyPoint*>::iterator ita = pts.begin()+index1 + 1;
			ita < pts.begin()+index2; ++ita) {
				CMyPoint* pt = *ita;
				if (pt) {
					delete pt;
					pt = NULL;
				}
		}
		pts.erase(pts.begin()+index1 + 1,pts.begin()+index2);
		pts.insert(pts.begin() + index1 + 1, myPoint2);
	}
	else  {
		CMyPoint* myPoint = new CMyPoint(CPoint(-1, posInCommonLayer));
		//for (int i = index1 + 1; i < pts.size(); i++) {
	
			//pts[i] = NULL;
		//}
		for (vector<CMyPoint*>::iterator ita = pts.begin()+index1 + 1;
			ita != pts.end(); ++ita) {
				CMyPoint* pt = *ita;
				if (pt) {
					delete pt;
					pt = NULL;
				}
		}
		pts.erase(pts.begin()+index1 + 1,pts.end());
		//�Ƿ��С��size
		pts.push_back(myPoint);
		pts.push_back(this->pts[index2]);
		//for (int i = 0; i < index2; i++) {
		//	//delete pts[i];
		//	pts[i] = NULL;
		//}
		for (vector<CMyPoint*>::iterator ita = pts.begin();
			ita < pts.begin()+index2; ++ita) {
				CMyPoint* pt = *ita;
				if (pt) {
					delete pt;
					pt = NULL;
				}
		}
		pts.erase(pts.begin(), pts.begin() + index2);	
		int a=1;
	}
}
//�㵽�߶ε���̾���
double CGeoPolyline::MinsPointToSeg(CMyPoint * pt, CMyPoint * ptOri, CMyPoint * ptEnd)
{
	double d = 0;
	double a = getTwoPointDis(pt->GetCPoint(), ptOri->GetCPoint());
	double b = getTwoPointDis(pt->GetCPoint(), ptEnd->GetCPoint());
	double c = getTwoPointDis(ptOri->GetCPoint(), ptEnd->GetCPoint());
	double oriAngle = (a*a + c*c - b*b) / (2 * a*c);
	double endAngle = (b*b + c*c - a*a) / (2 * b*c);
	if (oriAngle <= 0) {
		d = a;
	}
	else if (endAngle <= 0) {
		d = b;
	}
	else {
		double p = (a + b + c) / 2;
		double s = sqrt(p*(p - a)*(p - b)*(p - c));
		/*s = 0.5*(pt->Getx()*ptOri->Gety() + ptOri->Getx()*ptEnd->Gety() + ptEnd->Getx()*pt->Gety() -
			pt->Getx()*ptEnd->Gety() - ptOri->Getx()*pt->Gety() - ptEnd->Getx()*ptOri->Gety());*/
		d = 2 * s / c;
	}
	return d;
}

//double CGeoPolyline::distanceBetweenTwoPoints(CMyPoint * pt1, CMyPoint * pt2)
//{
//	double A = (double)pt1->Getx() - (double)pt2->Getx();
//	double B = (double)pt1->Gety() - (double)pt2->Gety();
//	return sqrt(A*A + B*B);
//}

int CGeoPolyline::getMaxArcLength(vector<CMyPoint*> pts, int start, int end, double & maxLength)
{
	int numPoint = 0;
	double dee = 0;
	maxLength = 0;
	CMyPoint* ptOri = pts[start];
	CMyPoint* ptEnd = pts[end];
	int size = pts.size();
	for (int i =start+1; i < end; i++) {
		dee = MinsPointToSeg(pts[i], ptOri, ptEnd);
		if (maxLength < dee) {
			maxLength = dee;
			numPoint = i;
		}
	}
	return numPoint;
}

int CGeoPolyline::getFarestPointNum(vector<CMyPoint*> pts)
{
	int pointNum = 0;
	double length = 0;
	double d = 0;
	int size = pts.size();
	for (int i = 1; i < size - 1; i++) {
		d = getTwoPointDis(pts[0]->GetCPoint(), pts[i]->GetCPoint());
		if (d > length) {
			length = d;
			pointNum = i;
		}
	}
	return pointNum;
}

void CGeoPolyline::DouglasPeuckerDataCompress(double tolerance)
{
	vector<CMyPoint*> pts1;
	CUIntArray  aPtNum;
	DouglasPeuckerDataCompress(this->pts, aPtNum, tolerance);
	for (int i = 0; i < aPtNum.GetSize(); i++) {
		int n = aPtNum[i];
		CMyPoint* point = pts[n];//����ᵼ�¶˵�ĵ�ַ��һ��
		pts1.push_back(point);
	}
	pts.erase(pts.begin(),pts.end());
	this->pts.resize(pts1.size());
	this->pts = pts1;
}


void CGeoPolyline::DouglasPeuckerDataCompress(vector<CMyPoint*> pts, CUIntArray & aPtNum, double tolerance)
{
	CUIntArray NumPoint;
	aPtNum.RemoveAll();
	ColInfoArray aStack;
	COLINFO from_to;
	vector<int> aaa;
	vector<COLINFO> bbb;

	double deepth;
	int nSt, nEd;
	int i, j, tempNum;
	nSt = 0;
	nEd = pts.size()-1;
	NumPoint.Add(nSt);
	aaa.push_back(nSt);
	NumPoint.Add(nEd);
	aaa.push_back(nEd);
	CPoint pt1 = pts[0]->GetCPoint();
	CPoint pt2 = pts.back()->GetCPoint();
	if (pt1==pt2) {
		int size = pts.size();
		if (size== 4) {
			NumPoint.Add(1);
			NumPoint.Add(2);
			goto loop;
		}
		else {
			tempNum = getFarestPointNum(pts);
			NumPoint.Add(tempNum);
			aaa.push_back(tempNum);
			double maxLength1, maxLength2, tempNum1, tempNum2;
			tempNum1 = getMaxArcLength(pts, nSt, tempNum, maxLength1);
			tempNum2 = getMaxArcLength(pts, tempNum, nEd, maxLength2);
			if (maxLength1 < maxLength2) {
				NumPoint.Add(tempNum2);
				aaa.push_back(tempNum2);
				from_to.from = tempNum2;
				from_to.to = nEd;
				aStack.Add(from_to);
				bbb.push_back(from_to);
				from_to.from = tempNum;
				from_to.to = tempNum2;
				aStack.Add(from_to);
				bbb.push_back(from_to);
				from_to.from = nSt;
				from_to.to = tempNum;
				//aStack.Add(from_to);
				//bbb.push_back(from_to);
				nEd = tempNum;
			}
			else {
				NumPoint.Add(tempNum1);
				aaa.push_back(tempNum1);
				from_to.from = tempNum;
				from_to.to = nEd;
				aStack.Add(from_to);
				bbb.push_back(from_to);
				from_to.from = tempNum1;
				from_to.to = tempNum;
				aStack.Add(from_to);
				bbb.push_back(from_to);
				from_to.from = nSt;
				from_to.to = tempNum1;
				//aStack.Add(from_to);
				//bbb.push_back(from_to);
				nEd = tempNum1;
			}
			
		}
	}
	
		BOOL flag = TRUE;
		do
		{
			if (nEd - nSt > 1) {
				tempNum = getMaxArcLength(pts, nSt, nEd, deepth);
			}
			else deepth = 0;
			
			if (deepth>tolerance)
			{
				NumPoint.Add(tempNum);
				aaa.push_back(tempNum);
				from_to.from = tempNum;
				from_to.to = nEd;
				aStack.Add(from_to);
				bbb.push_back(from_to);
				nEd = tempNum;
			}
			else
			{
				int numStack = aStack.GetSize();
				if (numStack == 0)
					flag = FALSE;
				else
				{
					from_to = aStack.GetAt(numStack - 1);
					nSt = from_to.from;
					nEd = from_to.to;
					aStack.RemoveAt(numStack - 1);
				}
			}

		} while (flag);
	
	loop:
	int numPt = NumPoint.GetSize();
	int minNum, k;
	for (j = 0; j<numPt; j++)
	{
		minNum = NumPoint.GetAt(0);
		k = 0;
		for (i = 1; i<NumPoint.GetSize(); i++)
		{
			if (minNum>(int)NumPoint.GetAt(i))
			{
				minNum = NumPoint.GetAt(i);
				k = i;
			}
		}
		aPtNum.Add(minNum);
		if (k<NumPoint.GetSize())
		{
			NumPoint.RemoveAt(k);
		}

	}
	
	int f = 1;
	for (int k = 0; k < aaa.size(); k++) {
		aaa[k] = aPtNum.GetAt(k);
	}
	int a = aPtNum.GetAt(0);
	int b = aPtNum.GetAt(1);
}

bool CGeoPolyline::JudgeCanDeleted()
{
	bool temp = false;
	if (this->leftAttribute==this->rightAttribute) {
		temp = true;
	}
	this->canDeleted = temp;
	return temp;
}


void CGeoPolyline::setPoint(int index, CMyPoint* mypoint)
{
	pts[index] = mypoint;
}


// �õ�ĳ�����ĳ���
double CGeoPolyline::getLineLength()
{
	if (isNULL) {
		return 0;
	}

	int size = pts.size();
	int j = 0;
	double d = 0;
	double l = 0;
	for (int i = 0; i < size-1; ) {
		
		j = i + 1;
		if (pts[j]->isNULL) {
			for (int m = j; m < size; m++) {
				if (!pts[m]->isNULL) {
					j = m;
					break;
				}
				if (m == size-1&&pts[size-1]->isNULL) j = -1;
			}
		}
		if (j == -1) break;
		l = getTwoPointDis(pts[i]->GetCPoint(), pts[j]->GetCPoint());
		d = d + l;
		i = j;
	}
	return d;
}


// �õ�һ������һ�����ļн�
double CGeoPolyline::getAnglePtToLine(CPoint pt)
{
	int length = pts.size();
	double sumAngle = 0;
	for(int i = 0;i<length-1;i++){
		CPoint pt1 = pts[i]->GetCPoint();
		CPoint pt2 = pts[i+1]->GetCPoint();
		double angle = getAngle(pt1,pt2,pt);
		sumAngle+=angle;
	}
	return sumAngle;
}


// pt1��ʾ�߶���㣬pt2��ʾ�߶��յ㣬pt��ʾ�����,���ص��Ǧ���
//double CGeoPolyline::getAngle(CPoint pt1, CPoint pt2, CPoint pt)
//{
//	int i=0;
//	CPoint pt22;//pt��pt2������
//	pt22.SetPoint((pt2.x-pt.x)/100.0,(pt2.y-pt.y)/100.0);
//	CPoint pt11;//pt��pt1������
//	pt11.SetPoint((pt1.x-pt.x)/100.0,(pt1.y-pt.y)/100.0);
//	
//	double chaji =pt22.x*pt11.y - pt22.y*pt11.x;
//	if(chaji>0)
//		i=1;
//	else if(chaji<0)
//		i=-1;
//	else
//		return 0;
//	double a = ((pt2.x-pt.x)/100.0)*((pt2.x-pt.x)/100.0)+((pt2.y-pt.y)/100.0)*((pt2.y-pt.y)/100.0);//pt2-pt
//	a=sqrt(a);
//	double b = ((pt2.x-pt1.x)/100.0)*((pt2.x-pt1.x)/100.0)+((pt2.y-pt1.y)/100.0)*((pt2.y-pt1.y)/100.0);//pt1-pt2
//	b=sqrt(b);
//	double c = ((pt1.x-pt.x)/100.0)*((pt1.x-pt.x)/100.0)+((pt1.y-pt.y)/100.0)*((pt1.y-pt.y)/100);//pt1-pt
//	c=sqrt(c);
//	double d = (a*a+c*c-b*b)/(2*a*c);
//	if(d>1){
//		d=1;
//		if(d-1>0.0001)
//			AfxMessageBox("�Ƕȳ��޹���");
//	}
//	else if(d<-1)
//		d=-1;
//	double e = acos(d);
//	if(_isnan(e)){
//		AfxMessageBox("������NaN");
//	}
//	e =i* e;
//	return e;
//	
//}


// pt1��ʾ�߶���㣬pt2��ʾ�߶��յ㣬pt��ʾ����㣬�ж�pt1��pt��pt2���ߵ����Ҳ࣬��෵��-1���Ҳ෵��1�����Ϸ���0
int CGeoPolyline::getPtLRtoLine(CPoint pt1, CPoint pt2, CPoint pt)
{
	//CPoint pt22;//pt��pt2������
	//pt22.SetPoint((pt2.x-pt.x)/100.0,(pt2.y-pt.y)/100.0);
	//CPoint pt11;//pt��pt1������
	//pt11.SetPoint((pt1.x-pt.x)/100.0,(pt1.y-pt.y)/100.0);
	//double chaji =pt22.x*pt11.y - pt22.y*pt11.x;
	return 0;
}


//�Ƴ�����ε�����㣬angleΪ0-20�㣬������С��angleͬʱ�����߼�������С��area������ɾ���ö��㣬ͬʱ��ʾ�������ݣ�Ҫע��˵㲻��ɾ
void CGeoPolyline::buildLineSimple(double angle, double area,double length)
{
	int ptNum = this->pts.size();
	if (ptNum==2)
	{
		if (getTwoPointDis(pts[0]->GetCPoint(),pts[1]->GetCPoint())<=length)
		{
			CMyPoint** tempPt = &pts[0];
			*tempPt = pts[1];
			//�������Ϊһ��
		}else{
			return;
		}
	}
	if (ptNum==3)
	{
		double angleB = getAngle(pts[0]->GetCPoint(),pts[2]->GetCPoint(),pts[1]->GetCPoint());
		
		double minAngle  = getMod(angleB,M_PI);
		double l1 = getTwoPointDis(pts[0]->GetCPoint(),pts[1]->GetCPoint());
		double l2 = getTwoPointDis(pts[1]->GetCPoint(),pts[2]->GetCPoint());
		double angleTemp = minAngle*180/M_PI;
		if (angleTemp<angle||l1<length||l2<length)//����нǽǶȹ�����߹�С���߱߳�̫С
		{
			double areaTemp = getTriArea(pts[0]->GetCPoint(),pts[2]->GetCPoint(),pts[1]->GetCPoint());
			if (areaTemp<=area)
			{
				pts[1]->isNULL = true;
			} 
			
		}
	}

	if (ptNum>3)//����Ҫ�жϼнǣ�ͬʱҪ����ȥС�ߴ���
	{
		int oriValidPtNum = 0;
		for(int m =1;m<ptNum-1;m++){
			if (getPrevalidPtNum(m)!=0)
			{
				break;
			}
			int nextNum = getNextvalidPtNum(m);
			double angleC = getAngle(pts[0]->GetCPoint(),pts[nextNum]->GetCPoint(),pts[m]->GetCPoint());
			double s01 = getTwoPointDis(pts[0]->GetCPoint(),pts[m]->GetCPoint());
			double angleTemp1 =angleC*180/M_PI; 
			double minAngle = getMod(angleTemp1,180);
			if (minAngle<angle||s01<length)
			{
				double areaTemp = getTriArea(pts[m]->GetCPoint(),pts[nextNum]->GetCPoint(),pts[0]->GetCPoint());
				if (areaTemp<=area)
				{
					pts[m]->isNULL = true;
				} 
				else{
					oriValidPtNum = m;
					break;
				}
			}else{
				oriValidPtNum = m;
				break;
			}
		}

		for (int i = oriValidPtNum+1;i<ptNum-1;i++)
		{
			if (pts[i]->isNULL)
			{
				continue;
			}
			int preValidNum = getPrevalidPtNum(i);
			int nextNum = getNextvalidPtNum(i);
			double angleD = getAngle(pts[preValidNum]->GetCPoint(),pts[nextNum]->GetCPoint(),pts[i]->GetCPoint());
			double sxy = getTwoPointDis(pts[preValidNum]->GetCPoint(),pts[i]->GetCPoint());
			double angleTemp1 =angleD*180/M_PI; 
			double minAngle = getMod(angleTemp1,180);
			if (minAngle<angle)//Ҫ�ҳ���һ������isNull�ĵ�
			{
				double areaTemp = getTriArea(pts[preValidNum]->GetCPoint(),pts[nextNum]->GetCPoint(),pts[i]->GetCPoint());
				if (areaTemp<area)
				{
					pts[i]->isNULL = true;
					continue;//���ɾ���õ��������ѭ��
				}
			}
			if (sxy<length)
			{
				int prepreNum = getPrevalidPtNum(preValidNum);
				this->buildFourptDelshortline(pts[prepreNum],pts[preValidNum],pts[i],pts[nextNum]);
			}
		}
		int preEndNum = getPrevalidPtNum(ptNum-1);//�յ�ǰ����Ч��
		double endL = getTwoPointDis(pts[ptNum-1]->GetCPoint(),pts[preEndNum]->GetCPoint());
		if (endL<length)
		{
			int prepreEndNum = getPrevalidPtNum(preEndNum);//�յ�ǰ�ĵ�����������Ч��
			double areaTemp = getTriArea(pts[ptNum-1]->GetCPoint(),pts[preEndNum]->GetCPoint(),pts[prepreEndNum]->GetCPoint());
			if (areaTemp<=area)
			{
				pts[preEndNum]->isNULL = true;
			} 
		}
	}
}




// �������ڽ��ĵ�ȥ�̱ߴ���
void CGeoPolyline::buildFourptDelshortline(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4)
{
	int style = getStyleFourptsUnit(pt1->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint(),pt4->GetCPoint());
	if (style==1)//ͬ�����U
	{
		this->buildFourptDelShortlineU(pt1,pt2,pt3,pt4);
	}else if (style==-1)
	{
		this->buildFourptDelShortlineZ(pt1,pt2,pt3,pt4);
	}
}


// �������ڽ��ĵ�Z��ȥ�̱ߴ���
void CGeoPolyline::buildFourptDelShortlineZ(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4)
{
	double l1 = MinsPointToLine(pt1->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint());
	double l2 = MinsPointToLine(pt4->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint());
	double x = pt2->Getx()+(pt3->Getx()-pt2->Getx())*l2/(l1+l2);//pt2-pt3�ϵĵ㣬��������������
	double y = pt2->Gety()+(pt3->Gety()-pt2->Gety())*l2/(l1+l2);
	double areaTemp1 = getTriArea(pt1->GetCPoint(),pt2->GetCPoint(),CPoint(x,y));
	double areaTemp2 = getTriArea(pt3->GetCPoint(),pt4->GetCPoint(),CPoint(x,y));
	if (abs(areaTemp1-areaTemp2)>10)
	{
		AfxMessageBox("�������Ȱ�");
	}else{
		pt3->isNULL = true;
		pt2->setPoint(CPoint(x,y));
	}

}


// �������ڽ��ĵ�U��ȥ�̱ߴ���
void CGeoPolyline::buildFourptDelShortlineU(CMyPoint* pt1, CMyPoint* pt2, CMyPoint* pt3, CMyPoint* pt4)
{
	double x1 = pt2->Getx()-pt1->Getx();
	double y1 = pt2->Gety()-pt1->Gety();
	double x2 = pt4->Getx()-pt3->Getx();
	double y2 = pt4->Gety()-pt3->Gety();
	double s1 = getTwoPointDis(pt1->GetCPoint(),pt2->GetCPoint());
	double s2 = getTwoPointDis(pt3->GetCPoint(),pt4->GetCPoint());
	double ab = x1*x2+y1*y2;//�����˻�
	double cosAngle = ab/(s1*s2);
	double angle = acos(cosAngle);
	bool isBottleNeckFourPt = isBottleNeck(pt1->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint(),pt4->GetCPoint());
	if (isBottleNeckFourPt)//��ͨ���㵽ֱ�ߵľ����ж��Ƿ���ƿ����
	{
		AfxMessageBox("�ڽ��ĵ���U�������ƿ����");
		pt2->isNULL = true;
		pt3->isNULL = true;
	}else if (angle<=90)//�ӳ��߽���,Ҫ����ƽ�е�����
	{
		SegMent segMent1(pt1->GetCPoint(),pt2->GetCPoint());
		SegMent segMent2(pt3->GetCPoint(),pt4->GetCPoint());
		CPoint tempPt = getLineCrossPt(segMent1,segMent2);
		if (tempPt==CPoint(0,0))
		{
			AfxMessageBox("������ֱ��");
		}else{//����180��ƽ��
			pt3->isNULL = true;
			pt2->setPoint(tempPt);
		}
	} 
	else
	{
		double s123 = MinsPointToLine(pt1->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint());
		double s423 = MinsPointToLine(pt4->GetCPoint(),pt2->GetCPoint(),pt3->GetCPoint());
		if (s123<=s423)
		{
			pt2->isNULL = true;
		} 
		else
		{
			pt3->isNULL = true;
		}
	}
	

}


// �ж��ĵ��Ƿ���ƿ����
bool CGeoPolyline::isBottleNeck(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4)
{
	double s123 = MinsPointToLine(pt1,pt2,pt3);
	double s423 = MinsPointToLine(pt4,pt2,pt3);
	if (s123<=s423)//˵��pt1��pt2-pt3��һЩ
	{
		double s134 = MinsPointToLine(pt1,pt3,pt4);
		double s234 = MinsPointToLine(pt2,pt3,pt4);
		if (s134<s234)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}else{
		double s312 = MinsPointToLine(pt3,pt1,pt2);
		double s412 = MinsPointToLine(pt4,pt1,pt2);
		if (s412<s312)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}
}


// �õ����е�i��ǰ��i�������Ч���±�
int CGeoPolyline::getPrevalidPtNum(int x)
{
	for (int i =x-1;i>=0;i--)
	{
		if (!pts[i]->isNULL)
		{
			return i;
		}
	}
	return 0;
}


// �õ����е�x����һ����Ч����±�
int CGeoPolyline::getNextvalidPtNum(int x)
{
	for (int i =x+1;i<pts.size();i++)
	{
		if (!pts[i]->isNULL)
		{
			return i;
		}
	}
}


 //��MyPoint��������תΪCPoint����
bool CGeoPolyline::MyPointsToCPoints(vector<CMyPoint*> pts, CPointArray &points)
{
	//vector<CPoint> points;
	int size = pts.size();
	for (int i = 0;i<size;i++)
	{
		CPoint pt = pts[i]->pt;
		points.Add(CPoint(pt));
	}
	return 1;
}




//zf,����ĳ���������
double CGeoPolyline::getArea(vector<CMyPoint*> pts)
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


// zf.��ȡ��СY
double CGeoPolyline::getMinY(vector<CMyPoint*> pts)
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


// zf,0623,������
void CGeoPolyline::ObjectFilter(void)
{
	AfxMessageBox("��   ����");
}

// zf�����ˣ�0701
void CGeoPolyline::ObjectFilterByArea(void)
{
}


// Ѱ��һ�����Ӿ��Σ�zf��0704
void CGeoPolyline::FindRectangle(void)
{

	//AfxMessageBox(" Ѱ����Ӿ��� ");
}


// ��CMyPoint����ת��ΪCPoint��,zf,0708
void CGeoPolyline::MyPoint2Point(vector<CMyPoint*> pts)
{
	if(odinPts.size() != 0)
		vector<CPoint*>().swap(odinPts);
	//int size = pts.size();
	for(int i = 0;i<pts.size();i++)
	{
		CPoint* temp = pts[i]->MyPoint2Point(pts[i]);
		odinPts.push_back(temp);
	}
}


// �趨�������ģ�zf��0712
void CGeoPolyline::setCenter(CPoint* pt)
{
	center.x = pt->x;
	center.y = pt->y;
}


// ���������ƽ�����룬zf,1003
double CGeoPolyline::calAvgDis(vector<CMyPoint*> pts)
{
	double distance = 0;
	for(int i = 0 ; i < (pts.size()-1) ; i++)
		distance = distance + calDis(pts[i],pts[i+1]);
	return (distance/(pts.size()-1));
}


// �����������룬zf��1003
double CGeoPolyline::calDis(CMyPoint* pt1, CMyPoint* pt2)
{
	double deltaX = pt1->Getx() - pt2->Getx();
	double deltaY = pt1->Gety() - pt2->Gety();
	return sqrt(deltaX*deltaX+deltaY*deltaY);
}
