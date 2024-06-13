import React, { useState } from "react";
import "../css/configuration.css";

function Configuration({ setConfiguration }) {
  const [selection, setSelection] = useState("");
  const [crossover, setCrossover] = useState("");
  const [populationSize, setPopulationSize] = useState("");
  const [generationLimit, setGenerationLimit] = useState("");

  const handleSave = () => {
    const updatedConfiguration = {
      selection: "tournament_selection",
      crossover: "twopointcrossover",
      population_size: "dynamic",
      generation_limit: "20",
    };
    if (selection != "") {
      updatedConfiguration.selection = selection
        .toLowerCase()
        .replace(/ /g, "_");
    }
    if (crossover != "") {
      updatedConfiguration.crossover = crossover.toLowerCase();
    }
    if (populationSize != "") {
      updatedConfiguration.population_size = populationSize.toLowerCase();
    }
    if (generationLimit != "") {
      updatedConfiguration.generation_limit = generationLimit.toLowerCase();
    }

    setConfiguration(updatedConfiguration);
    console.log("Updated configuration:", updatedConfiguration);
  };

  return (
    <div>
      <h4 className="titleClass">Configuration</h4>

      <div className="configurationSelectionContainer">
        <select
          value={selection}
          onChange={(e) => setSelection(e.target.value)}
        >
          <option value="">Selection</option>
          <option value="tournament_selection">Tournament Selection</option>
          <option value="rank_selection">Rank Selection</option>
          <option value="wheel_selection">Wheel Selection</option>
        </select>

        <select
          value={crossover}
          onChange={(e) => setCrossover(e.target.value)}
        >
          <option value="">Crossover</option>
          <option value="twopointcrossover">Twopointcrossover</option>
          <option value="onepointcrossover">Onepointcrossover</option>
          <option value="cyclecrossover">Cyclecrossover</option>
        </select>

        <select
          value={populationSize}
          onChange={(e) => setPopulationSize(e.target.value)}
        >
          <option value="">Population size</option>
          <option value="dynamic">Dynamic</option>
          <option value="small">Small</option>
          <option value="large">Large</option>
        </select>

        <select
          value={generationLimit}
          onChange={(e) => setGenerationLimit(e.target.value)}
        >
          <option value="">Generation limit</option>
          <option value="20">20</option>
          <option value="60">60</option>
          <option value="100">100</option>
        </select>
      </div>

      <button className="btn configBtn" type="button" onClick={handleSave}>
        Save Configuration
      </button>
    </div>
  );
}

export default Configuration;
