#include<iostream>
#include <fstream>
#include<list>
#include <tuple>

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
    list<node> nodes;
    list<tuple<int, int>> edges;

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



            }
        } catch (json::parse_error &e) {
            cerr << "Failed to parse JSON: "<< e.what() << endl;
            return false;
        }

        return true;
    }

public:
    graph()
    {
        cout << "graph created" << endl;
        if (initiate_stores() && add_customers())
        {
            cout << "stores initiated" << endl;
        }

        // separare in zone

    }

    void print_nodes()
    {
        for (auto node : nodes)
        {
            cout << "id: " << node.id << " uuid: " << node.uuid << " status: " << node.status << " location: " << node.location.address << " type: " << node.type << endl;
        }
    }








    
};

int main(){
    graph Graph;

    Graph.print_nodes();
    cout << "\n\n\n";


    return 0;
}