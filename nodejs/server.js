const fs = require('fs');
var json_file = "../data/graph_output.json";

const express = require('express');
const app = express();

app.get('/', async (req, res) => {
    try {
      const jsonData = await get_json('your_json_file.json');

      res.status(200).json(jsonData);
    } catch (error) {
      console.error('Error:', error);
      res.status(500).send('Internal Server Error');
    }
  });

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});

function get_json() {
    return new Promise((resolve, reject) => {
      fs.readFile(json_file, 'utf8', (err, data) => {
        if (err) {
          reject('Error reading JSON file: ' + err);
          return;
        }
        
        try {
          const jsonData = JSON.parse(data);
          resolve(jsonData);
        } catch (error) {
          reject('Error parsing JSON data: ' + error);
        }
      });
    });
  }