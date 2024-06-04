const ws_maps = new WebSocket("ws://localhost:3000/");

const loader = new google.maps.plugins.loader.Loader({
    apiKey: "AIzaSyDGu0tyuHQTdk35f9K5yWM8fEpRvWnS92E",
  version: "beta",
});

let dataSent, map,
  locations = { locations: [] },
  newLocations = { locations: [] },
  vehicles = { vehicles: [] };
let flag_run_algorithm = true;

document.addEventListener("DOMContentLoaded", function () {
  document
    .getElementById("fileInputLocations")
    .addEventListener("change", importFileLocations);
  document
    .getElementById("fileInputVehicles")
    .addEventListener("change", importFileVehicles);
  updateButtonState();
  initialize();
});

function importFileLocations() {
  const fileInputLocations = document.getElementById("fileInputLocations");
  const file = fileInputLocations.files[0];

  if (file) {
    const reader = new FileReader();
    reader.onload = function (event) {
      try {
        const importedData = JSON.parse(event.target.result);
        locations = importedData;
        displayLocations();
      } catch (error) {
        console.error("Error parsing JSON file:", error);
      }
    };
    reader.readAsText(file);
  } else {
    console.error("No file selected.");
  }
}

function importFileVehicles() {
  const fileInputVehicles = document.getElementById("fileInputVehicles");
  const fileVehicles = fileInputVehicles.files[0];

  if (fileVehicles) {
    const reader = new FileReader();
    reader.onload = function (event) {
      try {
        const importedData = JSON.parse(event.target.result);
        vehicles = importedData;
      } catch (error) {
        console.error("Error parsing JSON vehicle file:", error);
      }
    };
    reader.readAsText(fileVehicles);
  } else {
    console.error("No vehicle file selected.");
  }
}

function print_locations() {
  if (locations.locations.length > 0) {
    console.log(locations);
  }

  if (newLocations.locations.length > 0) {
    console.log(newLocations);
  }
}

async function initMap() {
  await loader.load();
  const { Map, Marker, DirectionsService, TravelMode, DirectionsStatus } =
    google.maps;

  map = new Map(document.getElementById("map"), {
    center: { lat: 47.1562245, lng: 27.5045704 },
    zoom: 8,
  });

  map.addListener("click", function (event) {
    addLocation(event.latLng);
  });
}

function displayLocations() {
  if (locations.locations.length > 0) {
    locations.locations.forEach(function (location) {
      var marker = new google.maps.Marker({
        position: { lat: location.latitude, lng: location.longitude },
        map: map,
        title: location.name,
      });
    });
  }

  if (newLocations.locations.length > 0) {
    newLocations.locations.forEach(function (location) {
      var marker = new google.maps.Marker({
        position: { lat: location.latitude, lng: location.longitude },
        map: map,
        title: location.name,
      });
    });
  }
}

function addLocation(latLng) {
  var name = prompt("Enter the name of the new location:");
  if (name) {
    var newLocation = {
      name: name,
      latitude: latLng.lat(),
      longitude: latLng.lng(),
      type: "CUSTOMER"
    };
    newLocations.locations.push(newLocation);
    displayLocations();
  }
}

async function initialize() {
  initMap();
}

function calculateDistance(location1, location2, callback) {
  var directionsService = new google.maps.DirectionsService();
  var request = {
    origin: new google.maps.LatLng(location1.latitude, location1.longitude),
    destination: new google.maps.LatLng(
      location2.latitude,
      location2.longitude
    ),
    travelMode: google.maps.TravelMode.DRIVING,
  };
  directionsService.route(request, function (response, status) {
    if (status == google.maps.DirectionsStatus.OK) {
      var distance = response.routes[0].legs[0].distance.value; // Distance in meters
      callback(distance/1000);
    } else {
      console.error("Error fetching directions: " + status);
      callback(null);
    }
  });
}

function calculateDistanceMatrix(locations) {
  const numLocations = locations.length;
  const distanceMatrix = [];

  function calculateDistancePromise(location1, location2) {
    return new Promise((resolve, reject) => {
      calculateDistance(location1, location2, (distance) => {
        resolve(distance);
      });
    });
  }

  async function calculateDistancesAsync() {
    for (let i = 0; i < numLocations; i++) {
      distanceMatrix[i] = [];
      for (let j = 0; j < numLocations; j++) {
        const distance = await calculateDistancePromise(
          locations[i],
          locations[j]
        );
        distanceMatrix[i][j] = distance;
      }
    }
    return distanceMatrix;
  }

  return calculateDistancesAsync();
}

function updateButtonState() {
  const button = document.getElementById("runButton");
  button.disabled = !flag_run_algorithm;
}

function toggleFlag() {
  flag_run_algorithm = !flag_run_algorithm;
  updateButtonState();
  console.log("Flag value after button click:", flag_run_algorithm);
}

function deleteOldFile() {
  fetch("/delete-json-maps", {
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
      // alert("File deleted successfully");
    })
    .catch((error) => {
      console.error("There was an error:", error);
      alert("Error deleting file: " + error.message);
    });
}

function talkToServer() {
  return new Promise((resolve, reject) => {
    console.log("talk to server");
    ws_maps.send("run_algorithm");

    ws_maps.onmessage = function (event) {
      console.log("received: " + event.data);
      if (event.data === "you can fetch data") {
        resolve(true);
      }
      resolve(false);
    };

    ws_maps.onerror = function (error) {
      reject(error);
    };
  });
}

async function run_algorithm() {
  console.log("aaa");

  if (vehicles.vehicles.length <= 0) {
    toggleFlag();
    alert("Nu exista vehicule");
    return;
  }

  if (locations.locations.length <= 0 && newLocations.locations.length <= 0) {
    alert("Nu exista locatii");
    toggleFlag();
    return;
  }

  if (locations.locations.length + newLocations.locations.length < 3) {
    alert("Nu sunt suficiente locatii -> minim 3");
    toggleFlag();
    return;
  }

  let data = { locations: [], vehicles: [], matrix: [] };
  for (let i = 0; i < locations.locations.length; i++) {
    data.locations.push(locations.locations[i]);
  }

  for (let i = 0; i < newLocations.locations.length; i++) {
    data.locations.push(newLocations.locations[i]);
  }

  for (let i = 0; i < vehicles.vehicles.length; i++) {
    data.vehicles.push(vehicles.vehicles[i]);
  }

  try {
    const matrix = await calculateDistanceMatrix(data.locations);

    data.matrix = matrix;
    console.log(data);
    sentData = data;

    deleteOldFile();
    // send data to server to save the file
    fetch("/sendData", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    })
      .then((response) => {
        if (response.ok) {
          console.log("Data sent successfully");

          talkToServer().then((result) => {
            if (result) {
              console.log("I can fetch!!!!!!!");

              displayRoute();
            } else {
              alert("Error fetching data");
            }
          });
        } else {
          console.error("Failed to send data:", response.statusText);
        }
      })
      .catch((error) => console.error("Error sending data:", error));
  } catch (error) {
    console.error("Error calculating distance matrix:", error);
  }

  toggleFlag();
}

function displayRoute() {
  fetch("/getData/maps/output_maps_cpp")
    .then((response) => {
      if (!response.ok) {
        throw new Error("Network response was not ok");
      }

      // console.log(response.json());
      return response.json();
    })
    .then((data) => {
      console.log("aiciaa");
      console.log(data);
      const receivedData = data;

      let directionsService = new google.maps.DirectionsService();
      let directionsRenderer = new google.maps.DirectionsRenderer({
        map: map,
        suppressMarkers: false,
      });

      receivedData.tours.forEach((tour, index) => {
        const depot = new google.maps.LatLng(
          sentData.locations[tour.route[0]].latitude,
          sentData.locations[tour.route[0]].longitude
        );

        const waypoints = tour.route.slice(1, -1).map((index) => ({
          location: new google.maps.LatLng(
            sentData.locations[index].latitude,
            sentData.locations[index].longitude
          ),
          stopover: true,
        }));

        const request = {
          origin: depot,
          destination: depot,
          waypoints: waypoints,
          travelMode: google.maps.TravelMode.DRIVING,
        };

        directionsService.route(request, (result, status) => {
          if (status == google.maps.DirectionsStatus.OK) {
            const renderer = new google.maps.DirectionsRenderer({
              map: map,
              directions: result,
              suppressMarkers: false,
              polylineOptions: {
                strokeColor: index % 2 == 0 ? "#0000FF" : "#FF0000",
                strokeOpacity: 0.2,
                strokeWeight: 5,
              },
            });
          } else {
            console.error("Error fetching directions", result);
          }
        });
      });

      displayInfo(receivedData);
    })
    .catch((error) => {
      console.error("There was an error:", error);
      alert("Error fetching route: " + error.message);
    });
}




























































































































































































function displayInfo(receivedData) {
  const div = document.createElement("div");
  div.className = "description";
  let html = "<ul>";
  html +=
    "<li>population_size: " +
    (receivedData.population_size || "N/A")+ "</li>";
  html +=
    "<li>generation_limit: " +
    (receivedData.generation_limit || "N/A") +
    "</li>";
  html +=
    "<li>crossover_prob: " + (receivedData.crossover_prob || "N/A") + "</li>";
  html +=
    "<li>mutation_rate: " + (receivedData.mutation_rate || "N/A") + "</li>";
  html += "<li>initial_cost: " + (receivedData.initial_cost || "N/A") + "</li>";
  html += "<li>final_cost: " + (receivedData.final_cost || "N/A") + "</li>";
  html += "<li>time: " + (receivedData.time || "N/A") + "</li>";

  html += "</ul>";

  div.innerHTML = html;

  document.getElementById("info").appendChild(div);
}
window.onload = initialize;
