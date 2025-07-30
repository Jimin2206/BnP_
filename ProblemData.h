#pragma once
#include <iostream>
#include <random>
#include <fstream>
#include <chrono>
#include "Highs.h"

using namespace std;

class ProblemData
{
public:
	static int LL;
	static int nL;
	static int* ReqL;
	static int* b;

	//static void PopulateParamts();
};