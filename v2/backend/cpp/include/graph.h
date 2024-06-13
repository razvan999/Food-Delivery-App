#pragma once
#include <vector>
#include "node.h"
#include <iostream>

using namespace std;

class Graph {
private:
    vector<Node> customers;
    vector<Node> depots;
public:
    Graph() {

    }

    ~Graph() = default;

    void add_customer(Node customer) {
        this->customers.push_back(customer);
    }

    void add_depot(Node depot) {
        this->depots.push_back(depot);
    }

    vector<Node> get_customers() {
        return this->customers;
    }

    vector<Node> get_depots() {
        return this->depots;
    }

    int get_locations_size() {
        return this->customers.size() + this->depots.size();
    }

    void print() {
        cout << "\nPrinting Graph: " << endl;
        cout << "Customers: " << endl;
        for (Node customer : this->customers) {
            cout << "ID: " << customer.id << " X: " << customer.location.x << " Y: " << customer.location.y << " Type: " << customer.type << " Order Weight: " << customer.order_weight << " Status: " << customer.status << endl;
        }

        cout << "Depots: " << endl;
        for (Node depot : this->depots) {
            cout << "ID: " << depot.id << " X: " << depot.location.x << " Y: " << depot.location.y << " Type: " << depot.type << " Order Weight: " << depot.order_weight << " Status: " << depot.status << endl;
        }
    }
};

