-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.


-- Query purpose: Creme scene reports on the day which accident happend:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.
select * from crime_scene_reports where day = 28 and month = 7 and year = 2020 and street = "Chamberlin Street";
-- | description
-- | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.



select * from interviews where day = 28 and month = 7;
-- id  | name    | year | month | day | transcript
-- 161 | Ruth    | 2020 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 162 | Eugene  | 2020 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- 163 | Raymond | 2020 | 7     | 28  | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


select * from phone_calls where day = 28 and month = 7 and duration < 60;
-- id  | caller         | receiver       | year | month | day | duration
-- 221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7     | 28  | 51
-- 224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7     | 28  | 36
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7     | 28  | 45
-- 251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7     | 28  | 50
-- 254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7     | 28  | 43
-- 255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7     | 28  | 49
-- 261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7     | 28  | 38
-- 279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7     | 28  | 55
-- 281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7     | 28  | 54


select * from atm_transactions where atm_location = "Fifer Street" and day = 28 and month = 7 and year = 2020 and transaction_type = "withdraw";


-- Query purpose: Finding activity in courthouse security logs for the time 10:15am
select * from courthouse_security_logs where hour = 10 and minute between 15 and 25 and activity = "exit";
-- id  | year | month | day | hour | minute | activity | license_plate
-- 459 | 2020 | 7     | 31  | 10   | 15     | exit     | 11J91FW
-- 460 | 2020 | 7     | 31  | 10   | 16     | exit     | PF37ZVK
-- 461 | 2020 | 7     | 31  | 10   | 20     | exit     | 1M92998
-- 462 | 2020 | 7     | 31  | 10   | 21     | exit     | XE95071
-- 463 | 2020 | 7     | 31  | 10   | 24     | exit     | IH61GO8
-- 464 | 2020 | 7     | 31  | 10   | 25     | exit     | 8P9NEU9
-- 260 | 2020 | 7     | 28  | 10   | 16     | exit     | 5P2BI95
-- 261 | 2020 | 7     | 28  | 10   | 18     | exit     | 94KL13X
-- 262 | 2020 | 7     | 28  | 10   | 18     | exit     | 6P58WS2
-- 263 | 2020 | 7     | 28  | 10   | 19     | exit     | 4328GD8
-- 264 | 2020 | 7     | 28  | 10   | 20     | exit     | G412CB7
-- 265 | 2020 | 7     | 28  | 10   | 21     | exit     | L93JTIZ
-- 266 | 2020 | 7     | 28  | 10   | 23     | exit     | 322W7JE
-- 267 | 2020 | 7     | 28  | 10   | 23     | exit     | 0NTHK55



select * from airports where full_name like "%Fiftyville%";
-- id | abbreviation | full_name                   | city
-- 8  | CSF          | Fiftyville Regional Airport | Fiftyville


select * from flights where origin_airport_id = 8 and day > 28 and day < 30 and month > 6 order by day, hour, minute;
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 36 | 8                 | 4                      | 2020 | 7     | 29  | 8    | 20
-- 43 | 8                 | 1                      | 2020 | 7     | 29  | 9    | 30
-- 23 | 8                 | 11                     | 2020 | 7     | 29  | 12   | 15
-- 53 | 8                 | 9                      | 2020 | 7     | 29  | 15   | 20
-- 18 | 8                 | 6                      | 2020 | 7     | 29  | 16   | 0



select * from passengers where flight_id = 36;
-- flight_id | passport_number | seat
-- 36        | 7214083635      | 2A
-- 36        | 1695452385      | 3B
-- 36        | 5773159633      | 4A
-- 36        | 1540955065      | 5C
-- 36        | 8294398571      | 6C
-- 36        | 1988161715      | 6D
-- 36        | 9878712108      | 7A
-- 36        | 8496433585      | 7B



select * from people where passport_number IN
    (select passport_number from passengers where flight_id = 36) INTERSECT
select * from people where license_plate IN
    (select license_plate from courthouse_security_logs where hour = 10 and minute between 15 and 25 and activity = "exit"
) INTERSECT
select * from people where phone_number IN
    (select caller from phone_calls where day = 28 and month = 7 and duration < 60);

-- id     | name     | phone_number   | passport_number | license_plate
-- 398010 | Roger    | (130) 555-0289 | 1695452385      | G412CB7
-- 560886 | Evelyn   | (499) 555-9472 | 8294398571      | 0NTHK55
-- 686048 | Ernest   | (367) 555-5533 | 5773159633      | 94KL13X




select account_number from bank_accounts where person_id IN
    (select id from people where passport_number IN
        (select passport_number from passengers where flight_id = 36) INTERSECT
    select id from people where license_plate IN
        (select license_plate from courthouse_security_logs where hour = 10 and minute between 15 and 25 and activity = "exit"
    ) INTERSECT
    select id from people where phone_number IN
        (select caller from phone_calls where day = 28 and month = 7 and duration < 60));

-- account_number
-- 49610011



select * from people where id IN
    (select person_id from bank_accounts where account_number = 49610011);
-- id     | name   | phone_number   | passport_number | license_plate
-- 686048 | Ernest | (367) 555-5533 | 5773159633      | 94KL13X


select * from people where phone_number = "(375) 555-8161";
-- id     | name     | phone_number   | passport_number | license_plate
-- 864400 | Berthold | (375) 555-8161 |                 | 4V16VO0