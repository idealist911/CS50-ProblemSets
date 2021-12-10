SELECT name from (SELECT * FROM (SELECT * FROM songs WHERE danceability > 0.75)
    WHERE energy > 0.75) WHERE valence > 0.75;