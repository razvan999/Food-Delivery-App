const express = require("express");
const router = express.Router();
const fs = require('fs');
const path = require("path");

router.get("/:type/:filename", (req, res) => { // type is either "input" or "output"
    var fileName = req.params.filename + ".json";
    var jsonDirectoryPath = "../json/";
    
    if (req.params.type === "input") {
        jsonDirectoryPath += "input_for_cpp";
    }
    else if (req.params.type === "output") {
        jsonDirectoryPath += "output_for_cpp";
    }
    else if (req.params.type === "maps") {
        jsonDirectoryPath += "maps_cpp";
    }

    fs.readFile(path.join(__dirname, jsonDirectoryPath, fileName), "utf8", (err, jsonData) => {
        if (err) {
            res.status(500).send("An error occurred while reading the JSON file");
        } else {
            res.json(JSON.parse(jsonData));

            console.log("Data sent to client");
            console.log(JSON.parse(jsonData));
        }
    });
});

module.exports = router;