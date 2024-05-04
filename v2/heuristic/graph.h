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
};

