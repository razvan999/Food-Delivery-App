#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

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
    int x, y;
};

enum node_type
{
    store,
    customer
};

struct node
{
    int id;
    // boost::uuids::uuid uuid;
    node_status status;
    node_location location;
    node_type type;
};

class graph
{
private:
    vector<node> nodes;
    vector<vector<tuple<int, int>>> edges;
    int stores_count;

    bool initiate_stores(string sores_path)
    {
        ifstream file(sores_path);
        if (!file.is_open())
        {
            cerr << "Failed to open file!" << endl;
            return false;
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        try
        {
            json data = json::parse(jsonContent);

            auto stores = data["stores"];
            stores_count = stores.size();
            for (const auto &store : stores)
            {
                int id = store["id"];
                node_location location = {store["location"]["x"], store["location"]["y"]};
                node_type type = store["type"] == "store" ? node_type::store : store["type"] == "customer" ? node_type::customer
                                                                                                           : node_type();

                node new_node = {id, node_status::open, location, type};
                nodes.push_back(new_node);
            }
        }
        catch (json::parse_error &e)
        {
            cerr << "Failed to parse JSON: " << e.what() << endl;
            return false;
        }

        return true;
    }

    bool add_customers(string customers_path)
    {
        ifstream file(customers_path);
        if (!file.is_open())
        {
            cerr << "Failed to open file!" << endl;
            return false;
        }

        string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        try
        {
            json data = json::parse(jsonContent);

            auto customers = data["customers"];
            for (const auto &customer : customers)
            {
                int id = customer["id"];
                node_location location = {customer["location"]["x"], customer["location"]["y"]};
                node_type type = node_type::customer;

                node new_node = {id, node_status::open, location, type};
                nodes.push_back(new_node);
            }
        }
        catch (json::parse_error &e)
        {
            cerr << "Failed to parse JSON: " << e.what() << endl;
            return false;
        }

        return true;
    }
    /*
        bool add_edges(string json_data)
        {
            ifstream file(json_data);
            if (!file.is_open())
            {
                cerr << "Failed to open file!" << endl;
                return false;
            }

            string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            try
            {
                json data = json::parse(jsonContent);

                auto edges = data["edges"];
                for (const auto &edge : edges)
                {
                    int id_a = edge["id_a"];
                    int id_b = edge["id_b"];
                    int weight = edge["weight"];

                    add_edge(id_a, id_b, weight);
                }
            }
            catch (json::parse_error &e)
            {
                cerr << "Failed to parse JSON: " << e.what() << endl;
                return false;
            }

            return true;
        }
    */

   
    void generate_edges()
    {
        edges.resize(nodes.size());
        for (int i = 0; i < nodes.size(); i++)
        {
            for (int j = i + 1; j < nodes.size(); j++)
            {
                int weight = sqrt(pow(nodes[i].location.x - nodes[j].location.x, 2) + pow(nodes[i].location.y - nodes[j].location.y, 2));
                add_edge(nodes[i].id, nodes[j].id, weight);
            }
        }
    }

public:
    graph(string stores_path, string customers_path)
    {
        initiate_stores(stores_path);
        add_customers(customers_path);
        generate_edges();
    }

    void print_nodes()
    {
        for (auto node : nodes)
        {
            cout << "id: " << node.id << " status: " << node.status << " location: " << node.location.x << ", " << node.location.x << " type: " << node.type << endl;
        }
    }

    void print_edges()
    {
        cout << "printing edges" << endl;

        for (int i = 0; i < nodes.size(); i++)
        {
            cout << "node " << nodes[i].id << " is connected to: ";
            for (int j = 0; j < edges[nodes[i].id].size(); j++)
            {
                // cout << get<0>(edges[nodes[i].id][j]) << " with weight " << get<1>(edges[nodes[i].id][j]) << ", ";
                cout << get<0>(edges[nodes[i].id][j]) << ", ";
            }

            cout << endl;
        }
    }

    void add_node(node new_node)
    {
        nodes.push_back(new_node);
    }

    void add_edge(int id_a, int id_b, int weight)
    {
        edges[id_a].push_back(make_tuple(id_b, weight));
        edges[id_b].push_back(make_tuple(id_a, weight));
    }

    int get_nodes_count()
    {
        return nodes.size();
    }

    int get_stores_count()
    {
        return stores_count;
    }

    node_location get_node_location(int id)
    {
        return nodes[id].location;
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