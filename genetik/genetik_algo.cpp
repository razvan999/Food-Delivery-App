#include "../classes/graph.h"
#include "../classes/courier.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <numeric> 

#define stores_path "./genetik_data/stores.json"
#define customers_path "./genetik_data/customers.json"
#define vehicles_path "./genetik_data/vehicles.json"

#define population_size 10
#define iteratii 200

using namespace std;

typedef vector<int> Individ;
typedef vector<Individ> Population;
typedef vector<courier> Vehicles;

struct Solution {
    Individ individ;
    float score = -1;
};

int get_random_int(int max, int min = 1) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

Individ generate_individ(graph g, vector<courier> vehicles) {
    Individ new_individ;
    vector<int> nodes_ids;
    auto nodes = g.get_nodes();
    for (int i = g.get_stores_count(); i < nodes.size(); i++) {
        nodes_ids.push_back(nodes[i].id);
    }

    for (int i = 0; i < vehicles.size(); i++) {
        new_individ.push_back(vehicles[i].get_id());

        int vehicle_stops = get_random_int(g.get_nodes_count() - vehicles.size() - g.get_stores_count());
        new_individ.push_back(vehicle_stops);
        for (int j = 0; j < vehicle_stops; j++) {
            new_individ.push_back(nodes_ids[get_random_int(nodes_ids.size(), 0)]);
        }
    }

    return new_individ;
}

Population generate_population(graph g, vector<courier> vehicles){
    Population new_population;

    for (int i = 0; i < population_size; i++) {
        new_population.push_back(generate_individ(g, vehicles));
    }

    return new_population;
}

float euclidian_distance_for_nodes(node_location a, node_location b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float euclidian_distance_for_couriers(courier_location a, node_location b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float calculate_distance(Individ individ, graph g, Vehicles vehicles) {
    float distance = 0;
    int i = 0;
    for (auto vehicle : vehicles) { // distanta pentru fiecare vehicul
        int vehicle_id = individ[i++];
        int vehicle_stops = individ[i++];
        
        distance += euclidian_distance_for_couriers(vehicles[vehicle_id].get_location(), g.get_node_location(individ[i]));
        int j;
        for (j = 1; j < vehicle_stops - 1; j++) {
            distance += euclidian_distance_for_nodes(g.get_node_location(individ[i + j]), g.get_node_location(individ[i + j + 1]));
        }
        distance += euclidian_distance_for_couriers(vehicles[vehicle_id].get_location(), g.get_node_location(individ[j]));

        i += vehicle_stops;
    }

    return distance;
}




void selection(Population &pop, graph g, vector<courier> vehicles) {
    // Population new_population;
    Population mating_pool;
    vector<float> fitness;
    float total_distance = 0;

    for (int i = 0; i < pop.size(); i++) {
        float distance = calculate_distance(pop[i], g, vehicles);
        total_distance += distance;
        // fitness.push_back(calculate_distance(pop[i], g, vehicles));
        // fitness.push_back(calculate_distance(pop[i], g, vehicles) / pop.size());
        fitness.push_back(1 / distance);

        // cout << "aaaa " << fitness[i] << endl;
    }

    // sortez in functie de fitness
    for (int i = 0; i < pop.size(); i++) {
        for (int j = i + 1; j < pop.size(); j++) {
            if (fitness[i] > fitness[j]) {
                swap(fitness[i], fitness[j]);
                swap(pop[i], pop[j]);
            }
        }
    }

    for (int i = 0; i < pop.size(); i++) {
        cout << fitness[i] << " ";
    }
    // cout << endl;
    // for (int i = 0; i < pop.size(); i++) {
    //     cout <<  (total_distance / fitness[i] / 1000000000) << " ";
    // }

    cout << endl << endl;

    double totalFitness = accumulate(fitness.begin(), fitness.end(), 0.0);

    // Calculate percentages
    std::vector<double> percentages;
    for (double fit : fitness) {
        percentages.push_back((fit / totalFitness) * 100);
    }



    int total = 0;
    for (int i = 0; i < pop.size(); i++) {
        // cout << percentages[i] << " ";
        total += int(percentages[i]);
    }
    // cout << endl << total << endl;
    // cout << endl << endl;

    for (int i = 0; i < pop.size(); i++) {
        // cout << percentages[i] << " ";
        // cout << int(percentages[i]) << " ";
        for (int j = 0; j < int(percentages[i]); j++) {
            mating_pool.push_back(pop[i]);
        }
    }

    pop = mating_pool;
}

void crossover(Population &mating_pool, graph g, vector<courier> vehicles) {
    Population new_population;
    for (int i = 0; i < population_size; i++) {
        Individ parent1 = mating_pool[get_random_int(mating_pool.size() - 1)];
        Individ parent2 = mating_pool[get_random_int(mating_pool.size() - 1)];

        Individ child;
        

        new_population.push_back(child);
    }

    mating_pool = new_population;
}

void mutation() {

}







Solution genetic_algorithm(Population &pop, graph g, vector<courier> vehicles) {
    Solution sol;
    float lower_bound = 12.5; // trebuie sa folosesc prim's algorithm sau nearest neighbour pentru lowerbound

    for (int i = 0; i < iteratii; i++) {

        selection(pop, g, vehicles);
        crossover(pop, g, vehicles);
        mutation();
    }

    return sol;
}

void print_individ(Individ ind);

int main() {
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    cout << "------Start------" << endl << endl;
    
    graph g(stores_path, customers_path);
    vector<courier> vehicles = initiate_vehicles(vehicles_path);

    Population pop = generate_population(g, vehicles);
    Solution best_solution = genetic_algorithm(pop, g, vehicles);

    cout << "Best solution: " << best_solution.score << endl;

    // int counter = 0;
    // vector<Solution> solutions;
    // Solution solution = {Population(), 1000000};
    // while (counter < 5) {
    //     Population new_pop = generate_population(g, vehicles);
    //     solutions.push_back(genetic_algorithm(new_pop, g, vehicles));

    //     if (solution.score > solutions[counter].score) {
    //         solution = solutions[counter];
    //         solution.individ = solutions[counter].individ;
    //     }
        
    //     counter++;
    // }

    // cout << "sol score: " << solution.score << endl;

    cout << endl << "------End------" << endl;
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "a durat: " << elapsed_seconds.count() << "s" << endl;

    return 0;
}

void print_individ(Individ ind) {
    for (int i = 0; i < ind.size(); i++) {
        cout << ind[i] << " ";
    }
    cout << endl;
}