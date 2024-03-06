#include <iostream>
#include <fstream>
#include <list>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#define MAX_WEIGHT 1000000

using namespace std;
using json = nlohmann::json;

string sores_path = "../data/stores.json";
string customers_path = "../data/customers.json";
string json_data = "../data/data.json";

boost::uuids::uuid generate_uuid()
{
    return boost::uuids::random_generator()();
}

enum node_status
{
    open,
    closed,
    placed,
    redy_for_delivery,
    delivery_in_progress,
    delivered
};

struct node_location
{
    string address;
};

enum node_type
{
    store,
    customer
};

struct node
{
    int id;
    boost::uuids::uuid uuid;
    node_status status;
    node_location location;
    node_type type;
};

class graph
{
private:
    vector<node> nodes;
    vector<vector<tuple<int, int>>> edges;

    bool initiate_stores(){
        ifstream file(sores_path);
        if (!file.is_open()) {
            cerr << "Failed to open file!" << endl;
            return false;
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

            try {
            json data = json::parse(jsonContent);

            auto stores = data["stores"];
            for (const auto& store : stores) {
                int id = store["id"];
                node_location location = {store["location"]};
                node_type type = store["type"] == "store" ? node_type::store : store["type"] == "customer" ? node_type::customer : node_type();

                node new_node = {id, generate_uuid(), node_status::open, location, type};
                nodes.push_back(new_node);
            }
        } catch (json::parse_error &e) {
            cerr << "Failed to parse JSON: " << e.what() << endl;
            return false;
        }

        return true;
    }

    bool add_customers(){ //pentru testare
        ifstream file(customers_path);
        if (!file.is_open()) {
            cerr << "Failed to open file!" << endl;
            return false;
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        try {
            json data = json::parse(jsonContent);

            auto customers = data["customers"];
            for (const auto& customer : customers) {
                int id = customer["id"];
                node_location location = {customer["location"]};
                node_type type = node_type::customer;

                node new_node = {id, generate_uuid(), node_status::open, location, type};
                nodes.push_back(new_node);
            }
        } catch (json::parse_error &e) {
            cerr << "Failed to parse JSON: "<< e.what() << endl;
            return false;
        }

        return true;
    }

    bool add_edges(){
        ifstream file(json_data);
        if (!file.is_open()) {
            cerr << "Failed to open file!" << endl;
            return false;
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        try {
            json data = json::parse(jsonContent);

            auto edges = data["edges"];
            for (const auto& edge : edges) {
                int id_a = edge["id_a"];
                int id_b = edge["id_b"];
                int weight = edge["weight"];

                add_edge(id_a, id_b, weight);
            }
        } catch (json::parse_error &e) {
            cerr << "Failed to parse JSON: "<< e.what() << endl;
            return false;
        }

        return true;
    }

public:
    graph() {
        initiate_stores();
        add_customers();

        edges.resize(nodes.size());
    }

    graph(bool flag)
    {
        initiate_stores();
        add_customers();

        edges.resize(nodes.size());
        add_edges();
        // separare in zone

    }

    void print_nodes()
    {
        for (auto node : nodes)
        {
            cout << "id: " << node.id << " uuid: " << node.uuid << " status: " << node.status << " location: " << node.location.address << " type: " << node.type << endl;
        }
    }

    void print_edges() {
        cout << "printing edges" << endl;

        for (int i = 0; i < edges.size(); i++) {
            cout << "node " << i << " is connected to: ";
            for (auto edge : edges[i]) {
                cout << get<0>(edge) << " with weight " << get<1>(edge) << ", ";
            }
            cout << endl;
        }
    }    

    void add_node(node new_node)
    {
        nodes.push_back(new_node);
    }

    void add_edge(int id_a, int id_b, int weight) {
        edges[id_a].push_back(make_tuple(id_b, weight));
        edges[id_b].push_back(make_tuple(id_a, weight));
    }

    void add(int id_a, int id_b, int weight) {
        edges[id_a].push_back(make_tuple(id_b, weight));
    }

    vector<node> get_nodes()
    {
        return nodes;
    }

    vector<vector<tuple<int, int>>> get_edges()
    {
        return edges;
    }
};

graph prim_algorithm(graph Graph){
// void prim_algorithm(graph Graph){
    graph mst;
    vector<node> unvisited = Graph.get_nodes();
    

    while(unvisited.size() > 1) {
        node node = unvisited[0];
        unvisited.erase(unvisited.begin());
        mst.add_node(node);

        int min_weight = MAX_WEIGHT, id_node = -1;
        for (int i = 0; i < Graph.get_edges()[node.id].size(); i++){
            int id = get<0>(Graph.get_edges()[node.id][i]);
            int weight = get<1>(Graph.get_edges()[node.id][i]);

            if (weight < min_weight){
                min_weight = weight;
                id_node = id;
            }
        }
        mst.add_edge(node.id, id_node, min_weight);
        // mst.add(node.id, id_node, min_weight);
        // unvisited.erase(unvisited.begin() + id_node);
    }

    return mst;
}

int main(){
    graph Graph(true);

    // Graph.print_nodes();
    // cout << "\n\n\n";

    // Graph.print_edges();

    graph mst = prim_algorithm(Graph);
    // prim_algorithm(Graph);
    mst.print_edges();
    
    return 0;
}