#include <iostream>
#include <vector>

#include "./include/graph.h"
#include "./include/vehicle.h"
#include "./include/read_file.h"

using namespace std;

typedef vector<Vehicle> Vehicles;

int main(int argc, char *argv[]) {
    Graph graph;
    Vehicles vehicles;
    initialize(argc, argv, graph, vehicles);

    cout << "Number of vehicles: " << vehicles.size() << endl;




    return 0;
}