#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "./include/genetic_algo.h"
#include "./include/printing.h"

using namespace std;

void print_vehicles(Vehicles vehicles);

void print_population(Population population);

//int main(int argc, char* argv[]) {
int main() {
    Graph graph;
    Vehicles vehicles;
    char name[100] = "ulysses-n16-k3";
    initialize(2, name, graph, vehicles);

     //graph.print();
     //print_vehicles(vehicles);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    cout << "------Start------" << endl << endl;

    Population population = generate_population(graph, vehicles);
    update_cost(population);
    selection(population);
    //print_population(population);

    //int generation = 0;
    //while (generation < generation_limit) {
    //    update_cost(population);
    //    selection(population);
    //    //crossover();
    //    //mutation();

    //    generation++;
    //}

    cout << "\n------End------" << endl;
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "a durat: " << elapsed_seconds.count() << "s" << endl;

    return 0;
}