#include <iostream>
#include <fstream>
#include <list>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#define MAX_WEIGHT 1000000
#define MAx_INT 1000000

using namespace std;
using json = nlohmann::json;

string sores_path = "../data/stores.json";
string customers_path = "../data/customers.json";
string json_data = "../data/data.json";
string graph_output = "../data/graph_output.json";

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
        ifstream file("../data/nodes.json");
        // ifstream file(customers_path);
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
        // ifstream file(json_data);
        ifstream file("../data/mst.json");
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

                // add_edge(id_a, id_b, weight);
                add(id_a, id_b, weight);
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

int min_key_index(vector<int> key, vector<bool> mst_set){
    int min_key = MAX_WEIGHT, min_index = -1;

    for (int i = 0; i < key.size(); i++) {
        if (mst_set[i] == false && key[i] < min_key) {
            min_key = key[i];
            min_index = i;
        }
    }

    return min_index;
}

graph prim_algorithm(graph Graph) {
    graph mst;

    vector<int> key = vector<int>(Graph.get_nodes().size(), MAX_WEIGHT);
    vector<bool> mst_set = vector<bool>(Graph.get_nodes().size(), false);
    vector<int> parent = vector<int>(Graph.get_nodes().size(), -1);
    key[0] = 0;

    node current_node = Graph.get_nodes()[0];
    bool flag = false;
    while (flag == false) {
        int min_key = min_key_index(key, mst_set);
        mst_set[min_key] = true;
        current_node = Graph.get_nodes()[min_key];


        for (int i = 0; i < Graph.get_edges()[current_node.id].size(); i++) {
            int id = get<0>(Graph.get_edges()[current_node.id][i]);
            int weight = get<1>(Graph.get_edges()[current_node.id][i]);

            if (mst_set[id] == false && weight < key[id]) {
                key[id] = weight;
                parent[id] = min_key;
            }
        }

        flag = true;
        for (int i = 0; i < mst_set.size(); i++) {
            if (mst_set[i] == false) {
                flag = false;
                break;
            }
        }
    }

    for (int i = 0; i < parent.size(); i++) {
        if (parent[i] != -1) {
            node node = Graph.get_nodes()[i];
            mst.add_edge(node.id, Graph.get_nodes()[parent[i]].id, key[i]);
        }
    }
    
    return mst;
}

void print_graph_to_json(graph mst, string path){
    json nodes = json::array();
    for (int i = 0; i < mst.get_nodes().size(); i++) {
        json node;
        node["id"] = mst.get_nodes()[i].id;
        node["uuid"] = boost::uuids::to_string(mst.get_nodes()[i].uuid);
        node["status"] = mst.get_nodes()[i].status;
        node["location"] = mst.get_nodes()[i].location.address;
        if (mst.get_nodes()[i].type == node_type::store) {
            node["type"] = "store";
        } else if (mst.get_nodes()[i].type == node_type::customer) {
            node["type"] = "customer";
        }

        nodes.push_back(node);
    }

    json edges = json::array();
    // for (int i = 0; i < mst.get_edges().size(); i++) {

    //     for (int j = 0; j < mst.get_edges()[i].size(); j++) {

    //     }
    // }

    json data = {{


        "graph", {
            {"nodes", nodes},
            {"edges", edges}
        }}
    };

    string jsonString = data.dump(4);
    
    ofstream outputFile(graph_output);
    if (outputFile.is_open()) {
        outputFile << jsonString;
        outputFile.close();
    } else {
        cerr << "Failed to open file!" << endl;
        return;
    }
}

int distance_between_nodes(node store, node customer) {
    return 0;
}

vector<graph> graph_splitter(graph Graph){ // nu ii gata
    vector<graph> zones;
    vector<node> stores;

    for (int i = 0; i < Graph.get_nodes().size(); i++) {
        if (Graph.get_nodes()[i].type == node_type::store) {
            stores.push_back(Graph.get_nodes()[i]);
        }
    }

    if (stores.size() == 1) {
        zones.push_back(Graph);
        return zones;
    }

    for (int i = 0; i < stores.size(); i++) {
        graph zone;
        zone.add_node(stores[i]);
        zones.push_back(zone);
    }

    for (int i = 0; i < Graph.get_nodes().size(); i++) {
        if (Graph.get_nodes()[i].type == node_type::store) {
            continue;
        }

        int min_distance = MAx_INT, closest_store_index = -1;
        for (int j = 0; j < stores.size(); j++) {
            int distance = distance_between_nodes(stores[j], Graph.get_nodes()[i]);
            if (distance < min_distance) {
                min_distance = distance;
                closest_store_index = j;
            }
        }

        zones[closest_store_index].add_node(Graph.get_nodes()[i]);
    }

    return zones;
}

int main(){
    graph Graph(true);

    // Graph.print_nodes();
    // cout << "\n\n\n";

    // Graph.print_edges();

    // graph mst = prim_algorithm(Graph);
    // mst.print_edges();

    // print_graph_to_json(mst, graph_output);


    vector<graph> zones = graph_splitter(Graph);
    
    return 0;
}