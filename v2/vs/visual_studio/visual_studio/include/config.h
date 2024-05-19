#pragma once
#include <vector>
#include <cstdlib>

#include "./graph.h"
#include "./vehicle.h"
#include "./read_file.h"

#define population_size 400 // mereu par
#define generation_limit 8
#define crossover_prob 0.84
#define mutation_rate 0.05
#define epsilon 0.0001
#define tournament_size 15

typedef vector<Vehicle> Vehicles;
typedef vector<Node> Route;
typedef vector<Route> Routes;

struct Solution {
	float cost;
	Routes routes;
	vector<int> vehicles_indexes;
};

struct Population {
	vector<Solution> solutions;
	float total_cost;
};

struct crossover_probs {
	float prob;
	int index;
};

struct parents_for_crossover_indices {
	int parent1;
	int parent2;
};

struct simplified {
	int id, i, j;
};

struct Switch {
	simplified s1, s2;
};