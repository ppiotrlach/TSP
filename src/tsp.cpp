#include "../lib/TSP.h"
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <queue>
#include <random>
#include <chrono>

#define INFINITYY INT32_MAX;

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
        for (int i = 0; i < path.size(); i++)
        {
            if (i == path.size() - 1)
                cout << path[i] << endl;
            else
                cout << path[i] << " —> ";
        }
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
    int shortest_path = INFINITYY;
    int first_found_path = INFINITYY;

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

vector<int> create_random_path(int number_of_cities)
{
    vector<int> random_path;

    for (int i = 1; i < number_of_cities; i++)
    {
        random_path.push_back(i);
    }

    //to create different sequences of shuffles each time it is run
    random_device rd;
    auto rng = default_random_engine{rd()};
    shuffle(random_path.begin(), random_path.end(), rng);

    random_path.insert(random_path.begin(), 0); //first city = 0
    random_path.push_back(0);                   //last city = 0

    return random_path;
}

vector<int> create_nearest_neighbor_path(int number_of_cities, vector<vector<int>> weight_matrix)
{

    vector<int> nearest_neighbor_path;
    nearest_neighbor_path.push_back(0);
    int from = 0;

    while (nearest_neighbor_path.size() < number_of_cities)
    {
        int to = 0;
        int min = INT32_MAX;
        for (int j = 0; j < number_of_cities; j++)
        {
            if (!(find(nearest_neighbor_path.begin(), nearest_neighbor_path.end(), j) != nearest_neighbor_path.end()) && weight_matrix[from][j] < min)
            {
                min = weight_matrix[from][j];
                to = j;
            }
        }
        nearest_neighbor_path.push_back(to);
        from = to;
    }
    nearest_neighbor_path.push_back(0);

    return nearest_neighbor_path;
}

int calculate_path_cost(vector<int> path, vector<vector<int>> weight_matrix) //calculate first path cost
{
    int path_cost = 0;
    int next_city = path[0];

    for (int i = 0; i < path.size(); i++)
    {
        if (weight_matrix[next_city][path[i]] != INT32_MAX)
        {
            path_cost += weight_matrix[next_city][path[i]];
            //cout << weight_matrix[next_city][path[i]] << endl;
            next_city = path[i];
        }
    }
    //cout << endl << "path_cost = " << path_cost << endl;

    return path_cost;
}

//if we have knowledge about which cities was swaped there is no need to calculate whole cost again
int calculate_path_cost(int recent_path_cost, vector<int> recent_path, vector<int> current_path, vector<vector<int>> weight_matrix, int first_city_index, int second_city_index)
{
    if (first_city_index > second_city_index)
    {
        int buff = first_city_index;
        first_city_index = second_city_index;
        second_city_index = buff;
    }

    int new_cost;
    int distance_beetween_positions = first_city_index - second_city_index;
    if (distance_beetween_positions == -1 || distance_beetween_positions == 1)
    {
        new_cost =
            //minus connections beetween old cities
            recent_path_cost - weight_matrix[recent_path[first_city_index - 1]][recent_path[first_city_index]] - weight_matrix[recent_path[second_city_index - 1]][recent_path[second_city_index]] - weight_matrix[recent_path[second_city_index]][recent_path[second_city_index + 1]]

            //plus new connections
            + weight_matrix[current_path[first_city_index - 1]][current_path[first_city_index]] + weight_matrix[current_path[second_city_index - 1]][current_path[second_city_index]] + weight_matrix[current_path[second_city_index]][current_path[second_city_index + 1]];
    }
    else
    {
        new_cost =
            //minus connections beetween old cities
            recent_path_cost - weight_matrix[recent_path[first_city_index - 1]][recent_path[first_city_index]] - weight_matrix[recent_path[first_city_index]][recent_path[first_city_index + 1]] - weight_matrix[recent_path[second_city_index - 1]][recent_path[second_city_index]] - weight_matrix[recent_path[second_city_index]][recent_path[second_city_index + 1]]

            //plus new connections
            + weight_matrix[current_path[first_city_index - 1]][current_path[first_city_index]] + weight_matrix[current_path[first_city_index]][current_path[first_city_index + 1]] + weight_matrix[current_path[second_city_index - 1]][current_path[second_city_index]] + weight_matrix[current_path[second_city_index]][current_path[second_city_index + 1]];
    }
    return new_cost;
}

int get_random_int(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void swap_cities(vector<int> path, int pos1_to_swap, int pos2_to_swap)
{
    iter_swap(path.begin() + pos1_to_swap, path.begin() + pos2_to_swap);
    matrix_operations::print_path(path);
}

double get_probability(int current_cost, int bigger_cost, float temperature)
{
    int cost_delta = current_cost - bigger_cost;
    return exp(cost_delta / temperature);
}

double get_random_probability()
{
    return ((double)rand() / RAND_MAX);
}

int TSP::simulated_annealing(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method)
{
    srand(time(NULL)); // Seed the time

    chrono::steady_clock::time_point begin, end;
    int algorithm_duration_time;
    begin = chrono::steady_clock::now();

    int number_of_cities = weight_matrix[0].size();
    int path_cost = 0, new_path_cost = 0;

    int iteration_counter = 0;
    int best_path_iteration_numb = 0;

    float temperature = 10000;
    float start_temperature = temperature;

    float cooling_rate = 0.9999; //geometric cooling scheme

    vector<int> path;
    if (initial_solution_method == "random")
        path = create_random_path(number_of_cities); //create random path, first and last city is always 0
    else
        path = create_nearest_neighbor_path(number_of_cities, weight_matrix); //create nearest neighbour path, first and last city is always 0

    vector<int> temp_path = path;

    vector<int> best_path = path;
    path_cost = calculate_path_cost(path, weight_matrix);

    float PRD = (float)(path_cost - optimum_cost) * 100 / optimum_cost;
    printf("0\t%d\t%.2f%%\t%.4f \n", path_cost, PRD, temperature);

    int first_path_cost = path_cost;
    int best_path_cost = path_cost;

    int no_improvement_iteration_counter = 0;

    while (temperature > 0.000001 && algorithm_duration_time < 60)
    {
        temp_path = path;

        iteration_counter++;

        if (iteration_counter % 100 == 0)
        {
            temperature = start_temperature * cooling_rate;
            start_temperature = temperature;
        }

        int pos1_to_swap = get_random_int(1, number_of_cities - 2);
        int pos2_to_swap = get_random_int(1, number_of_cities - 2);

        while (pos1_to_swap == pos2_to_swap)
        {
            pos2_to_swap = get_random_int(1, number_of_cities - 2);
        }

        iter_swap(temp_path.begin() + pos1_to_swap, temp_path.begin() + pos2_to_swap); //swap cities in vector

        new_path_cost = calculate_path_cost(path_cost, path, temp_path, weight_matrix, pos1_to_swap, pos2_to_swap);

        if (new_path_cost < path_cost)
        {
            path = temp_path;
            path_cost = new_path_cost;

            if (best_path_cost > path_cost)
            {
                best_path_cost = path_cost;
                best_path = path;
                best_path_iteration_numb = iteration_counter;

                PRD = (float)(path_cost - optimum_cost) * 100 / optimum_cost;
                printf("%d\t%d\t%.2f%%\t\n", iteration_counter, path_cost, PRD);

                no_improvement_iteration_counter = 0;
            }
        }
        else if (new_path_cost >= path_cost)
        {
            double rand_prob = get_random_probability();
            double prob = get_probability(path_cost, new_path_cost, temperature);

            if (rand_prob <= prob)
            {
                path = temp_path;
                path_cost = new_path_cost;
            }
            no_improvement_iteration_counter++;
        }

        // if (no_improvement_iteration_counter > pow(number_of_cities, 2) * 10000)
        // {
        //     path = create_random_path(number_of_cities); //shuffle
        //     path_cost = calculate_path_cost(path, weight_matrix);
        //     no_improvement_iteration_counter = 0;
        //     cout << "path shuffled" << endl;
        // }

        end = chrono::steady_clock::now();
        algorithm_duration_time = chrono::duration_cast<chrono::seconds>(end - begin).count();
    }

    if (best_path_cost < path_cost)
    {
        PRD = (float)(best_path_cost - optimum_cost) * 100 / optimum_cost;
        printf("%d\t%d\t%.2f%% best path found before  \n", best_path_iteration_numb, best_path_cost, PRD);
    }

    PRD = (float)(first_path_cost - optimum_cost) * 100 / optimum_cost;
    printf("0\t%d\t%.2f%% \n\n", first_path_cost, PRD);
    matrix_operations::print_path(path);
    return path_cost;
}

vector<vector<int>> create_empty_matrix(int width)
{
    vector<vector<int>> matrix(width);

    for (int i = 0; i < width; i++)
        matrix[i].resize(width);

    return matrix;
}

void print_matrix(vector<vector<int>> matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

struct Move
{
    int first_position = -1;
    int second_position = -1;
    int progress = 0;
};

vector<Move> find_best_moves(int number_of_moves_to_check, vector<int> current_path, vector<vector<int>> weight_matrix, int current_path_cost)
{
    int number_of_cities = weight_matrix[0].size();
    vector<Move> best_moves(number_of_moves_to_check);
    vector<int> temp_path;

    int new_path_cost = -1;
    int progress = INT32_MAX;

    for (int i = 0; i < number_of_moves_to_check; i++)
    {
        Move temp_move;
        temp_path = current_path;

        int pos1_to_swap = get_random_int(1, number_of_cities - 2);
        int pos2_to_swap = get_random_int(1, number_of_cities - 2);

        while (pos1_to_swap == pos2_to_swap)
        {
            pos2_to_swap = get_random_int(1, number_of_cities - 2);
        }

        iter_swap(temp_path.begin() + pos1_to_swap, temp_path.begin() + pos2_to_swap); //swap cities in vector

        // new_path_cost = calculate_path_cost(current_path_cost, current_path, temp_path, weight_matrix, pos1_to_swap, pos2_to_swap);
        new_path_cost = calculate_path_cost(temp_path, weight_matrix);

        progress = current_path_cost - new_path_cost;

        if (pos1_to_swap > pos2_to_swap)
        {
            int buff = pos1_to_swap;
            pos1_to_swap = pos2_to_swap;
            pos2_to_swap = buff;
        }

        temp_move.first_position = pos1_to_swap;
        temp_move.second_position = pos2_to_swap;
        temp_move.progress = progress;

        best_moves[i] = temp_move;
    }

    sort(best_moves.begin(), best_moves.end(), [](const Move &lhs, const Move &rhs)
         { return lhs.progress > rhs.progress; });

    return best_moves;
}

void print_moves(vector<Move> moves)
{
    for (int i = 0; i < moves.size(); i++)
        cout << moves[i].first_position << " : " << moves[i].second_position << " progress = " << moves[i].progress << endl;
}

int TSP::tabu_search(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method)
{

    srand(time(NULL)); // Seed the time

    chrono::steady_clock::time_point begin, end;
    int algorithm_duration_time = 0;
    begin = chrono::steady_clock::now();

    int number_of_cities = weight_matrix[0].size();
    int path_cost = 0, new_path_cost = 0;

    int iteration_counter = 0;
    int best_path_iteration_numb = 0;

    vector<int> path;
    if (initial_solution_method == "random")
        path = create_random_path(number_of_cities); //create random path, first and last city is always 0
    else
        path = create_nearest_neighbor_path(number_of_cities, weight_matrix); //create nearest neighbour path, first and last city is always 0

    vector<int> temp_path = path;

    vector<int> best_path = path;
    path_cost = calculate_path_cost(path, weight_matrix);

    int first_path_cost = path_cost;
    int best_path_cost = path_cost;
    int temp_path_cost = path_cost;

    int no_improvement_iteration_counter = 0;

    int number_of_moves_to_check = number_of_cities * 3;

    int tabu_tenure = number_of_cities / 2;

    vector<vector<int>> tabu_matrix = create_empty_matrix(number_of_cities);

    vector<Move> best_moves;

    float PRD = -1;

    bool is_progress_in_iteration = false;

    while (algorithm_duration_time < 60)
    {
        is_progress_in_iteration = false;
        iteration_counter++;
        temp_path = path;

        best_moves = find_best_moves(number_of_moves_to_check, temp_path, weight_matrix, path_cost);
        for (int i = 0; i < number_of_cities; i++)
        {
            for (int j = 0; j < number_of_cities; j++)
            {
                if (tabu_matrix[i][j] > 0)
                {
                    tabu_matrix[i][j]--;
                }
            }
        }

        for (int i = 0; i < number_of_moves_to_check; i++)
        {
            if (best_path_cost > (temp_path_cost - best_moves[i].progress))
            {
                is_progress_in_iteration = true;
                tabu_matrix[best_moves[i].second_position][best_moves[i].first_position] = tabu_tenure;
                tabu_matrix[best_moves[i].first_position][best_moves[i].second_position] = tabu_tenure;

                iter_swap(temp_path.begin() + best_moves[i].first_position, temp_path.begin() + best_moves[i].second_position); //swap cities in vector
                best_path = temp_path;
                path = temp_path;

                best_path_cost = calculate_path_cost(best_path, weight_matrix);
                path_cost = best_path_cost;
                temp_path_cost = path_cost;

                best_path_iteration_numb = iteration_counter;

                PRD = (float)(path_cost - optimum_cost) * 100 / optimum_cost;
                printf("%d\t%d\t%.2f%%\t\n", iteration_counter, path_cost, PRD);

                break;
            }
        }
        if (!is_progress_in_iteration)
        {
            for (int i = 0; i < number_of_moves_to_check; i++)
            {
                if (tabu_matrix[best_moves[i].second_position][best_moves[i].first_position] == 0)
                {
                    is_progress_in_iteration = true;
                    tabu_matrix[best_moves[i].second_position][best_moves[i].first_position] = tabu_tenure;
                    tabu_matrix[best_moves[i].first_position][best_moves[i].second_position] = tabu_tenure;
                    iter_swap(temp_path.begin() + best_moves[i].first_position, temp_path.begin() + best_moves[i].second_position); //swap cities in vector

                    path = temp_path;

                    temp_path_cost = calculate_path_cost(path, weight_matrix);
                    path_cost = temp_path_cost;

                    no_improvement_iteration_counter++;
                    break;
                }
            }
        }

        // if (no_improvement_iteration_counter > pow(number_of_cities, 2) * 100)
        // {
        //     path = create_random_path(number_of_cities); //shuffle
        //     path_cost = calculate_path_cost(path, weight_matrix);
        //     no_improvement_iteration_counter = 0;
        //     cout << "path shuffled" << endl;
        // }

        end = chrono::steady_clock::now();
        algorithm_duration_time = chrono::duration_cast<chrono::seconds>(end - begin).count();
    }

    // PRD = (float)(temp_path_cost - optimum_cost) * 100 / optimum_cost;
    // printf("%d\t%d\t%.2f%%\t best \n", 0, temp_path_cost, PRD);

    PRD = (float)(first_path_cost - optimum_cost) * 100 / optimum_cost;
    printf("%d\t%d\t%.2f%%\t first \n", 0, first_path_cost, PRD);
    matrix_operations::print_path(temp_path);
    return path_cost;
}

struct Path //individual (chromosome)
{
    vector<int> path;
    int cost = 0;

    Path(vector<int> &path, int cost)
    {
        this->cost = cost;
        this->path = path;
    }

    bool operator<(const Path &str) const //for the sort method
    {
        return (cost < str.cost);
    }
};

int TSP::genetic_algorithm(vector<vector<int>> weight_matrix, int optimum_cost, string initial_solution_method)
{

    srand(time(NULL)); // Seed the time

    const int NUMBER_OF_GENERATIONS = 6000;
    const int POPULATION_SIZE = 1200;
    const int ELITE_SIZE = 150;
    const float MUTATION_CHANCE = 0.05;
    const int CHROMOSOME_SIZE = weight_matrix[0].size(); //number of cities, city = gene

    chrono::steady_clock::time_point begin, end;
    int algorithm_duration_time = 0;
    begin = chrono::steady_clock::now();

    vector<Path> population;

    for (int i = 0; i < POPULATION_SIZE; i++)
    {

        vector<int> chromosome = create_random_path(CHROMOSOME_SIZE); //chromosome contains genes, chromosome = path
        population.push_back(Path(chromosome, calculate_path_cost(chromosome, weight_matrix)));
    }

    int best_path_cost = population.at(0).cost; //fittest chromosome cost
    int first_path_cost = best_path_cost;
    vector<int> best_path;

    double PRD = (double)(first_path_cost - optimum_cost) * 100 / optimum_cost;
    printf("S\t%d\t%.2f%%\n", first_path_cost, PRD);

    for (int i = 0; i < NUMBER_OF_GENERATIONS; i++)
    {
        if (algorithm_duration_time >= 60)
            break;

        sort(population.begin(), population.end()); //sorting ascending by the cost
        if (population.at(0).cost < best_path_cost)
        {
            best_path_cost = population.at(0).cost;
            best_path = population.at(0).path;
            PRD = (double)(best_path_cost - optimum_cost) * 100 / optimum_cost;
            printf("%d\t%d\t%.2f%%\n", i, best_path_cost, PRD);
        }

        vector<Path> mating_pool;

        //survival of the fittest
        for (int i = 0; i < ELITE_SIZE; i++)
        {
            mating_pool.push_back(population.at(i));
        }

        vector<Path> nextGenerationPopulation;

        //genetic crossover of individuals
        while (nextGenerationPopulation.size() < POPULATION_SIZE)
        // while (mating_pool.size() < POPULATION_SIZE)
        {
            //find random positions
            int pos1 = get_random_int(1, CHROMOSOME_SIZE - 2);
            int pos2 = get_random_int(1, CHROMOSOME_SIZE - 2);

            //make sure positions are different
            while (pos1 == pos2)
                pos2 = get_random_int(1, CHROMOSOME_SIZE - 2);

            //make sure pos1 is lower index
            if (pos1 > pos2)
            {
                int buff = pos1;
                pos1 = pos2;
                pos2 = buff;
            }

            //random parents
            vector<int> random_elite_chromosome1 = mating_pool.at(get_random_int(0, mating_pool.size() - 1)).path;
            vector<int> random_elite_chromosome2 = mating_pool.at(get_random_int(0, mating_pool.size() - 1)).path;

            //kids
            vector<int> elite_chromosome_descendant1;
            vector<int> elite_chromosome_descendant2;

            elite_chromosome_descendant1.push_back(0);
            elite_chromosome_descendant2.push_back(0);

            //give the children a randomized string of genes
            for (int i = pos1; i <= pos2; i++)
            {
                elite_chromosome_descendant1.push_back(random_elite_chromosome1[i]);
                elite_chromosome_descendant2.push_back(random_elite_chromosome2[i]);
            }

            //give the children the remaining genes from the other parent (index from 1 because first and last gene is always 0)
            for (int i = 1; i < CHROMOSOME_SIZE; i++)
            {
                if (find(elite_chromosome_descendant1.begin(), elite_chromosome_descendant1.end(), random_elite_chromosome2.at(i)) == elite_chromosome_descendant1.end())
                    elite_chromosome_descendant1.push_back(random_elite_chromosome2[i]);
                if (find(elite_chromosome_descendant2.begin(), elite_chromosome_descendant2.end(), random_elite_chromosome1.at(i)) == elite_chromosome_descendant2.end())
                    elite_chromosome_descendant2.push_back(random_elite_chromosome1[i]);
            }

            // parents:
            // 1 2 3 4 5 6 7 8 9
            // 9 8 7 6 5 4 3 2 1

            // random_index1 = 2
            // random_index2 = 5

            // 1 2 [3 4 5 6] 7 8 9
            // 9 8 [7 6 5 4] 3 2 1

            // kids :
            // [3 4 5 6] 9 8 7 2 1
            // [7 6 5 4] 1 2 3 8 9

            elite_chromosome_descendant1.push_back(0);
            elite_chromosome_descendant2.push_back(0);

            // mating_pool.push_back(Path(elite_chromosome_descendant1, calculate_path_cost(elite_chromosome_descendant1, weight_matrix)));
            // mating_pool.push_back(Path(elite_chromosome_descendant2, calculate_path_cost(elite_chromosome_descendant2, weight_matrix)));
            nextGenerationPopulation.push_back(Path(elite_chromosome_descendant1, calculate_path_cost(elite_chromosome_descendant1, weight_matrix)));
            nextGenerationPopulation.push_back(Path(elite_chromosome_descendant2, calculate_path_cost(elite_chromosome_descendant2, weight_matrix)));
        }

        //mutation
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            if (get_random_probability() <= MUTATION_CHANCE)
            {
                int pos1_to_swap = get_random_int(1, CHROMOSOME_SIZE - 2); //gene index
                int pos2_to_swap = get_random_int(1, CHROMOSOME_SIZE - 2);

                while (pos1_to_swap == pos2_to_swap)
                {
                    pos2_to_swap = get_random_int(1, CHROMOSOME_SIZE - 2);
                }

                // vector<int> chromosome_before_mutation = mating_pool[i].path;
                // iter_swap(mating_pool[i].path.begin() + pos1_to_swap, mating_pool[i].path.begin() + pos2_to_swap); //swap cities in vector

                // int new_path_cost = calculate_path_cost(mating_pool[i].cost, chromosome_before_mutation, mating_pool[i].path, weight_matrix, pos1_to_swap, pos2_to_swap);
                // mating_pool[i].cost = new_path_cost;

                vector<int> chromosome_before_mutation = nextGenerationPopulation[i].path;
                iter_swap(nextGenerationPopulation[i].path.begin() + pos1_to_swap, nextGenerationPopulation[i].path.begin() + pos2_to_swap); //swap cities in vector

                int new_path_cost = calculate_path_cost(nextGenerationPopulation[i].cost, chromosome_before_mutation, nextGenerationPopulation[i].path, weight_matrix, pos1_to_swap, pos2_to_swap);
                nextGenerationPopulation[i].cost = new_path_cost;
            }
        }

        // population = mating_pool;
        population = nextGenerationPopulation;

        end = chrono::steady_clock::now();
        algorithm_duration_time = chrono::duration_cast<chrono::seconds>(end - begin).count();
    }

    sort(population.begin(), population.end()); //sorting ascending by the cost

    PRD = (double)(first_path_cost - optimum_cost) * 100 / optimum_cost;
    printf("%d\t%d\t%.2f%%\n", 0, first_path_cost, PRD);

    if (best_path_cost > population.at(0).cost)
    {
        best_path_cost = population.at(0).cost;
        best_path = population.at(0).path;
    }

    PRD = (double)(best_path_cost - optimum_cost) * 100 / optimum_cost;
    printf("F\t%d\t%.2f%%\n", best_path_cost, PRD);

    matrix_operations::print_path(best_path);

    return best_path_cost;
}