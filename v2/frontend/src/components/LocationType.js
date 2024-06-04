import React, { useState } from "react";

function LocationType({ setLocationType }) {
  const [type, setType] = useState("DEPOT");

  const handleTypeChange = (e) => {
    console.log("handle type change");

    const selectedType = e.target.value;
    setType(selectedType);

    if (selectedType === "DEPOT") {
      setLocationType(true);
    } else if (selectedType === "CUSTOMER") {
      setLocationType(false);
    }
  };

  return (
    <div>
      <select
        className="form-control"
        value={type}
        onChange={handleTypeChange}
      >
        <option value="DEPOT">Depot</option>
        <option value="CUSTOMER">Customer</option>
      </select>
    </div>
  );
}

export default LocationType;