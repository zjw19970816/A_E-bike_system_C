#pragma once
#include"CNode.h"
#include"CLink.h"
#include"COrigin.h"
#include"CRoute.h"
#include"CSolution.h"
#include<vector>
#include<string>
#include <fstream>
#include <cstring>
using namespace std;

#define INF 0x3f3f3f3f //Infinite number

class CNetwork
{
public:

	////Upper-level model
	// Enumeration
	string str = "";
	//所有possible的type
	int node_type[4][3] = { {1, -1, -1}, {-1, 2, -1}, {-1, -1, 3}, {1, 2, -1 } }; //一个node，可能的状态的所有情况
	double node_capacity[3] = { 10,10,10 };

	vector<CSolution*> m_Solution; //Solution set
	vector<CSolution*> m_Solution2; //Solution set
	CSolution* BestSolution;
	vector<CNode*> m_Node; //网络节点集合
	vector<CNode*> m_Depot; //网络节点集合
	vector<CLink*> m_Link; //网络路段集合
	vector<CLink*> m_DepotNode;
	vector<COrigin*> m_Origin; //网络的起点集合
	vector<CRoute*> m_Route; //网络的route集合
	int m_nSolution;
	int m_nNode; //节点数
	int m_nDepot; //节点数
	int m_nLink; //路段数量
	int m_nDepotNode;
	int m_nOrigin; //起点数量
	int m_nODpair; //Number of OD pair
	int m_nRoute; //route数量
	double** NodeDistance; //任何两点的distance
	double* RouteFlow; //路段流量指针
	double* RouteFlow_K1; //路段流量指针k-1的
	double* ANRouteFlow; // 辅助Route flow
	double* ANRouteFlow_K1; // 辅助Route flow k-1的
	double beita;// SRAM更新的step size
	double Tao = 1.75; //8
	double gama = 0.25; //1.5
	vector<double> LinkTravelTime; //路段走行时间
	//double MaxUEGap = 1.0e-4; //UE的最大误差 
	double MaxUEGap = 1.0e-4; //UE的最大误差 
	double UEGap; //UE误差
	double* ShortestPathCost;//临时变量，所有节点到起点的最短路 free
	int* ShortestPathParent;//最短路上，所有节点到起点的在最短路上的前继路段  free
	int temp_do;
	int n_swap;
#define NP 10 // The number of food sources equals the half of the colony size
#define FoodNumber NP/2 // The number of food sources equals the half of the colony size
#define maxCycle 500 //The number of cycles for foraging {a stopping criteria}*/
#define crossover_rate 0.3 //Crossover rate 0.1
#define mutation_rate 0.2 //Mutation rate 0.2
#define time_interval 0.25 //parking node service time
#define average_charging_time 2.0 //charging plie average charging time
#define ValueOfWalkingTime 2.0
#define WalkingSpeed 4.0
#define ValueOfRidingTime 1
#define ValueOfWaitingTime 1
#define CapUpper 15
#define CapLower 5
#define ArtificialRoute 10000 //虚拟路的cost
	const double arfa3 = 21;//Battery consumption coefficient
	const double E_max = 672;//Maximum battery capacity
	const double days = 10; //一天运行10小时

	//可调参数
	double EnvironmantalPenalty = 100;//Environment penalty effect 100
	double ConstructionCost[2] = { 10, 20 }; //P和C的construction cost, N的为0   5 10 maintenance cost
	double UnitCapCostPC[2] = { 5, 10 }; //Capacity cost, 5,10
	double xita1 = 1; //Unsatisfied and loss demand cost 1
	double xita5 = 15; //battery swap的价格是10
	double xita6_1 = 1; //relocation for P and C 1
	double xita6_2 = 2; //relocation for N 2
	double incentive = 5; //停到C的奖励 5
	double penalty = 10; //停到N的惩罚 10
	double b_be_2 = 8;//unit rental fare 8
	CNetwork(double EnPe, double PaM, double CaM, double PaC, double CaC, double xit1, double xit5, double xit6_1, double xit6_2, double incen, double penal, double b_be2, double tao, double gam)
	{
		EnvironmantalPenalty = EnPe;
		ConstructionCost[0] = PaM;
		ConstructionCost[1] = CaM;
		UnitCapCostPC[0] = PaC;
		UnitCapCostPC[1] = CaC;
		xita1 = xit1;
		xita5 = xit5;
		xita6_1 = xit6_1;
		xita6_2 = xit6_2;
		incentive = incen;
		penalty = penal;
		b_be_2 = b_be2;
		Tao = tao; //1.5~2
		gama = gam; //0.01~0.5
	}

public:
	void ReadNode(string DataPath);   // 函数：读取节点文件
	void ReadDepot(string DataPath);  // 函数：读取depot文件
	void ReadLink(string DataPath);   // 函数：读取路段文件
	void ReadDistance(string DataPath);   // 函数：读取Depot和node的距离文件
	void ReadODpairs(string DataPath); // 函数：读取OD对文件
	void GeneratePossible(int n);
	void EnumerationNodeType(); //Upper-level枚举node的type
	void NodeType(int sol, vector<CSolution*> m_Solution); //根据上层来给每个node type赋值
	double Dijstra(int Start, int End); //Dijstra算法
	void CalNodeDistance(); //计算任何两点之间的距离
	void UpdateRouteCost(double* RouteFlow); //根据RouteFlow来更新所有的RouteCost，同时也更新对应的PickupNode和DropoffNode的flow
	void GenerateRoute(); //为每一对ODpair生成route
	void AllorNothingAssignment(double* RouteFlow, double* ANRouteFlow);   // 全有全无交通分配：输入为初始路段流量（索引对应CLink中的ID）
	double GetUEGap(double* RouteFlow);  // 间隙函数
	void battery_swap();
	double cal_obj(int sol, vector<CSolution*> m_Solution);
	void RecordBest(int sol, vector<CSolution*> m_Solution);
	void PrintUERouteFlow(double UEGap, double* RouteFlow);
	void PrintUENodeFlow();
	void PrintUERouteCost();
	void PrintNodeInf();
	void PrintBestSolution();
	void EnumerationProgram();
	void InitialNodeType();
	void RepairSolution(vector<CSolution*> m_Solution);
	void WheelSelect();
	void Crossover();
	void Mutation();
	void Selection();
	void GAProgram();


private:
	//void strcpy(char* strs, string name);
	inline bool exists(const std::string name)
	{
		ifstream f(name.c_str());
		return f.good();
	}

	vector<string> split(const string& str, const string& delim)
	{
		vector<string> res;
		if ("" == str) return res;
		//先将要切割的字符串从string类型转换为char*类型  
		char* strs = new char[str.length() + 1]; //不要忘了  
		strcpy(strs, str.c_str());

		char* d = new char[delim.length() + 1];
		strcpy(d, delim.c_str());

		char* p = strtok(strs, d);
		while (p)
		{
			string s = p; //分割得到的字符串转换为string类型  
			res.push_back(s); //存入结果数组  
			p = strtok(NULL, d);
		}

		return res;
	}
};

