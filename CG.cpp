#include "CG.h"
#include "Highs.h"
#include "ProblemData.h"
#include "Functions.h"
#include <iostream>
#include <cmath>
#include <iomanip>

bool SolveRMP(BnPNode& node)
{
	int Log_Length = ProblemData::LL;
	int nLength = ProblemData::nL;
	int* Req_Length = ProblemData::ReqL;
	int* b = ProblemData::b;

	// when node already has the patterns
	vector<int*>& Pattern = node.patterns;

	// root node
	// generate initial patterns for RMP
	if (Pattern.empty())
	{
		for (int i = 0; i < nLength; i++)
		{
			int* Pat0 = new int[nLength]();
			Pat0[i] = std::floor(Log_Length / Req_Length[i]);

			Pattern.push_back(Pat0);
		}
	}

	vector<double> final_sol;

	int iter_count = 0;

	while (true)
	{
		iter_count++;
		// Solve the restricted master problem (RMP) using HiGHS

		Highs highs;
		HighsModel model;
		model.lp_.sense_ = ObjSense::kMinimize;

		int num_vars = Pattern.size();
		int num_cons = nLength;

		// objective coefficients
		model.lp_.col_cost_.assign(num_vars, 1.0);

		// variable range
		model.lp_.col_lower_.assign(num_vars, 0.0);
		model.lp_.col_upper_.assign(num_vars, std::numeric_limits<double>::infinity());

		for (const auto& bc : node.var_bounds)
		{
			int idx = bc.first;
			model.lp_.col_lower_[idx] = bc.second.first;
			model.lp_.col_upper_[idx] = bc.second.second;
		}

		// constraints
		model.lp_.row_lower_.assign(b, b + nLength);
		model.lp_.row_upper_.assign(num_cons, std::numeric_limits<double>::infinity());

		// coefficient matrix A
		vector<int> Astart(num_vars + 1);
		vector<int> Aindex;
		vector<double> Avalue;

		int nnz = 0;	// number of nonzeros in each column
		for (int p = 0; p < num_vars; ++p)
		{
			Astart[p] = nnz;
			for (int i = 0; i < nLength; ++i)
			{
				if (Pattern[p][i] > 0)
				{
					Aindex.push_back(i);
					Avalue.push_back(Pattern[p][i]);
					nnz++;
				}
			}
		}
		Astart[num_vars] = nnz;

		model.lp_.num_col_ = num_vars;
		model.lp_.num_row_ = num_cons;
		model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;
		model.lp_.a_matrix_.start_ = Astart;
		model.lp_.a_matrix_.index_ = Aindex;
		model.lp_.a_matrix_.value_ = Avalue;

		highs.passModel(model);
		highs.setOptionValue("output_flag", false);
		HighsStatus status = highs.run();

		if (status != HighsStatus::kOk)
		{
			cerr << "RMP: HiGHS optimization failed!" << endl;
			throw - 1;
		}

		const vector<double>& sol = highs.getSolution().col_value;	// optimal variable values
		double obj = highs.getInfo().objective_function_value;		// optimal objective value

		final_sol = sol;
		node.solution = sol;
		node.lp_bound = obj;

		node.is_integral = true;
		for (auto val : sol)
		{
			if (abs(val - round(val)) > 1e-5)
			{
				node.is_integral = false;
				break;
			}
		}

		cout << "\n\t Iteration: " << iter_count << "\t Required Number of logs: " << obj << endl;	// first iteration: Required number of logs: 23.4167

		// get the dual variables from the RMP
		double* Duals = new double[nLength]();
		const vector<double>& duals = highs.getSolution().row_dual;

		for (int i = 0; i < nLength; i++)
		{
			Duals[i] = duals[i];	// dual variable for each demand constraint
			//cout << Duals[i] << endl;	// 0.1, 0.05, 0.166667, 0.2, 0.2, 0.333333
		}

		//int jj = 0;
		// generate a new pattern from the subproblem (knapsack problem)
		int* NewPat = new int[nLength]();
		double SP_obj = SubProblem(Duals, NewPat);

		// check the optimality condition/ add the new patterns
		if (1 - SP_obj >= 0) { break; }
		else
		{
			Pattern.push_back(NewPat);
		}
	}

	for (int p = 0; p < Pattern.size(); p++)
	{
		cout << "Pattern " << p << " [";
		for (int i = 0; i < nLength; i++)
		{
			cout << " " << Pattern[p][i];
		}
		cout << " ]: used " << final_sol[p] << " times" << endl;
	}

	cout << "node " << node.node_id << endl;
	cout << "\t" << "LP bound : " << node.lp_bound << endl;
	
	cout << std::boolalpha;
	cout << "\t" << "integrality : " << node.is_integral << endl;
	
	return true;
}