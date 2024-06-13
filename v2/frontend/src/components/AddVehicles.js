import React, { useState, useEffect } from "react";
import "../css/addVehicles.css";

function VehicleForm({ index, formData, handleChange, depotIndexes, notify, highlightMarker }) {
  const selectOptions = depotIndexes.map((type, index) => (
    <option key={index} value={index}>
      Depot {index}
    </option>
  ));

  return (
    <div className="vehicleForm">
      <form>
        <select
          className="inputClass"
          name="depot_index"
          value={formData.depot_index || ""}
          onFocus={() => {
            if (depotIndexes.length === 0) {
              notify("Adaugati cel putin un depot!");
            }
          }}
          onChange={(event) => handleChange(index, event, true)}
          id={`depotNumber${index}`}
        >
          <option value="">Select Depot</option>
          {selectOptions}
        </select>
        <br />
        <input
          className="inputClass"
          type="number"
          placeholder="capacity"
          name="capacity"
          value={formData.capacity || ""}
          onChange={(event) => handleChange(index, event)}
          id={`capacity${index}`}
          min="0"
        />
        <br />
        <input
          className="inputClass"
          type="email"
          placeholder="quickyBoy@couriers.com"
          name="email"
          value={formData.email || ""}
          onChange={(event) => handleChange(index, event)}
          id={`email${index}`}
        />
        <br />
      </form>
    </div>
  );
}

function AddVehicles({ setVehicles, locations, highlightMarker, notify }) {
  const [selectedVehicles, setSelectedVehicles] = useState(1);
  const [vehicleData, setVehicleData] = useState([
    { id: 0, depot_index: "", capacity: "", email: "" },
  ]);

  function getDepotIndexes() {
    let depotIndexes = [];
    locations.forEach((location) => {
      if (location.type === "DEPOT") {
        depotIndexes.push(location);
      }
    });
    return depotIndexes;
  }

  useEffect(() => {
    const depotIndexes = getDepotIndexes();
    const nrveh = depotIndexes.length;

    // Update selectedVehicles if it's less than the number of depots
    if (nrveh > selectedVehicles) {
      setSelectedVehicles(nrveh);
    }

    // Update vehicleData to match the number of selected vehicles, keeping existing data
    setVehicleData((prevData) => {
      const newData = Array(Math.max(selectedVehicles, nrveh))
        .fill({ id: "", depot_index: "", capacity: "", email: "" })
        .map((data, index) => {
          const previousData = prevData[index] || {};
          return {
            ...data,
            id: index,
            depot_index: previousData.depot_index !== undefined ? parseInt(previousData.depot_index, 10) : "",
            capacity: previousData.capacity !== undefined ? parseInt(previousData.capacity, 10) : "",
            ...previousData, // Preserve existing data
          };
        });
      return newData;
    });
    
    
  }, [locations, selectedVehicles]);

  const handleChange = (index, event, flag = false) => {
    if (flag) {
      highlightMarker(event.target.value);
    }

    const { name, value } = event.target;

    setVehicleData((prevData) =>
      prevData.map((data, i) =>
        i === index ? {
          ...data,
          [name]: name === "depot_index" || name === "capacity" ? parseInt(value, 10) : value
        } : data
      )
    );
  };

  const handleSubmit = (event) => {
    event.preventDefault();
    const selectedData = vehicleData.slice(0, selectedVehicles);

    const isUnique = selectedData.every(
      (vehicle, index, self) =>
        index === self.findIndex((v) => v.email === vehicle.email)
    );

    if (!isUnique) {
      alert("Fiecare curier trebuie să fie unic! Verificați adresele de email.");
      return;
    }

    setVehicles(selectedData);
    console.log("Vehicle Data:", JSON.stringify(selectedData, null, 2));
  };

  return (
    <div className="vehiclesContainer">
      <h4 className="titleClass">Vehicles</h4>

      <div className="vehiclesContainer">
        <div className="vehicleSelector">
          <p>Number of vehicles:</p>
          <select
            value={selectedVehicles}
            onChange={(event) =>
              setSelectedVehicles(parseInt(event.target.value, 10))
            }
          >
            {[...Array(25).keys()].map((i) => (
              <option key={i + 1} value={i + 1}>
                {i + 1}
              </option>
            ))}
          </select>
        </div>

        <div className="formConteiner">
          {[...Array(selectedVehicles)].map((_, i) => (
            <VehicleForm
              key={i}
              index={i}
              formData={
                vehicleData[i] || {
                  id: "",
                  depot_index: "",
                  capacity: "",
                  email: "",
                }
              }
              handleChange={handleChange}
              depotIndexes={getDepotIndexes()}
              notify={notify}
              highlightMarker={highlightMarker}
            />
          ))}
        </div>

        <button className="btn btnWidth" type="submit" onClick={handleSubmit}>
          Save Vehicles
        </button>
      </div>
    </div>
  );
}

export default AddVehicles;










































































// import React, { useState, useEffect } from "react";
// import "../css/addVehicles.css";

// function VehicleForm({ index, formData, handleChange, depotIndexes, notify, highlightMarker }) {
//   const selectOptions = depotIndexes.map((type, index) => (
//     <option key={index} value={index}>
//       Depot {index}
//     </option>
//   ));

//   return (
//     <div className="vehicleForm">
//       <form>
//         <select
//           className="inputClass"
//           name="depot_index"
//           value={formData.depot_index || ""}
//           onFocus={() => {
//             if (depotIndexes.length === 0) {
//               notify("Adaugati cel putin un depot!");
//             }
//           }}
//           // offFocus={() => {highlightMarker(0, true)}}
//           offFocus={() => {console.log("Off Focus")}}
//           onChange={(event) => handleChange(index, event, true)}
//           id={`depotNumber${index}`}
//         >
//           <option value="">Select Depot</option>
//           {selectOptions}
//         </select>
//         <br />
//         <input
//           className="inputClass"
//           type="number"
//           placeholder="capacity"
//           name="capacity"
//           value={formData.capacity || ""}
//           onChange={(event) => handleChange(index, event)}
//           id={`capacity${index}`}
//           min="0"
//         />
//         <br />
//         <input
//           className="inputClass"
//           type="email"
//           placeholder="quickyBoy@couriers.com"
//           name="email"
//           value={formData.email || ""}
//           onChange={(event) => handleChange(index, event)}
//           id={`email${index}`}
//         />
//         <br />
//       </form>
//     </div>
//   );
// }

// function AddVehicles({ setVehicles, locations, highlightMarker, notify }) {
//   let nrveh = 1;

//   const [selectedVehicles, setSelectedVehicles] = useState(nrveh);
//   const [vehicleData, setVehicleData] = useState(
//     Array(2)
//       .fill({ id: "", depot_index: "", capacity: "", email: "" })
//       .map((data, index) => ({ ...data, id: index }))
//   );

//   function getDepotIndexes() {
//     let depotIndexes = [];
//     locations.forEach((location) => {
//       if (location.type === "DEPOT") {
//         depotIndexes.push(location);
//       }
//     });
//     return depotIndexes;
//   }

//   useEffect(() => {
//     const newData = Array(selectedVehicles)
//       .fill({ id: "", depot_index: "", capacity: "", email: "" })
//       .map((data, index) => ({ ...data, id: index }));
//     setVehicleData(newData);

//     if (selectedVehicles.length > 0) {
//       nrveh = getDepotIndexes().length;
//       setSelectedVehicles(nrveh);
//     }

//   }, [selectedVehicles, locations]);

//   const handleChange = (index, event, flag = false) => {
//     if (flag) {
//       // console.log("Depot Index:", event.target.value);
//       highlightMarker(event.target.value);
//     }

//     const { name, value } = event.target;
//     setVehicleData((prevData) =>
//       prevData.map((data, i) =>
//         i === index ? { ...data, [name]: value } : data
//       )
//     );
//   };

//   const handleSubmit = (event) => {
//     event.preventDefault();
//     const selectedData = vehicleData.slice(0, selectedVehicles);

//     const isUnique = selectedData.every(
//       (vehicle, index, self) =>
//         index === self.findIndex((v) => v.email === vehicle.email)
//     );

//     if (!isUnique) {
//       alert(
//         "Fiecare curier trebuie să fie unic! Verificați adresele de email."
//       );
//       return;
//     }

//     setVehicles(selectedData);
//     console.log("Vehicle Data:", JSON.stringify(selectedData, null, 2));
//   };

//   return (
//     <div className="vehiclesContainer">
//       <h4 className="titleClass">Vehicles</h4>

//       <div className="vehiclesContainer">
//         <div className="vehicleSelector">
//           <p>Number of vehicles:</p>
//           <select
//             value={selectedVehicles}
//             onChange={(event) =>
//               setSelectedVehicles(parseInt(event.target.value, 10))
//             }
//           >
//             {[...Array(25).keys()].map((i) => (
//               <option key={i + 1} value={i + 1}>
//                 {i + 1}
//               </option>
//             ))}
//           </select>
//         </div>

//         <div className="formConteiner">
//           {[...Array(selectedVehicles)].map((_, i) => (
//             <VehicleForm
//               key={i}
//               index={i}
//               formData={
//                 vehicleData[i] || {
//                   id: "",
//                   depot_index: "",
//                   capacity: "",
//                   email: "",
//                 }
//               }
//               handleChange={handleChange}
//               depotIndexes={getDepotIndexes()}
//               notify={notify}
//               highlightMarker={highlightMarker}
//             />
//           ))}
//         </div>

//         <button className="btn btnWidth" type="submit" onClick={handleSubmit}>
//           Save Vehicles
//         </button>
//       </div>
//     </div>
//   );
// }

// export default AddVehicles;
