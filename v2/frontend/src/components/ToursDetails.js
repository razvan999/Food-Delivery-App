import React from "react";
import "../css/toursDetails.css";

function Tour({ tour, data }) {
    const vehicle = data.vehicles[parseInt(tour.vehicle)];
    const locations = [];

    tour.route.forEach((location) => {
        locations.push(data.locations[parseInt(location)]);
    });

  return <div className="justAToour">
        {/* <p>Courier email: {vehicle.email}</p> */}

        <div className="det"><p className="first">Courier email:</p> <p className="second">razvanciobanu@gmail.com</p></div>
        {/* <p>Courier email: razvanciobanu@gmail.com</p> */}
        <div className="det"><p className="first">Route const:</p> <p className="second">{tour.cost}</p></div>
        <button  className="btn btnWidth" type="submit">Highlight tour</button>
  </div>;
}

function ToursDetails({receivedData, data}) {
  return (
    <div>
      <h4 className="titleClass">ToursDetails</h4>
    <div style={{margin: "20px 0px 20px 0px"}}>
      <div className="generaDetails">
        <p>Cost Total: {receivedData.final_cost}</p>
        <p>Timp de rulare: {receivedData.time}</p>
      </div>

      <div className="toursContainer">
        {receivedData.tours.map((tour, index) => (
          <Tour key={index} tour={tour} data={data} />
        ))}
      </div>
      </div>
    </div>
  );
}

export default ToursDetails;
