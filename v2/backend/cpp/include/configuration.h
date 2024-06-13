#include <iostream>
#include <string>

using namespace std;

class Configuration
{
private:
    int selection_type, crossover_type, population_type, generation_type;

public:
    Configuration() {
        selection_type = 0;
        crossover_type = 0;
        population_type = 0;
        generation_type = 20;
    }

    Configuration(string& selection, string& crossover, string& population_size, string& generation_limit) {
        if (selection == "rank_selection") {
            selection_type = 0;
        } else if (selection == "tournament_selection") {
            selection_type = 1;
        } else if (selection == "wheel_selection") {
            selection_type = 2;
        } else {
            selection_type = 0;
        }

        if (crossover == "onepointcrossover") {
            crossover_type = 0;
        } else if (crossover == "twopointcrossover") {
            crossover_type = 1;
        } else if (crossover == "cyclecrossover") {
            crossover_type = 2;
        } else {
            crossover_type = 0;
        }

        if (population_size == "dynamic") {
            population_type = 0;
        } else if (population_size == "small") {
            population_type = 1;
        } else if (population_size == "large") {
            population_type = 2;
        } else {
            population_type = 0;
        }

        if (generation_limit == "20") {
            generation_type = 20;
        } else if (generation_limit == "60") {
            generation_type = 60;
        } else if (generation_limit == "100") {
            generation_type = 100;
        } else {
            generation_type = 20;
        }
    }

    int get_selection_type()  {
        return selection_type;
    }

    int get_crossover_type()  {
        return crossover_type;
    }

    int get_population_type()  {
        return population_type;
    }

    int get_generation_type()  {
        return generation_type;
    }

    void set_selection_type(int selection) {
        selection_type = selection;
    }

    void set_crossover_type(int crossover) {
        crossover_type = crossover;
    }

    void set_population_type(int population) {
        population_type = population;
    }

    void set_generation_type(int generation) {
        generation_type = generation;
    }

    void print() const {
        cout << "Selection Type: " << selection_type << endl;
        cout << "Crossover Type: " << crossover_type << endl;
        cout << "Population Type: " << population_type << endl;
        cout << "Generation Type: " << generation_type << endl;
    }
};
