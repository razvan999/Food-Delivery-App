const express = require('express');
const app = express();
const path = require('path');
const fs = require('fs');

app.get('/', async (req, res) => {
  res.sendFile(path.join(__dirname, './html/view_ngraph.html'));
});

app.get('/get_graph', (req, res) => {
  fs.readFile('../data/graph_output.json', 'utf8', (err, data) => {
    if (err) {
      console.error('Error reading the JSON file:', err);
      res.status(500).send('Internal Server Error');
      return;
    }
    
    res.json(JSON.parse(data));
  });
});

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});