#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "./config.h"

typedef vector<Vehicle> Vehicles;

using namespace std;

#define data_folder_path "./data/"

bool readFile(char* filePath, Graph& graph, Vehicles& vehicles) {
    ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return false;
    }

    string line;
    getline(file, line);
    int numberOfTrucks = stoi(line);

    getline(file, line);
    int numberOfNodes = stoi(line);

    getline(file, line);
    int numberOfDepos = stoi(line);

    getline(file, line);
    int capacity = stoi(line);

    for (int i = 0; i < numberOfDepos; i++) {
        string node;
        getline(file, node);
        stringstream ss(node);
        vector<string> tokens;
        string token;
        while (getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        graph.add_depot(
            Node{
                stoi(tokens[0]),
                {stof(tokens[1]), stof(tokens[2])},
                Node_type::DEPOT,
                0.0,
                Node_status::UNVISITED
            }
        );
    }

    for (int i = 0; i < numberOfNodes - numberOfDepos; i++) {
        string node;
        getline(file, node);
        stringstream ss(node);
        vector<string> tokens;
        string token;
        while (getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        graph.add_customer(
            Node{
                stoi(tokens[0]),
                {stof(tokens[1]), stof(tokens[2])},
                Node_type::CUSTOMER,
                1.0,
                Node_status::UNVISITED
            }
        );
    }

    auto depots = graph.get_depots();
    for (int i = 0; i < numberOfTrucks; i++) {
        vehicles.push_back(Vehicle(i, capacity, depots[0]));
    }

    file.close();
    return true;
}

//void initialize(int argc, char* argv[], Graph& graph, Vehicles& vehicles) {
void initialize(int argc, char* argv, Graph& graph, Vehicles& vehicles) {

    if (argc < 2) {
        cout << "You did not enter the filename... O crapat" << endl;
        return;
    }

    char fileName[100];
    //strcpy(fileName, argv[1]);
    strcpy(fileName, argv);

    char filePath[100];
    // strcpy(filePath, data_folder_path); // pentru terminal
    strcpy(filePath, "./cpp/data/");
    strcat(filePath, fileName);
    strcat(filePath, "/");
    strcat(filePath, fileName); // folder name
    strcat(filePath, ".vrp");

    if (!readFile(filePath, graph, vehicles)) {
        return;
    }
}