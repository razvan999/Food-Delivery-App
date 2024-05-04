// {
//     "type": "output",
//     "data": [
//       { "node_index": 1, "x": 38.24, "y": 20.42 },
//       { "node_index": 2, "x": 39.57, "y": 26.15 },
//       { "node_index": 3, "x": 40.56, "y": 25.32 },
//       { "node_index": 4, "x": 36.26, "y": 23.12 },
//       { "node_index": 5, "x": 33.48, "y": 10.54 },
//       { "node_index": 6, "x": 37.56, "y": 12.19 },
//       { "node_index": 7, "x": 38.42, "y": 13.11 },
//       { "node_index": 8, "x": 37.52, "y": 20.44 },
//       { "node_index": 9, "x": 41.23, "y": 9.1 },
//       { "node_index": 10, "x": 41.17, "y": 3.05 },
//       { "node_index": 11, "x": 36.08, "y": -5.21 },
//       { "node_index": 12, "x": 38.47, "y": 15.13 },
//       { "node_index": 13, "x": 38.15, "y": 15.35 },
//       { "node_index": 14, "x": 37.51, "y": 15.17 },
//       { "node_index": 15, "x": 35.49, "y": 1.32 },
//       { "node_index": 16, "x": 39.36, "y": 1.56 }
//     ]
//   }

// ulysses-n16-k3

const ws = new WebSocket("ws://localhost:3000/details");

document.addEventListener("DOMContentLoaded", function () {
  document
    .getElementById("submitInstance")
    .addEventListener("click", function () {
      const selectElement = document.getElementById("instance");
      const selectedOption = selectElement.options[selectElement.selectedIndex];
      const selectedValue = selectedOption.value;

      initialization(selectedValue);
      fetchInputData(selectedValue);

      talkToServer(selectedValue).then(result => {
        if (result) {
          console.log("I can fetch!!!!!!!");
          fetchOutputData(selectedValue);
        } else {
          alert("Error fetching data");
        }
      });
    });
});

function talkToServer(selectedValue) {
  return new Promise((resolve, reject) => {
    const fileName = selectedValue.replace(/\/+$/, "").split("/");
    ws.send("run_algorithm_" + fileName[fileName.length - 1]);

    ws.onmessage = function (event) {
      // console.log("received: " + event.data);
      if (event.data === "you can fetch data") {
        resolve(true);
      }
      resolve(false);
    };

    ws.onerror = function (error) {
      reject(error);
    };
  });
}

function initialization(value) {
  clearSvg();
  deleteOutputFile(value);
}

function deleteOutputFile(value) {
  let fileName = value.replace(/\/+$/, "").split("/");
  fileName = fileName[fileName.length - 1];
  console.log(fileName);

  fetch("/details/delete-file/" + fileName, {
    method: "DELETE",
  })
    .then((response) => {
      if (!response.ok) {
        throw new Error("Network response was not ok");
      }
      return response.text();
    })
    .then((data) => {
      console.log(data);
      alert("File deleted successfully");
    })
    .catch((error) => {
      console.error("There was an error:", error);
      alert("Error deleting file: " + error.message);
    });
}

function clearSvg() {
  const svgInput = document.getElementById("container_input");
  const svgOutput = document.getElementById("container_output");

  if (svgInput.hasChildNodes()) {
    while (svgInput.firstChild) {
      svgInput.removeChild(svgInput.firstChild);
    }
  }

  if (svgOutput.hasChildNodes()) {
    while (svgOutput.firstChild) {
      svgOutput.removeChild(svgOutput.firstChild);
    }
  }
}

function fetchInputData(selectedValue) {
  fetch(selectedValue)
    .then((response) => {
      if (!response.ok) {
        alert("Network response was not ok");
        throw new Error("Network response was not ok");
      }

      return response.json();
    })
    .then((receivedData) => {
      inputData = receivedData.data;
      // console.log(inputData);
      drawGraph(inputData, true);
    })
    .catch((error) => {
      console.error(
        "There has been a problem with your fetch operation:",
        error
      );
    });
}

function fetchOutputData(selectedValue) {
  const newValue = selectedValue.replace("input", "output");
  fetch(newValue)
    .then((response) => {
      if (!response.ok) {
        alert("Network response was not ok");
        throw new Error("Network response was not ok");
      }

      return response.json();
    })
    .then((receivedData) => {
      outputData = receivedData.data;
        console.log(outputData);
      drawGraph(outputData, false);
    })
    .catch((error) => {
      console.error(
        "There has been a problem with your fetch operation:",
        error
      );
    });
}

function drawGraph(data, isInputData) {
  const width = 600;
  const height = 600;
  const spacing = 100;

  const tours = [
    { route: [10, 8, 9, 6, 5], cost: 12 },
    { route: [14, 13, 12, 11, 4], cost: 15 },
    { route: [3, 1, 2, 15, 7], cost: 4 },
  ];

  let container;
  if (isInputData) {
    container = d3.select("#container_input");
  } else {
    container = d3.select("#container_output");
  }

  const svg = container
    .append("svg")
    .attr("width", width)
    .attr("height", height)
    .style("background", "pink")
    .append("g")
    .attr("transform", "translate(" + spacing / 2 + "," + spacing / 2 + ")");

  const xScale = d3
    .scaleLinear()
    .domain([d3.min(data, (d) => d.x), d3.max(data, (d) => d.x)])
    .range([0, width - spacing]);

  const yScale = d3
    .scaleLinear()
    .domain([d3.min(data, (d) => d.y), d3.max(data, (d) => d.y)])
    .range([height - spacing, 0]);

  const xAxis = d3.axisBottom(xScale);
  const yAxis = d3.axisLeft(yScale);

  svg
    .append("g")
    .attr("transform", "translate (0," + (height - spacing) + ")")
    .call(xAxis);
  svg.append("g").call(yAxis);

  const colors = [
    "red",
    "navy",
    "purple",
    "green",
    "orange",
    "gray",
    "brown",
    "pink",
    "lime",
    "steelblue",
    "cyan",
    "magenta",
    "yellow",
    "teal",
    "violet",
    "indigo",
  ];
  if (colors.length < tours.length) {
    alert("Not enough colors for all tours");
  }

  const lineGenerator = d3
    .line()
    .x((d) => xScale(d[0]))
    .y((d) => yScale(d[1]));

  for (let i = 0; i < tours.length; i++) {
    const linePoints = [];
    for (let j = 0; j < tours[i].route.length; j++) {
      const pointIndex = tours[i].route[j] - 1;
      linePoints.push([data[pointIndex].x, data[pointIndex].y]);
    }
    linePoints.push([
      data[tours[i].route[0] - 1].x,
      data[tours[i].route[0] - 1].y,
    ]);

    svg
      .append("path")
      .datum(linePoints)
      .attr("fill", "none")
      .attr("stroke", colors[i])
      .attr("stroke-width", 2)
      .attr("d", lineGenerator);

    svg
      .selectAll(null)
      .data(linePoints)
      .enter()
      .append("circle")
      .attr("cx", (d) => xScale(d[0]))
      .attr("cy", (d) => yScale(d[1]))
      .attr("r", 5)
      .style("fill", colors[i]);
  }

  let div;
  const paragraph = document.createElement("p");
  paragraph.className = "description";

  if (isInputData) {
    div = document.getElementById("container_input");
    paragraph.textContent = "Input";
  } else {
    div = document.getElementById("container_output");
    paragraph.textContent = "Output";
  }

  div.appendChild(paragraph);
}
