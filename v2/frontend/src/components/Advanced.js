import React, { useState } from "react";
import "../css/advanced.css";
import "../css/addVehicles.css";

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faChevronUp, faChevronDown } from "@fortawesome/free-solid-svg-icons";

function Advanced({importFileLocations, importFileVehicles}) {
  const [expanded, setExpanded] = useState(false);

  const toggleExpand = () => {
    setExpanded(!expanded);
  };

  return (
    <div>
      <h4 onClick={toggleExpand} style={{ cursor: "pointer" }} className="titleClass">
        {expanded ? (
          <FontAwesomeIcon icon={faChevronUp} />
        ) : (
          <FontAwesomeIcon icon={faChevronDown} />
        )}
        Advanced
      </h4>

      {expanded && (
        <div className="advanceContainer">
          <button className="btn btnWidth" onClick={importFileLocations} accept=".json" >Import locatii</button>
          <button className="btn btnWidth" accept=".json">Import vehicule</button>
          <button className="btn btnWidth">Download data</button>
          <button className="btn btnWidth">Print data</button>
        </div>
      )}

      <div style={{width: "100%", marginBottom: "20px"}}></div>
    </div>
  );
}

export default Advanced;
