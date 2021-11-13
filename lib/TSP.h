#pragma once

#include "graph.h"
#include <stdint.h> //INT32_MAX

using namespace std;

class TSP
{
private:
public:
    TSP();

    int brute_force(Graph graph, int optimum_cost);
    int branch_and_bound(vector<vector<int>> weight_matrix);
};
