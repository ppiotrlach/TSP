#pragma once

#include <string>
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
    int simulated_annealing(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method);
    int tabu_search(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method);
    int genetic_algorithm(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method);
};
