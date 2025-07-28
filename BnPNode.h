#pragma once
#include <vector>
#include <algorithm>
#include "ProblemData.h"

#include <utility>

using namespace std;

struct BnPNode
{
	int node_id;
	
	// ��忡�� ������� ���� ����
	vector<int*> patterns;

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

	// ���� ������: ���� deep copy ����
	BnPNode(const BnPNode& other) {
		node_id = other.node_id;
		lp_bound = other.lp_bound;
		solution = other.solution;
		is_integral = other.is_integral;
		var_bounds = other.var_bounds;

		// patterns deep copy
		for (auto pat : other.patterns) {
			int* new_pat = new int[ProblemData::nL];
			std::copy(pat, pat + ProblemData::nL, new_pat);
			patterns.push_back(new_pat);
		}
	}

	// �Ҹ���: patterns ����
	~BnPNode() {
		for (auto pat : patterns) {
			delete[] pat;
		}
	}
};