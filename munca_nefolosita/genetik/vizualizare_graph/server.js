const express = require('express')
const app = express()
const path = require('path')
const port = 3000

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, './html/test.html'))
})

app.get('/graph', (req, res) => {
    res.sendFile(path.join(__dirname, './html/graph.html'))
})

app.get('/graph.json', (req, res) => {
    res.sendFile(path.join(__dirname, '../graph.json'))
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
})