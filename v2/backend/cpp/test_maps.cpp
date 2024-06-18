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
    Configuration configuration;
    initialize_maps(graph, vehicles, configuration);

    graph.print();
    print_vehicles(vehicles);
    configuration.print();

    updateConfiguration(configuration, population_size, generation_limit, graph.get_locations_size(), vehicles.size());


    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    cout << "------Start------" << endl;

    Population population;
    population.solutions.reserve(population_size);
    population = generate_population(graph, vehicles, true);
    Population best_solutions;
    best_solutions.solutions.reserve(population_size);

    // print_population(population);
    update_cost(population);
    Solution best_initial_solution = get_best_sol_from_current_pop(population);
    cout << "Initial best solution: " << best_initial_solution.cost << endl;


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

    // // Solution best_solution = generate_solution(graph, vehicles);
    // // print_routes(best_solution);

    cout << "best_cost: " << best_solution.cost << endl;

    // // print_matrix();
    write_json_output_maps(best_solution, elapsed_seconds);


    // cout << "-----------------------------" << endl;

    // cout<<best_solution.cost<<endl;
    // cout<<best_solution.routes.size()<<endl;

    // print_best_solution(population);

    return 0;
}

    void test_variety_prob(Graph graph, Vehicles vehicles) { // trebuie modificat si variety_prob
            population_size = 5;
    Population population;
    population.solutions.reserve(population_size);
    population = generate_population(graph, vehicles, true);

    cout << endl;
    print_population(population);

    for (int i = 0; i < population_size; i++)
    {
        cout << "veh " <<  population.solutions[i].vehicles_indexes.size() << endl;
    }

    cout << endl << population_size << endl;
    }