#pragma once
#include <vector>

#include "./graph.h"
#include "./vehicle.h"
#include "./read_file.h"

#define population_size 10
#define generation_limit 5

typedef vector<Vehicle> Vehicles;
typedef vector<Node> Route;
typedef vector<Route> Routes;

struct Solution {
	float cost;
	Routes routes;
	vector<int> vehicles_indexes;
};

typedef vector<Solution> Population;