#include <vector>

using namespace std;

class Vehicle {
private:
    int id;
    float capacity;
    float used_capacity = 0;
    Node depot;
    Node_location location;
    vector<Node> route;

public:
    Vehicle(int id, float capacity, Node depot){
        this->id = id;
        this->capacity = capacity;
        this->depot = depot;
        this->location = depot.location;
    }

    ~Vehicle() = default;

    int get_id(){
        return this->id;
    }

    float get_capacity(){
        return this->capacity;
    }

    float get_used_capacity(){
        return this->used_capacity;
    }

    void set_used_capacity(float used_capacity){
        this->used_capacity = used_capacity;
    }

    void clear_route(){
        this->route.clear();
    }

    void set_route(vector<Node> route){
        this->route = route;
    }

    Node get_depot(){
        return this->depot;
    }

    void print(){
        cout << "Vehicle ID: " << this->id << " Capacity: " << this->capacity << " Used Capacity: " << this->used_capacity << " Depot ID: " << this->depot.id << " X: " << this->depot.location.x << " Y: " << this->depot.location.y << endl;
    }
};