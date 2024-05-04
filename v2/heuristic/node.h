#include "enums.h"

struct Node_location {
    int x, y;
};

struct Node {
    int id;
    Node_location location;
    Node_type type;
    float order_weight;
    Node_status status;
};