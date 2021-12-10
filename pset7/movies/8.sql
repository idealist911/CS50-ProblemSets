-- SELECT name FROM people WHERE id IN
--     (SELECT person_id FROM stars WHERE movie_id IN
--     (SELECT id FROM movies WHERE title = 'Toy Story'));

SELECT name FROM people
    JOIN stars ON stars.person_id = people.id
    JOIN movies ON stars.movie_id = movies.id
    WHERE title = 'Toy Story';