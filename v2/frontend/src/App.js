import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Home from './pages/Home';
import Benchmark from './pages/Benchmark';

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/benchmark" element={<Benchmark />} />
      </Routes>
    </Router>
  );
}

export default App;
