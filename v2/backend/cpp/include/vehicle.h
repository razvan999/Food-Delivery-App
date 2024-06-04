#pragma once
#include <vector>
#include "../nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Vehicle {
private:
    int id, depot_id;
    float capacity;
    float used_capacity = 0;
    Node depot;
    Node_location location;
    vector<Node> route;

public:
    Vehicle(int id, float capacity, Node depot) {
        this->id = id;
        this->capacity = capacity;
        this->depot = depot;
        this->location = depot.location;
    }

    //     Vehicle(const json& data) {
    //     // Parse JSON data to initialize members
    //     id = data.at("id").get<int>();
    //     // capacity = data.at("capacity").get<float>();
    //     // depot_id = data.at("depot_id").get<int>(); // Assuming depot_id is present in JSON
    //     // Initialize depot using JSON data, assuming depot is a Node
    //     // depot = Node(data.at("depot")); // Assuming Node has a constructor that takes JSON data
    //     // location = depot.location;
    // }

    ~Vehicle() = default;

    int get_id() {
        return this->id;
    }

    float get_capacity() {
        return this->capacity;
    }

    float get_used_capacity() {
        return this->used_capacity;
    }

    void set_used_capacity(float new_capacity) {
        if (new_capacity >= 0 && new_capacity <= this->capacity) {
            this->used_capacity = new_capacity;
        }
        else {
            cout << "Invalid capacity!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        }
    }

    void clear_route() {
        this->route.clear();
    }

    void set_route(vector<Node> route) {
        this->route = route;
    }

    Node get_depot() {
        return this->depot;
    }

    void print() {
        cout << "Vehicle ID: " << this->id << " Capacity: " << this->capacity << " Used Capacity: " << this->used_capacity << " Depot ID: " << this->depot.id << " X: " << this->depot.location.x << " Y: " << this->depot.location.y << endl;
    }

    void set_depot_id(int id) {
        this->depot_id = id;
    }

    int get_depot_id() {
        return this->depot_id;
    }
};