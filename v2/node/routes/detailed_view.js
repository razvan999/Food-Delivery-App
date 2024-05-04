const express = require("express");
const router = express.Router();
const fs = require("fs");
const path = require("path");
const jsonRouter = require("./get_data");

const WebSocket = require("ws");
const wss = new WebSocket.Server({ noServer: true });

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

router.delete("/delete-file/:filename", (req, res) => {
  const fileName = req.params.filename;
  const filePath = path.join(
    __dirname,
    "../json/output_for_cpp",
    fileName + ".json"
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

// un fel de notificare...
wss.on("connection", function connection(ws) {
  console.log("Client connected");

  ws.on("message", function incoming(message) {
    const string = message.toString();
    const parts = string.split("_");
    const header = parts[0] + "_" + parts[1];

    if (header == "run_algorithm") {
      console.log("RUN THE ALGORITHM");

      // run cpp
      runCpp();

      const fileName = parts[parts.length - 1];
      const filePath = path.join(
        __dirname,
        "../json/output_for_cpp",
        fileName + ".json"
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











// function runCpp() {
//   console.log("Running C++ program...");
//   console.log("Current working directory:", process.cwd());

//   const { exec } = require("child_process");
//   const fullPath = path.join(__dirname, "..", "cpp", "a.out");
//   console.log("Full path:", fullPath);
//   exec(fullPath, (error, stdout, stderr) => {
//     if (error) {
//       console.error(`exec error: ${error}`);
//       return;
//     }
//     console.log(`stdout: ${stdout}`);
//     console.error(`stderr: ${stderr}`);
//   });
// }

function runCpp() {
  console.log("Running C++ program...");
  
  // Get the full path to the C++ program
  const cppFilePath = path.join(__dirname, "..", "cpp", "main.cpp");

  // Compile the C++ program
  const { exec } = require("child_process");
  exec(`g++ ${cppFilePath} -o ${__dirname}/../cpp/a.out`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Compilation error: ${error}`);
      return;
    }

    // Run the compiled executable
    exec(`${__dirname}/../cpp/a.out`, (error, stdout, stderr) => {
      if (error) {
        console.error(`Execution error: ${error}`);
        return;
      }
      console.log(`stdout: ${stdout}`);
      console.error(`stderr: ${stderr}`);
    });
  });
}






router.use("/getdata", jsonRouter);

module.exports = { router, wss };
