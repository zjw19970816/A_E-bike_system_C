#pragma once
#include<vector>
using namespace std;

class CNode
{
public:
	int ID; // 节点的编号，从零开始编号 （node number）
	double PositionX; // 节点的X坐标
	double PositionY; // 节点的Y坐标
	int Origin_ID = -1; // 节点对应的起点编号，-1表示不是起点 （标识符，判断是不是起点，默认先不是）
	vector<int> IncomingLink; // 进入节点的link编号集合 （所以是directed graph）
	vector<int> OutgoingLink; // 离开节点的link编号集合 
	//int Node_type; // 节点对应的车站类型，1: parking node, 2: plug-in charging station, 3: non-parking node.
	int NodeType[3]; // 储存节点对应的车站类型
	double Capacity[3]; //node的capacity
	double CapacityPick[3]; //node的pick capacity
	double CapacityDrop[3]; //node的drop capacity
	double CapacityPickAu[3]; //node的pick capacity辅助
	double CapacityDropAu[3]; //node的drop capacity辅助
	double CapacityPick_Temp[3]; //node的pick capacity
	double CapacityDrop_Temp[3]; //node的drop capacity
	double Incentive; //奖励金
	double Penalty; //惩罚金
	double Pickupflow[2];
	double Dropoffflow[3];
	double PickupUseRate[2];
	double DropoffUseRate[3];
	double PickupNoSharedEbike[2]; //1
	double DropofffNoParkingSpace[3]; //2
	double PickupLossRate[2]; 
	double DropoffLossRate[3]; 
	double PickupQueue[2];
	double DropoffQueue[3];
	double PickupEffectiveRate[2];
	double DropoffEffectiveRate[3];
	double Waiting_time_cost_pickup[3] = { 0,0,0 }; //pick-up waiting time cost
	double Waiting_time_cost_dropoff[3] = { 0,0,0 }; //drop-off waiting time cost
	double ValueDerivativePick[2]; //Pick-up node对capacity求导
	double ValueDerivativeDrop[3]; //drop node对capacity求导
	double ValueDerivativeNonMarginalFunctionPick[2];//Pick-up node对capacity求导后对当前route flow积分的值
	double ValueDerivativeNonMarginalFunctionDrop[3];
	double ValueDerivativeMarginalFunctionPick[2];//Pick-up node对capacity求导后对得到的UE route flow积分的值
	double ValueDerivativeMarginalFunctionDrop[3];
	double GeneralizedPickCapacity[2];
	double GeneralizedDropCapacity[3];
};
