#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "Hello World" << std::endl;

    std::ofstream outfile ("./json/output_for_cpp/ulysses-n16-k3.json");
    if (!outfile) {
        std::cerr << "Error: Unable to create file" << std::endl;
        return 1;
    }

std::string jsonContent = R"(
{
    "type": "output",
    "data": [
      { "node_index": 1, "x": 38.24, "y": 20.42 },
      { "node_index": 2, "x": 39.57, "y": 26.15 },
      { "node_index": 3, "x": 40.56, "y": 25.32 },
      { "node_index": 4, "x": 36.26, "y": 23.12 },
      { "node_index": 5, "x": 33.48, "y": 10.54 },
      { "node_index": 6, "x": 37.56, "y": 12.19 },
      { "node_index": 7, "x": 38.42, "y": 13.11 },
      { "node_index": 8, "x": 37.52, "y": 20.44 },
      { "node_index": 9, "x": 41.23, "y": 9.1 },
      { "node_index": 10, "x": 41.17, "y": 3.05 },
      { "node_index": 11, "x": 36.08, "y": -5.21 },
      { "node_index": 12, "x": 38.47, "y": 15.13 },
      { "node_index": 13, "x": 38.15, "y": 15.35 },
      { "node_index": 14, "x": 37.51, "y": 15.17 },
      { "node_index": 15, "x": 35.49, "y": 1.32 },
      { "node_index": 16, "x": 39.36, "y": 1.56 }
    ]
  }
)";
    outfile << jsonContent<< std::endl;
    outfile.close();

    return 0;
}
