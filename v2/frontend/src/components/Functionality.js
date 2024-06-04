  import React, { useRef, useEffect, useState } from "react";
  import AddVehicles from "./AddVehicles";
  import Advanced from "./Advanced";
  import Configuration from "./Configuration";
  import Actions from "./Actions";
  import ToursDetails from "./ToursDetails";
  import LocationType from "./LocationType";
  import Alert from "./Alert";
  import "../css/functionality.css";
  import "../css/config.css";

  import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
  import {
    faBars,
    faXmark,
    faMagnifyingGlass,
  } from "@fortawesome/free-solid-svg-icons";

  function Functionality({
    mapInstance,
    importFileLocations,
    setLocationType,
    locationType,
    locations,
  }) {
    const [configuration, setConfiguration] = useState({"selection": "rank_selection", "crossover": "onepointcrossover", "population_size": "dynamic", "generation_limit": "20"});
    const [vehicles, setVehicles] = useState([]);

    const searchInputRef = useRef(null);
    const [displayFunctionality, setDisplayFunctionality] = useState("none");
    const [menuFlag, setMenuFlag] = useState(true);
    const [displayToggle, setDisplayToggle] = useState("none");
    const [toursFlag, setToursFlag] = useState(false);

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

        return () => {
          window.removeEventListener("resize", handleResize);
        };
      });
    }, [mapInstance]);

    return (
      <div id="functionality-container">
        <div id="card-container">
          <div id="card" style={{ display: displayFunctionality }}>
            <div id="title">
              <p>Food Delivery App.</p>
            </div>

            {/* {toursFlag ? <ToursDetails /> : null} */}
            {/* <ToursDetails /> */}
            <LocationType setLocationType={setLocationType} loc={locationType} />
            <AddVehicles setVehicles={setVehicles} />
            <Configuration setConfiguration={setConfiguration} />
            <Advanced importFileLocations={importFileLocations} />
            <Actions locations={locations} vehicles={vehicles} configuration={configuration} />

            <Alert />
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
              {/* <FontAwesomeIcon icon={faMagnifyingGlass} className="search-icon"/> */}
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
