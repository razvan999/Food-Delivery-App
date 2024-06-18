#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

using json = nlohmann::json;

bool check_line(const string &line)
{
  size_t found = line.find("tours");
  if (found != std::string::npos)
  {
    return true;
  }

  return false;
}

string read_input_json(char *fileName)
{
  // ifstream file("../json/input_for_cpp/" + string(fileName)); // terminal
  ifstream file("./json/input_for_cpp/" + string(fileName));
  if (!file.is_open())
  {
    cerr << "Failed to open file - json input.\n";
    return "";
  }

  string line;
  string data;
  while (getline(file, line))
  {
    if (check_line(line))
    {

      return data;
    }

    data += line + "\n";
  }

  file.close();
  return data;
}

char *get_filename(const char *argv)
{
  char *fileName = new char[100];
  strcpy(fileName, argv);
  strcat(fileName, ".json");
  return fileName;
}

void write_json_ouput(char *fileName, string data_from_input_json, Solution best_solution, chrono::duration<double> elapsed_seconds)
{
  auto time = elapsed_seconds.count();

  ofstream outfile("./json/output_for_cpp/" + string(fileName)); // asta ii pt browser
  // ofstream outfile("../json/output_for_cpp/" + string(fileName)); // asta ii pt test terminal
  if (!outfile)
  {
    cerr << "Error: Unable to create file" << endl;
    return;
  }

  string jsonContent = data_from_input_json;
  jsonContent += "\"tours\": [\n";

  for (int i = 0; i < best_solution.routes.size(); i++)
  {
    jsonContent += "{\n";
    jsonContent += "\"vehicle\": " + to_string(best_solution.vehicles_indexes[i]) + ",\n";
    jsonContent += "\"route\": [";
    for (int j = 0; j < best_solution.routes[i].size(); j++)
    {
      jsonContent += to_string(best_solution.routes[i][j].id);
      if (j != best_solution.routes[i].size() - 1)
      {
        jsonContent += ", ";
      }
    }
    jsonContent += "]\n";
    jsonContent += "}";
    if (i != best_solution.routes.size() - 1)
    {
      jsonContent += ",\n";
    }
  }

  jsonContent += "],\n";
  jsonContent += "\"population_size\": " + to_string(population_size) + ",\n";
  jsonContent += "\"generation_limit\": " + to_string(generation_limit) + ",\n";
  jsonContent += "\"crossover_prob\": " + to_string(crossover_prob) + ",\n";
  jsonContent += "\"mutation_rate\": " + to_string(mutation_rate) + ",\n";
  jsonContent += "\"initial_cost\": " + to_string(best_solution.cost) + ",\n";
  jsonContent += "\"final_cost\": " + to_string(best_solution.cost) + ",\n";
  jsonContent += "\"time\": " + to_string(time) + "}\n";

  outfile << jsonContent << endl;
  outfile.close();
}

struct aux
{ // reparat prost
  int id, depot_id;
  float capacity;
};

void from_json(const json &j, Node &location)
{
  if (j.at("type") == "DEPOT")
  {
    location.type = Node_type::DEPOT;
  }
  else
  {
    location.type = Node_type::CUSTOMER;
  }
  j.at("latitude").get_to(location.location.x);
  j.at("longitude").get_to(location.location.y);
  // j.at("weight").get_to(location.order_weight);
}

void from_json(const json &j, aux &vehicle)
{
  j.at("id").get_to(vehicle.id);
  j.at("capacity").get_to(vehicle.capacity);
  j.at("depot_index").get_to(vehicle.depot_id);
}

bool readJsonFile(const char *filePath, Graph &graph, Vehicles &vehicles, Configuration &configuration)
{
  ifstream file;
  file.open(filePath);
  if (!file.is_open())
  {
    cout << "File not found" << endl;
    return false;
  }

  json jsonData;
  file >> jsonData;

  vector<Node> locations;
  vector<aux> vehicles2;
  jsonData.at("locations").get_to(locations);
  jsonData.at("vehicles").get_to(vehicles2);

  int i = 0;
  for (Node location : locations)
  {
    location.id = i++;
    location.status = Node_status::UNVISITED;
    location.order_weight = 1.0;
    if (location.type == Node_type::DEPOT)
    {
      graph.add_depot(location);
    }
    else
    {
      graph.add_customer(location);
    }
  }

  for (auto vehicle : vehicles2)
  {
    Vehicle v(vehicle.id, vehicle.capacity, graph.get_depots()[vehicle.depot_id]);
    vehicles.push_back(v);
  }

  cost_matrix = jsonData.at("matrix").get<vector<vector<double>>>();

  string selection = jsonData.at("selection").get<string>();
  string crossover = jsonData.at("crossover").get<string>();
  string population_sizeaa = jsonData.at("population_size").get<string>();
  string generation_limitaa = jsonData.at("generation_limit").get<string>();

  configuration = Configuration(selection, crossover, population_sizeaa, generation_limitaa);

  return true;
}

void initialize_maps(Graph &graph, Vehicles &vehicles, Configuration &configuration)
{
  maps_flag = true;
  // const char *filePath = "./json/maps_cpp/input_maps_cpp.json"; // browser
  const char *filePath = "../json/maps_cpp/input_maps_cpp.json"; // terminal
  if (!readJsonFile(filePath, graph, vehicles, configuration))
  {
    return;
  }
}

void write_json_output_maps(const Solution &best_solution, std::chrono::duration<double> elapsed_seconds)
{
    auto time = elapsed_seconds.count();

    // std::ofstream outfile("./json/maps_cpp/output_maps_cpp.json"); // Pentru browser
    std::ofstream outfile("../json/maps_cpp/output_maps_cpp.json"); // Pentru testare în terminal
    if (!outfile.is_open())
    {
        std::cerr << "Error: Unable to create file" << std::endl;
        return;
    }

    json output;
    output["tours"] = json::array();
    for (int i = 0; i < best_solution.routes.size(); i++)
    {
        json tour;
        tour["vehicle"] = best_solution.vehicles_indexes[i];
        tour["route"] = json::array();
        for (const auto &node : best_solution.routes[i])
        {
            tour["route"].push_back(node.id);
        }
        // tour["cost"] = best_solution.cost;
        tour["cost"] = static_cast<int>(calculare_cost_ruta(best_solution.routes[i]));
        output["tours"].push_back(tour);
    }
    output["final_cost"] = static_cast<int>(best_solution.cost);
    output["time"] = time;

    outfile << std::setw(4) << output << std::endl;
    outfile.close();
}