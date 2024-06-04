#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "./include/genetic_algo.h"
#include "./include/printing.h"
#include "./include/aux.h"

using namespace std;

int main(int argc, char *argv[])
{ // ulysses-n16-k3
  char *fileName = get_filename(argv[1]);

  Graph graph;
  Vehicles vehicles;
  initialize(2, argv[1], graph, vehicles);

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

  float initial_cost = population.total_cost;
  cout << "------Start Genetic Algorithm------" << endl;
  int generation = 0;
  while (generation < generation_limit)
  {
    selection(population);
    crossover(vehicles, population);
    mutation(vehicles, population);

    cout << generation << endl;
    generation++;
  }

  cout << "------End Genetic Algorithm------" << endl;
  cout << "initial cost: " << initial_cost << endl;
  cout << "final cost: " << population.total_cost << endl;

  cout << endl << endl;
  Solution best_solution = get_best_sol_from_current_pop(population);
  // cout << "Best solution: " << best_solution.cost << endl;

  print_routes(best_solution);

  cout << "\n------End------" << endl;
  end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end - start;
  cout << "a durat: " << elapsed_seconds.count() << "s" << endl;


  write_json_ouput(fileName, read_input_json(fileName), best_solution, elapsed_seconds);

  return 0;
}