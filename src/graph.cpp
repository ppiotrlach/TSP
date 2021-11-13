#include "../lib/graph.h"
#include <iostream>

using namespace std;

Graph::Graph() {}

void Graph::set_amount_of_vertices(int amount_of_vertices)
{
    this->amount_of_vertices = amount_of_vertices;
    this->weight_matrix = vector<vector<int>>(amount_of_vertices, vector<int>(amount_of_vertices, INT32_MAX));
    this->adjency_list = vector<vector<Edge>>(amount_of_vertices);
}

int Graph::get_amount_of_vertices(){
    return this->amount_of_vertices;
}

Graph::Graph(int amount_of_vertices)
{
    this->amount_of_vertices = amount_of_vertices;
    this->weight_matrix = vector<vector<int>>(amount_of_vertices, vector<int>(amount_of_vertices, INT32_MAX));
    this->adjency_list = vector<vector<Edge>>(amount_of_vertices);
}

void Graph::add_edge_to_matrix(int first_vertex, int second_vertex, int edge_weight)
{
    if(first_vertex==second_vertex) weight_matrix[first_vertex][second_vertex] = INT32_MAX;
    else weight_matrix[first_vertex][second_vertex] = edge_weight;
}

void Graph::add_edge_to_adjency_list(int first_vertex, int second_vertex, int edge_weight)
{
    adjency_list[first_vertex].push_back(Edge(first_vertex, second_vertex, edge_weight));
}

void Graph::print_weight_matrix()
{
    cout << "Weight matrix:" << endl;

    for (int i = 0; i < amount_of_vertices; i++)
    {
        for (int j = 0; j < amount_of_vertices; j++)
        {
            if (weight_matrix[i][j] == INT32_MAX)
            {
                cout << "i"
                     << " ";
                continue;
            }
            cout << Graph::weight_matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::print_adjency_list()
{
    cout << "Adjency list:" << endl;

    for (int i = 0; i < amount_of_vertices; i++)
    {
        cout << i << " -> ";
        for (int j = 0; j < adjency_list[i].size(); j++)
        {

            cout << "[" << adjency_list[i][j].v2 << "," << adjency_list[i][j].weight << "]";
            if (j != adjency_list[i].size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }
}

vector<vector<int>> Graph::get_weight_matrix()
{
    return this->weight_matrix;
}

void Graph::set_weight_matrix(vector<vector<int>>  weight_matrix)
{
    this->weight_matrix = weight_matrix;
}

vector<vector<Edge>> Graph::get_adjency_list()
{
    return this->adjency_list;
}

int Graph::get_size(){
    return this->amount_of_vertices;
}