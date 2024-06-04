import React, { useRef, useState } from "react";
import Map from "../components/Map";
import Functionality from "../components/Functionality";

import "../css/home.css";

function Home() {
  const [mapInstance, setMapInstance] = useState(null);
  const [importFileLocations, setImportFileLocations] = useState(() => {});
  const [locationType, setLocationType] = useState(true);
  const [locations, setLocations] = useState([]);

  return (
    <div>
      <Functionality
        mapInstance={mapInstance}
        importFileLocations={importFileLocations}
        setLocationType={setLocationType}
        locationType={locationType}
        locations={locations}
      />
      <Map
        setMapInstance={setMapInstance}
        setImportFileLocations={setImportFileLocations}
        locationType={locationType}
        locations={locations}
        setLocations={setLocations}
      />
    </div>
  );
}

export default Home;












// import React, { useRef, useState } from "react";
// import Map from "../components/Map";
// import Functionality from "../components/Functionality";

// import "../css/home.css";

// function Home() {
//   const handleClick = () => {
//     console.log("Button clicked!");
//   };

//   const [mapInstance, setMapInstance] = useState(null);
//   const [clickFunction, setClickFunction] = useState(() => handleClick);
//   const [locationType, setLocationType] = useState(true);

//   return (
//     <div>
//       {/* <Functionality
//         mapInstance={mapInstance}
//         clickFunction={clickFunction}
//         setLocationType={setLocationType}
//         locationType={locationType}
//       /> */}
//       <Map
//         setMapInstance={setMapInstance}
//         setClickFunction={setClickFunction}
//         locationType={locationType}
//       />

//       {/* <button onClick={setLocationType(false)} style={{position:"absolute"}}>Click me!</button> */}
//     </div>
//   );
// }

// export default Home;
