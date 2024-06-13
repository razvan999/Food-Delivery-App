#pragma once
#include <random>
#include <algorithm>
#include <numeric>
#include <set>

#include "./config.h"

void mutate_solution(Vehicles vehicles, Solution& solution);

float get_random_01() {
	random_device r;
	mt19937 gen(r());
	uniform_real_distribution<> distrib(0, 1);

	return distrib(gen);
}

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

float addVariety(float available_capacity, float current_weight) {
	if (get_random_01() > variety_prob) {
		return 0;
	}

	return available_capacity - current_weight;
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
			// int index_lucky_node = get_random_int(good_nodes.size() - 1);
			// current_vehicle.set_used_capacity(current_vehicle.get_used_capacity() + customers[good_nodes[index_lucky_node]].order_weight);
			// new_route.push_back(customers[good_nodes[index_lucky_node]]);
			///////////////////////////////////////////////////////////////////////////////////////////////

			int index_lucky_node = get_random_int(good_nodes.size() - 1);
			current_vehicle.set_used_capacity(current_vehicle.get_used_capacity() + customers[good_nodes[index_lucky_node]].order_weight + addVariety(current_vehicle.get_unused_capacity(), customers[good_nodes[index_lucky_node]].order_weight));
			new_route.push_back(customers[good_nodes[index_lucky_node]]);

			///////////////////////////////////////////////////////////////////////////////////////////////
			//unallocated_nodes_index.erase(unallocated_nodes_index.begin() + good_nodes[index_lucky_node]);
			unallocated_nodes_index.erase(unallocated_nodes_index.begin() + index_lucky_node);
			good_nodes.clear();
		}
		new_route.push_back(current_vehicle.get_depot()); // adaug nodul depozitului
		solution.routes.push_back(new_route);
	}

	return solution;
}

Population generate_population(Graph graph, Vehicles vehicles, bool flag = false) {
	if (flag) {
		maps_flag = true;
	}

	Population population;

	for (int i = 0; i < population_size; i++) {
		population.solutions.push_back(generate_solution(graph, vehicles));
	}

	return population;
}

float maps_distance(Node a, Node b) {
	// return euclidian_distance(a, b);
	return cost_matrix[a.id][b.id];
}

float calculare_cost_ruta(Route route) {
	float cost = 0;
	for (int i = 0; i < route.size() - 1; i++) {
		if (maps_flag) {
			cost += maps_distance(route[i], route[i + 1]);
		}
		else {
			cost += euclidian_distance(route[i], route[i + 1]);
		}
	}

	if (maps_flag) {
		cost += maps_distance(route[route.size() - 1], route[0]);
	}
	else {
		cost += euclidian_distance(route[route.size() - 1], route[0]);
	}

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
	float total_cost = 0;
	for (int i = 0; i < population.solutions.size(); i++) {
		float cost = calculare_cost_solutie(population.solutions[i]);
		total_cost += cost;
		population.solutions[i].cost = cost;
	}

	population.total_cost = total_cost;
}

int get_random_selection_index() {
	return get_random_int(population_size - 1);
}
//////////////////////////////
template<typename T>
std::vector<T> sample_without_replacement(const std::vector<T>& population, size_t sampleSize) {
	std::vector<T> sample;
	std::random_device rd;
	std::mt19937 gen(rd());

	size_t populationSize = population.size();
	for (size_t i = 0; i < sampleSize; ++i) {
		std::uniform_int_distribution<size_t> dist(0, populationSize - i - 1);
		size_t selectedIndex = dist(gen);
		sample.push_back(population[selectedIndex]);
		std::swap(population[selectedIndex], population[populationSize - i - 1]);
	}

	return sample;
}

int getRandomIndex(int populationSize) {
	static default_random_engine generator(time(0)); // Seed with current time
	uniform_int_distribution<int> distribution(0, populationSize - 1);
	return distribution(generator);
}







































void rank_selection(Population& population) {
	update_cost(population);

    // Sort population by cost in ascending order (best solutions have lower cost)
    std::sort(population.solutions.begin(), population.solutions.end(), 
              [](const Solution& a, const Solution& b) {
                  return a.cost < b.cost;
              });

    // Calculate total rank sum for the probabilities
    int population_size = population.solutions.size();
    int total_rank = (population_size * (population_size + 1)) / 2;

    // Generate cumulative rank probabilities
    std::vector<float> cumulative_probabilities(population_size, 0.0f);
    cumulative_probabilities[0] = static_cast<float>(population_size) / total_rank;
    for (int i = 1; i < population_size; ++i) {
        cumulative_probabilities[i] = cumulative_probabilities[i - 1] + 
                                      static_cast<float>(population_size - i) / total_rank;
    }

    Population mating_pool;
    mating_pool.solutions.reserve(population_size);

    for (int i = 0; i < population_size; ++i) {
        float random_value = get_random_01();
        for (int j = 0; j < population_size; ++j) {
            if (random_value <= cumulative_probabilities[j]) {
                mating_pool.solutions.push_back(population.solutions[j]);
                break;
            }
        }
    }

    population = mating_pool;
    update_cost(population);
}

void tournament_selection(Population& population, int tournamentSize = tournament_size) {
//void selection(Population& population, int tournamentSize = population_size) {
	update_cost(population);

	Population mating_pool;
	mating_pool.solutions.reserve(population.solutions.size());
	while (mating_pool.solutions.size() < population.solutions.size()) {
		std::vector<Solution> tournament;
		for (int i = 0; i < tournamentSize; ++i) {
			int random_index = getRandomIndex(population.solutions.size());
			tournament.push_back(population.solutions[random_index]);
		}

		Solution best_solution = tournament[0];
		for (int i = 1; i < tournamentSize; ++i) {
			if (tournament[i].cost < best_solution.cost) {
				best_solution = tournament[i];
			}
		}

		mating_pool.solutions.push_back(best_solution);
	}

	population = mating_pool;
	update_cost(population);
}

void wheel_selection(Population& population) {
	update_cost(population);

	Population mating_pool;
	mating_pool.solutions.reserve(population_size);
	vector<float> probability;
	probability.reserve(population_size);

	for (int i = 0; i < population_size; i++) {
		probability.push_back(population.solutions[i].cost / population.total_cost);
	}

	for (int i = 0; i < population_size; i++) {
		float random_01 = get_random_01();
		float probability_sum = 0;

		for (int j = 0; j < population_size; j++) {
			probability_sum += probability[j];
			if (random_01 <= probability_sum + epsilon) {
				mating_pool.solutions.push_back(population.solutions[j]);
				break;
			}
		}
	}

	population = mating_pool;
	update_cost(population);
}

void selection(Population& population, int selection_type) {
	if (selection_type == 0) {
		rank_selection(population);
	}

	if (selection_type == 1) {
		tournament_selection(population);
	}

	if (selection_type == 2) {
		wheel_selection(population);
	}
}








































































Solution get_best_sol_from_current_pop(Population current_population) {
	float best_cost = current_population.solutions[0].cost;
	int best_index = 0;
	for (int i = 1; i < population_size; i++) {
		if (current_population.solutions[i].cost < best_cost) {
			best_cost = current_population.solutions[i].cost;
			best_index = i;
		}
	}

	return current_population.solutions[best_index];
}

void update_best(Population current_population, Population &best_solutions) {
	Solution best_solution = get_best_sol_from_current_pop(current_population);
	best_solutions.solutions.push_back(best_solution);
}

bool check_switch(vector<simplified> child, simplified val) {
	for (int i = 0; i < child.size(); i++) {
		if (child[i].id == val.id) {
			return false;
		
		}
	}

	return true;
}

int poz_in_switch(vector<Switch> switches, simplified value, vector<Switch> prev_values) {
	for (int i = 0; i < switches.size(); i++) {
		if (switches[i].s1.id == value.id || switches[i].s2.id == value.id) {
			bool flag = true;
			for (int j = 0; j < prev_values.size(); j++) {
				if ((switches[i].s1.id == prev_values[j].s1.id && switches[i].s2.id == prev_values[j].s2.id) || (switches[i].s1.id == prev_values[j].s2.id && switches[i].s2.id == prev_values[j].s1.id)) {
					flag = false;
					break;
				}
			}

			if (flag) {
				return i;
			}
		}
	}

	return -1;
}

bool exists_in_vector(vector<simplified> vector, int value) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i].id == value) {
			return true;
		
		}
	}

	return false;
}

void print_simplified_child(vector<simplified> child, int cut_point1, int cut_point2) {
	for (int i = 0; i < child.size(); i++) {
		if (i == cut_point1 || i == cut_point2) {
			cout << " | ";
		}

		cout << child[i].id << " ";
	}
	cout << endl;
}

bool cycle_in_switches(vector<Switch> switches, Switch new_switch) {
	for (int i = 0; i < switches.size(); i++) {
		if ((switches[i].s1.id == new_switch.s1.id && switches[i].s2.id == new_switch.s2.id) || (switches[i].s1.id == new_switch.s2.id && switches[i].s2.id == new_switch.s1.id)) {
			return true;
		}
	}

	return false;
}

vector<float> calculateNormalizedProbabilities(const Population& population) {
	vector<float> probabilities;
	probabilities.reserve(population.solutions.size());

	float total_fitness = population.total_cost;
	for (const auto& solution : population.solutions) {
		probabilities.push_back(1 - (solution.cost / total_fitness));
	}

	// Normalize probabilities
    float sum = accumulate(probabilities.begin(), probabilities.end(), 0.0f);
	for (float& prob : probabilities) {
		prob /= sum;
	}

	return probabilities;
}

//parents_for_crossover_indices get_parents(const Population& population, const vector<float>& probabilities) {
//	parents_for_crossover_indices parents_indices;
//	parents_indices.parent1 = -1;
//	parents_indices.parent2 = -1;
//
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_real_distribution<float> dis(0.0f, 1.0f);
//	float rand1 = dis(gen);
//	float rand2 = dis(gen);
//
//	float cumulative_probability = 0.0f;
//	for (size_t i = 0; i < population.solutions.size(); ++i) {
//		cumulative_probability += probabilities[i];
//		if (cumulative_probability >= rand1 && parents_indices.parent1 == -1) {
//			parents_indices.parent1 = static_cast<int>(i);
//		}
//		if (cumulative_probability >= rand2 && parents_indices.parent2 == -1 && static_cast<int>(i) != parents_indices.parent1) {
//			parents_indices.parent2 = static_cast<int>(i);
//		}
//		if (parents_indices.parent1 != -1 && parents_indices.parent2 != -1) {
//			break;
//		}
//	}
//
//	if (parents_indices.parent2 == -1) {
//		do {
//			parents_indices.parent2 = (parents_indices.parent2 + 1) % static_cast<int>(population.solutions.size());
//		} while (parents_indices.parent2 == parents_indices.parent1);
//	}
//
//	return parents_indices;
//}

bool check_equal_solutions(const Solution& sol1, const Solution& sol2) {
	if (std::abs(sol1.cost - sol2.cost) > epsilon) {
		return false;
	}

	if (sol1.routes.size() != sol2.routes.size()) {
		return false;
	}

	for (size_t i = 0; i < sol1.routes.size(); ++i) {
		if (sol1.routes[i].size() != sol2.routes[i].size()) {
			return false;
		}

		for (size_t j = 0; j < sol1.routes[i].size(); ++j) {
			if (sol1.routes[i][j].id != sol2.routes[i][j].id) {
				return false;
			}
		}
	}

	return true;
}

struct SolutionComparator {
	bool operator()(const Solution& lhs, const Solution& rhs) const {
		// Compare by cost first
		if (lhs.cost != rhs.cost) {
			return lhs.cost < rhs.cost;
		}
		// Then compare by routes
		if (lhs.routes.size() != rhs.routes.size()) {
			return lhs.routes.size() < rhs.routes.size();
		}
		for (size_t i = 0; i < lhs.routes.size(); ++i) {
			if (lhs.routes[i].size() != rhs.routes[i].size()) {
				return lhs.routes[i].size() < rhs.routes[i].size();
			}
			for (size_t j = 0; j < lhs.routes[i].size(); ++j) {
				if (lhs.routes[i][j].id != rhs.routes[i][j].id) {
					return lhs.routes[i][j].id < rhs.routes[i][j].id;
				}
			}
		}
		return false;
	}
};

parents_for_crossover_indices get_parents(Vehicles vehicles, Population& population, const std::vector<float>& probabilities) {
	parents_for_crossover_indices parents_indices;
	parents_indices.parent1 = -1;
	parents_indices.parent2 = -1;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	float rand1 = dis(gen);
	float rand2 = dis(gen);

	float cumulative_probability = 0.0f;
	for (size_t i = 0; i < population.solutions.size(); ++i) {
		cumulative_probability += probabilities[i];
		if (cumulative_probability >= rand1 && parents_indices.parent1 == -1) {
			parents_indices.parent1 = static_cast<int>(i);
		}
		if (cumulative_probability >= rand2 && parents_indices.parent2 == -1 && static_cast<int>(i) != parents_indices.parent1) {
			parents_indices.parent2 = static_cast<int>(i);
		}
		if (parents_indices.parent1 != -1 && parents_indices.parent2 != -1) {
			break;
		}
	}

	if (parents_indices.parent2 == -1) {
		do {
			parents_indices.parent2 = (parents_indices.parent2 + 1) % static_cast<int>(population.solutions.size());
		} while (parents_indices.parent2 == parents_indices.parent1);
	}

	set<Solution, SolutionComparator> unique_solutions(population.solutions.begin(), population.solutions.end());
	if (unique_solutions.size() < 2) {
		cout << "Not enough unique parents in the population." << endl;
		
		cout << population.solutions[parents_indices.parent1].cost << " ";
		mutate_solution(vehicles, population.solutions[parents_indices.parent2]);
		update_cost(population);
		cout << population.solutions[parents_indices.parent2].cost << endl;


		return parents_indices;
	}

	while (check_equal_solutions(population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2])) {
		parents_indices.parent2 = (parents_indices.parent2 + 1) % static_cast<int>(population.solutions.size());
	}

	return parents_indices;
}



















void twocut_crossover(Vehicles vehicles, Solution parent1, Solution parent2, Solution &child1, Solution &child2) {
	vector<simplified> smplified_parent1, smplified_parent2, simplified_child1, simplified_child2;
	for (int i = 0; i < parent1.routes.size(); i++) {
		Route route = parent1.routes[i];

		for (int j = 0; j < route.size(); j++) {
			if (route[j].type == Node_type::CUSTOMER) {
				smplified_parent1.push_back({ route[j].id , i, j});
				simplified_child1.push_back({ -1, i, j });
			}
		}
	}

	for (int i = 0; i < parent2.routes.size(); i++) {
		Route route = parent2.routes[i];

		for (int j = 0; j < route.size(); j++) {
			if (route[j].type == Node_type::CUSTOMER) {
				smplified_parent2.push_back({ route[j].id , i, j });
				simplified_child2.push_back({ -1, i, j });
			}
		}
	}

	int cut_point1 = get_random_int(smplified_parent1.size() - 3, 1);
	int cut_point2 = get_random_int(smplified_parent1.size() - 2, cut_point1 + 1);

	//cut_point1 = 5;
	//cut_point2 = 8;

	//cout << endl;
	//cout << "towcut crossover" << endl;
	//print_simplified_child(smplified_parent1, cut_point1, cut_point2);
	//cout << endl;
	//print_simplified_child(smplified_parent2, cut_point1, cut_point2);
	//cout << endl;
	//cout << "Cut points: " << cut_point1 << " " << cut_point2 << endl;

	int size = smplified_parent1.size();
	vector<Switch> switches;
	for (int i = cut_point1; i < cut_point2; i ++) {
		simplified_child1[i] = smplified_parent1[i];
		simplified_child2[i] = smplified_parent2[i];

		if (smplified_parent1[i].id != smplified_parent2[i].id) {
			if (cycle_in_switches(switches, { smplified_parent1[i], smplified_parent2[i] }) == false) {
				switches.push_back({ smplified_parent1[i], smplified_parent2[i] });
			}
		}
	}

	// adaug ce pot din partile 1 si 3 astfel incat sa nu am duplicate
	for (int i = 0; i < cut_point1; i ++) {
		if (exists_in_vector(simplified_child1, smplified_parent2[i].id) == false) {
			simplified_child1[i] = smplified_parent2[i];
		}

		if (exists_in_vector(simplified_child2, smplified_parent1[i].id) == false) {
			simplified_child2[i] = smplified_parent1[i];
		}
	}

	for (int i = cut_point2; i < size; i++) {
		if (exists_in_vector(simplified_child1, smplified_parent2[i].id) == false) {
			simplified_child1[i] = smplified_parent2[i];
		}

		if (exists_in_vector(simplified_child2, smplified_parent1[i].id) == false) {
			simplified_child2[i] = smplified_parent1[i];
		}
	}

	for (int i = 0; i < cut_point1; i++) {
		if (simplified_child1[i].id == -1) {
			simplified parent_value = smplified_parent2[i];
			vector<Switch> prev_values;
			do {
				int index_switch = poz_in_switch(switches, parent_value, prev_values);
				if (index_switch == -1) {
					cout << "index_switch == -1" << endl;
					exit(111);
				}

				simplified new_val = switches[index_switch].s1;
				prev_values.push_back(switches[index_switch]);
				if (new_val.id == parent_value.id) {
					new_val = switches[index_switch].s2;
				}

				if (check_switch(simplified_child1, new_val)) {
					simplified_child1[i] = new_val;
					break;
				}
				
				parent_value = new_val;
			} while (true);
		}

		if (simplified_child2[i].id == -1) {
			simplified parent_value = smplified_parent1[i];
			vector<Switch> prev_values;
			do {
				int index_switch = poz_in_switch(switches, parent_value, prev_values);
				if (index_switch == -1) {
					cout << "index_switch == -1" << endl;
					exit(111);
				}

				simplified new_val = switches[index_switch].s1;
				prev_values.push_back(switches[index_switch]);
				if (new_val.id == parent_value.id) {
					new_val = switches[index_switch].s2;
				}

				if (check_switch(simplified_child2, new_val)) {
					simplified_child2[i] = new_val;
					break;
				}

				parent_value = new_val;
			} while (true);
		}
	}
	
	for (int i = cut_point2; i < size; i++) {
		if (simplified_child1[i].id == -1) {
			simplified parent_value = smplified_parent2[i];
			vector<Switch> prev_values;
			do {
				int index_switch = poz_in_switch(switches, parent_value, prev_values);
				if (index_switch == -1) {
					cout << "index_switch == -1" << endl;
					exit(111);
				}

				simplified new_val = switches[index_switch].s1;
				prev_values.push_back(switches[index_switch]);
				if (new_val.id == parent_value.id) {
					new_val = switches[index_switch].s2;
				}

				if (check_switch(simplified_child1, new_val)) {
					simplified_child1[i] = new_val;
					break;
				}

				parent_value = new_val;
			} while (true);
		}

		if (simplified_child2[i].id == -1) {
			simplified parent_value = smplified_parent1[i];
			vector<Switch> prev_values;
			do {
				int index_switch = poz_in_switch(switches, parent_value, prev_values);
				if (index_switch == -1) {
					cout << "index_switch == -1" << endl;
					exit(111);
				}

				simplified new_val = switches[index_switch].s1;
				prev_values.push_back(switches[index_switch]);
				if (new_val.id == parent_value.id) {
					new_val = switches[index_switch].s2;
				}

				if (check_switch(simplified_child2, new_val)) {
					simplified_child2[i] = new_val;
					break;
				}

				parent_value = new_val;
			} while (true);
		}
	}

	Route route1, route2;
	for (int i = 0; i < cut_point1; i++) {
		route1.push_back(parent2.routes[simplified_child1[i].i][simplified_child1[i].j]);
		route2.push_back(parent1.routes[simplified_child2[i].i][simplified_child2[i].j]);
	}

	for (int i = cut_point1; i < cut_point2; i++) {
		route1.push_back(parent1.routes[simplified_child1[i].i][simplified_child1[i].j]);
		route2.push_back(parent2.routes[simplified_child2[i].i][simplified_child2[i].j]);
	}

	for (int i = cut_point2; i < size; i++) {
		route1.push_back(parent2.routes[simplified_child1[i].i][simplified_child1[i].j]);
		route2.push_back(parent1.routes[simplified_child2[i].i][simplified_child2[i].j]);
	}

	//acum, trebuie atribuite vehiculelor
	int route1_index = 0, veh_counter = 0;
	while (route1_index < route1.size() && veh_counter < parent1.vehicles_indexes.size()) {
		int vehicle_index = parent1.vehicles_indexes[veh_counter];

		Route route;
		route.push_back(vehicles[vehicle_index].get_depot());
		float capacity = 0.0, max_capacity = vehicles[vehicle_index].get_capacity();
		while (route1_index < route1.size() && capacity + route1[route1_index].order_weight <= max_capacity) {
			capacity += route1[route1_index].order_weight;
			route.push_back(route1[route1_index]);

			route1_index++;
		}

		route.push_back(vehicles[vehicle_index].get_depot());
		child1.routes.push_back(route);
		child1.vehicles_indexes.push_back(vehicle_index);
		veh_counter++;
	}

	if (route1_index < route1.size()) {
		//mai am nevoie de vehicule
		while (route1_index < route1.size()) {
			int index_lucky_vehicle = get_random_int(vehicles.size() - 1);
			Route route;
			route.push_back(vehicles[index_lucky_vehicle].get_depot());
			float capacity = 0.0, max_capacity = vehicles[index_lucky_vehicle].get_capacity();
			while (route1_index < route1.size() && capacity + route1[route1_index].order_weight <= max_capacity) {
				capacity += route1[route1_index].order_weight;
				route.push_back(route1[route1_index]);
				route1_index++;
			}

			route.push_back(vehicles[index_lucky_vehicle].get_depot());
			child1.routes.push_back(route);
			child1.vehicles_indexes.push_back(index_lucky_vehicle);
		}
	}

	int route2_index = 0;
	veh_counter = 0;
	while (route2_index < route2.size() && veh_counter < parent2.vehicles_indexes.size()) {
		int vehicle_index = parent2.vehicles_indexes[veh_counter];

		Route route;
		route.push_back(vehicles[vehicle_index].get_depot());
		float capacity = 0.0, max_capacity = vehicles[vehicle_index].get_capacity();
		while (route2_index < route2.size() && capacity + route2[route2_index].order_weight <= max_capacity) {
			capacity += route2[route2_index].order_weight;
			route.push_back(route2[route2_index]);

			route2_index++;
		}

		route.push_back(vehicles[vehicle_index].get_depot());
		child2.routes.push_back(route);
		child2.vehicles_indexes.push_back(vehicle_index);
		veh_counter++;
	}

	if (route2_index < route2.size()) {
		//mai am nevoie de vehicule
		while (route2_index < route2.size()) {
			int index_lucky_vehicle = get_random_int(vehicles.size() - 1);
			Route route;
			route.push_back(vehicles[index_lucky_vehicle].get_depot());
			float capacity = 0.0, max_capacity = vehicles[index_lucky_vehicle].get_capacity();
			while (route2_index < route2.size() && capacity + route[route2_index].order_weight <= max_capacity) {
				capacity += route2[route2_index].order_weight;
				route.push_back(route2[route2_index]);
				route2_index++;
			}

			route.push_back(vehicles[index_lucky_vehicle].get_depot());
			child2.routes.push_back(route);
			child2.vehicles_indexes.push_back(index_lucky_vehicle);
		}
	}

	//cout << "moooooooooooooooooor" << endl;
	//for (int i = 0; i < route1.size(); i++) {
	//	cout << route1[i].id << " ";
	//}
	//cout << endl;
	//for (int i = 0; i < route2.size(); i++) {
	//	cout << route2[i].id << " ";
	//}
}

void onecut_crossover(Vehicles vehicles, Solution parent1, Solution parent2, Solution &child1, Solution &child2) {
    vector<simplified> smplified_parent1, smplified_parent2, simplified_child1, simplified_child2;
    
    // Simplify parent1 routes
    for (int i = 0; i < parent1.routes.size(); i++) {
        Route route = parent1.routes[i];
        for (int j = 0; j < route.size(); j++) {
            if (route[j].type == Node_type::CUSTOMER) {
                smplified_parent1.push_back({ route[j].id, i, j });
                simplified_child1.push_back({ -1, i, j });
            }
        }
    }
    
    // Simplify parent2 routes
    for (int i = 0; i < parent2.routes.size(); i++) {
        Route route = parent2.routes[i];
        for (int j = 0; j < route.size(); j++) {
            if (route[j].type == Node_type::CUSTOMER) {
                smplified_parent2.push_back({ route[j].id, i, j });
                simplified_child2.push_back({ -1, i, j });
            }
        }
    }
    
    // Generate a single cut point
    int cut_point = get_random_int(smplified_parent1.size() - 1, 1);
    
    // Swap segments after the cut point between parents to generate children
    for (int i = 0; i < cut_point; i++) {
        simplified_child1[i] = smplified_parent1[i];
        simplified_child2[i] = smplified_parent2[i];
    }
    for (int i = cut_point; i < smplified_parent1.size(); i++) {
        simplified_child1[i] = smplified_parent2[i];
        simplified_child2[i] = smplified_parent1[i];
    }
    
    // Ensure no duplicates exist in children routes
    for (int i = 0; i < smplified_parent1.size(); i++) {
        if (simplified_child1[i].id == -1) {
            simplified parent_value = smplified_parent2[i];
            if (exists_in_vector(simplified_child1, parent_value.id) == false) {
                simplified_child1[i] = parent_value;
            }
        }
        if (simplified_child2[i].id == -1) {
            simplified parent_value = smplified_parent1[i];
            if (exists_in_vector(simplified_child2, parent_value.id) == false) {
                simplified_child2[i] = parent_value;
            }
        }
    }
    
    // Create routes for child1
    Route route1, route2;
    for (int i = 0; i < cut_point; i++) {
        route1.push_back(parent2.routes[simplified_child1[i].i][simplified_child1[i].j]);
        route2.push_back(parent1.routes[simplified_child2[i].i][simplified_child2[i].j]);
    }
    for (int i = cut_point; i < smplified_parent1.size(); i++) {
        route1.push_back(parent1.routes[simplified_child1[i].i][simplified_child1[i].j]);
        route2.push_back(parent2.routes[simplified_child2[i].i][simplified_child2[i].j]);
    }
    
    // Assign vehicles to child1
    int route1_index = 0, veh_counter = 0;
    while (route1_index < route1.size() && veh_counter < parent1.vehicles_indexes.size()) {
        int vehicle_index = parent1.vehicles_indexes[veh_counter];
        Route route;
        route.push_back(vehicles[vehicle_index].get_depot());
        float capacity = 0.0, max_capacity = vehicles[vehicle_index].get_capacity();
        while (route1_index < route1.size() && capacity + route1[route1_index].order_weight <= max_capacity) {
            capacity += route1[route1_index].order_weight;
            route.push_back(route1[route1_index]);
            route1_index++;
        }
        route.push_back(vehicles[vehicle_index].get_depot());
        child1.routes.push_back(route);
        child1.vehicles_indexes.push_back(vehicle_index);
        veh_counter++;
    }
    
    // Handle remaining nodes for child1
    while (route1_index < route1.size()) {
        int index_lucky_vehicle = get_random_int(vehicles.size() - 1);
        Route route;
        route.push_back(vehicles[index_lucky_vehicle].get_depot());
        float capacity = 0.0, max_capacity = vehicles[index_lucky_vehicle].get_capacity();
        while (route1_index < route1.size() && capacity + route1[route1_index].order_weight <= max_capacity) {
            capacity += route1[route1_index].order_weight;
            route.push_back(route1[route1_index]);
            route1_index++;
        }
        route.push_back(vehicles[index_lucky_vehicle].get_depot());
        child1.routes.push_back(route);
        child1.vehicles_indexes.push_back(index_lucky_vehicle);
    }
    
    // Create routes for child2
    int route2_index = 0;
    veh_counter = 0;
    while (route2_index < route2.size() && veh_counter < parent2.vehicles_indexes.size()) {
        int vehicle_index = parent2.vehicles_indexes[veh_counter];
        Route route;
        route.push_back(vehicles[vehicle_index].get_depot());
        float capacity = 0.0, max_capacity = vehicles[vehicle_index].get_capacity();
        while (route2_index < route2.size() && capacity + route2[route2_index].order_weight <= max_capacity) {
            capacity += route2[route2_index].order_weight;
            route.push_back(route2[route2_index]);
            route2_index++;
        }
        route.push_back(vehicles[vehicle_index].get_depot());
        child2.routes.push_back(route);
        child2.vehicles_indexes.push_back(vehicle_index);
        veh_counter++;
    }
    
    // Handle remaining nodes for child2
    while (route2_index < route2.size()) {
        int index_lucky_vehicle = get_random_int(vehicles.size() - 1);
        Route route;
        route.push_back(vehicles[index_lucky_vehicle].get_depot());
        float capacity = 0.0, max_capacity = vehicles[index_lucky_vehicle].get_capacity();
        while (route2_index < route2.size() && capacity + route2[route2_index].order_weight <= max_capacity) {
            capacity += route2[route2_index].order_weight;
            route.push_back(route2[route2_index]);
            route2_index++;
        }
        route.push_back(vehicles[index_lucky_vehicle].get_depot());
        child2.routes.push_back(route);
        child2.vehicles_indexes.push_back(index_lucky_vehicle);
    }
}

void cyclecrossover(Vehicles vehicles, Solution parent1, Solution parent2, Solution &child1, Solution &child2) {}

void crossover(Vehicles vehicles, Population& population, int crossover_type) {
	Population new_population;
	new_population.solutions.reserve(population.solutions.size());
	vector<float> probabilities = calculateNormalizedProbabilities(population);

	while (new_population.solutions.size() < population.solutions.size()) {
		parents_for_crossover_indices parents_indices = get_parents(vehicles, population, probabilities);

		float prob = get_random_01();
		if (prob >= crossover_prob) {
			//cout << "Crossocer" << endl;
			new_population.solutions.push_back(population.solutions[parents_indices.parent1]);
			new_population.solutions.push_back(population.solutions[parents_indices.parent2]);
			continue;
		}

		Solution child1, child2;
		if (crossover_type == 0) {
			onecut_crossover(vehicles, population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2], child1, child2);
		}

		if (crossover_type == 1) {
			twocut_crossover(vehicles, population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2], child1, child2);
		}

		if (crossover_type == 2) {
			cyclecrossover(vehicles, population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2], child1, child2);
		}

		new_population.solutions.push_back(child1);
		new_population.solutions.push_back(child2);
	}

	population = new_population;
	update_cost(population);
}







































































//
//void crossover(Vehicles vehicles, Population &population) {
//	Population new_population;
//	new_population.solutions.reserve(population_size);
//
//	vector<crossover_probs> population_probs;
//	population_probs.reserve(population_size);
//
//	for (int i = 0; i < population_size; i++) {
//		crossover_probs prob;
//		prob.prob = population.solutions[i].cost / population.total_cost;
//		prob.index = i;
//		population_probs.push_back(prob);
//	}
//
//	while (new_population.solutions.size() < population_size) {
//		//parents_for_crossover_indices parents_indices = get_parents(population_probs);
//		parents_for_crossover_indices parents_indices = get_parents(population);
//
//		float prob = get_random_01();
//		if (prob >= crossover_prob) {
//			new_population.solutions.push_back(population.solutions[parents_indices.parent1]);
//			new_population.solutions.push_back(population.solutions[parents_indices.parent2]);
//
//			continue;
//		}
//
//		Solution child1, child2;
//		twocut_crossover(vehicles, population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2], child1, child2);
//
//		new_population.solutions.push_back(child1);
//		new_population.solutions.push_back(child2);
//	}
//
//	population = new_population;
//	update_cost(population);
//}

void mutate_solution(Vehicles vehicles, Solution& solution) {
	Solution new_solution;
	new_solution.routes.reserve(solution.routes.size());
	new_solution.vehicles_indexes.reserve(solution.vehicles_indexes.size());

	int random_route_index = get_random_int(solution.routes.size() - 1);
	// nu modific nimic din celelalte rute

	for (int i = 0; i < random_route_index; i++) {
		new_solution.routes.push_back(solution.routes[i]);
		new_solution.vehicles_indexes.push_back(solution.vehicles_indexes[i]);
	}

	// acum modific ruta

	int index1, index2; // pozitiile din ruta de mai sus care se vor inversa
	// sa nu iau depozitelee
	index1 = get_random_int(solution.routes[random_route_index].size() - 2, 1);
	do {
		index2 = get_random_int(solution.routes[random_route_index].size() - 2, 1);
	} while (index1 == index2 && solution.routes[random_route_index].size() > 3);

	auto aux = solution.routes[random_route_index][index1];
	Route mutated_route;
	for (int i = 0; i < solution.routes[random_route_index].size(); i++) {
		if (i == index1) {
			mutated_route.push_back(solution.routes[random_route_index][index2]);
		}
		else if (i == index2) {
			mutated_route.push_back(aux);
		}
		else {
			mutated_route.push_back(solution.routes[random_route_index][i]);
		}
	}

	new_solution.routes.push_back(mutated_route);
	new_solution.vehicles_indexes.push_back(solution.vehicles_indexes[random_route_index]);

	for (int i = random_route_index + 1; i < solution.routes.size(); i++) {
		new_solution.routes.push_back(solution.routes[i]);
		new_solution.vehicles_indexes.push_back(solution.vehicles_indexes[i]);
	}

	//cout << "random_route_index: " << random_route_index << endl;

	solution = new_solution;
}

void mutation(Vehicles vehicles, Population& population) {
	for (int i = 0; i < population_size; i++) {
		if (get_random_01() < mutation_rate) {
			mutate_solution(vehicles, population.solutions[i]);
		}
	}

	update_cost(population);
}



//aaaaaaaaaaaaaaaaaa


// void selection(Population& population) {
// 	update_cost(population);

// 	Population mating_pool;
// 	mating_pool.solutions.reserve(population_size);

// 	// Calculate cumulative probabilities and store them in a separate vector
// 	vector<float> cumulative_probability(population_size, 0.0f);
// 	float total_cost = population.total_cost;
// 	cumulative_probability[0] = population.solutions[0].cost / total_cost;
// 	for (int i = 1; i < population_size; i++) {
// 		cumulative_probability[i] = cumulative_probability[i - 1] + population.solutions[i].cost / total_cost;
// 	}

// 	// Randomly select starting point
// 	float start_point = get_random_01() * (1.0f / population_size);

// 	// Perform stochastic universal sampling
// 	float step_size = 1.0f / population_size;
// 	float pointer = start_point;
// 	for (int i = 0; i < population_size; i++) {
// 		while (pointer > cumulative_probability[i]) {
// 			i++;
// 		}
// 		mating_pool.solutions.push_back(population.solutions[i % population_size]);
// 		pointer += step_size;
// 	}

// 	population = mating_pool;
// 	update_cost(population);
// }

//void crossover(const Vehicles& vehicles, Population& population) {
//	update_cost(population);
//
//	Population new_population;
//	new_population.solutions.reserve(population_size);
//
//	vector<crossover_probs> population_probs;
//	population_probs.reserve(population_size);
//
//	// Calculate probabilities for parent selection
//	float total_cost = population.total_cost;
//	for (int i = 0; i < population_size; ++i) {
//		crossover_probs prob;
//		prob.prob = population.solutions[i].cost / total_cost;
//		prob.index = i;
//		population_probs.push_back(prob);
//	}
//
//	// Perform crossover
//	for (int i = 0; i < population_size / 2; ++i) {
//		// Select parents
//		parents_for_crossover_indices parents_indices = get_parents(population_probs);
//
//		// Perform crossover with probability crossover_prob
//		if (get_random_01() < crossover_prob) {
//			Solution child1, child2;
//			twocut_crossover(vehicles, population.solutions[parents_indices.parent1], population.solutions[parents_indices.parent2], child1, child2);
//
//			// Add offspring to the new population
//			new_population.solutions.push_back(child1);
//			new_population.solutions.push_back(child2);
//		}
//		else {
//			// Add parents to the new population without crossover
//			new_population.solutions.push_back(population.solutions[parents_indices.parent1]);
//			new_population.solutions.push_back(population.solutions[parents_indices.parent2]);
//		}
//	}
//
//	// Resize new_population if necessary (due to rounding)
//	new_population.solutions.resize(population_size);
//
//	// Update population with the new offspring
//	population = new_population;
//	update_cost(population);
//}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
--ORIGINAL SELECTION-- // wheel selection
void selection(Population& population) {
	update_cost(population);

	Population mating_pool;
	mating_pool.solutions.reserve(population_size);
	vector<float> probability;
	probability.reserve(population_size);

	for (int i = 0; i < population_size; i++) {
		probability.push_back(population.solutions[i].cost / population.total_cost);
	}

	for (int i = 0; i < population_size; i++) {
		float random_01 = get_random_01();
		float probability_sum = 0;

		for (int j = 0; j < population_size; j++) {
			probability_sum += probability[j];
			if (random_01 <= probability_sum + epsilon) {
				mating_pool.solutions.push_back(population.solutions[j]);
				break;
			}
		}
	}

	population = mating_pool;
	update_cost(population);
}



*/