import React, { useRef, useState } from "react";
import Map from "../components/Map";
import Functionality from "../components/Functionality";


import { ToastContainer, toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';

import "../css/home.css";

function Home() {
  const [mapInstance, setMapInstance] = useState(null);
  const [importFileLocations, setImportFileLocations] = useState(() => {});
  const [locationType, setLocationType] = useState(true);
  const [locations, setLocations] = useState([]);
  const [highlightMarker, setHighlightMarker] = useState(() => {});

  const notify = (message) => toast(message);

  return (
    <div>
      <Functionality
        mapInstance={mapInstance}
        importFileLocations={importFileLocations}
        setLocationType={setLocationType}
        locationType={locationType}
        locations={locations}
        highlightMarker={highlightMarker}
        notify={notify}
        setLocations={setLocations}
      />
      <Map
        setMapInstance={setMapInstance}
        setImportFileLocations={setImportFileLocations}
        locationType={locationType}
        locations={locations}
        setLocations={setLocations}
        setHighlightMarker={setHighlightMarker}
      />
      
      <ToastContainer />
    </div>
  );
}

export default Home;