import json
import random

# Generate a pool of unique weights
weights_pool = list(range(3, 501))  # Pool contains numbers from 3 to 500

# Generate edges for complete graph
edges = []
for id_a in range(1, 15):
    for id_b in range(id_a + 1, 15):
        weight = random.choice(weights_pool)  # Randomly select a weight from the pool
        edges.append({"id_a": id_a, "id_b": id_b, "weight": weight})

# Create dictionary with edges
graph = {"edges": edges}

# Write dictionary to JSON file
with open("data.json", "w") as json_file:
    json.dump(graph, json_file, indent=4)

print("JSON file 'data.json' generated successfully.")

