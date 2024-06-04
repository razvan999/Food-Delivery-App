import React, { useRef, useEffect, useState } from "react";
import { Loader } from "@googlemaps/js-api-loader";

const apiKey = require("../apiKey.json").key;

function Map({
  setMapInstance,
  setImportFileLocations,
  locationType,
  locations,
  setLocations,
}) {
  const mapRef = useRef(null);
  const [map, setMap] = useState(null);

  function printLocations() {
    if (locations.length > 0) {
      console.log(locations);
    }
  }

  function addLocation(latLng) {
    console.log("Adding location...");

    const name = prompt("Enter the name of the new location:");
    if (name) {
      const newLocation = {
        name,
        latitude: latLng.lat(),
        longitude: latLng.lng(),
        type: locationType ? "DEPOT" : "CUSTOMER",
      };
      setLocations((prevLocations) => [...prevLocations, newLocation]);
      displayLocations();
      printLocations();
    }
  }

  function displayLocations() {
    if (locations.length > 0 && map) {
      locations.forEach((location) => {
        new window.google.maps.Marker({
          position: { lat: location.latitude, lng: location.longitude },
          map: map,
          title: location.name,
          icon:
            location.type === "DEPOT"
              ? require("../icons/blue_pin.png")
              : require("../icons/red_pin.png"),
        });
      });
    }
  }

  const handleFileButtonClick = () => {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = ".json";
    input.onchange = (event) => {
      const file = event.target.files[0];
      importFileLocations(file);
    };
    input.click();
  };

  function importFileLocations(file) {
    if (file instanceof Blob) {
      const reader = new FileReader();
      reader.onload = (event) => {
        try {
          const importedData = JSON.parse(event.target.result);
          setLocations(importedData.locations || []);
          console.log(importedData);
        } catch (error) {
          console.error("Error parsing JSON file:", error);
        }
      };
      reader.readAsText(file);
    } else {
      console.error("Invalid file type.");
    }
  }

  useEffect(() => {
    const loader = new Loader({
      apiKey: apiKey,
      version: "beta",
      libraries: ["places"],
    });

    loader
      .load()
      .then(() => {
        const mapInstance = new window.google.maps.Map(mapRef.current, {
          center: { lat: 47.1562245, lng: 27.5045704 },
          zoom: 12,
          fullscreenControl: false,
          mapTypeControl: false,
          streetViewControl: false,
          keyboardShortcuts: false,
        });

        setMap(mapInstance);
        setMapInstance(mapInstance);
      })
      .catch((e) => {
        console.error("Error loading Google Maps API:", e);
      });
  }, [setMapInstance]);

  useEffect(() => {
    if (map) {
      const clickListener = map.addListener("click", (event) => {
        addLocation(event.latLng);
      });

      setImportFileLocations(() => handleFileButtonClick);
      displayLocations();

      return () => {
        window.google.maps.event.removeListener(clickListener);
      };
    }
  }, [map, locationType, locations, setLocations]);

  return (
    <div id="map" ref={mapRef} style={{ height: "100vh", width: "100%" }}></div>
  );
}

export default Map;












































// import React, { useRef, useEffect } from "react";
// import { Loader } from "@googlemaps/js-api-loader";

// const apiKey = require("../apiKey.json").key;
// ////////////////////////////////////////////////////////////////
// // const ws_maps = new WebSocket("ws://localhost:3000/");

// function Map({
//   setMapInstance,
//   setImportFileLocations,
//   locationType,
//   locations,
//   setLocations,
// }) {
//   const mapRef = useRef(null);
//   let map,
//     flag_run_algorithm = true;

//   function printLocations() {
//     if (locations.length > 0) {
//       console.log(locations);
//     }
//   }

//   function addLocation(latLng) {
//     console.log("Adding location...");

//     const name = prompt("Enter the name of the new location:");
//     if (name) {
//       const newLocation = {
//         name,
//         latitude: latLng.lat(),
//         longitude: latLng.lng(),
//         type: locationType ? "DEPOT" : "CUSTOMER",
//       };
//       setLocations((prevLocations) => [...prevLocations, newLocation]);
//       displayLocations();
//       printLocations();
//     }
//   }

//   function displayLocations() {
//     if (locations.length > 0) {
//       locations.forEach((location) => {
//         new window.google.maps.Marker({
//           position: { lat: location.latitude, lng: location.longitude },
//           map: map,
//           title: location.name,
//           icon:
//             location.type === "DEPOT"
//               ? require("../icons/blue_pin.png")
//               : require("../icons/red_pin.png"),
//         });
//       });
//     }
//   }

//   // function calculateDistance(location1, location2, callback) {
//   //   var directionsService = new window.google.maps.DirectionsService();
//   //   var request = {
//   //     origin: new window.google.maps.LatLng(location1.latitude, location1.longitude),
//   //     destination: new window.google.maps.LatLng(
//   //       location2.latitude,
//   //       location2.longitude
//   //     ),
//   //     travelMode: window.google.maps.TravelMode.DRIVING,
//   //   };
//   //   directionsService.route(request, function (response, status) {
//   //     if (status == window.google.maps.DirectionsStatus.OK) {
//   //       var distance = response.routes[0].legs[0].distance.value; // Distance in meters
//   //       callback(distance/1000);
//   //     } else {
//   //       console.error("Error fetching directions: " + status);
//   //       callback(null);
//   //     }
//   //   });
//   // }

//   // function calculateDistanceMatrix(locations) {
//   //   const numLocations = locations.length;
//   //   const distanceMatrix = [];

//   //   function calculateDistancePromise(location1, location2) {
//   //     return new Promise((resolve, reject) => {
//   //       calculateDistance(location1, location2, (distance) => {
//   //         resolve(distance);
//   //       });
//   //     });
//   //   }

//   //   async function calculateDistancesAsync() {
//   //     for (let i = 0; i < numLocations; i++) {
//   //       distanceMatrix[i] = [];
//   //       for (let j = 0; j < numLocations; j++) {
//   //         const distance = await calculateDistancePromise(
//   //           locations[i],
//   //           locations[j]
//   //         );
//   //         distanceMatrix[i][j] = distance;
//   //       }
//   //     }
//   //     return distanceMatrix;
//   //   }

//   //   return calculateDistancesAsync();
//   // }

//   // function deleteOldFile() {
//   //   fetch("/delete-json-maps", {
//   //     method: "DELETE",
//   //   })
//   //     .then((response) => {
//   //       if (!response.ok) {
//   //         throw new Error("Network response was not ok");
//   //       }
//   //       return response.text();
//   //     })
//   //     .then((data) => {
//   //       console.log(data);
//   //       // alert("File deleted successfully");
//   //     })
//   //     .catch((error) => {
//   //       console.error("There was an error:", error);
//   //       alert("Error deleting file: " + error.message);
//   //     });
//   // }

//   // const handleFileButtonClick = () => {
//   //   const input = document.createElement('input');
//   //   input.type = 'file';
//   //   input.accept = '.json';
//   //   input.onchange = (event) => {
//   //     const file = event.target.files[0];
//   //     importFileLocations(file);
//   //   };
//   //   input.click();
//   // };

//   // function importFileLocations(file) {
//   //   if (file instanceof Blob) {
//   //     const reader = new FileReader();
//   //     reader.onload = function (event) {
//   //       try {
//   //         const importedData = JSON.parse(event.target.result);
//   //         locations = importedData;

//   //         console.log(importedData);
//   //         // displayLocations();
//   //       } catch (error) {
//   //         console.error("Error parsing JSON file:", error);
//   //       }
//   //     };
//   //     reader.readAsText(file);
//   //   } else {
//   //     console.error("Invalid file type.");
//   //   }
//   // }

//   const handleFileButtonClick = () => {
//     const input = document.createElement("input");
//     input.type = "file";
//     input.accept = ".json";
//     input.onchange = (event) => {
//       const file = event.target.files[0];
//       importFileLocations(file);
//     };
//     input.click();
//   };

//   function importFileLocations(file) {
//     if (file instanceof Blob) {
//       const reader = new FileReader();
//       reader.onload = (event) => {
//         try {
//           const importedData = JSON.parse(event.target.result);
//           setLocations(importedData.locations || []);
//           console.log(importedData);
//         } catch (error) {
//           console.error("Error parsing JSON file:", error);
//         }
//       };
//       reader.readAsText(file);
//     } else {
//       console.error("Invalid file type.");
//     }
//   }

//   useEffect(() => {
//     const loader = new Loader({
//       apiKey: apiKey,
//       version: "beta",
//       libraries: ["places"],
//     });

//     loader
//       .load()
//       .then(async () => {
//         map = new window.google.maps.Map(mapRef.current, {
//           center: { lat: 47.1562245, lng: 27.5045704 },
//           zoom: 12,
//           fullscreenControl: false,
//           mapTypeControl: false,
//           streetViewControl: false,
//           keyboardShortcuts: false,
//         });

//         await setMapInstance(map);
//       })
//       .catch((e) => {
//         console.error("Error loading Google Maps API:", e);
//       });
//   }, [setMapInstance]);


//   useEffect(() => {
//     map.addListener("click", (event) => {
//       addLocation(event.latLng);
//     });

//     setImportFileLocations(() => handleFileButtonClick);
//     displayLocations();
//   }, [locationType, locations, setLocations]);

//   return (
//     <div id="map" ref={mapRef} style={{ height: "100vh", width: "100%" }}></div>
//   );
// }

// export default Map;
