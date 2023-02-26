#pragma once
#include "CNode.h"
#include<vector>
using namespace std;

class CSolution
{
public:
	int ID; // Solution的编号
	vector<vector<int>> node_type_upper_level; //对下层模型来说，上层生成的node_type是constant
	//int node_type_upper_level[4][3];
	vector<vector<double>> node_capacity_upper_level; //对下层模型来说，上层生成的node_capacity是constant
	vector<vector<double>> node_capacity_upper_level_temp; //对下层模型来说，上层生成的node_capacity是constant
	vector<vector<double>> picknode_capacity_upper_level; //对下层模型来说，上层生成的node_capacity是constant
	vector<vector<double>> dropnode_capacity_upper_level; //对下层模型来说，上层生成的node_capacity是constant
	//double node_capacity_upper_level[4][3];
	double ObjectiveValue;
	double ObjectiveValue_temp;
	double Fitness;
	double Probability;

	double UEcost = 0;
	double BatterySwapCost = 0;
	double UnmetDemandCost = 0;
	double ConstructionCost= 0;
	double CapacityCost = 0;
	double Reposition1 = 0;
	double Reposition2 = 0;
	double UnsatisfiedDemandCost = 0;
	double LossDemandCost = 0;
	double PenaltyCost = 0;
	double IncentiveCost = 0;
	double RentalFare = 0;

	double G_x_f; //value of gap function
	double G_x_f_2; //value of gap function(下一次)

	double L_x_f; //Value of Lagrange function
	double L_x_f_Au; //Value of Au Lagrange function

	double G_x_f_real; //value of gap function
	double G_x_f_2_real; //value of gap function(下一次)
};