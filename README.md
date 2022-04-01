
# Airport-Manager

Mid term project for Introduction to Algorithms and Data Structures

## Introduction

This project consists in a flight management system between airports. For this, the system must allow the definition of airports and flights, and also consulting them.

The interaction with the program occurs through a set of lines composed by a letter (command) and a number of arguments that depends on the command to execute. The commands are listed in the following table.

|Command|Action|
|--|--|
|q|Terminates the program|
|a|Adds a new airport to the system|
|l|Lists the airports|
|v|Adds a flight or lists all flights
|p|Lists all airports with departure from an airport|
|c|Lists all airports with an arrival at an airport|
|t|Advances system's date

## Problem Specification

The objective is to obtain a commercial flight management system between airports. In order to simulate the flight and reservation management, time passing is controlled by the application through a command that allows the current system's date to be defined. 

### Defining Data
#### Date and Time
When starting the program, the current date is considered to be 1st January 2022. The program will not be tested with dates after 31st December 2023, so leap years won't occur.

In the system, date's input and output refers to the format DD-MM-YYYY where DD represents the day (number between 1 and last month's day), MM represents the month (number between 1 and 12) and YYYY the year (positive number).

Time always has the format HH:MM where HH represents the hour (number between 0 and 23) and MM represents the minutes (number between 0 and 59).
It is not necessary to validate if the days, minutes, years, etc. meet the required limits.

When the value is lower than 10, then the tens character is 0. For example, the 1st January 2022 is represented by 01-01-2022 and 9 hours and 5 minutes is represented by 09:05. Any time periods (like flight durations) follow hours's format.

#### Airports
Each airport is characterized by an identifier composed by 3 uppercase letters, a country (non-empty string with 30 maximum characters, with only upper or lower case letters) and a city (non-empty string with 50 maximum characters where white spaces may occur).

The system allows a maximum of 40 airports.

#### Flights
Each flight is characterized by a flight code, a departure and an arrival airports, departure date and time, flight duration and maximum passenger count.

Flight code is a string composed by 2 uppercase letters followed by a number between 1 and 9999 that never starts with 0. Multiple flights with the same code may exist, as long two flights with the same code don't have the same departure date. The maximum passenger count is a number between 10 and 100 and a flight duration can't exceed 12 hours.

The system allows a maximum of 30000 airports.

### Entry Data
The program reads input from the terminal.

During execution, instructions must be read from the terminal (standard input) as a set of lines initiated by a character (command), followed by a number of information depending on the command to execute. Commands and information are separated by at least a blank character.

The available commands are listed next, where `< >` is only used when describing the commands to indicate the arguments. Optional arguments are shown with `[ ]`. `...` represents a possible repetition of the arguments.

If a command generates more than one error, only the first one is shown.

 - **q** - terminates the program
	 - Input: `q`
	 - Output: 
- **a** - adds a new airport to the system
	- Input: `a <airportID> <country> <city>`
	- Output: `airport <airportID>` where `<airportID>` is the created airport's identifier
	- Errors
		- `invalid airport ID` if `airportID` is not a string with only uppercase letters
		- `too many airports` if the new airport, if created, exceeds the maximum number of airports allowed by the system
		- `duplicate airport` if an airport with `airportID` as it's identifier already exists
- **l** - lists the airports
	- Input: `l [ <airportID> <airportID> ... ]`
	- Output: `<airportID> <city> <country> #flights for each airport, where #flights is the amount of flights with departure from the airport`
		- If the command is executed with no arguments, all airports are listed alphabetically from their ID
		- If the command is executed with an ID list, airports are listed in order of their identifiers
	- Errors
		- `<airportID>: no such airport ID` if no airports exist with `<airportID>` as it's identifier
	
