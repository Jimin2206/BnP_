#pragma once
#include <vector>
#include <utility>

using namespace std;

struct BnPNode
{
	int node_id;
	
	//// ��忡�� ������� ���� ����
	//vector<int*> patterns;

	// ��忡 ����� �б� ���ǵ�
	// int: �б� ��� ���� �ε��� (ex. x3)
	// double, double: �ش� ���� ����, ����
	vector<pair<int, pair<double, double>>> var_bounds;

	// ��忡�� RMP�� Ǯ���� �� ���� lower bound
	double lp_bound;

	// ��忡�� Ǯ�� RMP�� ���� ������ ���� x
	vector<double> solution;

	// ����� �ذ� �������� ����
	bool is_integral;

	// ����Ʈ ������
	BnPNode() : node_id(-1), lp_bound(0.0), is_integral(false) {}
};