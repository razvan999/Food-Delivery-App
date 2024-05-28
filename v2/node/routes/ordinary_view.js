const express = require("express");
const router = express.Router();
const fs = require("fs");
const path = require("path");
const jsonRouter = require("./get_data");
const bodyParser = require('body-parser');

const WebSocket = require("ws");
const ws_maps = new WebSocket.Server({ noServer: true });
const { exec } = require("child_process");







router.use(bodyParser.json());

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

router.delete("/delete-json-maps", (req, res) => {
  const filePath = path.join(
    __dirname,
    "../json/maps_cpp/input_maps_cpp.json"
  );

  if (fs.existsSync(filePath)) {
    fs.unlink(filePath, (err) => {
      if (err) {
        res.status(500).send("An error occurred while deleting the file" + err);
      } else {
        res.send("File deleted successfully");
      }
    });
  }
});

// primesc data si salvez in fisier
router.post('/sendData', (req, res) => {
  const receivedData = req.body;
  // console.log('Received data from client:', receivedData);

const jsonString = JSON.stringify(receivedData, null, 2);
  fs.writeFile('./json/maps_cpp/input_maps_cpp.json', jsonString, 'utf8', (err) => {
    if (err) {
      console.error('Error writing JSON to file:', err);
      return;
    }
    console.log('JSON data has been saved to input_maps_cpp.json');
  });

  res.status(200).send('Data received successfully');
});

// un fel de notificare...
ws_maps.on("connection", function connection(ws) {
  console.log("Client connected");

  ws.on("message", function incoming(message) {
    const string = message.toString();
    console.log("Received message from client: " + string);

    if (string == "run_algorithm") {
      console.log("RUN THE ALGORITHM for maps");

      runCpp((success) => {
        if (success) {
          const filePath = path.join(
            __dirname,
            "../json/maps_cpp/input_maps_cpp.json"
          );
  
          const interval = setInterval(async () => {
            const fileExists = await checkFileExists(filePath, interval);
            if (fileExists) {
              console.log("File created!");
              ws.send("you can fetch data");
            }
          }, 1000);
        }
      });
    }
  });

  ws.on("error", function (error) {
    console.error("WebSocket error:", error);
  });

  ws.on("close", function close() {
    console.log("Client disconnected");
  });
});

function checkFileExists(filePath, interval) {
  return new Promise((resolve, reject) => {
    fs.access(filePath, fs.constants.F_OK, (err) => {
      if (!err) {
        clearInterval(interval);
        resolve(true);
      } else {
        console.log("File does not exist. Waiting for it to appear...");
        resolve(false);
      }
    });
  });
}


function runCpp(callback) {
  console.log("Running C++ program...");

  const cppFilePath = path.join(__dirname, "..", "cpp", "main_maps.cpp");
  exec(`g++ ${cppFilePath} -o ${__dirname}/../cpp/a.out`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Compilation error: ${error}`);
      callback(false);
      return;
    }

    exec(`${__dirname}/../cpp/a.out`, (error, stdout, stderr) => {
      if (error) {
        console.error(`Execution error: ${error}`);
        callback(false);
        return;
      }
      console.log(`stdout: ${stdout}`);
      console.error(`stderr: ${stderr}`);

      if (!stderr) {
        console.log("C++ program executed successfully");
        callback(true);
      }
    });
  });
}




























router.use("/getdata", jsonRouter);

module.exports = { router, ws_maps };