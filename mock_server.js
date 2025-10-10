const express = require('express');
const app = express();

// Generate 30 artists with random albums
const artists = [];
const albumsByArtist = {};
const artistNames = [
  "The Electric Suns", "Neon Skyline", "Velvet Echoes", "Crimson Tide", "Midnight Mirage",
  "Golden Avenue", "Silver Lining", "Blue Horizon", "Urban Nomads", "Wildflower Drive",
  "Echo Chamber", "Starlight Parade", "Paper Planes", "Gravity Wells", "Silent Theory",
  "Dreamcatcher", "Northern Lights", "Retrograde", "Parallel Lines", "Fever Dream",
  "The Wanderers", "Shadowplay", "Sunset Boulevard", "Crystal Lake", "The Outsiders",
  "Firefly", "Moonstone", "Atlas", "The Roamers", "Nova Sound"
];
const albumWords = [
  "Reflections", "Odyssey", "Pulse", "Fragments", "Voyage", "Spectrum", "Origins", "Mirage",
  "Horizons", "Echoes", "Gravity", "Shadows", "Lights", "Dreams", "Waves", "Stories",
  "Blueprints", "Signals", "Canvas", "Motion", "Chapters", "Frequencies", "Elements", "Fields"
];

function getRandomAlbumName() {
  const word1 = albumWords[Math.floor(Math.random() * albumWords.length)];
  const word2 = albumWords[Math.floor(Math.random() * albumWords.length)];
  return `${word1} of ${word2}`;
}

for (let i = 1; i <= 30; i++) {
  artists.push({ id: i, name: artistNames[i-1] });
  const albumCount = Math.floor(Math.random() * 8) + 1;
  albumsByArtist[i] = [];
  for (let j = 1; j <= albumCount; j++) {
    albumsByArtist[i].push({ id: i * 100 + j, name: getRandomAlbumName() });
  }
}

app.get('/artists', (req, res) => {
  res.json(artists);
});

app.get('/artists/:id/albums', (req, res) => {
  const id = req.params.id;
  const albums = albumsByArtist[id] || [];
  res.json(albums);
});

app.listen(5000, () => console.log('Mock server running on port 5000'));
