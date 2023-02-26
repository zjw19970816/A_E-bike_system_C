#include "CNetwork.h"
#include<iostream>
#include <fstream>
#include<math.h>
using namespace std;

////4 nodes的network
//int node_type_upper_level[4][3] = { {1, -1, -1}, {-1, 2, -1}, {-1, -1, 3}, {1, -1, -1 } }; //对下层模型来说，上层生成的node_type是constant
//double node_capacity_upper_level[4][3] = { {10,INF,INF},{INF,10,INF}, {INF,INF,INF}, {10,INF,INF} }; //对下层模型来说，上层生成的node_capacity是constant

////13 nodes的network
//int node_type_upper_level[13][3] = { {-1, -1, 3}, {1, -1, -1}, {-1, -1, 3}, {-1, -1, 3}, {-1, 2, -1}, {1, 2, -1}, {-1, -1, 3}, {-1, -1, 3}, {-1, -1, 3}, {-1, -1, 3}, {1, 2, -1}, {1, -1, -1}, {-1, 2, -1} }; //对下层模型来说，上层生成的node_type是constant
//double node_capacity_upper_level[13][3] = { {INF,INF,INF}, {10,INF,INF}, {INF,INF,INF}, {INF,INF,INF}, {INF,10,INF}, {10,10,INF}, {INF,INF,INF}, {INF,INF,INF}, {INF,INF,INF},{INF,INF,INF}, {10,10,INF}, {10,INF,INF}, {INF,10,INF} }; //对下层模型来说，上层生成的node_capacity是constant

//double FreeflowSpeed = 20;
//double Capacity = 50;

//读Node information
void CNetwork::ReadNode(string DataPath)
{
	ifstream in(DataPath); //ifstream 读文件 in() 读文件
	string row;
	vector<string> Data;

	m_Node.clear();//初始化  vector<CNode*> m_Node; //网络节点集合
	m_nNode = 0;//初始化 节点数

	if (in) //找到 "DataPath" 文件
	{
		while (getline(in, row)) //按一整行的方式读取，存储在string row中
		{
			if (row.empty())
			{
				continue;
			}
			Data = split(row, "\t"); //将row中的东西按照 (制表符) 分割开，row是字符串，split函数是在CNetwork中自己设计的，它返回一个vector<string>数据。
			CNode* pNode = new CNode; //创建一个新的CNode的类，用指针pNode表示
			pNode->ID = m_nNode;
			pNode->PositionX = atof(Data[1].c_str()); //将string转换成double的数据类型
			pNode->PositionY = atof(Data[2].c_str());
			pNode->Incentive = incentive;
			pNode->Penalty = penalty;
			m_nNode++;
			m_Node.push_back(pNode);
		}
		in.close();
	}
	else //没找到 "DataPath" 文件
	{
		cout << DataPath << " does not exist!";
	}
}

//读Depot information
void CNetwork::ReadDepot(string DataPath)
{
	ifstream in(DataPath); //ifstream 读文件 in() 读文件
	string row;
	vector<string> Data;

	m_Depot.clear();//初始化  vector<CNode*> m_Depot; //网络节点集合
	m_nDepot = 0;//初始化 节点数

	if (in) //找到 "DataPath" 文件
	{
		while (getline(in, row)) //按一整行的方式读取，存储在string row中
		{
			if (row.empty())
			{
				continue;
			}
			Data = split(row, "\t"); //将row中的东西按照 (制表符) 分割开，row是字符串，split函数是在CNetwork中自己设计的，它返回一个vector<string>数据。
			CNode* pDepot = new CNode; //创建一个新的CNode的类，用指针pNode表示
			pDepot->ID = m_nDepot;
			pDepot->PositionX = atof(Data[1].c_str()); //将string转换成double的数据类型
			pDepot->PositionY = atof(Data[2].c_str());
			pDepot->Incentive = incentive;
			pDepot->Penalty = penalty;
			m_nDepot++;
			m_Depot.push_back(pDepot);
		}
		in.close();
	}
	else //没找到 "DataPath" 文件
	{
		cout << DataPath << " does not exist!";
	}
}

//读Link information
void CNetwork::ReadLink(string DataPath)
{
	ifstream in(DataPath);
	string row;
	vector<string> Data;

	m_Link.clear();
	m_nLink = 0;

	if (in)
	{
		while (getline(in, row))
		{
			if (row.empty())
			{
				continue;
			}
			Data = split(row, "\t");
			CLink* pLink = new CLink();

			pLink->ID = m_nLink;
			auto inNodeIndex = atoi(Data[0].c_str()) - 1; //auto是自动数据类型 
			pLink->pInNode = m_Node[inNodeIndex]; //用为在上边已经将所有的node都读取了，这里用node number直接对应上link的起点node
			auto outNodeIndex = atoi(Data[1].c_str()) - 1;
			pLink->pOutNode = m_Node[outNodeIndex];
			//pLink->FreeFlowTravelTime = atof(Data[2].c_str()); //string转double
			pLink->LinkDistance = atof(Data[2].c_str()); //string转double
			//pLink->Capacity = atof(Data[3].c_str());
			pLink->pInNode->OutgoingLink.push_back(pLink->ID); //对一条link编号为A，例如（1, 2），对于node1，它是outgoinglink，对于node2，它是Incominglink
			pLink->pOutNode->IncomingLink.push_back(pLink->ID);
			m_nLink++;
			m_Link.push_back(pLink);
		}

		in.close();
	}
	else
	{
		cout << DataPath << " does not exist!";
	}
}

//读Link information
void CNetwork::ReadDistance(string DataPath)
{
	ifstream in(DataPath);
	string row;
	vector<string> Data;

	m_DepotNode.clear();
	m_nDepotNode = 0;

	if (in)
	{
		while (getline(in, row))
		{
			if (row.empty())
			{
				continue;
			}
			Data = split(row, "\t");
			CLink* pDepotNode = new CLink();

			pDepotNode->ID = m_nDepotNode;
			auto inNodeIndex = atoi(Data[0].c_str()) - 1; //auto是自动数据类型 
			pDepotNode->pInNode = m_Depot[inNodeIndex]; //用为在上边已经将所有的node都读取了，这里用node number直接对应上link的起点node
			auto outNodeIndex = atoi(Data[1].c_str()) - 1;
			pDepotNode->pOutNode = m_Node[outNodeIndex];
			//pLink->FreeFlowTravelTime = atof(Data[2].c_str()); //string转double
			pDepotNode->LinkDistance = atof(Data[2].c_str()); //string转double
			//pLink->Capacity = atof(Data[3].c_str());
			pDepotNode->pInNode->OutgoingLink.push_back(pDepotNode->ID); //对一条link编号为A，例如（1, 2），对于node1，它是outgoinglink，对于node2，它是Incominglink
			pDepotNode->pOutNode->IncomingLink.push_back(pDepotNode->ID);
			m_nDepotNode++;
			m_DepotNode.push_back(pDepotNode);
		}
		in.close();
	}
	else
	{
		cout << DataPath << " does not exist!";
	}
}

//读O/D demand information
void CNetwork::ReadODpairs(string DataPath)
{
	ifstream in(DataPath);
	string row;
	vector<string> Data;

	m_Origin.clear();
	m_nOrigin = 0;
	m_nODpair = 0;
	COrigin* pOrigin;
	CNode* pNode;

	if (in)
	{
		while (getline(in, row))
		{
			if (row.empty()) continue;
			Data = split(row, "\t");
			auto Origin_node = atoi(Data[0].c_str()) - 1;
			pNode = m_Node[Origin_node]; //起点node还是用编号来直接对应
			if (pNode->Origin_ID == -1) //若该点标识符不是起点，一开始默认每个点都不是起点
			{
				pOrigin = new COrigin();//对于新的起点node, 创建一个COrigin类
				pOrigin->ID = m_nOrigin;
				m_nOrigin++;
				pOrigin->pOriginNode = m_Node[Origin_node];
				//pOrigin->ID = m_Origin.size();
				//pOrigin->pOriginNode =  m_Node[atoi(Data[0].c_str()) - 1];
				pNode->Origin_ID = pOrigin->ID;//将标识符Origin_ID赋值为起点的ID
				m_Origin.push_back(pOrigin);
			}
			else
			{
				pOrigin = m_Origin[m_Node[Origin_node]->Origin_ID];
			}
			auto Destination_node = atoi(Data[1].c_str()) - 1;
			pNode = m_Node[Destination_node];
			pOrigin->DestinationNode.push_back(pNode->ID); //放的是destination node的number
			double demand = atof(Data[2].c_str());
			pOrigin->ODDemand.push_back(demand);
			pOrigin->ODpairNumber.push_back(m_nODpair);
			m_nODpair++;
			//cout << m_nODpair << endl;
		}

		in.close();
	}
	else
	{
		cout << DataPath << " does not exist!";
	}
}

//Upper-level枚举node的type
void CNetwork::GeneratePossible(int n)
{
	if (n == 0)
	{
		//cout << str << ", ";
		//cout << endl;
		int QAQ = stoi(str);
		vector<int> temp;
		while (QAQ)
		{
			int u = QAQ % 10;
			temp.push_back(u);
			QAQ = QAQ / 10;
		}
		CSolution* pSolution = new CSolution;
		pSolution->ID = m_nSolution;
		//cout << " temp.size(): " << temp.size() << endl;
		for (int size = 0; size < temp.size(); size++)
		{
			vector<int> node_type_temp;
			vector<double> capacity_type_temp;
			for (int type = 0; type < 3; type++)
			{
				node_type_temp.push_back(node_type[temp[size] - 1][type]);
				if (node_type[temp[size] - 1][type] == -1 || node_type[temp[size] - 1][type] == 3) //没开=-1或者=3（non-parking node），capacity = 0
				{
					capacity_type_temp.push_back(0);
				}
				if (node_type[temp[size] - 1][type] == 1 || node_type[temp[size] - 1][type] == 2) //开是1（parking）或者2（charging），capacity随机一个值
				{
					//double capacity_temp = (rand() % (50 - 0)) + 0 + 1; //capacity随机生成(0, 50]
					int capacity_temp = (rand() % (CapUpper - CapLower + 1)) + CapLower; // capacity随机生成[5, 15]
					capacity_type_temp.push_back(capacity_temp);
				}
			}
			//cout << endl;
			pSolution->node_type_upper_level.push_back(node_type_temp);
			pSolution->node_capacity_upper_level.push_back(capacity_type_temp);
		}
		m_Solution.push_back(pSolution);
		m_nSolution++;
		str.erase(str.size() - 1); // erase(position) will delete all the characters after the specified position.
		return;
		//continue;
	}

	for (int i = 1; i < 5; i++)  //i = 1, 2, 3, 4
	{
		str = str + to_string(i); //to_string，将int i转换为string数值
		GeneratePossible(n - 1);
	}

	if (str.size() != 0)
	{
		str.erase(str.size() - 1);
	}
}
void CNetwork::EnumerationNodeType()
{
	m_Solution.clear();//初始化 vector<CSolution*> m_Solution; //网络节点集合
	m_nSolution = 0;

	GeneratePossible(m_nNode);
}

//Shortest path algorithm for link information
//返回一个vector 包含link number
double CNetwork::Dijstra(int Start, int End)
{
	CNode* pNode;
	CLink* pLink;
	// 初始化
	int startposition = 0;//两个用来判断while的标识符
	int endposition = 1;
	ShortestPathCost = new double[m_nNode]; //Cost
	ShortestPathParent = new int[m_nNode]; //前驱节点
	int* checkList = new int[m_nNode];//队列，循环使用
	//cout << (&checkList)[3] << endl;
	bool* binCheckList = new bool[m_nNode]; //是否在队列中
	bool* bscanStatus = new bool[m_nNode];

	for (int node = 0; node < m_nNode; node++)
	{
		ShortestPathCost[node] = INF;//初始每个点的cost都设为无穷
		ShortestPathParent[node] = -1;//每一个点都标记-1
		//ShortestPathCost[node] = numeric_limits<double>::max(); //好像是将inf给扩大
		//cout << ShortestPathCost[node] << endl;	
		binCheckList[node] = false;//所有的node标记为false
	}
	ShortestPathCost[Start] = 0;//初始起始点的cost为0
	checkList[0] = Start; //PList

	//开始while循环  这里主要是计算ShortestPathCost，记录以start点，到网络中所有点的最短travel cost。
	while (startposition != endposition)
	{
		if (startposition >= m_nNode)
		{
			startposition = 0;
		}
		int i = checkList[startposition]; //起点number
		startposition++; //1, 2
		pNode = m_Node[i]; //起点给pNode, (当前点的class，给pNode)
		for (int index = 0; index < pNode->OutgoingLink.size(); index++) //index: 离开路段的个数 OutgoingLink.size()指有多少个以pNode为起点的link
		{
			pLink = m_Link[pNode->OutgoingLink[index]];//起点连接的所有link，给pLink。对于i点，连接它的link
			int j = pLink->pOutNode->ID; //与起点相连的link的终点， （j: link的终点）
			double value = pLink->LinkDistance;//（value: Link travel time）
			if (ShortestPathCost[j] > ShortestPathCost[i] + value) //INF > 0 + link travel time
			{
				ShortestPathCost[j] = ShortestPathCost[i] + value;
				ShortestPathParent[j] = i;//记录一下j前边是i
				// 添加到队列尾部
				if (endposition >= m_nNode)
				{
					endposition = 0;
				}
				checkList[endposition] = j;
				endposition++; //2, 3
				bscanStatus[j] = true;
			}
		}
	}

	return ShortestPathCost[End];
}

//计算任何两点之间的距离
void CNetwork::CalNodeDistance()
{
	NodeDistance = new double* [m_nNode]; //任何两点的distance
	for (int node = 0; node < m_nNode; node++)
	{
		NodeDistance[node] = new double[m_nNode];
	}
	CNode* pNode1;
	CNode* pNode2;
	for (int node1 = 0; node1 < m_nNode; node1++)
	{
		pNode1 = m_Node[node1];
		int Startnode = pNode1->ID;
		for (int node2 = 0; node2 < m_nNode; node2++)
		{
			pNode2 = m_Node[node2];
			int Endnode = pNode2->ID;
			double distance = Dijstra(Startnode, Endnode);
			NodeDistance[Startnode][Endnode] = distance;
		}
	}
}

//根据上层给node type，capacity进行标识
void CNetwork::NodeType(int sol, vector<CSolution*> m_Solution)
{
	//测试4 node
	for (int node = 0; node < m_nNode; node++)
	{
		//m_Node[node]->Node_type = node_type_upper_level[node];
		for (int type = 0; type < 3; type++)
		{
			m_Node[node]->NodeType[type] = m_Solution[sol]->node_type_upper_level[node][type];
			m_Node[node]->Capacity[type] = m_Solution[sol]->node_capacity_upper_level[node][type];
			//m_Node[node]->Capacity[type] = 10;
		}
	}
}

//根据RouteFlow情况更新Route cost，同时也更新对应的PickupNode和DropoffNode的flow
//需要增加判断node type，以及考虑non-parking node和charging的奖励和惩罚，还有non-parking node不用queue，因为会导致计算错误
void CNetwork::UpdateRouteCost(double* RouteFlow)
{
	//计算zero flow情况下，每条route的route cost
	for (int route = 0; route < m_nRoute; route++)
	{
		//if (m_Route[route]->DropoffNode->Incentive != 5)
		//{
		//	cout << "Hellow worldHellow worldHellow world: " << m_Route[route]->DropoffNode->Incentive << endl;
		//}
		//cout << m_Route[route]->PickupNode->ID << " " << m_Route[route]->DropoffNode->ID << endl;
		// 对于所有的route，在当前的flow condition下计算它的route cost
		double PickupWalkingtimeCost = ValueOfWalkingTime * m_Route[route]->Pickupdistance / WalkingSpeed;
		double DropoffWalkingtimeCost = ValueOfWalkingTime * m_Route[route]->Dropoffdistance / WalkingSpeed;
		double RouteFlowTemp = 0;
		for (int route2 = 0; route2 < m_nRoute; route2++)
		{
			if (m_Route[route]->PickupNode == m_Route[route2]->PickupNode && m_Route[route]->DropoffNode == m_Route[route2]->DropoffNode)
			{
				RouteFlowTemp += RouteFlow[route2];
			}
		}
		double RidingSpeed = 0.5 * m_Route[route]->FreeFlowTravelSpeed * (1 + pow((1 - 4 * RouteFlowTemp / (m_Route[route]->FreeFlowTravelSpeed * m_Route[route]->Capacity)), 0.5));
		//double RidingSpeed = 0.5 * m_Route[route]->FreeFlowTravelSpeed * (1 + pow((1 - 4 * RouteFlow[route] / (m_Route[route]->FreeFlowTravelSpeed * m_Route[route]->Capacity)), 0.5));
		double RidingtimeCost = ValueOfRidingTime * m_Route[route]->Ridingdistance / RidingSpeed;
		double RidingRentalCost = b_be_2 * m_Route[route]->Ridingdistance / RidingSpeed; //租金cost
		m_Route[route]->PickupWalkingCost = PickupWalkingtimeCost;
		m_Route[route]->DropWalkingCost = DropoffWalkingtimeCost;
		m_Route[route]->RidingTimeCost = RidingtimeCost;
		m_Route[route]->RentalCost = RidingRentalCost;
		if (m_Route[route]->PickupType == 1) //计算在parking node取车的waiting time cost
		{
			double PickupArrivalRate = m_Route[route]->PickupNode->Pickupflow[0];
			//cout << "PickupArrivalRate: " << PickupArrivalRate << endl;
			double PickupServiceRate = 1 / time_interval; //这里要怎么考虑一个node多个类型
			//cout << "PickupServiceRate: " << PickupServiceRate << endl;
			double PickupUseRate = PickupArrivalRate / PickupServiceRate;
			//cout << "PickupUseRate: " << PickupUseRate << endl;
			double Capacity = m_Route[route]->PickupNode->Capacity[0];
			//cout << "Capacity: " << Capacity << endl;
			if (PickupArrivalRate == 0)
			{
				m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] = 0;
				m_Route[route]->PickupNode->PickupNoSharedEbike[0] = 0;
				m_Route[route]->PickupNode->PickupLossRate[0] = 0;
				m_Route[route]->PickupNode->PickupQueue[0] = 0;
				m_Route[route]->PickupNode->PickupEffectiveRate[0] = 0;
				//cout << "pick = 0" << endl;
				//PickupArrivalRate = 0.001;
				//PickupUseRate = PickupArrivalRate / PickupServiceRate;
				//double OProbability = (1 - PickupUseRate) / (1 - pow(PickupUseRate, Capacity + 1));
				////cout << "PickupOProbability: " << OProbability << endl;
				//double PickupLossRate = pow(PickupUseRate, Capacity) * OProbability;
				////cout << "PickupLossRate: " << PickupLossRate << endl;
				//double LengthPickupQueue = PickupUseRate / (1 - PickupUseRate) - (PickupUseRate * (1 + Capacity * pow(PickupUseRate, Capacity))) / (1 - pow(PickupUseRate, (Capacity + 1)));
				////cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
				//double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
				////cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
				//double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
				////cout << "PickupWaitingtime: " << Waitingtime << endl;
				//double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
				//m_Route[route]->PickupNode->PickupNoSharedEbike[0] = OProbability;
				//m_Route[route]->PickupNode->PickupLossRate[0] = PickupLossRate;
				//m_Route[route]->PickupNode->PickupQueue[0] = LengthPickupQueue;
				//m_Route[route]->PickupNode->PickupEffectiveRate[0] = EffectiveArrivalRate;
				//m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] = WaitingtimeCost;
				////cout << "Waiting_time_cost_pickup: " << WaitingtimeCost << endl;
			}
			else
			{
				if (PickupUseRate != 1)
				{
					double OProbability = (1 - PickupUseRate) / (1 - pow(PickupUseRate, Capacity + 1));
					//cout << "PickupOProbability: " << OProbability << endl;
					double PickupLossRate = pow(PickupUseRate, Capacity) * OProbability;
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthPickupQueue = PickupUseRate / (1 - PickupUseRate) - (PickupUseRate * (1 + Capacity * pow(PickupUseRate, Capacity))) / (1 - pow(PickupUseRate, (Capacity + 1)));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
					//cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
					//cout << "PickupWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->PickupNode->PickupNoSharedEbike[0] = OProbability;
					m_Route[route]->PickupNode->PickupLossRate[0] = PickupLossRate;
					m_Route[route]->PickupNode->PickupQueue[0] = LengthPickupQueue;
					m_Route[route]->PickupNode->PickupEffectiveRate[0] = EffectiveArrivalRate;
					m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] = WaitingtimeCost;
					//cout << "Waiting_time_cost_pickup: " << WaitingtimeCost << endl;
				}
				else
				{
					double OProbability = 1 / (1 + Capacity);
					//cout << "PickupOProbability: " << OProbability << endl;
					double PickupLossRate = 1 / (1 + Capacity);
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthPickupQueue = (Capacity * (Capacity - 1)) / (2 * (Capacity + 1));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
					//cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
					//cout << "PickupWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->PickupNode->PickupNoSharedEbike[0] = OProbability;
					m_Route[route]->PickupNode->PickupLossRate[0] = PickupLossRate;
					m_Route[route]->PickupNode->PickupQueue[0] = LengthPickupQueue;
					m_Route[route]->PickupNode->PickupEffectiveRate[0] = EffectiveArrivalRate;
					m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] = WaitingtimeCost;
				}
			}
		}
		if (m_Route[route]->PickupType == 2) //计算在plug-in charging station取车的waiting time cost
		{
			double PickupArrivalRate = m_Route[route]->PickupNode->Pickupflow[1];
			//cout << "PickupArrivalRate: " << PickupArrivalRate << endl;
			double Capacity = m_Route[route]->PickupNode->Capacity[1];
			double PickupServiceRate = 1 / average_charging_time; //这里要怎么考虑一个node多个类型
			//cout << "PickupServiceRate: " << PickupServiceRate << endl;
			double PickupUseRate = PickupArrivalRate / PickupServiceRate;
			//cout << "PickupUseRate: " << PickupUseRate << endl;
			if (PickupArrivalRate == 0)
			{
				m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] = 0;
				m_Route[route]->PickupNode->PickupNoSharedEbike[1] = 0;
				m_Route[route]->PickupNode->PickupLossRate[1] = 0;
				m_Route[route]->PickupNode->PickupQueue[1] = 0;
				m_Route[route]->PickupNode->PickupEffectiveRate[1] = 0;
				//cout << "pick = 0" << endl;

				//PickupArrivalRate = 0.001;
				//PickupUseRate = PickupArrivalRate / PickupServiceRate;
				//double OProbability = (1 - PickupUseRate) / (1 - pow(PickupUseRate, Capacity + 1));
				////cout << "PickupOProbability: " << OProbability << endl;
				//double PickupLossRate = pow(PickupUseRate, Capacity) * OProbability;
				////cout << "PickupLossRate: " << PickupLossRate << endl;
				//double LengthPickupQueue = PickupUseRate / (1 - PickupUseRate) - (PickupUseRate * (1 + Capacity * pow(PickupUseRate, Capacity))) / (1 - pow(PickupUseRate, (Capacity + 1)));
				////cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
				//double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
				////cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
				//double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
				////cout << "PickupWaitingtime: " << Waitingtime << endl;
				//double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
				//m_Route[route]->PickupNode->PickupNoSharedEbike[0] = OProbability;
				//m_Route[route]->PickupNode->PickupLossRate[0] = PickupLossRate;
				//m_Route[route]->PickupNode->PickupQueue[0] = LengthPickupQueue;
				//m_Route[route]->PickupNode->PickupEffectiveRate[0] = EffectiveArrivalRate;
				//m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] = WaitingtimeCost;
				////cout << "Waiting_time_cost_pickup: " << WaitingtimeCost << endl;
			}
			else
			{
				if (PickupUseRate != 1)
				{
					double OProbability = (1 - PickupUseRate) / (1 - pow(PickupUseRate, Capacity + 1));
					//cout << "PickupOProbability: " << OProbability << endl;
					double PickupLossRate = pow(PickupUseRate, Capacity) * OProbability;
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthPickupQueue = PickupUseRate / (1 - PickupUseRate) - (PickupUseRate * (1 + Capacity * pow(PickupUseRate, Capacity))) / (1 - pow(PickupUseRate, (Capacity + 1)));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
					//cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
					//cout << "PickupWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->PickupNode->PickupNoSharedEbike[1] = OProbability;
					m_Route[route]->PickupNode->PickupLossRate[1] = PickupLossRate;
					m_Route[route]->PickupNode->PickupQueue[1] = LengthPickupQueue;
					m_Route[route]->PickupNode->PickupEffectiveRate[1] = EffectiveArrivalRate;
					m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] = WaitingtimeCost;
				}
				else
				{
					double OProbability = 1 / (1 + Capacity);
					//cout << "PickupOProbability: " << OProbability << endl;
					double PickupLossRate = 1 / (1 + Capacity);
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthPickupQueue = (Capacity * (Capacity - 1)) / (2 * (Capacity + 1));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = PickupArrivalRate * (1 - PickupLossRate);
					//cout << "PickupEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthPickupQueue / EffectiveArrivalRate;
					//cout << "PickupWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->PickupNode->PickupNoSharedEbike[1] = OProbability;
					m_Route[route]->PickupNode->PickupLossRate[1] = PickupLossRate;
					m_Route[route]->PickupNode->PickupQueue[1] = LengthPickupQueue;
					m_Route[route]->PickupNode->PickupEffectiveRate[1] = EffectiveArrivalRate;
					m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] = WaitingtimeCost;
				}
			}
		}

		if (m_Route[route]->DropoffType == 1) //计算在parking node停车的waiting time cost
		{
			double DropoffArrivalRate = m_Route[route]->DropoffNode->Dropoffflow[0];
			//cout << "DropoffArrivalRate: " << DropoffArrivalRate << endl;
			double Capacity = m_Route[route]->DropoffNode->Capacity[0];
			double DropoffServiceRate = 1 / time_interval; //这里要怎么考虑一个node多个类型
			//cout << "DropoffServiceRate: " << DropoffServiceRate << endl;
			double DropoffUseRate = DropoffArrivalRate / DropoffServiceRate;
			//cout << "DropoffUseRate: " << DropoffUseRate << endl;
			if (DropoffArrivalRate == 0)
			{
				m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] = 0;
				m_Route[route]->DropoffNode->DropofffNoParkingSpace[0] = 0;
				m_Route[route]->DropoffNode->DropoffLossRate[0] = 0;
				m_Route[route]->DropoffNode->DropoffQueue[0] = 0;
				m_Route[route]->DropoffNode->DropoffEffectiveRate[0] = 0;
				//cout << "drop = 0" << endl;

				//DropoffArrivalRate = 0.001;
				//DropoffUseRate = DropoffArrivalRate / DropoffServiceRate;
				//double OProbability = (1 - DropoffUseRate) / (1 - pow(DropoffUseRate, Capacity + 1)); //出错是因为 non-parking node capacity设置为INF，太大了
				////cout << "DropoffOProbability: " << OProbability << endl;
				//double DropoffLossRate = pow(DropoffUseRate, Capacity) * OProbability;
				////cout << "DropoffLossRate: " << DropoffLossRate << endl;
				//double LengthDropoffQueue = DropoffUseRate / (1 - DropoffUseRate) - (DropoffUseRate * (1 + Capacity * pow(DropoffUseRate, Capacity))) / (1 - pow(DropoffUseRate, (Capacity + 1)));
				////cout << "LengthDropoffQueue: " << LengthDropoffQueue << endl;
				//double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
				////cout << "DropoffEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
				//double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
				////cout << "DropoffWaitingtime: " << Waitingtime << endl;
				//double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
				//m_Route[route]->DropoffNode->DropofffNoParkingSpace[0] = OProbability;
				//m_Route[route]->DropoffNode->DropoffLossRate[0] = DropoffLossRate;
				//m_Route[route]->DropoffNode->DropoffQueue[0] = LengthDropoffQueue;
				//m_Route[route]->DropoffNode->DropoffEffectiveRate[0] = EffectiveArrivalRate;
				//m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] = WaitingtimeCost;
			}
			else
			{
				if (DropoffUseRate != 1)
				{
					double OProbability = (1 - DropoffUseRate) / (1 - pow(DropoffUseRate, Capacity + 1)); //出错是因为 non-parking node capacity设置为INF，太大了
					//cout << "DropoffOProbability: " << OProbability << endl;
					double DropoffLossRate = pow(DropoffUseRate, Capacity) * OProbability;
					//cout << "DropoffLossRate: " << DropoffLossRate << endl;
					double LengthDropoffQueue = DropoffUseRate / (1 - DropoffUseRate) - (DropoffUseRate * (1 + Capacity * pow(DropoffUseRate, Capacity))) / (1 - pow(DropoffUseRate, (Capacity + 1)));
					//cout << "LengthDropoffQueue: " << LengthDropoffQueue << endl;
					double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
					//cout << "DropoffEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
					//cout << "DropoffWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->DropoffNode->DropofffNoParkingSpace[0] = OProbability;
					m_Route[route]->DropoffNode->DropoffLossRate[0] = DropoffLossRate;
					m_Route[route]->DropoffNode->DropoffQueue[0] = LengthDropoffQueue;
					m_Route[route]->DropoffNode->DropoffEffectiveRate[0] = EffectiveArrivalRate;
					m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] = WaitingtimeCost;
				}
				else
				{
					double OProbability = 1 / (1 + Capacity);
					//cout << "DropoffOProbability: " << OProbability << endl;
					double DropoffLossRate = 1 / (1 + Capacity);
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthDropoffQueue = (Capacity * (Capacity - 1)) / (2 * (Capacity + 1));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
					//cout << "EffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
					//cout << "Waitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->DropoffNode->DropofffNoParkingSpace[0] = OProbability;
					m_Route[route]->DropoffNode->DropoffLossRate[0] = DropoffLossRate;
					m_Route[route]->DropoffNode->DropoffQueue[0] = LengthDropoffQueue;
					m_Route[route]->DropoffNode->DropoffEffectiveRate[0] = EffectiveArrivalRate;
					m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] = WaitingtimeCost;
				}
			}
		}
		if (m_Route[route]->DropoffType == 2) //计算在pluh-in charging station停车的waiting time cost
		{
			double DropoffArrivalRate = m_Route[route]->DropoffNode->Dropoffflow[1];
			//cout << "DropoffArrivalRate: " << DropoffArrivalRate << endl;
			double Capacity = m_Route[route]->DropoffNode->Capacity[1];
			//cout << "Capacity!!!!: " << Capacity << endl;
			double DropoffServiceRate = 1 / average_charging_time; //这里要怎么考虑一个node多个类型
			//cout << "DropoffServiceRate: " << DropoffServiceRate << endl;
			double DropoffUseRate = DropoffArrivalRate / DropoffServiceRate;
			//cout << "DropoffUseRate: " << DropoffUseRate << endl;
			if (DropoffArrivalRate == 0)
			{
				m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1] = 0;
				m_Route[route]->DropoffNode->DropofffNoParkingSpace[1] = 0;
				m_Route[route]->DropoffNode->DropoffLossRate[1] = 0;
				m_Route[route]->DropoffNode->DropoffQueue[1] = 0;
				m_Route[route]->DropoffNode->DropoffEffectiveRate[1] = 0;
				//cout << "drop = 0" << endl;

				//DropoffArrivalRate = 0.001;
				//DropoffUseRate = DropoffArrivalRate / DropoffServiceRate;
				//double OProbability = (1 - DropoffUseRate) / (1 - pow(DropoffUseRate, Capacity + 1)); //出错是因为 non-parking node capacity设置为INF，太大了
				////cout << "DropoffOProbability: " << OProbability << endl;
				//double DropoffLossRate = pow(DropoffUseRate, Capacity) * OProbability;
				////cout << "DropoffLossRate: " << DropoffLossRate << endl;
				//double LengthDropoffQueue = DropoffUseRate / (1 - DropoffUseRate) - (DropoffUseRate * (1 + Capacity * pow(DropoffUseRate, Capacity))) / (1 - pow(DropoffUseRate, (Capacity + 1)));
				////cout << "LengthDropoffQueue: " << LengthDropoffQueue << endl;
				//double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
				////cout << "DropoffEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
				//double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
				////cout << "DropoffWaitingtime: " << Waitingtime << endl;
				//double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
				//m_Route[route]->DropoffNode->DropofffNoParkingSpace[0] = OProbability;
				//m_Route[route]->DropoffNode->DropoffLossRate[0] = DropoffLossRate;
				//m_Route[route]->DropoffNode->DropoffQueue[0] = LengthDropoffQueue;
				//m_Route[route]->DropoffNode->DropoffEffectiveRate[0] = EffectiveArrivalRate;
				//m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] = WaitingtimeCost;
			}
			else
			{
				if (DropoffUseRate != 1)
				{
					double tempp = (1 - pow(DropoffUseRate, Capacity + 1));
					//cout << "Capacity: " << Capacity << endl;
					//cout << "tempp: " << tempp << endl;
					double OProbability = (1 - DropoffUseRate) / tempp; //出错是因为 non-parking node capacity设置为INF，太大了
					//cout << "DropoffOProbability: " << OProbability << endl;
					double DropoffLossRate = pow(DropoffUseRate, Capacity) * OProbability;
					//cout << "DropoffLossRate: " << DropoffLossRate << endl;
					double LengthDropoffQueue = DropoffUseRate / (1 - DropoffUseRate) - (DropoffUseRate * (1 + Capacity * pow(DropoffUseRate, Capacity))) / (1 - pow(DropoffUseRate, (Capacity + 1)));
					//cout << "LengthDropoffQueue: " << LengthDropoffQueue << endl;
					double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
					//cout << "DropoffEffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
					//cout << "DropoffWaitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->DropoffNode->DropofffNoParkingSpace[1] = OProbability;
					m_Route[route]->DropoffNode->DropoffLossRate[1] = DropoffLossRate;
					m_Route[route]->DropoffNode->DropoffQueue[1] = LengthDropoffQueue;
					m_Route[route]->DropoffNode->DropoffEffectiveRate[1] = EffectiveArrivalRate;
					m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1] = WaitingtimeCost;
				}
				else
				{
					double OProbability = 1 / (1 + Capacity);
					//cout << "DropoffOProbability: " << OProbability << endl;
					double DropoffLossRate = 1 / (1 + Capacity);
					//cout << "PickupLossRate: " << PickupLossRate << endl;
					double LengthDropoffQueue = (Capacity * (Capacity - 1)) / (2 * (Capacity + 1));
					//cout << "LengthPickupQueue: " << LengthPickupQueue << endl;
					double EffectiveArrivalRate = DropoffArrivalRate * (1 - DropoffLossRate);
					//cout << "EffectiveArrivalRate: " << EffectiveArrivalRate << endl;
					double Waitingtime = LengthDropoffQueue / EffectiveArrivalRate;
					//cout << "Waitingtime: " << Waitingtime << endl;
					double WaitingtimeCost = ValueOfWaitingTime * Waitingtime;
					m_Route[route]->DropoffNode->DropofffNoParkingSpace[1] = OProbability;
					m_Route[route]->DropoffNode->DropoffLossRate[1] = DropoffLossRate;
					m_Route[route]->DropoffNode->DropoffQueue[1] = LengthDropoffQueue;
					m_Route[route]->DropoffNode->DropoffEffectiveRate[1] = EffectiveArrivalRate;
					m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1] = WaitingtimeCost;
				}
			}
		}
		if (m_Route[route]->DropoffType == 3) //计算在non-parking node停车的waiting time cost
		{
			m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2] = 0;
		}

		if (m_Route[route]->PickupType == 1 && m_Route[route]->DropoffType == 1)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0];
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[0];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0];
			//cout << "m_Route[route]->PickupWalkingtimeCost: " << PickupWalkingtimeCost << endl;
			//cout << "m_Route[route]->RidingtimeCost: " << RidingtimeCost << endl;
			//cout << "m_Route[route]->DropoffWalkingtimeCost: " << DropoffWalkingtimeCost << endl;
			//cout << RidingtimeCost << endl;
		}
		if (m_Route[route]->PickupType == 1 && m_Route[route]->DropoffType == 2)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1]
				- m_Route[route]->DropoffNode->Incentive;
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[0];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1];
			//cout << "m_Route[route]->PickupNode->Waiting_time_cost_pickup[0]: " << m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] << endl;
			//cout << "m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1]: " << m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1] << endl;
		}
		if (m_Route[route]->PickupType == 1 && m_Route[route]->DropoffType == 3)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2]
				+ m_Route[route]->DropoffNode->Penalty;
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[0];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2];
			//cout << "m_Route[route]->PickupNode->Waiting_time_cost_pickup[0]: " << m_Route[route]->PickupNode->Waiting_time_cost_pickup[0] << endl;
			//cout << "m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2]: " << m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2] << endl;
		}
		if (m_Route[route]->PickupType == 2 && m_Route[route]->DropoffType == 1)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0];
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[1];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0];
			//cout << "m_Route[route]->PickupNode->Waiting_time_cost_pickup[1]: " << m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] << endl;
			//cout << "m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0]: " << m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[0] << endl;
		}
		if (m_Route[route]->PickupType == 2 && m_Route[route]->DropoffType == 2)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1]
				- m_Route[route]->DropoffNode->Incentive;
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[1];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1];
			//cout << "m_Route[route]->PickupNode->Waiting_time_cost_pickup[1]: " << m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] << endl;
			//cout << "m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1]: " << m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[1] << endl;
		}
		if (m_Route[route]->PickupType == 2 && m_Route[route]->DropoffType == 3)
		{
			m_Route[route]->RouteCost = PickupWalkingtimeCost + RidingtimeCost + RidingRentalCost + DropoffWalkingtimeCost + m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] + m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2]
				+ m_Route[route]->DropoffNode->Penalty;
			m_Route[route]->PickupWaitingCost = m_Route[route]->PickupNode->Waiting_time_cost_pickup[1];
			m_Route[route]->DropWaitingCost = m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2];
			//cout << "m_Route[route]->PickupNode->Waiting_time_cost_pickup[1]: " << m_Route[route]->PickupNode->Waiting_time_cost_pickup[1] << endl;
			//cout << "m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2]: " << m_Route[route]->DropoffNode->Waiting_time_cost_dropoff[2] << endl;
		}
		if (m_Route[route]->PickupType == -1 && m_Route[route]->DropoffType == -1)
		{
			m_Route[route]->RouteCost = ArtificialRoute;
			m_Route[route]->PickupWaitingCost = 0;
			m_Route[route]->DropWaitingCost = 0;
			//cout << RidingtimeCost << endl;
		}

		//if (m_Route[route]->DropoffNode->Incentive != 5)
		//{
		//	cout << "Hellow world: " << m_Route[route]->DropoffNode->Incentive << endl;
		//}
		//if (m_Route[route]->RouteCost < -90)
		//{
		//	cout << "Hellow world: " << m_Route[route]->DropoffNode->Incentive << endl;
		//}
		//cout << "m_Route[route]->EnergyConsumption: " << m_Route[route]->EnergyConsumption << " " << route << endl;
		// 初始化全有全无路段流量 = 0
	}
}

//生成route
void CNetwork::GenerateRoute()
{
	m_Route.clear();
	m_nRoute = 0;
	CRoute* pRoute;

	for (int origin = 0; origin < m_nOrigin; origin++) //取一个起点
	{
		m_Origin[origin]; //起点（包含终点）
		for (int destination = 0; destination < m_Origin[origin]->DestinationNode.size(); destination++) //取一个终点
		{
			int m_nRoute_temp = m_nRoute;
			//生成一条虚拟route, 保证每一对OD可以出行
			m_Node[origin]; //取车点为起点
			//cout << m_Origin[origin]->DestinationNode[destination] + 1 << endl;
			m_Node[m_Origin[origin]->DestinationNode[destination]]; //还车点为终点
			pRoute = new CRoute();//对 每一对ODpair，创建一个route class
			pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
			pRoute->PickupNode = m_Node[origin];
			pRoute->DropoffNode = m_Node[m_Origin[origin]->DestinationNode[destination]];
			//cout << m_Node[origin]->ID + 1 << " " << m_Node[m_Origin[origin]->DestinationNode[destination]]->ID + 1 << endl;
			pRoute->PickupType = -1;
			pRoute->DropoffType = -1;
			//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
			double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[origin]->ID];
			double RidingDistance = NodeDistance[m_Node[origin]->ID][m_Node[m_Origin[origin]->DestinationNode[destination]]->ID];
			//cout << m_Node[origin]->ID << " " << m_Node[m_Origin[origin]->DestinationNode[destination]]->ID << " " << RidingDistance << endl;
			double DropoffWalkingDistance = NodeDistance[m_Node[m_Origin[origin]->DestinationNode[destination]]->ID][m_Origin[origin]->DestinationNode[destination]];
			pRoute->Pickupdistance = PickupWalkingDistance;
			pRoute->Ridingdistance = RidingDistance;
			pRoute->EnergyConsumption = 0;
			pRoute->Dropoffdistance = DropoffWalkingDistance;
			pRoute->RouteID = m_nRoute; //Route number
			m_nRoute++;
			m_Route.push_back(pRoute);

			for (int picknode = 0; picknode < m_nNode; picknode++) //取网络中的任一个点作为pick_up点
			{
				m_Node[picknode]; //取车点
				for (int type1 = 0; type1 < 3; type1++) //取车点可以多种类型
				{
					if (m_Node[picknode]->NodeType[type1] == 1) //在parking node取车
					{
						for (int dropnode = 0; dropnode < m_nNode; dropnode++) //取网络中的任一个点作为drop_off点
						{
							m_Node[dropnode]; //还车点
							if (m_Node[dropnode]->ID != m_Node[picknode]->ID) //取车和还车点不同
							{
								for (int type2 = 0; type2 < 3; type2++)
								{
									if (m_Node[dropnode]->NodeType[type2] == 1) //在parking node还车
									{
										pRoute = new CRoute();//对 每一对ODpair，创建一个route class
										pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
										pRoute->PickupNode = m_Node[picknode];
										pRoute->DropoffNode = m_Node[dropnode];
										pRoute->PickupType = m_Node[picknode]->NodeType[type1];
										pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
										//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
										double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
										double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
										//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
										double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
										pRoute->Pickupdistance = PickupWalkingDistance;
										pRoute->Ridingdistance = RidingDistance;
										pRoute->EnergyConsumption = arfa3 * RidingDistance;
										pRoute->Dropoffdistance = DropoffWalkingDistance;
										if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
										{
											pRoute->RouteID = m_nRoute; //Route number
											m_nRoute++;
											m_Route.push_back(pRoute);
										}
									}
									if (m_Node[dropnode]->NodeType[type2] == 2) //在plug-in charging station还车
									{
										pRoute = new CRoute();//对 每一对ODpair，创建一个route class
										pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
										pRoute->PickupNode = m_Node[picknode];
										pRoute->DropoffNode = m_Node[dropnode];
										pRoute->PickupType = m_Node[picknode]->NodeType[type1];
										pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
										//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
										double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
										double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
										//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
										double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
										pRoute->Pickupdistance = PickupWalkingDistance;
										pRoute->Ridingdistance = RidingDistance;
										pRoute->EnergyConsumption = arfa3 * RidingDistance;
										pRoute->Dropoffdistance = DropoffWalkingDistance;
										if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
										{
											pRoute->RouteID = m_nRoute; //Route number
											m_nRoute++;
											m_Route.push_back(pRoute);
										}
									}
									if (m_Node[dropnode]->NodeType[type2] == 3) //在non-parking node还车
									{
										pRoute = new CRoute();//对 每一对ODpair，创建一个route class
										pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
										pRoute->PickupNode = m_Node[picknode];
										pRoute->DropoffNode = m_Node[dropnode];
										pRoute->PickupType = m_Node[picknode]->NodeType[type1];
										pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
										//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
										double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
										double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
										//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
										double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
										pRoute->Pickupdistance = PickupWalkingDistance;
										pRoute->Ridingdistance = RidingDistance;
										pRoute->EnergyConsumption = arfa3 * RidingDistance;
										pRoute->Dropoffdistance = DropoffWalkingDistance;
										if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
										{
											pRoute->RouteID = m_nRoute; //Route number
											m_nRoute++;
											m_Route.push_back(pRoute);
										}
									}
								}
							}
						}
					}
					if (m_Node[picknode]->NodeType[type1] == 2) //在Charging node取车
					{
						for (int dropnode = 0; dropnode < m_nNode; dropnode++) //取网络中的任一个点作为drop_off点
						{
							m_Node[dropnode]; //还车点
							{
								if (m_Node[dropnode]->ID != m_Node[picknode]->ID) //取车和还车点不同
								{
									for (int type2 = 0; type2 < 3; type2++)
									{
										if (m_Node[dropnode]->NodeType[type2] == 1) //在parking node还车
										{
											pRoute = new CRoute();//对 每一对ODpair，创建一个route class
											pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
											pRoute->PickupNode = m_Node[picknode];
											pRoute->DropoffNode = m_Node[dropnode];
											pRoute->PickupType = m_Node[picknode]->NodeType[type1];
											pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
											//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
											double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
											double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
											//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
											double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
											pRoute->Pickupdistance = PickupWalkingDistance;
											pRoute->Ridingdistance = RidingDistance;
											pRoute->EnergyConsumption = arfa3 * RidingDistance;
											pRoute->Dropoffdistance = DropoffWalkingDistance;
											if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
											{
												pRoute->RouteID = m_nRoute; //Route number
												m_nRoute++;
												m_Route.push_back(pRoute);
											}
										}
										if (m_Node[dropnode]->NodeType[type2] == 2) //在plug-in charging station还车
										{
											pRoute = new CRoute();//对 每一对ODpair，创建一个route class
											pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
											pRoute->PickupNode = m_Node[picknode];
											pRoute->DropoffNode = m_Node[dropnode];
											pRoute->PickupType = m_Node[picknode]->NodeType[type1];
											pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
											//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
											double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
											double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
											//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
											double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
											pRoute->Pickupdistance = PickupWalkingDistance;
											pRoute->Ridingdistance = RidingDistance;
											pRoute->EnergyConsumption = arfa3 * RidingDistance;
											pRoute->Dropoffdistance = DropoffWalkingDistance;
											if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
											{
												pRoute->RouteID = m_nRoute; //Route number
												m_nRoute++;
												m_Route.push_back(pRoute);
											}
										}
										if (m_Node[dropnode]->NodeType[type2] == 3) //在non-parking node还车
										{
											pRoute = new CRoute();//对 每一对ODpair，创建一个route class
											pRoute->ODID = m_Origin[origin]->ODpairNumber[destination];//OD pair number
											pRoute->PickupNode = m_Node[picknode];
											pRoute->DropoffNode = m_Node[dropnode];
											pRoute->PickupType = m_Node[picknode]->NodeType[type1];
											pRoute->DropoffType = m_Node[dropnode]->NodeType[type2];
											//cout << pRoute->PickupNode->ID << pRoute->DropoffNode->ID << endl;
											double PickupWalkingDistance = NodeDistance[m_Origin[origin]->pOriginNode->ID][m_Node[picknode]->ID];
											double RidingDistance = NodeDistance[m_Node[picknode]->ID][m_Node[dropnode]->ID];
											//cout << m_Node[picknode]->ID << " " << m_Node[dropnode]->ID << " " << RidingDistance << endl;
											double DropoffWalkingDistance = NodeDistance[m_Node[dropnode]->ID][m_Origin[origin]->DestinationNode[destination]];
											pRoute->Pickupdistance = PickupWalkingDistance;
											pRoute->Ridingdistance = RidingDistance;
											pRoute->EnergyConsumption = arfa3 * RidingDistance;
											pRoute->Dropoffdistance = DropoffWalkingDistance;
											if (PickupWalkingDistance < INF && RidingDistance < INF && DropoffWalkingDistance < INF)
											{
												pRoute->RouteID = m_nRoute; //Route number
												m_nRoute++;
												m_Route.push_back(pRoute);
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if (m_nRoute_temp == m_nRoute)
			{
				temp_do = 0;
			}
		}
	}
}

//assign all-or-nothing
//返回一个指针，指向一个double[m_nRoute]的数组，包含了route的流量
void CNetwork::AllorNothingAssignment(double* RouteFlow1, double* ANRouteFlow)
{
	//auto ANRouteFlow = new double[m_nRoute]; // 辅助Route flow
	for (int route = 0; route < m_nRoute; route++)
	{
		ANRouteFlow[route] = 0; //初始为0
	}
	////计算zero flow情况下，每条route的route cost
	//cout << "all or nothing " << endl;
	UpdateRouteCost(RouteFlow);
	//PrintUERouteCost();
	//cout << "hello wordl" << endl;
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	cout << "RouteCost: " << m_Route[route]->RouteCost << endl;
	//}
	//delete RouteFlow;
	for (int node = 0; node < m_nNode; node++)
	{
		for (int type = 0; type < 3; type++)
		{
			m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
			m_Node[node]->Dropoffflow[type] = 0;
		}
	}
	//cout << endl;
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	if (m_Route[route]->DropoffNode->Incentive != 5)
	//	{
	//		cout << "Hellow world: " << m_Route[route]->DropoffNode->Incentive << endl;
	//	}
	//}
	for (int origin = 0; origin < m_nOrigin; origin++) // 遍历origin的循环all or nothing
	{
		//int OriginNode = m_Origin[origin]->pOriginNode->ID; // 任意一个origin的编号 （拿出一个origin）
		for (int destination = 0; destination < m_Origin[origin]->DestinationNode.size(); destination++) //取出一个OD pair
		{
			int ODNumber = m_Origin[origin]->ODpairNumber[destination];
			double tem_cost = INF;
			int tem_ID = -1;
			for (int route = 0; route < m_nRoute; route++)
			{
				if (ODNumber == m_Route[route]->ODID) //如果对于一个ODpair，找到了该ODpair对应的route
				{
					if (tem_cost > m_Route[route]->RouteCost)
					{
						tem_cost = m_Route[route]->RouteCost;
						tem_ID = m_Route[route]->RouteID;
					}
				}
			}
			double Demand = m_Origin[origin]->ODDemand[destination];
			ANRouteFlow[tem_ID] += Demand;
			//找到这条route取车点和还车点对应的类型
			int Pickuptype = m_Route[tem_ID]->PickupType; //1, 2  //在全部都是3的情况下，没有取车点
			//cout << "Pickuptype：" << Pickuptype << endl;
			int Dropofftype = m_Route[tem_ID]->DropoffType; //1, 2, 3
			//cout << "Dropofftype：" << Dropofftype << endl;
			//cout << "tem_ID：" << tem_ID << endl;
			if (Pickuptype != -1 && Dropofftype != -1)
			{
				m_Route[tem_ID]->PickupNode->Pickupflow[Pickuptype - 1] += Demand;
				m_Route[tem_ID]->DropoffNode->Dropoffflow[Dropofftype - 1] += Demand;
			}
		}
	}
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	if (m_Route[route]->DropoffNode->Incentive != 5)
	//	{
	//		cout << "Hellow worldHellow worldHellow world: " << m_Route[route]->DropoffNode->Incentive << endl;
	//	}
	//}
	for (int route = 0; route < m_nRoute; route++)
	{
		RouteFlow1[route] = ANRouteFlow[route]; //初始为0
	}
}

//Stop criterion
double CNetwork::GetUEGap(double* RouteFlow)
{
	//cout << "计算gap" << endl;
	UpdateRouteCost(RouteFlow);
	//cout << endl;

	// 计算被减项分母 + 更新route trave cost (根据当前的route flow)
	double num1 = 0; //TSTT
	for (int route = 0; route < m_nRoute; route++)
	{
		num1 += RouteFlow[route] * m_Route[route]->RouteCost;
	}

	// 计算被减项分子
	double num2 = 0; //SPTT
	auto ANRouteFlow1 = new double[m_nRoute]; // 辅助Route flow，要能储存下来
	for (int route = 0; route < m_nRoute; route++)
	{
		ANRouteFlow1[route] = 0; //初始为0
	}
	for (int origin = 0; origin < m_nOrigin; origin++) // 遍历origin的循环
	{
		//int OriginNode = m_Origin[origin]->pOriginNode->ID; // 任意一个origin的编号 （拿出一个origin）
		for (int destination = 0; destination < m_Origin[origin]->DestinationNode.size(); destination++) //取出一个OD pair
		{
			int ODNumber = m_Origin[origin]->ODpairNumber[destination];
			double tem_cost = INF;
			int tem_ID = -1;
			for (int route = 0; route < m_nRoute; route++)
			{
				if (ODNumber == m_Route[route]->ODID) //如果对于一个ODpair，找到了该ODpair对应的route
				{
					if (tem_cost > m_Route[route]->RouteCost)
					{
						tem_cost = m_Route[route]->RouteCost;
						tem_ID = m_Route[route]->RouteID;
					}
				}
			}
			double Demand = m_Origin[origin]->ODDemand[destination];
			ANRouteFlow1[tem_ID] += Demand;
		}
	}

	for (int route = 0; route < m_nRoute; route++)
	{
		num2 += ANRouteFlow1[route] * m_Route[route]->RouteCost;
	}
	delete[] ANRouteFlow1;
	ANRouteFlow1 = NULL;
	// 计算间隙函数
	//cout << "num1: " << num1 << endl;
	//cout << "num2: " << num2 << endl;
	UEGap = 1 - num2 / num1;

	//double temp = 0;
	//// Calculate the auxiliary flow ANRouteFlow1
	//auto ANRouteFlow1 = new double[m_nRoute]; // 辅助Route flow，要能储存下来
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	ANRouteFlow1[route] = 0; //初始为0
	//}
	//for (int origin = 0; origin < m_nOrigin; origin++) // 遍历origin的循环
	//{
	//	//int OriginNode = m_Origin[origin]->pOriginNode->ID; // 任意一个origin的编号 （拿出一个origin）
	//	for (int destination = 0; destination < m_Origin[origin]->DestinationNode.size(); destination++) //取出一个OD pair
	//	{
	//		int ODNumber = m_Origin[origin]->ODpairNumber[destination];
	//		double tem_cost = INF;
	//		int tem_ID = -1;
	//		for (int route = 0; route < m_nRoute; route++)
	//		{
	//			if (ODNumber == m_Route[route]->ODID) //如果对于一个ODpair，找到了该ODpair对应的route
	//			{
	//				if (tem_cost > m_Route[route]->RouteCost)
	//				{
	//					tem_cost = m_Route[route]->RouteCost;
	//					tem_ID = m_Route[route]->RouteID;
	//				}
	//			}
	//		}
	//		double Demand = m_Origin[origin]->ODDemand[destination];
	//		ANRouteFlow1[tem_ID] += Demand;
	//	}
	//}

	//// Calculate the gap
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	temp += (RouteFlow[route] - ANRouteFlow1[route]) * (RouteFlow[route] - ANRouteFlow1[route]); //current route flow
	//}
	//UEGap = sqrt(temp);

	//cout << "UEGap: " << UEGap << endl;
	//cout << "UE平衡的辅助道路流量：" << endl;
	//int ODO = 0;
	//int coun = 0;
	//for (int i = 0; i < m_nRoute; ++i)
	//{
	//	ODO = m_Route[i]->ODID;
	//	if (ODO == coun)
	//	{
	//		if (ANRouteFlow1[i] > 0)
	//		{
	//			cout << ODO << " " << ANRouteFlow1[i] << " ";
	//			cout << m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ")";
	//			cout << endl;
	//		}
	//	}
	//	//cout << endl;
	//	if (ODO != coun)
	//	{
	//		if (ANRouteFlow1[i] > 0)
	//		{
	//			cout << ODO << " " << ANRouteFlow1[i] << ",";
	//			cout << m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ")";
	//			cout << endl;
	//		}
	//		coun++;
	//	}
	//}
	//cout << endl;

	//delete[] ANRouteFlow1;
	//ANRouteFlow1 = NULL;

	return UEGap;
}

//计算battery swap frequency
void CNetwork::battery_swap()
{
	double battery_max = 0;
	for (int route = 0; route < m_nRoute; route++)
	{
		if (m_Route[route]->EnergyConsumption > battery_max && m_Route[route]->EnergyConsumption > 0 && RouteFlow[route] > 0)
		{
			battery_max = m_Route[route]->EnergyConsumption;
		}
	}
	if (battery_max != 0)
	{
		n_swap = floor(E_max / battery_max);
	}
	else
	{
		n_swap = 0;
	}
	//cout << "battery_max: " << battery_max << endl;
	//cout << "n_swap: " << n_swap << endl;
}

//计算objective value
double CNetwork::cal_obj(int sol, vector<CSolution*> m_Solution)
{
	//////////////////计算当前的objective value
	battery_swap();
	double UE_cost = 0;
	double battery_swap_cost = 0;
	double unmet_demand_cost = 0;
	double construction_cost = 0;
	double capacity_cost = 0;
	double reposition_1 = 0;
	double reposition_2 = 0;
	double unsatisfied_demand = 0;
	double loss_demand = 0;
	double penalty_cost = 0;
	double incentive_cost = 0;
	double rental_fare = 0;
	for (int route = 0; route < m_nRoute; route++)
	{
		//cout << "RouteFlow[route]: " << RouteFlow[route] << " " << m_Route[route]->RouteCost << " " << route << " " << m_Route[route]->PickupNode->ID << " " << m_Route[route]->DropoffNode->ID << " " <<
			//m_Route[route]->PickupWaitingCost << " " << m_Route[route]->DropWaitingCost << endl;
		if (m_Route[route]->RouteCost < ArtificialRoute)
		{
			UE_cost += RouteFlow[route] * m_Route[route]->RouteCost; //total travel cost
			//cout << "RouteFlow[route]: " << RouteFlow[route] << " " << m_Route[route]->RouteCost << endl;
			//cout << "m_Route[route]->RouteCost: " << m_Route[route]->RouteCost << endl;
			//if (RouteFlow[route] > 0)
			//{
			//	cout << "RouteFlow[route]: " << RouteFlow[route] << " " << m_Route[route]->RouteCost << endl;
			//}
			if (m_Route[route]->DropoffType != 2)//total battery swap cost for parking and non-parking node, 排除了artificial route和plug-in charging station
			{
				//battery_swap_cost += xita5 * RouteFlow[route] * ceil((days / n_swap));
				//cout << n_swap << endl;
				if (n_swap != 0)
				{
					battery_swap_cost += xita5 * RouteFlow[route] * ceil((days / n_swap));
				}
			}
			rental_fare += RouteFlow[route] * m_Route[route]->RentalCost; //Total rental fare
		}
		if (m_Route[route]->RouteCost >= ArtificialRoute) //unmet demand or environmental penalty
		{
			unmet_demand_cost += EnvironmantalPenalty * RouteFlow[route];
		}
	}
	//cout << "battery_swap_cost: " << battery_swap_cost << endl;
	//cout << "UE_cost: " << UE_cost << endl;
	for (int node = 0; node < m_nNode; node++)
	{
		for (int type = 0; type < 2; type++)
		{
			if (m_Solution[sol]->node_type_upper_level[node][type] > 0) //total construction cost
			{
				construction_cost += ConstructionCost[type];
			}
			double reposition_temp1 = 0;
			if (m_Solution[sol]->node_type_upper_level[node][type] > 0 && m_Solution[sol]->node_type_upper_level[node][type] != 3) //Reposition cost1 for parking node and charging station
			{
				reposition_temp1 = abs(m_Node[node]->Pickupflow[type] - m_Node[node]->Dropoffflow[type]);
				for (int depotnode = 0; depotnode < m_nDepotNode; depotnode++)
				{
					if (node == m_DepotNode[depotnode]->pOutNode->ID)
					{
						reposition_1 += xita6_1 * m_DepotNode[depotnode]->LinkDistance * reposition_temp1;
					}
				}

				//for (int depot = 0; depot < m_nDepot; depot++)
				//{
				//	if(m_D)
				//	reposition_1 += xita6_1 * reposition_temp1;
				//}
			}
			capacity_cost += UnitCapCostPC[type] * m_Solution[sol]->node_capacity_upper_level[node][type]; //total capacity cost
			unsatisfied_demand += m_Node[node]->Pickupflow[type] * m_Node[node]->PickupNoSharedEbike[type] + m_Node[node]->Dropoffflow[type] * m_Node[node]->DropofffNoParkingSpace[type]; //number of pick and drop unsatisfied
			loss_demand += m_Node[node]->Pickupflow[type] * m_Node[node]->PickupLossRate[type] + m_Node[node]->Dropoffflow[type] * m_Node[node]->DropoffLossRate[type]; //number of pick and rop loss
		}
		double reposition_temp2 = 0;
		if (m_Solution[sol]->node_type_upper_level[node][2] == 3) //Reposition cost2 for non-parking node
		{
			reposition_temp2 = m_Node[node]->Dropoffflow[2];
			for (int depotnode = 0; depotnode < m_nDepotNode; depotnode++)
			{
				if (node == m_DepotNode[depotnode]->pOutNode->ID)
				{
					reposition_2 += xita6_2 * m_DepotNode[depotnode]->LinkDistance * reposition_temp2;
				}
			}
		}
		if (m_Node[node]->Dropoffflow[2] > 0)
		{
			penalty_cost += penalty * m_Node[node]->Dropoffflow[2]; //停在non-parking node的penalty cost, 是运营者的收入
		}
		if (m_Node[node]->Dropoffflow[1] > 0)
		{
			incentive_cost += incentive * m_Node[node]->Dropoffflow[1]; //停在charging station的incentive, 是运营者的支出
		}
	}
	m_Solution[sol]->UEcost = UE_cost;
	m_Solution[sol]->BatterySwapCost = battery_swap_cost;
	m_Solution[sol]->UnmetDemandCost = unmet_demand_cost;
	m_Solution[sol]->ConstructionCost = construction_cost;
	m_Solution[sol]->CapacityCost = capacity_cost;
	m_Solution[sol]->Reposition1 = reposition_1;
	m_Solution[sol]->Reposition2 = reposition_2;
	m_Solution[sol]->UnsatisfiedDemandCost = xita1 * unsatisfied_demand;
	m_Solution[sol]->LossDemandCost = xita1 * loss_demand;
	m_Solution[sol]->PenaltyCost = penalty_cost;
	m_Solution[sol]->IncentiveCost = incentive_cost;
	m_Solution[sol]->RentalFare = rental_fare;
	double Obj_temp = UE_cost + battery_swap_cost + unmet_demand_cost + construction_cost + capacity_cost + reposition_1 + reposition_2 + xita1 * unsatisfied_demand + xita1 * loss_demand - penalty_cost + incentive_cost - rental_fare;
	//cout << m_Solution[sol]->ObjectiveValue << endl;
	//cout << m_Solution[sol]->UEcost << endl;
	//cout << m_Solution[sol]->BatterySwapCost << endl;
	//cout << m_Solution[sol]->UnmetDemandCost << endl;
	//cout << m_Solution[sol]->ConstructionCost << endl;
	//cout << m_Solution[sol]->CapacityCost << endl;
	//cout << m_Solution[sol]->Reposition1 << endl;
	//cout << m_Solution[sol]->Reposition2 << endl;
	//cout << m_Solution[sol]->UnsatisfiedDemandCost << endl;
	//cout << m_Solution[sol]->LossDemandCost << endl;
	//cout << m_Solution[sol]->PenaltyCost << endl;
	//cout << m_Solution[sol]->IncentiveCost << endl;
	//cout << m_Solution[sol]->RentalFare << endl;
	m_Solution[sol]->Fitness = 1 / Obj_temp;
	return Obj_temp;

	//////////////////计算当前的objective value
}

//Record the Best solution
void CNetwork::RecordBest(int sol, vector<CSolution*> m_Solution)
{
	////记录当前solution的objective_value
	//double obj_temp = 0; //TSTT
	//for (int route = 0; route < m_nRoute; route++)
	//{
	//	obj_temp += RouteFlow[route] * m_Route[route]->RouteCost;
	//}
	//m_Solution[sol]->ObjectiveValue = obj_temp; //每个solution的obj都记录
	m_Solution[sol]->Fitness = 1 / m_Solution[sol]->ObjectiveValue;
	//cout << "temp: " << obj_temp << endl;
	//记录best solution
	if (BestSolution->ObjectiveValue > m_Solution[sol]->ObjectiveValue)
	{
		//cout << "hello" << endl;
		BestSolution->node_type_upper_level = m_Solution[sol]->node_type_upper_level;
		BestSolution->node_capacity_upper_level = m_Solution[sol]->node_capacity_upper_level;
		BestSolution->ObjectiveValue = m_Solution[sol]->ObjectiveValue;
		BestSolution->Fitness = m_Solution[sol]->Fitness;

		BestSolution->UEcost = m_Solution[sol]->UEcost;
		BestSolution->BatterySwapCost = m_Solution[sol]->BatterySwapCost;
		BestSolution->UnmetDemandCost = m_Solution[sol]->UnmetDemandCost;
		BestSolution->ConstructionCost = m_Solution[sol]->ConstructionCost;
		BestSolution->CapacityCost = m_Solution[sol]->CapacityCost;
		BestSolution->Reposition1 = m_Solution[sol]->Reposition1;
		BestSolution->Reposition2 = m_Solution[sol]->Reposition2;
		BestSolution->UnsatisfiedDemandCost = m_Solution[sol]->UnsatisfiedDemandCost;
		BestSolution->LossDemandCost = m_Solution[sol]->LossDemandCost;
		BestSolution->PenaltyCost = m_Solution[sol]->PenaltyCost;
		BestSolution->IncentiveCost = m_Solution[sol]->IncentiveCost;
		BestSolution->RentalFare = m_Solution[sol]->RentalFare;
	}
}

//Print RouteInf data
void CNetwork::PrintUERouteFlow(double UEGap, double* RouteFlow)
{
	cout << "UEGap: " << UEGap << endl;
	cout << "UE平衡的道路流量：" << endl;
	int ODO = 0;
	int coun = 0;
	for (int i = 0; i < m_nRoute; ++i)
	{
		ODO = m_Route[i]->ODID;
		if (ODO == coun)
		{
			if (RouteFlow[i] > 0)
			{
				cout << ODO << " " << RouteFlow[i] << " ";
				cout << m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ")";
				cout << endl;
			}
		}
		//cout << endl;
		if (ODO != coun)
		{
			if (RouteFlow[i] > 0)
			{
				cout << ODO << " " << RouteFlow[i] << ",";
				cout << m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ")";
				cout << endl;
			}
			coun++;
		}
	}
	cout << endl;
}

//Print NodeInf data
void CNetwork::PrintUENodeFlow()
{
	cout << "UE平衡的各个node的pickup和dropoff流量：" << endl;
	for (int node = 0; node < m_nNode; node++)
	{
		cout << "Node: " << node << endl;
		for (int type1 = 0; type1 < 2; type1++)
		{
			cout << " Pick type " << type1 << ": " << m_Node[node]->Pickupflow[type1] << " Waitingtimepick " << type1 << ": " << m_Node[node]->Waiting_time_cost_pickup[type1];
			cout << endl;
		}
		for (int type2 = 0; type2 < 3; type2++)
		{
			cout << " Drop type " << type2 << ": " << m_Node[node]->Dropoffflow[type2] << " Waitingtimedrop " << type2 << ": " << m_Node[node]->Waiting_time_cost_dropoff[type2] << endl;
		}
	}
	cout << endl;
}

//Print RouteCost data
void CNetwork::PrintUERouteCost()
{
	cout << "UE平衡的各条route的cost：" << endl;
	for (int i = 0; i < m_nRoute; ++i)
	{
		//if (RouteFlow[i] > 0)
		//{
		//	cout << "m_Route[route]->RouteCost: " << m_Route[i]->ODID << " " << m_Route[i]->RouteCost << " [" << RouteFlow[i] << "] " <<
		//		m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ") "
		//		<< endl;
		//}
		cout << "m_Route[route]->RouteCost: " << m_Route[i]->ODID << " " << m_Route[i]->RouteCost << " [" << RouteFlow[i] << "] " <<
			m_Route[i]->PickupNode->ID + 1 << "(" << m_Route[i]->PickupType << ") " << m_Route[i]->DropoffNode->ID + 1 << "(" << m_Route[i]->DropoffType << ") " << m_Route[i]->PickupWaitingCost << " " << m_Route[i]->DropWaitingCost << endl;
	}
	cout << endl;
}

//Print NodeInf data
void CNetwork::PrintNodeInf()
{
	cout << endl;
	cout << "各个Node的capacity：" << endl;
	for (int node = 0; node < m_nNode; node++)
	{
		cout << "m_Node[node]->Capacity: ";
		for (int type = 0; type < 3; type++)
		{
			cout << m_Node[node]->Capacity[type] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << endl;
	cout << "各个Node的WaitingTime：" << endl;
	for (int node = 0; node < m_nNode; node++)
	{
		cout << "Node[" << node << "]->PickupWaitingtime: ";
		for (int type = 0; type < 3; type++)
		{
			cout << m_Node[node]->Waiting_time_cost_pickup[type] << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (int node = 0; node < m_nNode; node++)
	{
		cout << "Node[" << node << "]->DropoffWaitingtime: ";
		for (int type = 0; type < 3; type++)
		{
			cout << m_Node[node]->Waiting_time_cost_dropoff[type] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//Print GlobalBest
void CNetwork::PrintBestSolution()
{
	cout << endl;
	cout << "BestSolution_NodeType: " << endl;
	for (int size1 = 0; size1 < BestSolution->node_type_upper_level.size(); size1++)
	{
		for (int size2 = 0; size2 < 3; size2++)
		{
			cout << BestSolution->node_type_upper_level[size1][size2] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "BestSolution_NodeCapacity: " << endl;
	for (int size1 = 0; size1 < BestSolution->node_capacity_upper_level.size(); size1++)
	{
		for (int size2 = 0; size2 < 3; size2++)
		{
			cout << BestSolution->node_capacity_upper_level[size1][size2] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "BestSolution_ObjectiveValue: " << BestSolution->ObjectiveValue << endl;
	cout << "BestSolution->UEcost: " << BestSolution->UEcost << endl;
	cout << "BestSolution->UnmetDemandCost: " << BestSolution->UnmetDemandCost << endl;
	cout << "BestSolution->ConstructionCost: " << BestSolution->ConstructionCost << endl;
	cout << "BestSolution->CapacityCost: " << BestSolution->CapacityCost << endl;
	cout << "BestSolution->UnsatisfiedDemandCost: " << BestSolution->UnsatisfiedDemandCost << endl;
	cout << "BestSolution->BatterySwapCost: " << BestSolution->BatterySwapCost << endl;
	cout << "BestSolution->Reposition1: " << BestSolution->Reposition1 << endl;
	cout << "BestSolution->Reposition2: " << BestSolution->Reposition2 << endl;
	cout << "BestSolution->LossDemandCost: " << BestSolution->LossDemandCost << endl;
	cout << "BestSolution->PenaltyCost: " << BestSolution->PenaltyCost << endl;
	cout << "BestSolution->IncentiveCost: " << BestSolution->IncentiveCost << endl;
	cout << "BestSolution->RentalFare: " << BestSolution->RentalFare << endl;
}

//修复解 (将C置于-1，capacity置于0)
void CNetwork::RepairSolution(vector<CSolution*> m_Solution)
{
	//修复solution and capacity
	for (int sol = 0; sol < m_nSolution; sol++)
	{
		for (int node = 0; node < m_nNode; node++) //一个一个点检测
		{
			if (m_Solution[sol]->node_type_upper_level[node][0] == 1)
			{
				m_Solution[sol]->node_type_upper_level[node][0] = -1;
				m_Solution[sol]->node_capacity_upper_level[node][0] = 0;
			}
		}
	}

	for (int sol = 0; sol < m_nSolution; sol++)
	{
		for (int node = 0; node < m_nNode; node++) //一个一个点检测
		{
			if (m_Solution[sol]->node_type_upper_level[node][0] == -1 && m_Solution[sol]->node_type_upper_level[node][1] == -1 && m_Solution[sol]->node_type_upper_level[node][2] == -1)
			{
				m_Solution[sol]->node_type_upper_level[node][2] = 3;
			}
		}
	}

	for (int sol = 0; sol < m_nSolution; sol++)
	{
		for (int node = 0; node < m_nNode; node++) //一个一个点检测
		{
			//cout << m_Solution[sol]->node_type_upper_level[node][2] << endl;
			if (m_Solution[sol]->node_type_upper_level[node][2] == 3)
			{
				//cout << "heelo" << endl;
				if (m_Solution[sol]->node_type_upper_level[node][0] == 1 || m_Solution[sol]->node_type_upper_level[node][1] == 2)
				{
					int y_k = rand() % 2;//产生的随机数对2取余，结果只有0, 1, ，
					//cout << y_k << endl;
					if (y_k == 0) //0的话把前两个关了
					{
						//cout << y_k << endl;
						m_Solution[sol]->node_type_upper_level[node][0] = -1;
						m_Solution[sol]->node_type_upper_level[node][1] = -1;
						m_Solution[sol]->ObjectiveValue = INF;
						m_Solution[sol]->Fitness = 0;
					}
					if (y_k == 1)// 1的话把non - parking node关了
					{
						m_Solution[sol]->node_type_upper_level[node][2] = -1;
						m_Solution[sol]->ObjectiveValue = INF;
						m_Solution[sol]->Fitness = 0;
					}
				}
			}

			if (m_Solution[sol]->node_type_upper_level[node][2] == -1)
			{
				//cout << "heelo" << endl;
				if (m_Solution[sol]->node_type_upper_level[node][0] == -1 && m_Solution[sol]->node_type_upper_level[node][1] == -1)
				{
					int y_k = rand() % 2;//产生的随机数对2取余，结果只有0, 1, ，
					int y_kkk = rand() % 3;
					//cout << y_kkk << "y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:y_kk:" << endl;
					//cout << y_k << endl;
					if (y_k == 0) //N关掉
					{
						int y_kk = (rand() % (2 - 0 + 1)) + 0;;//产生的随机数对2取余，结果只有0, 1, 3

						if (y_kk == 0) //把PC开了
						{
							m_Solution[sol]->node_type_upper_level[node][0] = 1;
							m_Solution[sol]->node_type_upper_level[node][1] = 2;
							int capacity_temp = (rand() % (CapUpper - CapLower + 1)) + CapLower; // capacity随机生成[5, 15]
							m_Solution[sol]->node_capacity_upper_level[node][0] = capacity_temp;
							capacity_temp = (rand() % (CapUpper - CapLower + 1)) + CapLower; // capacity随机生成[5, 15]
							m_Solution[sol]->node_capacity_upper_level[node][1] = capacity_temp;
							m_Solution[sol]->ObjectiveValue = INF;
							m_Solution[sol]->Fitness = 0;
						}
						if (y_kk == 1)//把P开了
						{
							m_Solution[sol]->node_type_upper_level[node][0] = 1;
							int capacity_temp = (rand() % (CapUpper - CapLower + 1)) + CapLower; // capacity随机生成[5, 15]
							m_Solution[sol]->node_capacity_upper_level[node][0] = capacity_temp;
							m_Solution[sol]->ObjectiveValue = INF;
							m_Solution[sol]->Fitness = 0;
						}
						if (y_kk == 2)//把C开了
						{
							m_Solution[sol]->node_type_upper_level[node][1] = 2;
							int capacity_temp = (rand() % (CapUpper - CapLower + 1)) + CapLower; // capacity随机生成[5, 15]
							m_Solution[sol]->node_capacity_upper_level[node][1] = capacity_temp;
							m_Solution[sol]->ObjectiveValue = INF;
							m_Solution[sol]->Fitness = 0;
						}
					}
					if (y_k == 1)// N开着
					{
						m_Solution[sol]->node_type_upper_level[node][2] = 3;
						m_Solution[sol]->ObjectiveValue = INF;
						m_Solution[sol]->Fitness = 0;
					}
				}
			}
		}
	}
}

//运行枚举法
void CNetwork::EnumerationProgram()
{
	BestSolution = new CSolution;
	BestSolution->ObjectiveValue = INF;
	BestSolution->Fitness = 0;
	//枚举出所有的可行solution, 并且随机生成capacity
	EnumerationNodeType();
	CalNodeDistance();

	//枚举5nodes用一下
	//m_Solution[40]->node_capacity_upper_level[0][0] = 5;
	//m_Solution[40]->node_capacity_upper_level[1][0] = 0;
	//m_Solution[40]->node_capacity_upper_level[2][0] = 0;
	//m_Solution[40]->node_capacity_upper_level[3][0] = 5;
	//m_Solution[40]->node_capacity_upper_level[4][0] = 5;

	//m_Solution[0]->node_type_upper_level[0][0] = 1;
	//m_Solution[0]->node_type_upper_level[0][1] = -1;
	//m_Solution[0]->node_type_upper_level[0][2] = -1;

	//m_Solution[0]->node_type_upper_level[1][0] = -1;
	//m_Solution[0]->node_type_upper_level[1][1] = -1;
	//m_Solution[0]->node_type_upper_level[1][2] = 3;

	//m_Solution[0]->node_type_upper_level[2][0] = -1;
	//m_Solution[0]->node_type_upper_level[2][1] = -1;
	//m_Solution[0]->node_type_upper_level[2][2] = 3;

	//m_Solution[0]->node_type_upper_level[3][0] = 1;
	//m_Solution[0]->node_type_upper_level[3][1] = -1;
	//m_Solution[0]->node_type_upper_level[3][2] = -1;

	//m_Solution[0]->node_type_upper_level[4][0] = 1;
	//m_Solution[0]->node_type_upper_level[4][1] = -1;
	//m_Solution[0]->node_type_upper_level[4][2] = -1;

	//m_Solution[40]->node_capacity_upper_level[0][0] = 5;
	//m_Solution[40]->node_capacity_upper_level[1][0] = 0;
	//m_Solution[40]->node_capacity_upper_level[2][0] = 0;
	//m_Solution[40]->node_capacity_upper_level[3][0] = 5;
	//m_Solution[40]->node_capacity_upper_level[4][0] = 5;
	//RepairSolution(m_Solution);

	//m_Solution[0]->node_type_upper_level[0][0] = -1;
	//m_Solution[0]->node_type_upper_level[0][1] = -1;
	//m_Solution[0]->node_type_upper_level[0][2] = 3;

	//m_Solution[0]->node_type_upper_level[1][0] = -1;
	//m_Solution[0]->node_type_upper_level[1][1] = -1;
	//m_Solution[0]->node_type_upper_level[1][2] = 3;

	//m_Solution[0]->node_type_upper_level[2][0] = 1;
	//m_Solution[0]->node_type_upper_level[2][1] = -1;
	//m_Solution[0]->node_type_upper_level[2][2] = -1;

	//m_Solution[0]->node_type_upper_level[3][0] = -1;
	//m_Solution[0]->node_type_upper_level[3][1] = 2;
	//m_Solution[0]->node_type_upper_level[3][2] = -1;

	//m_Solution[0]->node_capacity_upper_level[0][0] = 0;
	//m_Solution[0]->node_capacity_upper_level[1][0] = 0;
	//m_Solution[0]->node_capacity_upper_level[2][0] = 6;
	//m_Solution[0]->node_capacity_upper_level[3][1] = 9;

	//m_Solution[10]->node_type_upper_level[0][0] = -1;
	//m_Solution[10]->node_type_upper_level[0][1] = -1;
	//m_Solution[10]->node_type_upper_level[0][2] = 3;

	//m_Solution[10]->node_type_upper_level[1][0] = 1;
	//m_Solution[10]->node_type_upper_level[1][1] = -1;
	//m_Solution[10]->node_type_upper_level[1][2] = -1;

	//m_Solution[10]->node_type_upper_level[2][0] = -1;
	//m_Solution[10]->node_type_upper_level[2][1] = 2;
	//m_Solution[10]->node_type_upper_level[2][2] = -1;

	//m_Solution[10]->node_type_upper_level[3][0] = -1;
	//m_Solution[10]->node_type_upper_level[3][1] = -1;
	//m_Solution[10]->node_type_upper_level[3][2] = 3;

	//m_Solution[10]->node_capacity_upper_level[0][0] = 0;
	//m_Solution[10]->node_capacity_upper_level[1][0] = 5;
	//m_Solution[10]->node_capacity_upper_level[2][1] = 5;
	//m_Solution[10]->node_capacity_upper_level[3][1] = 0;
	//RepairSolution(m_Solution);

	//m_Solution[1]->node_type_upper_level[0][0] = -1;
	//m_Solution[1]->node_type_upper_level[0][1] = -1;
	//m_Solution[1]->node_type_upper_level[0][2] = 3;

	//m_Solution[1]->node_type_upper_level[1][0] = 1;
	//m_Solution[1]->node_type_upper_level[1][1] = -1;
	//m_Solution[1]->node_type_upper_level[1][2] = -1;

	//m_Solution[1]->node_type_upper_level[2][0] = -1;
	//m_Solution[1]->node_type_upper_level[2][1] = 2;
	//m_Solution[1]->node_type_upper_level[2][2] = -1;

	//m_Solution[1]->node_type_upper_level[3][0] = -1;
	//m_Solution[1]->node_type_upper_level[3][1] = -1;
	//m_Solution[1]->node_type_upper_level[3][2] = 3;

	//m_Solution[1]->node_capacity_upper_level[0][1] = 0;
	//m_Solution[1]->node_capacity_upper_level[1][0] = 5;
	//m_Solution[1]->node_capacity_upper_level[2][1] = 5;
	//m_Solution[1]->node_capacity_upper_level[3][0] = 0;
	//RepairSolution(m_Solution);

	//m_Solution[10]->node_type_upper_level[0][0] = -1;
	//m_Solution[10]->node_type_upper_level[0][1] = -1;
	//m_Solution[10]->node_type_upper_level[0][2] = 3;

	//m_Solution[10]->node_type_upper_level[1][0] = 1;
	//m_Solution[10]->node_type_upper_level[1][1] = -1;
	//m_Solution[10]->node_type_upper_level[1][2] = -1;

	//m_Solution[10]->node_type_upper_level[2][0] = -1;
	//m_Solution[10]->node_type_upper_level[2][1] = 2;
	//m_Solution[10]->node_type_upper_level[2][2] = -1;

	//m_Solution[10]->node_type_upper_level[3][0] = -1;
	//m_Solution[10]->node_type_upper_level[3][1] = -1;
	//m_Solution[10]->node_type_upper_level[3][2] = 3;

	//m_Solution[10]->node_capacity_upper_level[0][0] = 0;
	//m_Solution[10]->node_capacity_upper_level[1][0] = 5;
	//m_Solution[10]->node_capacity_upper_level[2][1] = 5;
	//m_Solution[10]->node_capacity_upper_level[3][1] = 0;
	//RepairSolution(m_Solution);
	RepairSolution(m_Solution);
	//cout << "Solution: " << m_nSolution << endl;
	//Lower-level
	for (int sol = 0; sol < m_nSolution; sol++)
		//for (int sol = 0; sol < 11; sol++)
	{

		NodeType(sol, m_Solution); //根据初始生成的solution, 为每个node赋值

		//if (m_Solution[sol]->node_type_upper_level[0][0] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[0][1] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[0][2] == 3 &&

		//	m_Solution[sol]->node_type_upper_level[1][0] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[1][1] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[1][2] == 3 &&

		//	m_Solution[sol]->node_type_upper_level[2][0] == 1 &&
		//	m_Solution[sol]->node_type_upper_level[2][1] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[2][2] == -1 &&

		//	m_Solution[sol]->node_type_upper_level[3][0] == -1 &&
		//	m_Solution[sol]->node_type_upper_level[3][1] == 2 &&
		//	m_Solution[sol]->node_type_upper_level[3][2] == -1

		//	//m_Solution[sol]->node_type_upper_level[4][0] == 1 &&
		//	//m_Solution[sol]->node_type_upper_level[4][1] == -1 &&
		//	//m_Solution[sol]->node_type_upper_level[4][2] == -1
		//	)
		//{
		//	//看下当前的solution配置
		//	for (int node = 0; node < m_nNode; node++)
		//	{
		//		for (int type = 0; type < 3; type++)
		//		{
		//			cout << m_Solution[sol]->node_type_upper_level[node][type] << " ";
		//		}
		//		cout << endl;
		//	}
		//	cout << endl;
		//	//看下当前的cap配置
		//	for (int node = 0; node < m_nNode; node++)
		//	{
		//		for (int type = 0; type < 3; type++)
		//		{
		//			cout << m_Solution[sol]->node_capacity_upper_level[node][type] << " ";
		//		}
		//		cout << endl;
		//	}
		//	cout << endl;
		//}

		////看下当前的solution配置
		//for (int node = 0; node < m_nNode; node++)
		//{
		//	for (int type = 0; type < 3; type++)
		//	{
		//		cout << m_Solution[sol]->node_type_upper_level[node][type] << " ";
		//	}
		//	cout << endl;
		//}
		//cout << endl;
		////看下当前的cap配置
		//for (int node = 0; node < m_Nnode; node++)
		//{
		//	for (int type = 0; type < 3; type++)
		//	{
		//		cout << m_Solution[sol]->node_capacity_upper_level[node][type] << " ";
		//	}
		//	cout << endl;
		//}
		//cout << endl;
		cout << "sol: " << sol << endl;
		temp_do = 1; //标识符, 如果每对OD至少都有一条Route, 那么为1, 如果有任何一条OD没有Route, 为0
		//释放内存
		for (auto it = m_Route.begin(); it != m_Route.end(); it++)
		{
			if (*it != NULL)
			{
				delete* it;
				*it = NULL;
			}
		}
		GenerateRoute();//为每一对OD生成route

		//cout << m_nRoute << endl;
		if (temp_do != 0)
		{

			////////////////////////////////////////UE////////////////////////////////////////
			// 初始化部分：
			double k = 1; // 迭代次数
			RouteFlow = new double[m_nRoute];
			RouteFlow_K1 = new double[m_nRoute];
			ANRouteFlow = new double[m_nRoute];
			ANRouteFlow_K1 = new double[m_nRoute];
			beita = 0;
			for (int route = 0; route < m_nRoute; route++)
			{
				RouteFlow[route] = 0; // 初始化所有route流量为0
				RouteFlow_K1[route] = 0;
				ANRouteFlow[route] = 0;
				ANRouteFlow_K1[route] = 0;
			}
			for (int node = 0; node < m_nNode; node++)
			{
				for (int type = 0; type < 3; type++)
				{
					m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
					m_Node[node]->Dropoffflow[type] = 0;
				}
			}
			AllorNothingAssignment(RouteFlow, ANRouteFlow); // 做全有全无分配，得到辅助路段流量 (zero-flow)
			// 生成辅助flow
			double* newRouteFlow = new double[m_nRoute];
			AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

			//PrintUERouteFlow(UEGap, newRouteFlow);
			//// 更新路段流量 (MSA)
			//{
			//	for (int route = 0; route < m_nRoute; route++)
			//	{
			//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
			//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
			//	}
			//	delete newRouteFlow;
			//}

			// 更新路段流量 (SRAM)
			{
				RouteFlow_K1; // k-1
				ANRouteFlow_K1; // k-1
				RouteFlow; //当前flow k=1
				newRouteFlow; //新flow k=1
				double GAP_k = 0;
				double GAP_k1 = 0;
				for (int route = 0; route < m_nRoute; route++)
				{
					GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
					GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
				}
				if (GAP_k >= GAP_k1)
				{
					beita += Tao;
					//beita = k;
				}
				else
				{
					beita += gama;
					//beita = k;
				}
				for (int route = 0; route < m_nRoute; route++)
				{
					//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
					RouteFlow_K1[route] = RouteFlow[route];
					ANRouteFlow_K1[route] = newRouteFlow[route];
					RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
				}
				//cout << "beita: " << beita << endl;
				delete newRouteFlow;
			}

			// 更新picku_Up和drop_off点的flow
			{
				for (int node = 0; node < m_nNode; node++)
				{
					for (int type = 0; type < 3; type++)
					{
						m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
						m_Node[node]->Dropoffflow[type] = 0;
					}
				}
				for (int route = 0; route < m_nRoute; route++)
				{
					int Pickuptype = m_Route[route]->PickupType; //1, 2
					//cout << "Pickuptype：" << Pickuptype << endl;
					int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
					//cout << "Dropofftype：" << Dropofftype << endl;
					if (Pickuptype != -1 && Dropofftype != -1)
					{
						m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
						m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
					}
				}
				k += 1;
			}

			// Calculate UEGap
			{
				UEGap = 0;
				double temp1 = 0;
				double temp2 = 0;
				for (int route = 0; route < m_nRoute; route++)
				{
					temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
					temp2 += RouteFlow_K1[route];
				}
				UEGap = sqrt(temp1) / temp2;
			}

			while (UEGap > MaxUEGap)
			{
				// 生成辅助flow
				double* newRouteFlow = new double[m_nRoute];
				AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

				//// 更新路段流量 (MSA)
				//{
				//	for (int route = 0; route < m_nRoute; route++)
				//	{
				//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
				//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
				//	}
				//	delete newRouteFlow;
				//}

				// 更新路段流量 (SRAM)
				{
					double GAP_k = 0;
					double GAP_k1 = 0;
					for (int route = 0; route < m_nRoute; route++)
					{
						GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
						GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
					}
					if (GAP_k >= GAP_k1)
					{
						beita += Tao;
						//beita = k;
					}
					else
					{
						beita += gama;
						//beita = k;
					}
					for (int route = 0; route < m_nRoute; route++)
					{
						//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
						RouteFlow_K1[route] = RouteFlow[route];
						ANRouteFlow_K1[route] = newRouteFlow[route];
						RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
					}
					//cout << "beita: " << beita << endl;
					delete newRouteFlow;
				}

				// 更新picku_Up和drop_off点的flow
				{
					for (int node = 0; node < m_nNode; node++)
					{
						for (int type = 0; type < 3; type++)
						{
							m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
							m_Node[node]->Dropoffflow[type] = 0;
						}
					}
					for (int route = 0; route < m_nRoute; route++)
					{
						int Pickuptype = m_Route[route]->PickupType; //1, 2
						//cout << "Pickuptype：" << Pickuptype << endl;
						int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
						//cout << "Dropofftype：" << Dropofftype << endl;
						if (Pickuptype != -1 && Dropofftype != -1)
						{
							m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
							m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
						}
					}
					k += 1;
				}

				// Calculate UEGap
				{
					UEGap = 0;
					double temp1 = 0;
					double temp2 = 0;
					for (int route = 0; route < m_nRoute; route++)
					{
						temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
						temp2 += RouteFlow_K1[route];
					}
					UEGap = sqrt(temp1) / temp2;
				}
			}

			//Calculate the objective value
			m_Solution[sol]->ObjectiveValue = cal_obj(sol, m_Solution);

			//PrintUERouteFlow(UEGap, RouteFlow);
			//PrintUERouteCost();

			delete RouteFlow;
			delete RouteFlow_K1;
			delete ANRouteFlow;
			delete ANRouteFlow_K1;
			RecordBest(sol, m_Solution);
			////////////////////////////////////////UE////////////////////////////////////////

			//Update Capacity (armijio method, integer capacity)
			m_Solution[sol]->node_capacity_upper_level_temp = m_Solution[sol]->node_capacity_upper_level;
			for (int node = 0; node < m_nNode; node++)
			{
				for (int type = 0; type < 2; type++)
				{
					//cout << "我在更新capacity: " << node << " " << type << endl;
					if (m_Solution[sol]->node_type_upper_level[node][type] > 0) //这个node的type是开放的
					{
						//cout << "我在更新capacity2: " << node << " " << type << endl;
						while (true) //Capacity增加1
						{
							m_Solution[sol]->node_capacity_upper_level[node][type]++; //Armijio serch先+1
							if (m_Solution[sol]->node_capacity_upper_level[node][type] > CapUpper)
							{
								m_Solution[sol]->node_capacity_upper_level[node][type]--;
								double temp = m_Solution[sol]->node_capacity_upper_level[node][type];
								m_Solution[sol]->node_capacity_upper_level = m_Solution[sol]->node_capacity_upper_level_temp;
								m_Solution[sol]->node_capacity_upper_level_temp[node][type] = temp;
								//cout << "hello" << endl;
								break;
							}
							NodeType(sol, m_Solution);

							////////////////////////////////////////UE////////////////////////////////////////
							// 初始化部分：
							RouteFlow = new double[m_nRoute];
							RouteFlow_K1 = new double[m_nRoute];
							ANRouteFlow = new double[m_nRoute];
							ANRouteFlow_K1 = new double[m_nRoute];
							beita = 0;
							for (int route = 0; route < m_nRoute; route++)
							{
								RouteFlow[route] = 0; // 初始化所有route流量为0
								RouteFlow_K1[route] = 0;
								ANRouteFlow[route] = 0;
								ANRouteFlow_K1[route] = 0;
							}
							for (int node = 0; node < m_nNode; node++)
							{
								for (int type = 0; type < 3; type++)
								{
									m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
									m_Node[node]->Dropoffflow[type] = 0;
								}
							}
							AllorNothingAssignment(RouteFlow, ANRouteFlow); // 做全有全无分配，得到辅助路段流量 (zero-flow)
							// 生成辅助flow
							newRouteFlow = new double[m_nRoute];
							AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

							//PrintUERouteFlow(UEGap, newRouteFlow);
							//// 更新路段流量 (MSA)
							//{
							//	for (int route = 0; route < m_nRoute; route++)
							//	{
							//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
							//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
							//	}
							//	delete newRouteFlow;
							//}

							// 更新路段流量 (SRAM)
							{
								double GAP_k = 0;
								double GAP_k1 = 0;
								for (int route = 0; route < m_nRoute; route++)
								{
									GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
									GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
								}
								if (GAP_k >= GAP_k1)
								{
									beita += Tao;
									//beita = k;
								}
								else
								{
									beita += gama;
									//beita = k;
								}
								for (int route = 0; route < m_nRoute; route++)
								{
									//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
									RouteFlow_K1[route] = RouteFlow[route];
									ANRouteFlow_K1[route] = newRouteFlow[route];
									RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
								}
								//cout << "beita: " << beita << endl;
								delete newRouteFlow;
							}

							// 更新picku_Up和drop_off点的flow
							{
								for (int node = 0; node < m_nNode; node++)
								{
									for (int type = 0; type < 3; type++)
									{
										m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
										m_Node[node]->Dropoffflow[type] = 0;
									}
								}
								for (int route = 0; route < m_nRoute; route++)
								{
									int Pickuptype = m_Route[route]->PickupType; //1, 2
									//cout << "Pickuptype：" << Pickuptype << endl;
									int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
									//cout << "Dropofftype：" << Dropofftype << endl;
									if (Pickuptype != -1 && Dropofftype != -1)
									{
										m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
										m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
									}
								}
								k += 1;
							}

							// Calculate UEGap
							{
								UEGap = 0;
								double temp1 = 0;
								double temp2 = 0;
								for (int route = 0; route < m_nRoute; route++)
								{
									temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
									temp2 += RouteFlow_K1[route];
								}
								UEGap = sqrt(temp1) / temp2;
							}

							while (UEGap > MaxUEGap)
							{
								// 生成辅助flow
								double* newRouteFlow = new double[m_nRoute];
								AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

								//// 更新路段流量 (MSA)
								//{
								//	for (int route = 0; route < m_nRoute; route++)
								//	{
								//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
								//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
								//	}
								//	delete newRouteFlow;
								//}

								// 更新路段流量 (SRAM)
								{
									double GAP_k = 0;
									double GAP_k1 = 0;
									for (int route = 0; route < m_nRoute; route++)
									{
										GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
										GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
									}
									if (GAP_k >= GAP_k1)
									{
										beita += Tao;
										//beita = k;
									}
									else
									{
										beita += gama;
										//beita = k;
									}
									for (int route = 0; route < m_nRoute; route++)
									{
										//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
										RouteFlow_K1[route] = RouteFlow[route];
										ANRouteFlow_K1[route] = newRouteFlow[route];
										RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
									}
									//cout << "beita: " << beita << endl;
									delete newRouteFlow;
								}

								// 更新picku_Up和drop_off点的flow
								{
									for (int node = 0; node < m_nNode; node++)
									{
										for (int type = 0; type < 3; type++)
										{
											m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
											m_Node[node]->Dropoffflow[type] = 0;
										}
									}
									for (int route = 0; route < m_nRoute; route++)
									{
										int Pickuptype = m_Route[route]->PickupType; //1, 2
										//cout << "Pickuptype：" << Pickuptype << endl;
										int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
										//cout << "Dropofftype：" << Dropofftype << endl;
										if (Pickuptype != -1 && Dropofftype != -1)
										{
											m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
											m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
										}
									}
									k += 1;
								}

								// Calculate UEGap
								{
									UEGap = 0;
									double temp1 = 0;
									double temp2 = 0;
									for (int route = 0; route < m_nRoute; route++)
									{
										temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
										temp2 += RouteFlow_K1[route];
									}
									UEGap = sqrt(temp1) / temp2;
								}
							}
							////////////////////////////////////////UE////////////////////////////////////////

							m_Solution[sol]->ObjectiveValue_temp = cal_obj(sol, m_Solution);
							if (m_Solution[sol]->ObjectiveValue_temp >= m_Solution[sol]->ObjectiveValue)
							{
								m_Solution[sol]->node_capacity_upper_level[node][type]--;
								double temp = m_Solution[sol]->node_capacity_upper_level[node][type];
								m_Solution[sol]->node_capacity_upper_level = m_Solution[sol]->node_capacity_upper_level_temp;
								m_Solution[sol]->node_capacity_upper_level_temp[node][type] = temp;
								//cout << "hello" << endl;
								break;
							}
							if (m_Solution[sol]->ObjectiveValue_temp < m_Solution[sol]->ObjectiveValue)
							{
								m_Solution[sol]->ObjectiveValue = m_Solution[sol]->ObjectiveValue_temp;
							}
							delete RouteFlow;
							delete RouteFlow_K1;
							delete ANRouteFlow;
							delete ANRouteFlow_K1;
						}
						while (true)//Capacity减少1
						{
							m_Solution[sol]->node_capacity_upper_level[node][type]--; //Armijio serch先-1
							if (m_Solution[sol]->node_capacity_upper_level[node][type] < CapLower)
							{
								m_Solution[sol]->node_capacity_upper_level[node][type]++;
								m_Solution[sol]->node_capacity_upper_level[node][type] = m_Solution[sol]->node_capacity_upper_level_temp[node][type];
								break;
							}
							NodeType(sol, m_Solution);

							////////////////////////////////////////UE////////////////////////////////////////
							// 初始化部分：
							RouteFlow = new double[m_nRoute];
							RouteFlow_K1 = new double[m_nRoute];
							ANRouteFlow = new double[m_nRoute];
							ANRouteFlow_K1 = new double[m_nRoute];
							beita = 0;
							for (int route = 0; route < m_nRoute; route++)
							{
								RouteFlow[route] = 0; // 初始化所有route流量为0
								RouteFlow_K1[route] = 0;
								ANRouteFlow[route] = 0;
								ANRouteFlow_K1[route] = 0;
							}
							for (int node = 0; node < m_nNode; node++)
							{
								for (int type = 0; type < 3; type++)
								{
									m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
									m_Node[node]->Dropoffflow[type] = 0;
								}
							}
							AllorNothingAssignment(RouteFlow, ANRouteFlow); // 做全有全无分配，得到辅助路段流量 (zero-flow)
							// 生成辅助flow
							newRouteFlow = new double[m_nRoute];
							AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

							//PrintUERouteFlow(UEGap, newRouteFlow);
							//// 更新路段流量 (MSA)
							//{
							//	for (int route = 0; route < m_nRoute; route++)
							//	{
							//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
							//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
							//	}
							//	delete newRouteFlow;
							//}

							// 更新路段流量 (SRAM)
							{
								double GAP_k = 0;
								double GAP_k1 = 0;
								for (int route = 0; route < m_nRoute; route++)
								{
									GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
									GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
								}
								if (GAP_k >= GAP_k1)
								{
									beita += Tao;
									//beita = k;
								}
								else
								{
									beita += gama;
									//beita = k;
								}
								for (int route = 0; route < m_nRoute; route++)
								{
									//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
									RouteFlow_K1[route] = RouteFlow[route];
									ANRouteFlow_K1[route] = newRouteFlow[route];
									RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
								}
								//cout << "beita: " << beita << endl;
								delete newRouteFlow;
							}

							// 更新picku_Up和drop_off点的flow
							{
								for (int node = 0; node < m_nNode; node++)
								{
									for (int type = 0; type < 3; type++)
									{
										m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
										m_Node[node]->Dropoffflow[type] = 0;
									}
								}
								for (int route = 0; route < m_nRoute; route++)
								{
									int Pickuptype = m_Route[route]->PickupType; //1, 2
									//cout << "Pickuptype：" << Pickuptype << endl;
									int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
									//cout << "Dropofftype：" << Dropofftype << endl;
									if (Pickuptype != -1 && Dropofftype != -1)
									{
										m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
										m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
									}
								}
								k += 1;
							}

							// Calculate UEGap
							{
								UEGap = 0;
								double temp1 = 0;
								double temp2 = 0;
								for (int route = 0; route < m_nRoute; route++)
								{
									temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
									temp2 += RouteFlow_K1[route];
								}
								UEGap = sqrt(temp1) / temp2;
							}

							while (UEGap > MaxUEGap)
							{
								// 生成辅助flow
								double* newRouteFlow = new double[m_nRoute];
								AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

								//// 更新路段流量 (MSA)
								//{
								//	for (int route = 0; route < m_nRoute; route++)
								//	{
								//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
								//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
								//	}
								//	delete newRouteFlow;
								//}

								// 更新路段流量 (SRAM)
								{
									double GAP_k = 0;
									double GAP_k1 = 0;
									for (int route = 0; route < m_nRoute; route++)
									{
										GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
										GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
									}
									if (GAP_k >= GAP_k1)
									{
										beita += Tao;
										//beita = k;
									}
									else
									{
										beita += gama;
										//beita = k;
									}
									for (int route = 0; route < m_nRoute; route++)
									{
										//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
										RouteFlow_K1[route] = RouteFlow[route];
										ANRouteFlow_K1[route] = newRouteFlow[route];
										RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
									}
									//cout << "beita: " << beita << endl;
									delete newRouteFlow;
								}

								// 更新picku_Up和drop_off点的flow
								{
									for (int node = 0; node < m_nNode; node++)
									{
										for (int type = 0; type < 3; type++)
										{
											m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
											m_Node[node]->Dropoffflow[type] = 0;
										}
									}
									for (int route = 0; route < m_nRoute; route++)
									{
										int Pickuptype = m_Route[route]->PickupType; //1, 2
										//cout << "Pickuptype：" << Pickuptype << endl;
										int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
										//cout << "Dropofftype：" << Dropofftype << endl;
										if (Pickuptype != -1 && Dropofftype != -1)
										{
											m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
											m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
										}
									}
									k += 1;
								}

								// Calculate UEGap
								{
									UEGap = 0;
									double temp1 = 0;
									double temp2 = 0;
									for (int route = 0; route < m_nRoute; route++)
									{
										temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
										temp2 += RouteFlow_K1[route];
									}
									UEGap = sqrt(temp1) / temp2;
								}
							}
							////////////////////////////////////////UE////////////////////////////////////////

							m_Solution[sol]->ObjectiveValue_temp = cal_obj(sol, m_Solution);
							if (m_Solution[sol]->ObjectiveValue_temp >= m_Solution[sol]->ObjectiveValue)
							{
								m_Solution[sol]->node_capacity_upper_level[node][type] = m_Solution[sol]->node_capacity_upper_level_temp[node][type];
								break;
							}
							if (m_Solution[sol]->ObjectiveValue_temp < m_Solution[sol]->ObjectiveValue)
							{
								m_Solution[sol]->ObjectiveValue = m_Solution[sol]->ObjectiveValue_temp;
								m_Solution[sol]->node_capacity_upper_level_temp[node][type] = m_Solution[sol]->node_capacity_upper_level[node][type];
							}
							delete RouteFlow;
							delete RouteFlow_K1;
							delete ANRouteFlow;
							delete ANRouteFlow_K1;
							//cout << m_Solution[sol]->ObjectiveValue << endl;
						}
					}
				}
			}

			NodeType(sol, m_Solution);

			////////////////////////////////////////UE////////////////////////////////////////
			// 初始化部分：
			RouteFlow = new double[m_nRoute];
			RouteFlow_K1 = new double[m_nRoute];
			ANRouteFlow = new double[m_nRoute];
			ANRouteFlow_K1 = new double[m_nRoute];
			beita = 0;
			for (int route = 0; route < m_nRoute; route++)
			{
				RouteFlow[route] = 0; // 初始化所有route流量为0
				RouteFlow_K1[route] = 0;
				ANRouteFlow[route] = 0;
				ANRouteFlow_K1[route] = 0;
			}
			for (int node = 0; node < m_nNode; node++)
			{
				for (int type = 0; type < 3; type++)
				{
					m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
					m_Node[node]->Dropoffflow[type] = 0;
				}
			}
			AllorNothingAssignment(RouteFlow, ANRouteFlow); // 做全有全无分配，得到辅助路段流量 (zero-flow)
			// 生成辅助flow
			newRouteFlow = new double[m_nRoute];
			AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

			//PrintUERouteFlow(UEGap, newRouteFlow);
			//// 更新路段流量 (MSA)
			//{
			//	for (int route = 0; route < m_nRoute; route++)
			//	{
			//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
			//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
			//	}
			//	delete newRouteFlow;
			//}

			// 更新路段流量 (SRAM)
			{
				double GAP_k = 0;
				double GAP_k1 = 0;
				for (int route = 0; route < m_nRoute; route++)
				{
					GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
					GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
				}
				if (GAP_k >= GAP_k1)
				{
					beita += Tao;
					//beita = k;
				}
				else
				{
					beita += gama;
					//beita = k;
				}
				for (int route = 0; route < m_nRoute; route++)
				{
					//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
					RouteFlow_K1[route] = RouteFlow[route];
					ANRouteFlow_K1[route] = newRouteFlow[route];
					RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
				}
				//cout << "beita: " << beita << endl;
				delete newRouteFlow;
			}

			// 更新picku_Up和drop_off点的flow
			{
				for (int node = 0; node < m_nNode; node++)
				{
					for (int type = 0; type < 3; type++)
					{
						m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
						m_Node[node]->Dropoffflow[type] = 0;
					}
				}
				for (int route = 0; route < m_nRoute; route++)
				{
					int Pickuptype = m_Route[route]->PickupType; //1, 2
					//cout << "Pickuptype：" << Pickuptype << endl;
					int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
					//cout << "Dropofftype：" << Dropofftype << endl;
					if (Pickuptype != -1 && Dropofftype != -1)
					{
						m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
						m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
					}
				}
				k += 1;
			}

			// Calculate UEGap
			{
				UEGap = 0;
				double temp1 = 0;
				double temp2 = 0;
				for (int route = 0; route < m_nRoute; route++)
				{
					temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
					temp2 += RouteFlow_K1[route];
				}
				UEGap = sqrt(temp1) / temp2;
			}

			while (UEGap > MaxUEGap)
			{
				// 生成辅助flow
				double* newRouteFlow = new double[m_nRoute];
				AllorNothingAssignment(newRouteFlow, ANRouteFlow); //生成了辅助flowk

				//// 更新路段流量 (MSA)
				//{
				//	for (int route = 0; route < m_nRoute; route++)
				//	{
				//		//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
				//		RouteFlow[route] = RouteFlow[route] + (1 / k) * (newRouteFlow[route] - RouteFlow[route]);
				//	}
				//	delete newRouteFlow;
				//}

				// 更新路段流量 (SRAM)
				{
					double GAP_k = 0;
					double GAP_k1 = 0;
					for (int route = 0; route < m_nRoute; route++)
					{
						GAP_k += abs(RouteFlow[route] - newRouteFlow[route]);
						GAP_k1 += abs(RouteFlow_K1[route] - ANRouteFlow_K1[route]);
					}
					if (GAP_k >= GAP_k1)
					{
						beita += Tao;
						//beita = k;
					}
					else
					{
						beita += gama;
						//beita = k;
					}
					for (int route = 0; route < m_nRoute; route++)
					{
						//RouteFlow[route] = (k / (k + 1)) * RouteFlow[route] + (1 / (k + 1)) * newRouteFlow[route];
						RouteFlow_K1[route] = RouteFlow[route];
						ANRouteFlow_K1[route] = newRouteFlow[route];
						RouteFlow[route] = RouteFlow[route] + (1 / beita) * (newRouteFlow[route] - RouteFlow[route]);
					}
					//cout << "beita: " << beita << endl;
					delete newRouteFlow;
				}

				// 更新picku_Up和drop_off点的flow
				{
					for (int node = 0; node < m_nNode; node++)
					{
						for (int type = 0; type < 3; type++)
						{
							m_Node[node]->Pickupflow[type] = 0; // 初始化所有node的流量为0
							m_Node[node]->Dropoffflow[type] = 0;
						}
					}
					for (int route = 0; route < m_nRoute; route++)
					{
						int Pickuptype = m_Route[route]->PickupType; //1, 2
						//cout << "Pickuptype：" << Pickuptype << endl;
						int Dropofftype = m_Route[route]->DropoffType; //1, 2, 3
						//cout << "Dropofftype：" << Dropofftype << endl;
						if (Pickuptype != -1 && Dropofftype != -1)
						{
							m_Route[route]->PickupNode->Pickupflow[Pickuptype - 1] += RouteFlow[route];
							m_Route[route]->DropoffNode->Dropoffflow[Dropofftype - 1] += RouteFlow[route];
						}
					}
					k += 1;
				}

				// Calculate UEGap
				{
					UEGap = 0;
					double temp1 = 0;
					double temp2 = 0;
					for (int route = 0; route < m_nRoute; route++)
					{
						temp1 += (RouteFlow[route] - RouteFlow_K1[route]) * (RouteFlow[route] - RouteFlow_K1[route]);
						temp2 += RouteFlow_K1[route];
					}
					UEGap = sqrt(temp1) / temp2;
				}
			}

			//Calculate the objective value
			m_Solution[sol]->ObjectiveValue = cal_obj(sol, m_Solution);

			//PrintUERouteFlow(UEGap, RouteFlow);
			//PrintUERouteCost();

			delete RouteFlow;
			delete RouteFlow_K1;
			delete ANRouteFlow;
			delete ANRouteFlow_K1;
			RecordBest(sol, m_Solution);
			////////////////////////////////////////UE////////////////////////////////////////
		}
		if (temp_do == 0)
		{
			cout << "System does not work because no pick-up node!!!" << endl; //sol = 170
		}
	}

	//输出全局最优solution
	PrintBestSolution();
}