#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "./include/genetic_algo.h"
#include "./include/printing.h"

using namespace std;

Solution get_second_best_sol_from_current_pop(Population population, Solution best_solution) {
	Solution second;
	second.cost = 999999.0;
	for (int i = 0; i < population_size; i++) {
		if (population.solutions[i].cost != best_solution.cost) {
			if (population.solutions[i].cost < second.cost) {
				second = population.solutions[i];
			}
		}
	}

	return second;
}

struct freq {
	float cost;
	Solution sol;
	int freq;
};

bool in_freq(Solution sol, vector<freq> frecventa) {
	for (int i = 0; i < frecventa.size(); i++) {
		if (sol.cost == frecventa[i].cost) {
			if (check_equal_solutions(sol, frecventa[i].sol))
				return true;
		}
	}

	return false;
}

void update_frecventa(Solution sol, vector<freq> &frecventa) {
	for (int i = 0; i < frecventa.size(); i++) {
		if (sol.cost == frecventa[i].cost) {
			if (check_equal_solutions(sol, frecventa[i].sol)) {
				frecventa[i].freq++;
			}
		}
	}
}

void disperare(Vehicles vehicles, Population &population) {
	update_cost(population);

	//vector<freq> frecventa;
	//frecventa.reserve(population_size / 2);

	//for (int i = 0; i < population_size; i++) {
	//	if (in_freq(population.solutions[i], frecventa)) {
	//		update_frecventa(population.solutions[i], frecventa);
	//	}
	//	else {
	//		frecventa.push_back({ population.solutions[i].cost, population.solutions[i], 1 });
	//	}
	//}

	//// print
	//cout << "Frecventa: " << endl;
	//cout << "Size: " << frecventa.size() << endl;
	//for (int i = 0; i < frecventa.size(); i++) {
	//	cout << "Cost: " << frecventa[i].cost << " Freq: " << frecventa[i].freq << endl;
	//}

	Solution best_solution = get_best_sol_from_current_pop(population);
	cout << "best inainte de disperare -> " << best_solution.cost << endl;

	Solution second_best_solution = get_second_best_sol_from_current_pop(population, best_solution);
	for (int i = 0; i < population_size; i++) {
		if (population.solutions[i].cost != best_solution.cost && population.solutions[i].cost != second_best_solution.cost) {
			mutate_solution(vehicles, population.solutions[i]);
		}
		else {
			if (check_equal_solutions(population.solutions[i], best_solution)) {
				continue;
			}

			if (check_equal_solutions(population.solutions[i], second_best_solution)) {
				continue;
			}
		}
	}

	update_cost(population);
}




void test_mutation(Vehicles vehicles);
void test_crossover(Graph graph, Vehicles vehicles);

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

	Population population;
	population.solutions.reserve(population_size);
	population = generate_population(graph, vehicles);
	Population best_solutions;
	best_solutions.solutions.reserve(population_size);

	update_cost(population);

	Solution best_initial_solution = get_best_sol_from_current_pop(population);
	cout << "Initial best solution: " << best_initial_solution.cost << endl;

	cout << endl << endl;

	//print_population(population);
	float initial_cost = population.total_cost;
	cout << "------Start Genetic Algorithm------" << endl;
	int generation = 0;
	while (generation < generation_limit) {
	    selection(population);
		//return 0;
	    crossover(vehicles, population);
	    mutation(vehicles, population);

		// if (generation % 10 == 0) 
		// 	disperare(vehicles, population);

		//cout << population.total_cost << endl;
		cout << generation << endl;
	    generation++;
	}

	cout << "------End Genetic Algorithm------" << endl;
	//print_population(population);
	cout << "initial cost: " << initial_cost << endl;
	cout << "final cost: " << population.total_cost << endl;

	cout << endl << endl;
	Solution best_solution = get_best_sol_from_current_pop(population);
	cout << "Best solution: " << best_solution.cost << endl;

	print_routes(best_solution);

	cout << "\n------End------" << endl;
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "a durat: " << elapsed_seconds.count() << "s" << endl;

	return 0;
}

void test_mutation(Vehicles vehicles) {
	Solution solution;
	solution.routes = { {{9, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{11, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{8, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{15, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{5, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}},
{{14, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{13, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{2, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{12, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{16, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}}, {{6, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{7, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{3, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{10, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{4, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}} };
	solution.vehicles_indexes = { 0, 1, 2 };
	solution.cost = calculare_cost_solutie(solution);
	
	cout << "SOLUTIE" << endl;
	cout << "Cost: " << solution.cost << endl;
	print_routes(solution);

	//mutation(vehicles, solution); // asta face mutatie pe solutie
	solution.cost = calculare_cost_solutie(solution);

	cout << "DUPA MUTATION" << endl;
	cout << "Cost: " << solution.cost << endl;
	print_routes(solution);
}

void test_crossover(Graph graph, Vehicles vehicles) {
	Solution parent1, parent2, child1, child2;

	parent1.routes = { {{9, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{11, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{8, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{15, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{5, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}},
{{14, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{13, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{2, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{12, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{16, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}}, {{6, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{7, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{3, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{10, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{4, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}} };
	parent1.vehicles_indexes = { 0, 1, 2 };

	parent2.routes = { {{2, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{5, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{11, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{4, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{3, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}}, {{16, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{13, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{8, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{15, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{10, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}}, {{6, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{14, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{7, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{9, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED},
{12, {0.0, 0.0}, Node_type::CUSTOMER, 1.0, Node_status::UNVISITED}} };
	parent2.vehicles_indexes = { 0, 1, 2 };

	//parent1 = generate_solution(graph, vehicles);
	//parent2 = generate_solution(graph, vehicles);

	parent1.cost = calculare_cost_solutie(parent1);
	parent2.cost = calculare_cost_solutie(parent2);

	cout << "Parent 1: " << endl;
	cout << "Cost: " << parent1.cost << endl;
	print_routes(parent1);


	cout << "Parent 2: " << endl;
	cout << "Cost: " << parent2.cost << endl;
	print_routes(parent2);

	cout << endl << endl;

	twocut_crossover(vehicles, parent1, parent2, child1, child2);

	cout << "gata crossover" << endl;

	child1.cost = calculare_cost_solutie(child1);
	child2.cost = calculare_cost_solutie(child2);

	cout << "Child 1: " << endl;
	cout << "Cost: " << child1.cost << endl;
	print_routes(child1);

	cout << endl;

	cout << "Child 2: " << endl;
	cout << "Cost: " << child2.cost << endl;
	print_routes(child2);
}



/*

*/

// EOF