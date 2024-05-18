#pragma once
#include "./config.h"

void print_vehicles(Vehicles vehicles) {
    cout << "\nPrinting Vehicles: " << endl;
    for (Vehicle vehicle : vehicles) {
        vehicle.print();
    }
}

void print_routes(Solution solution) {
    auto routes = solution.routes;
    for (int i = 0; i < routes.size(); i++) {
        cout << "\n\tVehicle " << solution.vehicles_indexes[i] << "'s Route(size = " << routes[i].size() << "): " << endl;
        for (int j = 0; j < routes[i].size(); j++) {
            if (routes[i][j].type == Node_type::CUSTOMER) {
                cout << "\t\tCustomer " << routes[i][j].id << " -> " << " x: " << routes[i][j].location.x << " y: " << routes[i][j].location.y << " weight: " << routes[i][j].order_weight << endl;
            }
            //cout << "\t\Node " << routes[i][j].id << " -> " << " x: " << routes[i][j].location.x << " y: " << routes[i][j].location.y << " weight: " << routes[i][j].order_weight << endl;
        }
    }
}

void print_population(Population population) {
    cout << "\nPrinting Population: " << endl;
    cout << "\nPopulation total_cost: " << population.total_cost << endl;
    for (int i = 0; i < population_size; i++) {
        cout << "Solution " << i << " Cost: " << population.solutions[i].cost << endl;
        cout << "Printing Routes: " << endl;
        print_routes(population.solutions[i]);
    }
}

void print_best_solution(Population best_solutions) {
    Solution best_solution = get_best_sol_from_current_pop(best_solutions);

    cout << "\Best Solution: " << endl;
    cout << "Cost: " << best_solution.cost << endl;
    print_routes(best_solution);
}