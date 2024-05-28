#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "./nlohmann/json.hpp"

#include "./include/genetic_algo.h"
#include "./include/printing.h"
#include "./include/aux.h"

using namespace std;

int main()
{
    Graph graph;
    Vehicles vehicles;
    initialize_maps(graph, vehicles);

    // graph.print();
    // print_vehicles(vehicles);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    cout << "------Start------" << endl
         << endl;

    Population population;
    population.solutions.reserve(population_size);
    population = generate_population(graph, vehicles, true);
    Population best_solutions;
    best_solutions.solutions.reserve(population_size);

    update_cost(population);
    Solution best_initial_solution = get_best_sol_from_current_pop(population);
    cout << "Initial best solution: " << best_initial_solution.cost << endl;
    cout << endl
         << endl;

    float initial_cost = population.total_cost;
    cout << "------Start Genetic Algorithm------" << endl;
    int generation = 0;
    while (generation < generation_limit)
    {
        selection(population);
        crossover(vehicles, population);
        mutation(vehicles, population);

        cout << generation << endl;
        generation++;
    }

    cout << "------End Genetic Algorithm------" << endl;
    cout << "initial cost: " << initial_cost << endl;
    cout << "final cost: " << population.total_cost << endl;

    cout << endl
         << endl;
    Solution best_solution = get_best_sol_from_current_pop(population);

    cout << "\n------End------" << endl;
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "a durat: " << elapsed_seconds.count() << "s" << endl;

    // Solution best_solution = generate_solution(graph, vehicles);
    // print_routes(best_solution);

    // print_matrix();
    write_json_output_maps(best_solution, elapsed_seconds);

    return 0;
}