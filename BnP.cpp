#include "BnP.h"
#include "CG.h"
#include "BnPNode.h"
#include "ProblemData.h"
#include <stack>
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

//void BnP()
int main()
{
	// 루트 노드 생성
	BnPNode root;

	stack<BnPNode> S;
	S.push(root);

	double best_obj = numeric_limits<double>::infinity();
	vector<double> best_sol;

	int node_id = 0;

	while (!S.empty())
	{
		BnPNode node = S.top();
		S.pop();

		node.node_id = node_id++;

		bool ok = SolveRMP(node);

		if (!ok) continue;	// Highs 실패한 경우 해당 노드 무시하고 다음으로 넘어감

		double obj = node.lp_bound;
		vector<double> sol = node.solution;

		if (obj >= best_obj) continue;

		if (node.is_integral)
		{
			best_obj = obj;
			best_sol = sol;
			cout << "New incumbent found! obj = " << best_obj << endl;
			continue;
		}

		int var = -1;
		double max_frac = 0.0;

		for (int i = 0; i < node.solution.size(); ++i)
		{
			double f = fabs(node.solution[i] - round(node.solution[i]));
			if (f > 1e-5 && f > max_frac)
			{
				max_frac = f;
				var = i;
			}
		}
		if (var == -1) continue;
		
		double val = node.solution[var];

		BnPNode left = node;
		left.var_bounds.push_back({ var, {0.0, floor(val)} });
		S.push(left);

		BnPNode right = node;
		right.var_bounds.push_back({ var, {ceil(val), 1e30} });
		S.push(right);
	}

	cout << endl;
	cout << "Best integer solution obj = " << best_obj << endl;
	for (int i = 0; i < best_sol.size(); ++i)
	{
		cout << "x_" << i << " = " << best_sol[i] << endl;
	}
}