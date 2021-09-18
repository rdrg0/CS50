SELECT name
FROM people 
JOIN stars
ON people.id = stars.person_id
JOIN movies
ON stars.movie_id = movies.id
WHERE year = 2004
GROUP by people.id
ORDER by birth;

