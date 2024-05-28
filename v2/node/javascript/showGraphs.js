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
      inputData = receivedData;
      console.log("Input data:");
      console.log(inputData);
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
  console.log(selectedValue);
  console.log(newValue);
  fetch(newValue)
    .then((response) => {
      if (!response.ok) {
        alert("Network response was not ok");
        throw new Error("Network response was not ok");
      }

      return response.json();
    })
    .then((receivedData) => {
      outputData = receivedData;
      console.log("Output data:");
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










































































































































































































function setParagraphData(data, isInput){
  let paragraphData = "";

  if (isInput) {
    paragraphData += "Input data <br>"
    
    data.tours.forEach((tour, index) => {
      paragraphData += `<br>tour: ${index + 1}<br>`;
      paragraphData += `route: ${tour.route} <br>`;
      paragraphData += `cost: ${tour.cost}<br>`;
    });
  }
  else {
    paragraphData += "Output data:<br>";

    data.tours.forEach((tour, index) => {
      paragraphData += `<br>tour: ${index + 1}<br>`;
      paragraphData += `route: ${tour.route} <br>`;
      paragraphData += `cost: ${tour.cost}<br>`;
    });

    paragraphData += `<br>population_size: ${data.population_size}<br>`;
    paragraphData += `generation_limit: ${data.generation_limit}<br>`;
    paragraphData += `crossover_prob: ${data.crossover_prob}<br>`;
    paragraphData += `mutation_rate: ${data.mutation_rate}<br>`;
    paragraphData += `initial_cost: ${data.initial_cost}<br>`;
    paragraphData += `final_cost: ${data.final_cost}<br>`;
    paragraphData += `time: ${data.time}s<br>`;
  }  

  return paragraphData;
}

const handleMouseOver = function(event, d) {
  d3.select("#info")
    .style("opacity", 1)
    .style("left", (event.pageX + 5) + "px")
    .style("top", (event.pageY - 28) + "px")
    .style("background", d.color)
    .style("color", "white")
    .html(`index: ${d.node_index}`);
};

const handleMouseOut = function(event, d) {
  d3.select("#info")
    .style("opacity", 0);
};

function drawGraph(data, isInputData) {
  const width = 600;
  const height = 600;
  const spacing = 100;

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
    .domain([d3.min(data.data, (d) => d.x), d3.max(data.data, (d) => d.x)])
    .range([0, width - spacing]);

  const yScale = d3
    .scaleLinear()
    .domain([d3.min(data.data, (d) => d.y), d3.max(data.data, (d) => d.y)])
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
  if (colors.length < data.tours.length) {
    alert("Not enough colors for all tours");
  }
  const lineGenerator = d3
    .line()
    .x((d) => xScale(d.x))
    .y((d) => yScale(d.y));

  for (let i = 0; i < data.tours.length; i++) {
    const linePoints = [];
    for (let j = 0; j < data.tours[i].route.length; j++) {
      const pointIndex = data.tours[i].route[j] - 1;
      let point = data.data[pointIndex];
      point.color = colors[i];
      linePoints.push(point);
    }
    let lastPoint = data.data[data.tours[i].route[0] - 1];
    lastPoint.color = colors[i];
    linePoints.push(lastPoint);

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
      .attr("cx", (d) => xScale(d.x))
      .attr("cy", (d) => yScale(d.y))
      .attr("r", 5)
      .style("fill", colors[i])
      .on("mouseover", handleMouseOver)
      .on("mouseout", handleMouseOut);
  }

  let div;
  const paragraph = document.createElement("p");
  paragraph.className = "description";


  if (isInputData) {
    div = document.getElementById("container_input");
    paragraph.innerHTML = setParagraphData(data, true);
  } else {
    div = document.getElementById("container_output");
    paragraph.innerHTML = setParagraphData(data, false);
  }

  div.appendChild(paragraph);
}
