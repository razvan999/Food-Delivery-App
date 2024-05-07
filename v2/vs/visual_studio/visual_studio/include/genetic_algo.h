#pragma once
#include <random>

#include "./config.h"

void clusterizare(Graph graph, Vehicles vehicles) {} // poate imi trebuie cand am mai multe depozite




int get_random_int(int max, int min = 0) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

float euclidian_distance(Node a, Node b) {
	return sqrt(pow(a.location.x - b.location.x, 2) + pow(a.location.y - b.location.y, 2));
}

vector<int> generate_unallocated_vehicle(Vehicles vehicles) {
	vector<int> unallocated_vehicles;
	for (int i = 0; i < vehicles.size(); i++) {
		unallocated_vehicles.push_back(vehicles[i].get_id());
	}

	return unallocated_vehicles;
}

bool extend_route(Vehicle current_vehicle, vector<int> unallocated_nodes_index, vector<Node> customers, vector<int> &GOOD_NODES) {
	float available_capacity = current_vehicle.get_capacity() - current_vehicle.get_used_capacity();

	if (available_capacity == 0) {
		return false;
	}

	vector<int> good_nodes; // indexurile nodurilor care pot fi adaugate in ruta

	for (int i = 0; i < unallocated_nodes_index.size(); i++) {
		if (available_capacity >= customers[unallocated_nodes_index[i]].order_weight) {
			good_nodes.push_back(unallocated_nodes_index[i]);
		}
	}

	if (good_nodes.size() > 0) {
		for (int i = 0; i < good_nodes.size(); i++) {
			GOOD_NODES.push_back(good_nodes[i]);
		}
		return true;
	}

	return false;
}

Solution generate_solution(Graph graph, Vehicles vehicles) {
	Solution solution;

	// am nevoie de un fel de vector de noduri vizitate -> sa nu pun acelasi nod in 2 rute diferite
	vector<int> unallocated_nodes_index;
	Route customers = graph.get_customers();
	for (int i = 0; i < customers.size(); i++) {
		unallocated_nodes_index.push_back(i);
	}

	// sa vad daca va fi nevoie de mai mult de o ruta pentru fiecare vehicul
	vector <int> unallocated_vehicles;
	while (unallocated_nodes_index.size() > 0) {
		if (unallocated_vehicles.size() == 0) {
			unallocated_vehicles = generate_unallocated_vehicle(vehicles);
		}

		int index_lucky_vehicle = get_random_int(unallocated_vehicles.size() - 1);
		Vehicle current_vehicle = vehicles[unallocated_vehicles[index_lucky_vehicle]];
		unallocated_vehicles.erase(unallocated_vehicles.begin() + index_lucky_vehicle);

		solution.vehicles_indexes.push_back(current_vehicle.get_id());
		Route new_route;
		new_route.push_back(current_vehicle.get_depot()); // adaug nodul depozitului
		vector<int> good_nodes;

		while (extend_route(current_vehicle, unallocated_nodes_index, customers, good_nodes)) {
			int index_lucky_node = get_random_int(good_nodes.size() - 1);
			current_vehicle.set_used_capacity(current_vehicle.get_used_capacity() + customers[good_nodes[index_lucky_node]].order_weight);
			new_route.push_back(customers[good_nodes[index_lucky_node]]);

			//unallocated_nodes_index.erase(unallocated_nodes_index.begin() + good_nodes[index_lucky_node]);
			unallocated_nodes_index.erase(unallocated_nodes_index.begin() + index_lucky_node);
			good_nodes.clear();
		}
		new_route.push_back(current_vehicle.get_depot()); // adaug nodul depozitului
		solution.routes.push_back(new_route);

	}

	return solution;
}

Population generate_population(Graph graph, Vehicles vehicles) {
	Population population;

	//for (int i = 0; i < population_size; i++) {
		population.push_back(generate_solution(graph, vehicles));
	//}

	return population;
}

float calculare_cost_ruta(Route route) {
	float cost = 0;
	for (int i = 0; i < route.size() - 1; i++) {
		cost += euclidian_distance(route[i], route[i + 1]);
	}
	cost += euclidian_distance(route[route.size() - 1], route[0]);

	return cost;
}

float calculare_cost_solutie(Solution solution) {
	float cost = 0;
	for (int i = 0; i < solution.routes.size(); i++) {
		cost += calculare_cost_ruta(solution.routes[i]);
	}

	return cost;
}

void update_cost(Population& population) {
	for (int i = 0; i < population.size(); i++) {
		population[i].cost = calculare_cost_solutie(population[i]);
	}
}










void selection(Population population) {

}