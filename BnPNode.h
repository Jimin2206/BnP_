#pragma once
#include <vector>
#include <utility>

using namespace std;

struct BnPNode
{
	int node_id;
	
	//// 노드에서 사용중인 패턴 집합
	//vector<int*> patterns;

	// 노드에 적용된 분기 조건들
	// int: 분기 대상 변수 인덱스 (ex. x3)
	// double, double: 해당 변수 하한, 상한
	vector<pair<int, pair<double, double>>> var_bounds;

	// 노드에서 RMP를 풀었을 때 얻은 lower bound
	double lp_bound;

	// 노드에서 풀린 RMP의 최적 변수값 벡터 x
	vector<double> solution;

	// 노드의 해가 정수인지 여부
	bool is_integral;

	// 디폴트 생성자
	BnPNode() : node_id(-1), lp_bound(0.0), is_integral(false) {}
};