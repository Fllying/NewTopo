#pragma once
#include<vector>
class MABR_ZF
{
public:
	MABR_ZF(void);
	~MABR_ZF(void);
	// ����ε���ţ�0708��zf
	int ID;
	// ����ε������zf��0708
	double area;

	// ����εĶ��㣬zf��0708
	vector<CPoint*> vertices;
};

