const express = require("express");
const path = require("path");
const app = express();
const port = 3000;

const detailedViewRouter = require("./routes/detailed_view");
const ordinaryViewRouter = require("./routes/ordinary_view");

app.use("/", ordinaryViewRouter);
app.use("/details", detailedViewRouter.router);
app.use("/javascript", express.static(path.join(__dirname, "./javascript")));
app.use("/css", express.static(path.join(__dirname, "./css")));

const server = app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});

server.on("upgrade", (request, socket, head) => {
  detailedViewRouter.wss.handleUpgrade(request, socket, head, (ws) => {
    detailedViewRouter.wss.emit("connection", ws);
  });
});
