#include <vector>
#include "node.h"

using namespace std;

class Graph {
private:
    vector<Node> customers;
    vector<Node> depots;
public:
    Graph(){

    }

    ~Graph() = default;

    void add_customer(Node customer){
        this->customers.push_back(customer);
    }

    void add_depot(Node depot){
        this->depots.push_back(depot);
    }

    vector<Node> get_customers(){
        return this->customers;
    }

    vector<Node> get_depots(){
        return this->depots;
    }
};

