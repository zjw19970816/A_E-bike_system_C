#pragma once
#include "CNode.h"
#include<vector>
using namespace std;

class COrigin
{
public:
	int ID; // 起点的编号，从零开始编号 (起点的number)
	CNode* pOriginNode; // 起点对应的节点 （起点是一个CNode的类）
	vector<int> DestinationNode; // OD对，只记录有需求的OD (记录该起点对应的所有终点)
	vector<double> ODDemand; // OD需求，只记录有需求的OD （记录OD demand）
	vector<int> ODpairNumber; // OD pair的number
};

