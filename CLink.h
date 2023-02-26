#pragma once
#include "CNode.h"

class CLink
{
public:
	int ID; // 路段的编号，从零开始编号 （link的number）
	CNode* pInNode; // 路段的起节点 （link起点，起点是一个CNode的类）
	CNode* pOutNode; // 路段的终节点 （link终点）
	double LinkDistance; // Free Flow Travel time
};

