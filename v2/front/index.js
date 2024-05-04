width = 500
height = 500
spacing = 10

data = [
    {"node_index": 1, "x": 38.24, "y": 20.42},
    {"node_index": 2, "x": 39.57, "y": 26.15},
    {"node_index": 3, "x": 40.56, "y": 25.32},
    {"node_index": 4, "x": 36.26, "y": 23.12},
    {"node_index": 5, "x": 33.48, "y": 10.54},
    {"node_index": 6, "x": 37.56, "y": 12.19},
    {"node_index": 7, "x": 38.42, "y": 13.11},
    {"node_index": 8, "x": 37.52, "y": 20.44},
    {"node_index": 9, "x": 41.23, "y": 9.10},
    {"node_index": 10, "x": 41.17, "y": 13.05},
    {"node_index": 11, "x": 36.08, "y": -5.21},
    {"node_index": 12, "x": 38.47, "y": 15.13},
    {"node_index": 13, "x": 38.15, "y": 15.35},
    {"node_index": 14, "x": 37.51, "y": 15.17},
    {"node_index": 15, "x": 35.49, "y": 14.32},
    {"node_index": 16, "x": 39.36, "y": 19.56}
]

var svg = d3.select("body").append("svg")
    .attr("width", width)
    .attr("height", height)
    .style("background", "pink")
    .append("g")
    .attr("transform", "translate(" + spacing/2 + "," + spacing/2 + ")");

var xScale = d3.d3.scaleLinear()
    .domain([d3.min(data, function(d) { return d.x; }), d3.max(data, function(d) { return d.x; })])
    .range([0, width - spacing]);

var yScale = d3.scaleLinear()
    .domain([d3.min(data, function(d) { return d.y; }), d3.max(data, function(d) { return d.y; })])
    .range([height - spacing, 0]);

var xAxis = d3.axisBottom(xScale);
var yAxis = d3.axisLeft(yScale);

svg.append("g").call(xAxis);
svg.append("g").call(yAxis);
