#pragma once
#include "CNode.h"
#include<vector>
using namespace std;

class CRoute
{
public:
	int ODID; // OD numerber, 目的是找该OD pair下的route
	int RouteID; // route number
	CNode* PickupNode; // 该ODpair的该条route的pick-up node
	CNode* DropoffNode; // 该ODpair的该条route的drop-off node
	int PickupType; //取車點的類型
	int DropoffType; //停車點的類型
	//Route cost
	//double FreeFlowTravelTime; // Free Flow Travel time
	double FreeFlowTravelSpeed = 15; // Free Flow Travel speed
	double Capacity = 400; // Route capacity
	double Pickupdistance;
	double Ridingdistance;
	double RentalCost;
	double EnergyConsumption;
	double Dropoffdistance;
	double PickupWalkingCost;
	double DropWalkingCost;
	double RidingTimeCost = 0; // link travel time
	double PickupWaitingCost;
	double DropWaitingCost;
	double RouteCost;

	double DropExtraCost;
	double ValueNonMarginalFunction; //V(x,f)
	double ValueMarginalFunction; //V(x,f(x)*)
	double ValueNonDerivativeMarginalFunctionPick;
	double ValueNonDerivativeMarginalFunctionDrop;
	//double ValueDerivativeMarginalFunctionPick;
	//double ValueDerivativeMarginalFunctionDrop;
	double DerivationRouteflow; //C_n_m_r对f_n_m_r求导
	double GeneralizedRouteCost; //用新的route cost做all or nothing
};

