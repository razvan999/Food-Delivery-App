#include <iostream>
#include <vector>

#include "../include/graph.h"
#include "../include/vehicle.h"
#include "../include/read_file.h"

using namespace std;

typedef vector<Vehicle> Vehicles;

void print_vehicles(Vehicles vehicles) {
    cout << "\nPrinting Vehicles: " << endl;
    for (Vehicle vehicle : vehicles) {
        vehicle.print();
    }
}

int main(int argc, char *argv[]) {
    Graph graph;
    Vehicles vehicles;
    initialize(argc, argv, graph, vehicles);

    // graph.print();
    // print_vehicles(vehicles);







    return 0;
}