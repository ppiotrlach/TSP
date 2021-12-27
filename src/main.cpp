#include <iostream>
#include <fstream>
#include "../lib/graph.h"
#include "../lib/TSP.h"
#include <chrono>


#include<time.h>					// do mierzenia czasu
#include<stdlib.h>					// do mierzenia czasu

using namespace std;

int main(int argc, char *argv[])
{
    Graph graph;
    TSP tsp;
    chrono::steady_clock::time_point begin, end;

    string filename = "instances/m9.atsp";

    string instancename;
    string temp;
    int amount_of_vertices, edge_weight, optimum_cost;
    bool call_brute_force = false, call_bnb = false, call_simulated_annealing = true; //as default run simulated_annealing

    if (argc > 1) //second argument = filename
    {
        string arg1 = argv[1];
        filename = "instances/" + arg1;
    }

    if (argc > 2) //third arg, 'b' means run brute force
    {
        string arg2 = argv[2];
        if (arg2 == "br")
        {
            call_brute_force = true;
            call_bnb = false;
            call_simulated_annealing = false;
        }
        else if (arg2 == "bnb")
        {
            call_brute_force = false;
            call_bnb = true;
            call_simulated_annealing = false;
        }
        else{
            call_brute_force = false;
            call_bnb = false;
            call_simulated_annealing = true;
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

    if (call_brute_force)
    {
        begin = chrono::steady_clock::now();
        tsp.brute_force(graph, optimum_cost);
        end = chrono::steady_clock::now();
        cout << "optimum_cost = " << optimum_cost << endl
             << endl;
        cout << "algorithm execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs] = ";
        cout << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
    }
    else if (call_bnb)
    {
        begin = chrono::steady_clock::now();
        tsp.branch_and_bound(graph.get_weight_matrix());
        end = chrono::steady_clock::now();
        cout << "optimum_cost = " << optimum_cost << endl
             << endl;
        cout << "algorithm execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs] = ";
        cout << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
    }
    else if(call_simulated_annealing){
        // clock_t start;
        begin = chrono::steady_clock::now();
        tsp.simulated_annealing(graph.get_weight_matrix(), optimum_cost);
        end = chrono::steady_clock::now();
        cout << "optimum_cost = " << optimum_cost << endl << endl;
        cout << "algorithm execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs] = ";
        cout << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
    }
}
