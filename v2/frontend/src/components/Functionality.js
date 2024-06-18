import React, { useRef, useEffect, useState } from "react";
import AddVehicles from "./AddVehicles";
import Advanced from "./Advanced";
import Configuration from "./Configuration";
import Actions from "./Actions";
import ToursDetails from "./ToursDetails";
import LocationType from "./LocationType";
import "../css/functionality.css";
import "../css/config.css";

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
  faBars,
  faXmark,
  faMagnifyingGlass,
} from "@fortawesome/free-solid-svg-icons";

const ws_maps = new WebSocket("ws://localhost:3000/");

function deleteOldFile() {
  fetch("http://localhost:3000/delete-json-maps", {
    method: "DELETE",
  })
    .then((response) => {
      if (!response.ok) {
        throw new Error("Network response was not ok");
      }
      return response.text();
    })
    .then((deleteData) => {
      console.log(deleteData);
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

async function run_algorithm(dataToSend, map1, setToursFlag, setReceivedData) {
  console.log("aaa - run_algorithm");

  try {
    deleteOldFile();

    fetch("http://localhost:3000/sendData", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(dataToSend),
    })
      .then((response) => {
        if (response.ok) {
          console.log("Data sent successfully");

          talkToServer().then((result) => {
            if (result) {
              console.log("I can fetch!!!!!!!");

              getOuputFromAlg(map1, dataToSend, setToursFlag, setReceivedData);
              console.log("display route");
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
}

function displayRoute(map, receivedData, sentData) {
  console.log("is in display route");

  const directionsService = new window.google.maps.DirectionsService();
  const directionsRenderers = [];

  receivedData.tours.forEach((tour, index) => {
    const depotLocation = new window.google.maps.LatLng(
      sentData.locations[tour.route[0]].latitude,
      sentData.locations[tour.route[0]].longitude
    );

    const waypoints = tour.route.slice(1, -1).map((locIndex) => ({
      location: new window.google.maps.LatLng(
        sentData.locations[locIndex].latitude,
        sentData.locations[locIndex].longitude
      ),
      stopover: true,
    }));

    const request = {
      origin: depotLocation,
      destination: depotLocation,
      waypoints: waypoints,
      travelMode: window.google.maps.TravelMode.DRIVING,
    };

    directionsService.route(request, (result, status) => {
      if (status === window.google.maps.DirectionsStatus.OK) {
        const directionsRenderer = new window.google.maps.DirectionsRenderer({
          map: map,
          directions: result,
          suppressMarkers: true, // Suppress the default markers
          polylineOptions: {
            strokeColor: index % 2 === 0 ? "#0000FF" : "#FF0000",
            strokeOpacity: 0.8,
            strokeWeight: 5,
          },
        });
        directionsRenderers.push(directionsRenderer);
      } else {
        console.error(`Error fetching directions for tour ${index}: ${status}`);
      }
    });
  });
}

async function getOuputFromAlg(mapInstance, data, setToursFlag, setReceivedData) {
  try {
    const response = await fetch("http://localhost:3000/getData/maps/output_maps_cpp");
    if (!response.ok) {
      throw new Error("Network response was not ok");
    }
    const receivedData = await response.json();
    console.log("aici");
    console.log(receivedData);
    console.log(data);
    setToursFlag(true);
    setReceivedData(receivedData);
    displayRoute(mapInstance, receivedData, data);
  } catch (error) {
    console.error("There was an error:", error);
    alert("Error fetching route: " + error.message);
  }
}

function Functionality({
  mapInstance,
  importFileLocations,
  setLocationType,
  locationType,
  highlightMarker,
  notify,
  locations,
  setLocations,
}) {
  const [receivedData, setReceivedData] = useState(null);
  const [configuration, setConfiguration] = useState({
    selection: "tournament_selection",
    crossover: "twopointcrossover",
    population_size: "dynamic",
    generation_limit: "20",
  });
  const [vehicles, setVehicles] = useState([]);

  const [data, setData] = useState({
    selection: configuration.selection,
    crossover: configuration.crossover,
    population_size: configuration.population_size,
    generation_limit: configuration.generation_limit,
    locations: [],
    vehicles: [],
    matrix: [],
  });

  function removeLocation(index) {
    setLocations((prevLocations) =>
      prevLocations.filter((_, i) => i !== parseInt(index, 10))
    );
    console.log("remove location " + index);
  }

  useEffect(() => {
    setData({
      selection: configuration.selection,
      crossover: configuration.crossover,
      population_size: configuration.population_size,
      generation_limit: configuration.generation_limit,
      locations: locations,
      vehicles: vehicles,
      matrix: [],
    });
  }, [configuration, locations, vehicles]);

  function clearData() {
    console.log("Clear data");

    setLocations([]);
    setVehicles([]);
    setConfiguration({
      selection: "rank_selection",
      crossover: "onepointcrossover",
      population_size: "dynamic",
      generation_limit: "20",
    });
    setData({
      selection: configuration.selection,
      crossover: configuration.crossover,
      population_size: configuration.population_size,
      generation_limit: configuration.generation_limit,
      locations: [],
      vehicles: [],
      matrix: [],
    });
  }

  const searchInputRef = useRef(null);
  const [displayFunctionality, setDisplayFunctionality] = useState("none");
  const [menuFlag, setMenuFlag] = useState(true);
  const [displayToggle, setDisplayToggle] = useState("none");
  const [toursFlag, setToursFlag] = useState(false);

  function importFileVehicles(file) {
    if (file instanceof Blob) {
      const reader = new FileReader();
      reader.onload = function (event) {
        try {
          const importedData = JSON.parse(event.target.result);
          setVehicles(importedData.vehicles || []);
        } catch (error) {
          console.error("Error parsing JSON vehicle file:", error);
        }
      };
      reader.readAsText(file);
    } else {
      console.error("No vehicle file selected.");
    }
  }

  const handleFileButtonClickVehicles = () => {
    console.log("macar atat");
    const input = document.createElement("input");
    input.type = "file";
    input.accept = ".json";
    input.onchange = (event) => {
      const file = event.target.files[0];
      importFileVehicles(file);
    };
    input.click();
  };

  const toggleFunctionality = () => {
    setDisplayFunctionality(displayFunctionality === "none" ? "block" : "none");
    setMenuFlag(menuFlag === true ? false : true);
  };

  useEffect(() => {
    const handleResize = () => {
      if (window.innerWidth <= 768) {
        setDisplayFunctionality("none");
        setMenuFlag(true);
        setDisplayToggle("block");
      } else {
        setDisplayFunctionality("block");
        setMenuFlag(false);
        setDisplayToggle("none");
      }
    };

    handleResize();
    window.addEventListener("resize", handleResize);

    if (!mapInstance) return;

    const autocomplete = new window.google.maps.places.Autocomplete(
      searchInputRef.current
    );
    autocomplete.bindTo("bounds", mapInstance);

    autocomplete.addListener("place_changed", () => {
      const place = autocomplete.getPlace();
      if (!place.geometry || !place.geometry.location) {
        console.error("No details available for input: '" + place.name + "'");
        return;
      }

      mapInstance.setCenter(place.geometry.location);
      mapInstance.setZoom(15);
    });

    return () => {
      window.removeEventListener("resize", handleResize);
    };
  }, [mapInstance]);

  return (
    <div id="functionality-container">
      <div id="card-container">
        <div id="card" style={{ display: displayFunctionality }}>
          <div id="title">
            <p>Food Delivery App.</p>
          </div>

          {toursFlag ? <ToursDetails receivedData={receivedData} data={data} /> : null}

          <LocationType
            setLocationType={setLocationType}
            loc={locationType}
            locations={data.locations}
            removeLocation={removeLocation}
            highlightMarker={highlightMarker}
          />
          <AddVehicles
            setVehicles={setVehicles}
            locations={locations}
            highlightMarker={highlightMarker}
            notify={notify}
          />
          <Configuration setConfiguration={setConfiguration} />
          <Advanced
            importFileLocations={importFileLocations}
            importFileVehicles={handleFileButtonClickVehicles}
          />
          <Actions clearData={clearData} data={data} notify={notify} run_algorithm={run_algorithm} map={mapInstance} setToursFlag={setToursFlag} setReceivedData={setReceivedData} />
        </div>
      </div>

      <div id="topbar">
        <div id="searchbar">
          <div
            id="toggleFunctionality"
            onClick={toggleFunctionality}
            style={{ display: displayToggle }}
          >
            <p>
              {menuFlag ? (
                <FontAwesomeIcon icon={faBars} />
              ) : (
                <FontAwesomeIcon icon={faXmark} />
              )}{" "}
            </p>
          </div>

          <div id="Seeeearch">
            <input
              ref={searchInputRef}
              type="text"
              placeholder="Search for a place"
            />
          </div>
        </div>
      </div>
    </div>
  );
}

export default Functionality;





















































































































































// import React, { useRef, useEffect, useState } from "react";
// import AddVehicles from "./AddVehicles";
// import Advanced from "./Advanced";
// import Configuration from "./Configuration";
// import Actions from "./Actions";
// import ToursDetails from "./ToursDetails";
// import LocationType from "./LocationType";
// import "../css/functionality.css";
// import "../css/config.css";

// import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
// import {
//   faBars,
//   faXmark,
//   faMagnifyingGlass,
// } from "@fortawesome/free-solid-svg-icons";

// const ws_maps = new WebSocket("ws://localhost:3000/");

// function deleteOldFile() {
//   // fetch("/delete-json-maps", {
//     fetch("http://localhost:3000/delete-json-maps", {
//     method: "DELETE",
//   })
//     .then((response) => {
//       if (!response.ok) {
//         throw new Error("Network response was not ok");
//       }
//       return response.text();
//     })
//     .then((deleteData) => {
//       console.log(deleteData);
//       // alert("File deleted successfully");
//     })
//     .catch((error) => {
//       console.error("There was an error:", error);
//       alert("Error deleting file: " + error.message);
//     });
// }

// function talkToServer() {
//   return new Promise((resolve, reject) => {
//     console.log("talk to server");
//     ws_maps.send("run_algorithm");

//     ws_maps.onmessage = function (event) {
//       console.log("received: " + event.data);
//       if (event.data === "you can fetch data") {
//         resolve(true);
//       }
//       resolve(false);
//     };

//     ws_maps.onerror = function (error) {
//       reject(error);
//     };
//   });
// }

// async function run_algorithm(dataToSend, map1) {
//   console.log("aaa - run_algorithm");

//   try {
//     deleteOldFile();

//     // fetch("/sendData", {
//     fetch("http://localhost:3000/sendData", {
//       method: "POST",
//       headers: {
//         "Content-Type": "application/json",
//       },
//       body: JSON.stringify(dataToSend),
//     })
//       .then((response) => {
//         if (response.ok) {
//           console.log("Data sent successfully");

//           talkToServer().then((result) => {
//             if (result) {
//               console.log("I can fetch!!!!!!!");

//               getOuputFromAlg(map);
//               console.log("display route");
//             } else {
//               alert("Error fetching data");
//             }
//           });
//         } else {
//           console.error("Failed to send data:", response.statusText);
//         }
//       })
//       .catch((error) => console.error("Error sending data:", error));
//   } catch (error) {
//     console.error("Error calculating distance matrix:", error);
//   }
// }

// function displayRoute(map, receivedData, sentData) {
//   console.log("is in display route");

//   const directionsService = new window.google.maps.DirectionsService();
//   const directionsRenderers = [];

//   receivedData.tours.forEach((tour, index) => {
//     const depotLocation = new window.google.maps.LatLng(
//       sentData.locations[tour.route[0]].latitude,
//       sentData.locations[tour.route[0]].longitude
//     );

//     const waypoints = tour.route.slice(1, -1).map((locIndex) => ({
//       location: new window.google.maps.LatLng(
//         sentData.locations[locIndex].latitude,
//         sentData.locations[locIndex].longitude
//       ),
//       stopover: true,
//     }));

//     const request = {
//       origin: depotLocation,
//       destination: depotLocation,
//       waypoints: waypoints,
//       travelMode: window.google.maps.TravelMode.DRIVING,
//     };

//     directionsService.route(request, (result, status) => {
//       if (status === window.google.maps.DirectionsStatus.OK) {
//         const directionsRenderer = new window.google.maps.DirectionsRenderer({
//           map: map,
//           directions: result,
//           suppressMarkers: false,
//           polylineOptions: {
//             strokeColor: index % 2 === 0 ? "#0000FF" : "#FF0000",
//             strokeOpacity: 0.8,
//             strokeWeight: 5,
//           },
//         });
//         directionsRenderers.push(directionsRenderer);
//       } else {
//         console.error(`Error fetching directions for tour ${index}: ${status}`);
//       }
//     });
//   });
// }














































// function Functionality({
//   mapInstance,
//   importFileLocations,
//   setLocationType,
//   locationType,
//   highlightMarker,
//   notify,
//   locations,
//   setLocations,
// }) {
//   const [configuration, setConfiguration] = useState({
//     selection: "tournament_selection",
//     crossover: "twopointcrossover",
//     population_size: "dynamic",
//     generation_limit: "20",
//   });
//   const [vehicles, setVehicles] = useState([]);

//   const [data, setData] = useState({
//     selection: configuration.selection,
//     crossover: configuration.crossover,
//     population_size: configuration.population_size,
//     generation_limit: configuration.generation_limit,
//     locations: [],
//     vehicles: [],
//     matrix: [],
//   });

//   // function getOuputFromAlg() {
//   //   // fetch("/getData/maps/output_maps_cpp")
//   //   fetch("http://localhost:3000/getData/maps/output_maps_cpp")
//   //     .then((response) => {
//   //       if (!response.ok) {
//   //         throw new Error("Network response was not ok");
//   //       }
  
//   //       displayRoute(mapInstance, receivedData, data.locations);
  
//   //       // console.log(response.json());
//   //       return response.json();
//   //     })
//   //     .then((data) => {
//   //       console.log("aiciaa");
//   //       console.log(data);
//   //       const receivedData = data;
//   //     })
//   //     .catch((error) => {
//   //       console.error("There was an error:", error);
//   //       alert("Error fetching route: " + error.message);
//   //     });
//   // }

//   let receivedData = {
//     tours: [
//       {
//         vehicle: 1,
//         route: [4, 0, 3, 2, 4],
//         cost: 474.573975,
//       },
//       {
//         vehicle: 0,
//         route: [4, 1, 5, 4],
//         cost: 474.573975,
//       },
//     ],
//     final_cost: 474.573975,
//     time: 11.056007,
//   };

//   function removeLocation(index) {
//     setLocations((prevLocations) =>
//       prevLocations.filter((_, i) => i !== parseInt(index, 10))
//     );
//     console.log("remove location " + index);
//   }

//   useEffect(() => {
//     setData({
//       selection: configuration.selection,
//       crossover: configuration.crossover,
//       population_size: configuration.population_size,
//       generation_limit: configuration.generation_limit,
//       locations: locations,
//       vehicles: vehicles,
//       matrix: [], // You might want to update this based on your logic
//     });
//   }, [configuration, locations, vehicles]);

//   function clearData() {
//     console.log("Clear data");

//     setLocations([]);
//     setVehicles([]);
//     setConfiguration({
//       selection: "rank_selection",
//       crossover: "onepointcrossover",
//       population_size: "dynamic",
//       generation_limit: "20",
//     });
//     setData({
//       selection: configuration.selection,
//       crossover: configuration.crossover,
//       population_size: configuration.population_size,
//       generation_limit: configuration.generation_limit,
//       locations: [],
//       vehicles: [],
//       matrix: [],
//     });
//   }

//   const searchInputRef = useRef(null);
//   const [displayFunctionality, setDisplayFunctionality] = useState("none");
//   const [menuFlag, setMenuFlag] = useState(true);
//   const [displayToggle, setDisplayToggle] = useState("none");
//   const [toursFlag, setToursFlag] = useState(false);

//   function importFileVehicles(file) {
//     if (file instanceof Blob) {
//       const reader = new FileReader();
//       reader.onload = function (event) {
//         try {
//           const importedData = JSON.parse(event.target.result);
//           setVehicles(importedData.vehicles || []);
//         } catch (error) {
//           console.error("Error parsing JSON vehicle file:", error);
//         }
//       };
//       reader.readAsText(file);
//     } else {
//       console.error("No vehicle file selected.");
//     }
//   }

//   const handleFileButtonClickVehicles = () => {
//     console.log("macar atat");
//     const input = document.createElement("input");
//     input.type = "file";
//     input.accept = ".json";
//     input.onchange = (event) => {
//       const file = event.target.files[0];
//       importFileVehicles(file);
//     };
//     input.click();
//   };

//   const toggleFunctionality = () => {
//     setDisplayFunctionality(displayFunctionality === "none" ? "block" : "none");
//     setMenuFlag(menuFlag === true ? false : true);
//   };

//   useEffect(() => {
//     const handleResize = () => {
//       if (window.innerWidth <= 768) {
//         setDisplayFunctionality("none");
//         setMenuFlag(true);
//         setDisplayToggle("block");
//       } else {
//         setDisplayFunctionality("block");
//         setMenuFlag(false);
//         setDisplayToggle("none");
//       }
//     };

//     handleResize();
//     window.addEventListener("resize", handleResize);

//     if (!mapInstance) return;

//     const autocomplete = new window.google.maps.places.Autocomplete(
//       searchInputRef.current
//     );
//     autocomplete.bindTo("bounds", mapInstance);

//     autocomplete.addListener("place_changed", () => {
//       const place = autocomplete.getPlace();
//       if (!place.geometry || !place.geometry.location) {
//         console.error("No details available for input: '" + place.name + "'");
//         return;
//       }

//       mapInstance.setCenter(place.geometry.location);
//       mapInstance.setZoom(15);
//     });

//     return () => {
//       window.removeEventListener("resize", handleResize);
//     };
//   }, [mapInstance]);

//   return (
//     <div id="functionality-container">
//       <div id="card-container">
//         <div id="card" style={{ display: displayFunctionality }}>
//           <div id="title">
//             <p>Food Delivery App.</p>
//           </div>

//           {/* {toursFlag ? <ToursDetails /> : null} */}
//           {/* <ToursDetails receivedData={receivedData} data={data} /> */}
//           <LocationType
//             setLocationType={setLocationType}
//             loc={locationType}
//             locations={data.locations}
//             removeLocation={removeLocation}
//             highlightMarker={highlightMarker}
//           />
//           <AddVehicles
//             setVehicles={setVehicles}
//             locations={locations}
//             highlightMarker={highlightMarker}
//             notify={notify}
//           />
//           <Configuration setConfiguration={setConfiguration} />
//           <Advanced
//             importFileLocations={importFileLocations}
//             importFileVehicles={handleFileButtonClickVehicles}
//           />
//           <Actions clearData={clearData} data={data} notify={notify} run_algorithm={run_algorithm} map={mapInstance} />
//         </div>
//       </div>

//       <div id="topbar">
//         <div id="searchbar">
//           <div
//             id="toggleFunctionality"
//             onClick={toggleFunctionality}
//             style={{ display: displayToggle }}
//           >
//             <p>
//               {menuFlag ? (
//                 <FontAwesomeIcon icon={faBars} />
//               ) : (
//                 <FontAwesomeIcon icon={faXmark} />
//               )}{" "}
//             </p>
//           </div>

//           <div id="Seeeearch">
//             {/* <FontAwesomeIcon icon={faMagnifyingGlass} className="search-icon"/> */}
//             <input
//               ref={searchInputRef}
//               type="text"
//               placeholder="Search for a place"
//             />
//           </div>
//         </div>
//       </div>
//     </div>
//   );
// }

// export default Functionality;
