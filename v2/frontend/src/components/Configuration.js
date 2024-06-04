import React, { useEffect, useState } from "react";
import "../css/configuration.css";

function Configuration({ setConfiguration }) {
  const [selection, setSelection] = useState("");
  const [crossover, setCrossover] = useState("");
  const [populationSize, setPopulationSize] = useState("");
  const [generationLimit, setGenerationLimit] = useState(""); 
  let configuration = {};

  const handleSave = () => {
    const configuration = {
      selection: selection.toLowerCase().replace(/ /g, "_"),
      crossover: crossover.toLowerCase(),
      population_size: populationSize.toLowerCase(),
      generation_limit: generationLimit.toLowerCase(),
    };
    setConfiguration(configuration);
    console.log("configuration:", configuration);
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
          <option value="Rank Selection">Rank Selection</option>
          <option value="Tournament Selection">Tournament Selection</option>
          <option value="Wheel Selection">Wheel Selection</option>
        </select>

        <select
          value={crossover}
          onChange={(e) => setCrossover(e.target.value)}
        >
          <option value="">Crossover</option>
          <option value="Onepointcrossover">Onepointcrossover</option>
          <option value="Twopointcrossover">Twopointcrossover</option>
          <option value="Cyclecrossover">Cyclecrossover</option>
        </select>

        <select
          value={populationSize}
          onChange={(e) => setPopulationSize(e.target.value)}
        >
          <option value="">Population size</option>
          <option value="Dynamic">Dynamic</option>
          <option value="Small">Small</option>
          <option value="Large">Large</option>
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