#pragma once
#include "enums.h"

struct Node_location {
    float x, y;
};

struct Node {
    int id;
    Node_location location;
    Node_type type;
    float order_weight;
    Node_status status;
};