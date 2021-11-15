#include "../lib/TSP.h"
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <queue>

#define INFINITY INT32_MAX;

using namespace std;

struct Node
{
    vector<int> path;
    vector<vector<int>> weight_matrix;

    int cost;   //lower bound
    int vertex; //current vertex
    int level;  //number of visited vertex
};

namespace matrix_operations
{
    void print_path(vector<int> const &path)
    {
        for (int i = 0; i < path.size() - 1; i++)
        {
            cout << path[i] << " —> ";
        }
        cout << 0 << endl;
    }

    int reduct_rows(Node *node)
    {
        int matrix_width = node->weight_matrix[0].size();
        int total_reduced_value = 0;

        for (int i = 0; i < matrix_width; i++)
        {
            int min_row_value = INT32_MAX;
            for (int j = 0; j < matrix_width; j++)
            {
                if (node->weight_matrix[i][j] < min_row_value)
                    min_row_value = node->weight_matrix[i][j];
            }

            if (min_row_value != INT32_MAX && min_row_value > 0)
            {
                total_reduced_value += min_row_value;

                for (int j = 0; j < matrix_width; j++)
                {
                    if (node->weight_matrix[i][j] == INT32_MAX)
                        continue; //inf - x = inf

                    node->weight_matrix[i][j] -= min_row_value;
                }
            }
        }
        return total_reduced_value;
    }

    int reduct_columns(Node *node)
    {
        int matrix_width = node->weight_matrix[0].size();
        int total_reduced_value = 0;

        for (int i = 0; i < matrix_width; i++)
        {
            int min_column_value = INT32_MAX;
            for (int j = 0; j < matrix_width; j++)
                if (node->weight_matrix[j][i] < min_column_value)
                    min_column_value = node->weight_matrix[i][j];

            if (min_column_value != INT32_MAX && min_column_value > 0)
            {
                total_reduced_value += min_column_value;

                for (int j = 0; j < matrix_width; j++)
                {
                    if (node->weight_matrix[j][i] == INT32_MAX)
                        continue; //inf - x = inf

                    node->weight_matrix[j][i] -= min_column_value;
                }
            }
        }
        return total_reduced_value;
    }


    int reduct_weight_matrix(Node *node) //returns lower bound, reducted cost 
    {
        int reducted_cost = reduct_rows(node) + reduct_columns(node);

        return reducted_cost;
    }
}

Node *new_node(Node *parent_node, int current_vertex)
{
    Node *node = new Node;
    int matrix_width = parent_node->weight_matrix[0].size();
    int infinity = INT32_MAX;

    node->path = parent_node->path;       //copy path from previous vertex
    node->path.push_back(current_vertex); // add current vertex to path

    node->weight_matrix = parent_node->weight_matrix; //copy weight_matrix from prev vertex

    for (int i = 0; i < matrix_width; i++)
    {
        node->weight_matrix[parent_node->vertex][i] = infinity; //parent_vertex edges (outgoing)= infinity
        node->weight_matrix[i][current_vertex] = infinity;      //current_vertex edges (incoming) = infinity
    }

    node->weight_matrix[current_vertex][0] = infinity; //can't return to start_vertex

    node->level = parent_node->level + 1;
    node->vertex = current_vertex;

    return node;
}

Node *create_root_node(vector<vector<int>> &weight_matrix)
{
    Node *root = new Node;
    root->path.push_back(0); //vertex 0 as start_vertex
    root->weight_matrix = weight_matrix;
    root->vertex = 0;
    root->level = 0;
    root->cost = matrix_operations::reduct_weight_matrix(root);

    return root;
}

struct nodes_comparator
{
    bool operator()(const Node *left_node, const Node *right_node)
    {
        return left_node->cost > right_node->cost;
    }
};

TSP::TSP() {}

int TSP::brute_force(Graph graph, int optimum_cost)
{

    int amount_of_vertices = graph.get_amount_of_vertices();
    vector<vector<int>> weight_matrix = graph.get_weight_matrix();
    vector<int> vertices; //vector of all vertices except the start_vertex
    string best_path = "";

    int start_vertex = 0;
    int shortest_path = INFINITY;
    int first_found_path = INFINITY;

    float PRD;

    //pushing all nodes of graph into vertices vector except the start_vertex
    for (int i = 0; i < amount_of_vertices; i++)
    {
        if (i != start_vertex)
            vertices.push_back(i);
    }

    int counter = 0;
    //checking all permutations of vertices-1 (n! complexity)
    while (next_permutation(vertices.begin(), vertices.end()))
    {
        int path_weight = 0;

        int j = start_vertex;
        for (int i = 0; i < vertices.size(); i++)
        {
            path_weight += weight_matrix[j][vertices[i]];
            // printf("weight_matrix[%d][%d] = %d \n",j, vertices[i], weight_matrix[j][vertices[i]]);
            j = vertices[i];
        }
        path_weight += weight_matrix[j][start_vertex]; //back to the first vertex
        // printf("path_weight = %d\n", path_weight);

        if (path_weight < shortest_path)
        {
            shortest_path = path_weight;
            PRD = (float)(shortest_path - optimum_cost) * 100 / optimum_cost;
            printf("%d %.2f%% \n", shortest_path, PRD);

            best_path = "";
            best_path.append(to_string(start_vertex));
            for (int i = 0; i < vertices.size(); i++)
            {
                best_path.append(" —> ");
                best_path.append(to_string(vertices[i]));
            }
            best_path.append(" —> ");
            best_path.append(to_string(start_vertex));
        }

        if (counter == 0)
        {
            first_found_path = shortest_path;
        }

        shortest_path = min(shortest_path, path_weight);

        counter++;
        // printf("shortest_path = %d\n", shortest_path);
    }
    PRD = (float)(first_found_path - optimum_cost) * 100 / optimum_cost;
    printf("%d %.2f%% \n", first_found_path, PRD);
    cout << best_path << endl;

    return shortest_path;
}

int TSP::branch_and_bound(vector<vector<int>> weight_matrix) //least cost version of BnB TSP algorithm
{
    priority_queue<Node *, vector<Node *>, nodes_comparator> priority_queue; //priority_queue stores nodes
    int matrix_width = weight_matrix[0].size();

    Node *root = create_root_node(weight_matrix); //root node, vertex 0
    priority_queue.push(root);

    while (!priority_queue.empty()) //least cost nodes goes first (nodes_comparator)
    {
        Node *min = priority_queue.top(); //least cost vertex at the top of queue
        priority_queue.pop();             //delete least cost node from priority_queue

        int i = min->vertex; //current vertex number

        if (min->level == matrix_width - 1) //if all vertices are visited
        {
            min->path.push_back(i); //back to start_vertex

            matrix_operations::print_path(min->path);

            int path_cost = min->cost;
            cout << "path_cost = " << path_cost << endl;

            while (!priority_queue.empty())
            { //cleaning before the algorithm is finished
                Node *node_to_delete = priority_queue.top();
                priority_queue.pop();

                delete node_to_delete;
            }

            delete min; //also delete taken out of the queue earlier min node

            return path_cost;
        }

        for (int j = 0; j < matrix_width; j++) //each ancestor of min node
        {
            if (min->weight_matrix[i][j] != INT32_MAX) //except for those that cannot be reached 
            {
                Node *ancestor = new_node(min, j);

                // ancestor_cost = cost of min node(parent) + edge_weight from min to ancestor + reduction cost of ancestor matrix
                ancestor->cost = min->cost + min->weight_matrix[i][j] + matrix_operations::reduct_weight_matrix(ancestor);
                priority_queue.push(ancestor); //push ancestor to queue
            }
        }
        delete min; // no need to store parent node at the end of iteration
    }
    return -1;
}