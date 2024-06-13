import React, { useRef, useEffect, useState } from "react";
import { Loader } from "@googlemaps/js-api-loader";
import "../css/home.css";

const apiKey = require("../apiKey.json").key;

function Map({
  setMapInstance,
  setImportFileLocations,
  locationType,
  locations,
  setLocations,
  setHighlightMarker,
}) {
  const mapRef = useRef(null);
  const [map, setMap] = useState(null);
  const [markers, setMarkers] = useState([]);

  function printLocations() {
    if (locations.length > 0) {
      console.log(locations);
    }
  }

  function addLocation(latLng) {
    console.log("Adding location...");

    const newLocation = {
      latitude: latLng.lat(),
      longitude: latLng.lng(),
      style: require("../json/google_map_style.json"),
      type: locationType ? "DEPOT" : "CUSTOMER",
    };
    setLocations((prevLocations) => [...prevLocations, newLocation]);
    printLocations();
  }

  function displayLocations() {
    markers.forEach((marker) => {
      marker.setMap(null); // Remove all existing markers from the map
    });

    const newMarkers = locations.map((location) => {
      return new window.google.maps.Marker({
        position: { lat: location.latitude, lng: location.longitude },
        map: map,
        title: location.name,
        icon:
          location.type === "DEPOT"
            ? require("../icons/blue_pin.png")
            : require("../icons/red_pin.png"),
      });
    });

    setMarkers(newMarkers);
  }

  function highlightMarker(index) {
    let selected_depot = -1;
    if (index >= 0 && index < markers.length) {
      markers.forEach((marker, i) => {
        marker.setIcon({
          url:
            locations[i].type === "DEPOT"
              ? (selected_depot += 1) == index
                ? require("../icons/green_pin.png")
                : require("../icons/blue_pin.png")
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
  }, [map, locationType, locations, setLocations, setImportFileLocations]);

  useEffect(() => {
    setHighlightMarker(() => highlightMarker);
  }, [markers]);

  return (
    <div id="map" ref={mapRef} style={{ height: "100vh", width: "100%" }}></div>
  );
}

export default Map;
