// const express = require("express");
// const path = require("path");
// const app = express();
// const port = 3000;

// const http = require('http');
// const httpServer = http.createServer((req, res) => {
//   res.writeHead(200, { 'Content-Type': 'text/plain' });
//   res.end('Hello World!');
// });

// const detailedViewRouter = require("./routes/detailed_view");
// const ordinaryViewRouter = require("./routes/ordinary_view");

// app.use("/", ordinaryViewRouter.router);
// app.use("/details", detailedViewRouter.router);
// app.use("/javascript", express.static(path.join(__dirname, "./javascript")));
// app.use("/css", express.static(path.join(__dirname, "./css")));

// const server = app.listen(port, () => {
//   console.log(`Server is listening on port ${port}`);
// });

// // server.on("upgrade", (request, socket, head) => {
// //   detailedViewRouter.wss.handleUpgrade(request, socket, head, (ws) => {
// //     detailedViewRouter.wss.emit("connection", ws);
// //   });
// // });





// // Handle HTTP upgrade requests for WebSocket server at "/"
// httpServer.on('upgrade', (request, socket, head) => {
//   if (request.url === '/') {
//     ordinaryViewRouter.ws_maps.handleUpgrade(request, socket, head, (ws) => {
//       ordinaryViewRouter.ws_maps.emit('connection', ws, request);
//     });
//   } else {
//     socket.destroy(); // Close socket if not upgrading to WebSocket
//   }
// });

// // Handle HTTP upgrade requests for WebSocket server at "/details"
// httpServer.on('upgrade', (request, socket, head) => {
//   if (request.url === '/details') {
//     detailedViewRouter.wss.handleUpgrade(request, socket, head, (ws) => {
//       detailedViewRouter.wss.emit('connection', ws, request);
//     });
//   } else {
//     socket.destroy(); // Close socket if not upgrading to WebSocket
//   }
// });




const express = require("express");
const path = require("path");
const http = require('http');
const ordinaryViewRouter = require("./routes/ordinary_view");
const detailedViewRouter = require("./routes/detailed_view");

const app = express();
const port = 3000;
const httpServer = http.createServer(app);

const cors = require('cors');
app.use(cors());

// app.use(cors({
//   origin: 'http://localhost:5000', // Allow requests only from this origin
//   credentials: true // Allow cookies to be sent along with the request
// }));








app.use("/", ordinaryViewRouter.router);
app.use("/details", detailedViewRouter.router);
app.use("/javascript", express.static(path.join(__dirname, "./javascript")));
app.use("/css", express.static(path.join(__dirname, "./css")));

const server = httpServer.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});

httpServer.on('upgrade', (request, socket, head) => {
  if (request.url === '/') {
    ordinaryViewRouter.ws_maps.handleUpgrade(request, socket, head, (ws) => {
      ordinaryViewRouter.ws_maps.emit('connection', ws, request);
    });
  } else if (request.url === '/details') {
    detailedViewRouter.wss.handleUpgrade(request, socket, head, (ws) => {
      detailedViewRouter.wss.emit('connection', ws, request);
    });
  } else {
    socket.destroy();
  }
});
