import React from "react";
import "../css/actions.css";

function Actions({locations, vehicles, configuration}) {
  function checkData() {
    let existsDepotIndex = [];
    for (let i = 0; i < vehicles.length; i++) {
      existsDepotIndex.push([vehicles[i].depot_index, false]);
    }

    for (let i = 0; i < locations.length; i++) {
        let flag = true;
        for (let j = 0; j < existsDepotIndex.length; j++) {
          if (i == existsDepotIndex[j][0]) {
            existsDepotIndex[j][1] = true;
            flag = false;
            break;
          }

        if (flag) {
          alert("Please assign a depot index to each vehicle!");
          return false;

        }
        }
      }

    for (let i = 0; i < existsDepotIndex.length; i++) {
      if (!existsDepotIndex[i][1]) {
        alert("Exista depozite fara vehicule alocate!");
        return false;
      }}

      return true;
    }
  








  function calculateDistanceMatrix() {
    return [];
  }

  function createData() {
    if (!checkData()) {
      return;
    }

    let data = {
      "selection": configuration.selection,
      "crossover": configuration.crossover,
      "population_size": configuration.population_size,
      "generation_limit": configuration.generation_limit,
      locations: locations,
      vehicles: vehicles,
      matrix: calculateDistanceMatrix()
    };

    console.log(data);
    return data;
  }




  return (
    <div>
      <h4 className="titleClass">Actions</h4>
      <div className="actionsContainer">
        <button className="btn btnWidth" onClick={createData}>Find routes</button>
        <button className="btn btnWidth" >Notify couriers</button>
        <button className="btn btnWidth" >Clear data</button>
        <button className="btn btnWidth" >Banchmarks</button>
      </div>
    </div> //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! trebuie sa verific inainte sa dau submit daca fiecare index al vehiculelor are o locatie
  );      // sa mut aici calcularea matricei de distante
}

export default Actions;
