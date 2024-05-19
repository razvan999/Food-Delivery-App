const express = require("express");
const router = express.Router();
const fs = require("fs");
const path = require("path");

router.get("/", (req, res) => {
  fs.readFile(
    path.join(__dirname, "../html/index.html"),
    "utf8",
    (err, data) => {
      if (err) {
        res.status(500).send("An error occurred while reading the file");
      } else {
        res.send(data);
      }
    }
  );
});

module.exports = router;