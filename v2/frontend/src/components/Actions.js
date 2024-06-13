import React from "react";
import "../css/actions.css";

function Actions({ clearData, data, notify, run_algorithm, map, setToursFlag, setReceivedData }) {
  function calculateDistance(location1, location2, callback) {
    var directionsService = new window.google.maps.DirectionsService();
    var request = {
      origin: new window.google.maps.LatLng(
        location1.latitude,
        location1.longitude
      ),
      destination: new window.google.maps.LatLng(
        location2.latitude,
        location2.longitude
      ),
      travelMode: window.google.maps.TravelMode.DRIVING,
    };
    directionsService.route(request, function (response, status) {
      if (status === "OK") {
        var distance = response.routes[0].legs[0].distance.value; // Distance in meters
        callback(distance / 1000);
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
      console.log(distanceMatrix);
      return distanceMatrix;
    }

    return calculateDistancesAsync();
  }

  function setsAreEqual(set1, set2) {
    if (set1.size !== set2.size) {
      return false;
    }

    for (let item of set1) {
      if (!set2.has(item)) {
        return false;
      }
    }
    return true;
  }

  function getDepots() {
    console.log(data);
    let depots = [];
    data.locations.forEach((location) => {
      if (location.type === "DEPOT") {
        depots.push(location);
      }
    });

    return depots;
  }

  function checkData() {
    if (data.locations.length == 0) {
      notify("Nu ati selectat nici o locatie.");
      return false;
    }

    if (data.vehicles.length == 0) {
      notify("Nu ati selectat nici un vehicul.");
      return false;
    }

    if (data.locations.length < 3) {
      notify("Trebuie sa selectati cel putin 3 locatii.");
      return false;
    }

    if (getDepots().length == 0) {
      notify("Trebuie sa existe cel putin un depozit!");
      return false;
    }

    if (data.vehicles.length == 0) {
      notify("Trebuie sa existe cel putin un vehicul");
      return false;
    }

    const depotIndexesSet = new Set();
    data.locations.forEach((location, index) => {
      if (location.type === "DEPOT") {
        depotIndexesSet.add(index);
      }
    });

    const vehicleIndexesSet = new Set();
    data.vehicles.forEach((vehicle) => {
      vehicleIndexesSet.add(parseInt(vehicle.depot_index, 10));
    });

    if (!setsAreEqual(depotIndexesSet, vehicleIndexesSet)) {
      notify("Fiecare depot trebui sa aiba cel putin un vehicul!");
      return false;
    }

    console.log("All good broooo");
    return true;
  }

  function printData() {
    console.log(data);
  }

  async function updateData() {
    if (checkData()) {
      data.matrix = await calculateDistanceMatrix(data.locations);
      console.log("Matrix updated!");
    }
  }

  async function handleFindRoutesButton() {
    try {
      if (!checkData()) {
        return;
      }

      console.log("printing data");
      printData();
  
      console.log("calculating matrix");
      await updateData(); // Wait for updateData to complete
      notify("Asteptati sa se calculeze rutele...");
  
      // now, set it to the server
      await run_algorithm(data, map, setToursFlag, setReceivedData);
    } catch (error) {
      console.error("Error handling find routes button:", error);
      notify("A apărut o eroare: " + error);
    }
  }

  return (
    <div>
      <h4 className="titleClass">Actions</h4>
      <div className="actionsContainer">
        <button className="btn btnWidth" onClick={handleFindRoutesButton}>
          Find routes
        </button>
        <button className="btn btnWidth">Notify couriers</button>
        <button className="btn btnWidth" onClick={clearData}>
          Clear data
        </button>
        <button className="btn btnWidth">Banchmarks</button>{" "}
        {/* la muii ista sa verific daca nu astept sa termine ceva si fut o notificareeee */}
      </div>
    </div>
  );
}

export default Actions;
