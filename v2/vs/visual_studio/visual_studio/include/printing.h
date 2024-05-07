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
        }
    }
}

void print_population(Population population) {
    cout << "\nPrinting Population: " << endl;
    for (int i = 0; i < population.size(); i++) {
        cout << "Solution " << i << " Cost: " << population[i].cost << endl;
        cout << "Printing Routes: " << endl;
        print_routes(population[i]);
    }
}