import React, { useState } from "react";

function LocationType({
  setLocationType,
  locations,
  removeLocation,
  highlightMarker,
}) {
  const [type, setType] = useState("DEPOT");
  const [isOpen, setIsOpen] = useState(false);

  const handleTypeChange = (selectedType) => {
    setType(selectedType);
    setIsOpen(false);

    if (selectedType === "DEPOT") {
      setLocationType(true);
    } else if (selectedType === "CUSTOMER") {
      setLocationType(false);
    }
  };

  const handleLocationClick = (index) => {
    removeLocation(index);
  };

  const toggleDropdown = () => {
    setIsOpen(!isOpen);
  };

  const selectOptions = locations.map((location, index) => (
    <div
      key={index}
      className="select-option"
      onClick={() => handleLocationClick(index)}
      onMouseOver={() => highlightMarker(index)}
      onMouseLeave={() => highlightMarker(null) }
    >
      Location {index}
    </div>
  ));

  return (
    <div className="dropdown-container">
      <h4 className="titleClass">Location type</h4>
      <select
        className="form-control"
        value={type}
        onChange={(e) => handleTypeChange(e.target.value)}
      >
        <option value="DEPOT">Depot</option>
        <option value="CUSTOMER">Customer</option>
      </select>
      <button className="dropdown-toggle" onClick={toggleDropdown}>
        Show Locations
      </button>
      <div className={`dropdown ${isOpen ? "open" : ""}`}>
        <div className="dropdown-content">{selectOptions}</div>
      </div>
      <style>
        {`
        .dropdown-container {
          position: relative;
        }

        .dropdown-toggle {
          cursor: pointer;
        }

        .dropdown {
          position: absolute;
          top: calc(100% + 10px);
          left: 0;
          background-color: #fff;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
          border: 1px solid #ccc;
          border-top: none;
          width: 100%;
          max-height: 200px;
          overflow-y: auto;
          display: none;
        }

        .dropdown.open {
          display: block;
        }

        .dropdown-content {
          padding: 5px;
        }

        .select-option {
          padding: 8px 12px;
          cursor: pointer;
        }

        .select-option:hover {
          background-color: #f0f0f0;
        }
        `}
      </style>
    </div>
  );
}

export default LocationType;






























































































































































































// import React, { useState } from "react";

// function LocationType({
//   setLocationType,
//   locations,
//   removeLocation,
//   highlightMarker,
// }) {
//   const [type, setType] = useState("DEPOT");

//   const selectOptions = locations.map((location, index) => (
//     <option key={index} value={index}>
//       Location {index}
//     </option>
//   ));

//   const handleTypeChange = (e) => {
//     console.log("handle type change");

//     const selectedType = e.target.value;
//     setType(selectedType);

//     if (selectedType === "DEPOT") {
//       setLocationType(true);
//     } else if (selectedType === "CUSTOMER") {
//       setLocationType(false);
//     }
//   };

//   const handleLocationChange = (e) => {
//     const selectedIndex = e.target.value;
//     if (selectedIndex !== "") {
//       removeLocation(selectedIndex);
//     }
//   };

//   return (
//     <div>
//       <h4 className="titleClass">Location type</h4>
//       <div
//         style={{
//           margin: "20px 0px 20px 0px",
//           display: "flex",
//           justifyContent: "space-around",
//         }}
//       >
//         <select
//           className="form-control"
//           value={type}
//           onChange={handleTypeChange}
//         >
//           <option value="DEPOT">Depot</option>
//           <option value="CUSTOMER">Customer</option>
//         </select>

//         <select className="form-control" onChange={handleLocationChange}>
//           <option value="">Remove locations</option>
//           {selectOptions}
//         </select>
//       </div>
//     </div>
//   );
// }

// export default LocationType;
