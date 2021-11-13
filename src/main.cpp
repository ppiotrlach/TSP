#include <iostream>
#include <fstream>
#include "../lib/graph.h"
#include "../lib/TSP.h"
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    Graph graph;
    TSP tsp;

    string filename = "../instances/m9.atsp";
    string instancename;
    string temp;
    int amount_of_vertices, edge_weight, optimum_cost;
    bool call_brute_force = false, call_bnb = true; //as default run bnb

    if (argc > 1) //second argument = filename
    {
        string arg1 = argv[1];
        filename = "../instances/" + arg1;
    }

    if (argc > 2) //third arg, 'b' means run brute force
    {
        string arg2 = argv[2];
        if (arg2 == "b")
        {
            call_brute_force = true;
            call_bnb = false;
        }
    }

    ifstream file(filename);
    string line;

    if (file.is_open())
    {
        file >> instancename;
        file >> amount_of_vertices;

        graph.set_amount_of_vertices(amount_of_vertices);

        for (int i = 0; i < amount_of_vertices; i++)
        {
            for (int j = 0; j < amount_of_vertices; j++)
            {
                file >> edge_weight;
                graph.add_edge_to_matrix(i, j, edge_weight);
            }
        }

        file >> optimum_cost;
    }
    else
    {
        cout << "Can not open file, restart the program and try again" << endl;
        return -1;
    }
    file.close();

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    if (call_brute_force)
        tsp.brute_force(graph, optimum_cost);
    else if (call_bnb)
        tsp.branch_and_bound(graph.get_weight_matrix());

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << endl
         << "optimum_cost = " << optimum_cost << endl
         << endl;
    cout << "algorithm execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs] = ";
    cout<< chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
}