#pragma once
#include <vector>

struct Edge
{
    int v1, v2; //connected vertices
    int weight;

    Edge() {}

    Edge(int v1, int v2, int weight) : v1(v1), v2(v2), weight(weight) {}
};

struct Vertex
{
    int id;
    int cost;

    Vertex() {}

    Vertex(int id, int cost) : id(id), cost(cost) {}
};

//comparator for stl priority queue
struct CompareWeight
{
    bool operator()(Edge const &e1, Edge const &e2)
    {
        return e1.weight > e2.weight;
    }

    bool operator()(Vertex const &e1, Vertex const &e2)
    {
        return e1.cost > e2.cost;
    }
};

class Graph
{
private:
    std::vector<std::vector<int>> weight_matrix;

    std::vector<std::vector<Edge>> adjency_list;

    int amount_of_vertices = 0;

public:
    Graph();

    //creates matrix and list (also replaces matrix and list with the new ones - if they existed before)
    void set_amount_of_vertices(int amount_of_vertices);
    int get_amount_of_vertices();

    Graph(int amount_of_vertices);

    //weight matrix
    void add_edge_to_matrix(int first_vertex, int second_vertex, int edge_weight);
    void print_weight_matrix();
    std::vector<std::vector<int>> get_weight_matrix();
    void set_weight_matrix(std::vector<std::vector<int>>  weight_matrix);

    //adjency list
    void add_edge_to_adjency_list(int first_vertex, int second_vertex, int edge_weight);
    void print_adjency_list();
    std::vector<std::vector<Edge>> get_adjency_list();

    int get_size();
};
