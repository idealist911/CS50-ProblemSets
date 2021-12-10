-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find out the number of suspects
SELECT COUNT(id) FROM people;
-- There are 200 suspects

-- Find out what the crime_scene_reports table is like
SELECT * FROM crime_scene_reports LIMIT 5;
-- It shows the date and location of each criminal event and number of witnesses

-- Find out the description of the CS50 duck theft
SELECT * FROM crime_scene_reports WHERE
    street = 'Chamberlin Street' AND year >= 2020 AND month >= 7 AND day >= 28;
-- There are three witnesses. They mentioned the courthouse. Interviews were conducted.
-- Crime id: 295, crime committed at 10:15 am.

-- Find out what the interviews table is like
SELECT * FROM interviews LIMIT 5;
-- Rather amusingly, some of the transcripts contain extracts from Sherlock Holmes.

-- Find out interviews after the event
SELECT * FROM interviews WHERE
    year >= 2020 AND month >= 7  AND day >= 28;
-- Witnesses: Ruth, Eugene, Raymond
-- All recorded on the day of event
-- Ruth (161): Ten minutes after theft, thief got away by car from courthouse parking lot
-- Eugene (162): Eugene recognized thief though don't known his name. Thief withdrew money from ATM on Fifer Street.
-- Raymond (163): Thief made a call, on call for less than 1 min. Thief planned to take earliest flight the next day. Thief asked callee to purchase ticket.

-- Find courthouse log on the day of event
SELECT * FROM courthouse_security_logs WHERE
    year = 2020 AND month = 7 AND day = 28;
-- There are 74 entries from 8 am to 5:47 pm, 37 distinct license plates

-- Find license plate where time out is after theft
SELECT license_plate, minute FROM courthouse_security_logs WHERE
    activity = 'exit' AND year = 2020 AND month = 7
    AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 15 + 10;

-- Time in and Time out of suspects
SELECT A.hour AS hour1, A.minute AS minute1,
    B.hour AS hour2, B.minute AS minute2,
    ((B.hour - A.hour)*60 + (B.minute - A.minute)) duration,
    A.license_plate
    FROM courthouse_security_logs A, courthouse_security_logs B
    WHERE B.hour = 10 AND B.minute >= 15
    AND A.activity = 'entrance' AND B.activity = 'exit'
    AND A.year = B.year AND A.month = B.month AND A.day = B.day
    AND A.license_plate = B.license_plate
    AND A.year = 2020 AND A.month = 7 AND A.day = 28
    ORDER BY (hour2*60 + minute2);

-- Find who the cars belong to
SELECT name, minute, people.license_plate FROM people
    JOIN courthouse_security_logs
    ON people.license_plate = courthouse_security_logs.license_plate
    WHERE activity = 'exit'
    AND year = 2020 AND month = 7 AND day = 28
    AND hour = 10 AND minute >= 15 AND minute <= 15 + 10;
-- Suspects:
--      Ernest
--      Amber
--      Danielle
--      Roger
--      Elizabeth
--      Russell
--      Evelyn
--      Madison

-- Find out their phone number and passport number
SELECT name, phone_number, passport_number FROM people WHERE name IN
    (SELECT name FROM people
        JOIN courthouse_security_logs
        ON people.license_plate = courthouse_security_logs.license_plate
        WHERE activity = 'exit'
        AND year = 2020 AND month = 7 AND day = 28
        AND hour = 10 AND minute >= 15 AND minute <= 15 + 10);

-- Look at phone_calls table
SELECT * FROM phone_calls WHERE
    year = 2020 AND month = 7 AND day = 28;
-- It seems like duration is denominated in seconds (not minutes)

-- Find out who called who (from list of suspects)
-- ... on theft day with duration less than a min
SELECT A.name AS caller_name, B.name AS receiver_name, duration
    FROM people A
    JOIN phone_calls
    ON phone_calls.caller = A.phone_number
    JOIN people B
    ON phone_calls.receiver = B.phone_number
    WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60
    AND caller_name IN
    (SELECT name FROM people
        JOIN courthouse_security_logs
        ON people.license_plate = courthouse_security_logs.license_plate
        WHERE activity = 'exit'
        AND year = 2020 AND month = 7 AND day = 28
        AND hour = 10 AND minute >= 15 AND minute <= 15 + 10);
-- Roger called Jack
-- Evelyn called Larry
-- Ernest called Berthold
-- Evelyn called Melissa
-- Russell called Philip

-- Look at atm table
SELECT * FROM atm_transactions WHERE
    year = 2020 AND month = 7 AND day = 28 LIMIT 5;

-- Find out transactions from Fifer Street
SELECT name, amount FROM people
    JOIN bank_accounts
    ON people.id = bank_accounts.person_id
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE transaction_type = 'withdraw'
    AND atm_location = 'Fifer Street'
    AND year = 2020 AND month = 7 AND day = 28
    AND
    name IN
    (SELECT A.name AS caller_name
        FROM people A
        JOIN phone_calls
        ON phone_calls.caller = A.phone_number
        JOIN people B
        ON phone_calls.receiver = B.phone_number
        WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60
        AND caller_name IN
        (SELECT name FROM people
            JOIN courthouse_security_logs
            ON people.license_plate = courthouse_security_logs.license_plate
            WHERE activity = 'exit'
            AND year = 2020 AND month = 7 AND day = 28
            AND hour = 10 AND minute >= 15 AND minute <= 15 + 10
        )
    );
-- Ernest withdrew 50
-- Russell withdrew 35

-- Find out destination city
SELECT name, city, hour, minute FROM airports
    JOIN flights
    ON airports.id = flights.destination_airport_id
    JOIN passengers
    ON flights.id = passengers.flight_id
    JOIN people
    ON passengers.passport_number = people.passport_number
    WHERE
    year = 2020 AND month = 7 AND day = 29
    AND name IN
    (SELECT name FROM people
        JOIN bank_accounts
        ON people.id = bank_accounts.person_id
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
        WHERE transaction_type = 'withdraw'
        AND atm_location = 'Fifer Street'
        AND year = 2020 AND month = 7 AND day = 28
        AND
        name IN
        (SELECT A.name AS caller_name
            FROM people A
            JOIN phone_calls
            ON phone_calls.caller = A.phone_number
            JOIN people B
            ON phone_calls.receiver = B.phone_number
            WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60
            AND caller_name IN
            (SELECT name FROM people
                JOIN courthouse_security_logs
                ON people.license_plate = courthouse_security_logs.license_plate
                WHERE activity = 'exit'
                AND year = 2020 AND month = 7 AND day = 28
                AND hour = 10 AND minute >= 15 AND minute <= 15 + 10
            )
        )
    );
-- Russell to Boston 16:00
-- Ernest to London 08:20


-- ALTERNATIVE: Find out suspect by INTERSECTing the various tables
-- From courthouse log
SELECT name FROM people
    JOIN courthouse_security_logs
    ON people.license_plate = courthouse_security_logs.license_plate
    WHERE activity = 'exit'
    AND year = 2020 AND month = 7 AND day = 28
    AND hour = 10 AND minute >= 15 AND minute <= 15 + 10

INTERSECT

-- Phone call
SELECT name FROM people
    JOIN phone_calls
    ON phone_calls.caller = phone_number
    WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60

INTERSECT

-- ATM
SELECT name FROM people
    JOIN bank_accounts
    ON people.id = bank_accounts.person_id
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE transaction_type = 'withdraw'
    AND atm_location = 'Fifer Street'
    AND year = 2020 AND month = 7 AND day = 28

INTERSECT

SELECT name FROM airports
    JOIN flights
    ON airports.id = flights.destination_airport_id
    JOIN passengers
    ON flights.id = passengers.flight_id
    JOIN people
    ON passengers.passport_number = people.passport_number
    WHERE
    year = 2020 AND month = 7 AND day = 29
    AND hour = 8;
-- Suspect: Ernest

-- GET ANSWER:
SELECT suspect.name, city.city, accomplice.accomplice
-- Find out suspect
FROM
    (

        -- From courthouse log
        SELECT name FROM people
        JOIN courthouse_security_logs
        ON people.license_plate = courthouse_security_logs.license_plate
        WHERE activity = 'exit'
        AND year = 2020 AND month = 7 AND day = 28
        AND hour = 10 AND minute >= 15 AND minute <= 15 + 10

        INTERSECT

        -- Phone call
        SELECT name FROM people
        JOIN phone_calls
        ON phone_calls.caller = phone_number
        WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60

        INTERSECT

        -- ATM
        SELECT name FROM people
        JOIN bank_accounts
        ON people.id = bank_accounts.person_id
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
        WHERE transaction_type = 'withdraw'
        AND atm_location = 'Fifer Street'
        AND year = 2020 AND month = 7 AND day = 28

        INTERSECT

        SELECT name FROM airports
        JOIN flights
        ON airports.id = flights.destination_airport_id
        JOIN passengers
        ON flights.id = passengers.flight_id
        JOIN people
        ON passengers.passport_number = people.passport_number
        WHERE
        year = 2020 AND month = 7 AND day = 29
        AND hour = 8

    ) suspect

LEFT JOIN

        -- Find out destination city
        (
            SELECT city, name FROM airports
            JOIN flights
            ON airports.id = flights.destination_airport_id
            JOIN passengers
            ON flights.id = passengers.flight_id
            JOIN people
            ON passengers.passport_number = people.passport_number
            WHERE
            year = 2020 AND month = 7 AND day = 29
        ) city

ON (suspect.name = city.name)

LEFT JOIN

    -- Find out accomplice
    (
        SELECT A.name AS suspect, B.name AS accomplice
        FROM people A
        JOIN phone_calls
        ON phone_calls.caller = A.phone_number
        JOIN people B
        ON phone_calls.receiver = B.phone_number
        WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60
    ) accomplice

ON (suspect.name = accomplice.suspect);

