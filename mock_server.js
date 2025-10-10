const express = require('express');
const app = express();

app.get('/artists', (req, res) => {
  res.json([
    {id: 1, name: 'Artist One'},
    {id: 2, name: 'Artist Two'},
    {id: 3, name: 'Artist Three'},
    {id: 4, name: 'Artist Four'},
    {id: 5, name: 'Artist Five'},
    {id: 6, name: 'Artist Six'},
    {id: 7, name: 'Artist Seven'},
    {id: 8, name: 'Artist Eight'},
    {id: 9, name: 'Artist Nine'},
    {id: 10, name: 'Artist Ten'},
    {id: 11, name: 'Artist Eleven'},
    {id: 12, name: 'Artist Twelve'},
    {id: 13, name: 'Artist Thirteen'},
    {id: 14, name: 'Artist Fourteen'},
    {id: 15, name: 'Artist Fifteen'},
    {id: 16, name: 'Artist Sixteen'},
    {id: 17, name: 'Artist Seventeen'},
    {id: 18, name: 'Artist Eighteen'},
    {id: 19, name: 'Artist Nineteen'},
    {id: 20, name: 'Artist Twenty'}
  ]);
});

app.listen(5000, () => console.log('Mock server running on port 5000'));
