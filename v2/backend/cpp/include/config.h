#pragma once
#include <vector>
#include <cstdlib>

#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>

#include "./graph.h"
#include "./vehicle.h"
#include "./read_file.h"
#include "./configuration.h"

bool maps_flag = false;
vector<vector<double>> cost_matrix;

#define crossover_prob 0.84
#define mutation_rate 0.05
#define epsilon 0.0001
#define tournament_size 15
#define variety_prob 0.12

int population_size = 0, generation_limit = 0;

int dynamicPopulationSize(int locations_size, int vehicles_size)
{
	int result = 20 * ceil(locations_size / 2.0) * ceil(vehicles_size / 2.0);
    return result % 2 == 0 ? result : result + 1;
}

void updateConfiguration(Configuration configuration, int &population_size, int &generation_limit, int locations_size, int vehicles_size)
{
	if (configuration.get_population_type() == 0)
	{
		population_size = dynamicPopulationSize(locations_size, vehicles_size);
	}
	else if (configuration.get_population_type() == 1)
	{
		population_size = 100;
	}
	else if (configuration.get_population_type() == 2)
	{
		population_size = 10000;
	}

	if (configuration.get_generation_type() == 20)
	{
		generation_limit = 20;
	}
	else if (configuration.get_generation_type() == 60)
	{
		generation_limit = 60;
	}
	else if (configuration.get_generation_type() == 100)
	{
		generation_limit = 100;
	}
}

typedef vector<Vehicle> Vehicles;
typedef vector<Node> Route;
typedef vector<Route> Routes;

struct Solution
{
	float cost;
	Routes routes;
	vector<int> vehicles_indexes;
};

struct Population
{
	vector<Solution> solutions;
	float total_cost;
};

struct crossover_probs
{
	float prob;
	int index;
};

struct parents_for_crossover_indices
{
	int parent1;
	int parent2;
};

struct simplified
{
	int id, i, j;
};

struct Switch
{
	simplified s1, s2;
};