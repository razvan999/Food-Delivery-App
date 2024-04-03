import json
import random

weights_pool = list(range(3, 501))

edges = []
for id_a in range(0, 14):
    for id_b in range(id_a + 1, 14):
        weight = random.choice(weights_pool)
        edges.append({"id_a": id_a, "id_b": id_b, "weight": weight})

graph = {"edges": edges}

with open("data.json", "w") as json_file:
    json.dump(graph, json_file, indent=4)

print("JSON file 'data.json' generated successfully.")

