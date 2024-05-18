#pragma once
#include <vector>
#include <cstdlib>

#include "./graph.h"
#include "./vehicle.h"
#include "./read_file.h"

#define population_size 100 // mereu par
#define generation_limit 500
#define crossover_prob 0.8
#define mutation_rate 0.1
#define epsilon 0.0001

/*

sa vad ce pot sa fac cand atindge un minim local
poate fac altfel mutatia

sa nu aleg aceeasi parinti in crossover



*/

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