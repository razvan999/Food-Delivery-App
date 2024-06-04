import React, { useState, useEffect } from "react";
import "../css/addVehicles.css";

function VehicleForm({ index, formData, handleChange }) {
  return (
    <div className="vehicleForm">
      <form>
        <input className="inputClass"
          type="number"
          placeholder="depot index"
          name="depot_index"
          value={formData.depot_index || ""}
          onChange={(event) => handleChange(index, event)}
          id={`depotNumber${index}`}
          min="0"
        />
        <br />
        <input className="inputClass"
          type="number"
          placeholder="capacity"
          name="capacity"
          value={formData.capacity || ""}
          onChange={(event) => handleChange(index, event)}
          id={`capacity${index}`}
          min="0"
        />
        <br />
        <input className="inputClass"
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

function AddVehicles({ setVehicles}) {
  const [selectedVehicles, setSelectedVehicles] = useState(1);
  const [vehicleData, setVehicleData] = useState(
    Array(2).fill({ id: "", depot_index: "", capacity: "", email: "" }).map((data, index) => ({ ...data, id: index }))
  );

  useEffect(() => {
    const newData = Array(selectedVehicles).fill({ id: "", depot_index: "", capacity: "", email: "" }).map((data, index) => ({ ...data, id: index }));
    setVehicleData(newData);
  }, [selectedVehicles]);

  const handleChange = (index, event) => {
    const { name, value } = event.target;
    setVehicleData((prevData) =>
      prevData.map((data, i) =>
        i === index ? { ...data, [name]: value } : data
      )
    );
  };

  const handleSubmit = (event) => {
    event.preventDefault();
    const selectedData = vehicleData.slice(0, selectedVehicles);

    const isUnique = selectedData.every((vehicle, index, self) => 
      index === self.findIndex((v) => (
        v.email === vehicle.email
      ))
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
          <select value={selectedVehicles} onChange={(event) => setSelectedVehicles(parseInt(event.target.value, 10))}>
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
              formData={vehicleData[i] || { id: "", depot_index: "", capacity: "", email: "" }}
              handleChange={handleChange}
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
