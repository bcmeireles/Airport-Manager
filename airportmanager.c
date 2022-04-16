#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSET 1

/* Maximum amount of characters an airport ID can have */
#define IDAPMAX 3
/* Maximum amount of characters a country name can have */
#define COUNTRYMAX 30
/* Maximum amount of characters a city name can have */
#define CITYMAX 50
/* Maximum amount of airports that can be created */
#define AIRPORTSMAX 40


/* Maximum amount of characters a flight ID can have */
#define FLIGHTIDMAX 6
/* Minimum passenger count a flight must have */
#define MINPASSENGERS 10
/* Maximum passenger count a flight can have */
#define MAXPASSENGERS 100
/* Maximum duration, in hours, a flight can have */
#define MAXFLIGHTDURATION 12
/* Maximum amount of flights that can be created */
#define FLIGHTSMAX 30000

/* Maximum amount of characters a date can be represented as*/
#define DATEMAX 10
/* Maximum amount of characters a time can be represented as*/
#define TIMEMAX 5

/* Default date's day */
#define STARTDAY 1
/* Default date's month*/
#define STARTMONTH 1
/* Default day's year*/
#define STARTYEAR 2022

/* Maximum amount of characters the user input can have */
#define INPUTLEN 65535

/* Airport errors */
#define INV_AP_ID_ERR "invalid airport ID"
#define CAP_AP_ERR "too many airports"
#define DUP_AP_ERR "duplicate airport"
#define NO_AP_ID_ERR "no such airport ID"
#define INV_DATE_ERR "invalid date"

/* Flight errors */
#define INV_FLIGHT_ID_ERR "invalid flight code"
#define CAP_FLIGHTS_ERR "too many flights"
#define DUP_FLIGHT_ERR "flight already exists"
#define INV_DUR_ERR "invalid duration"
#define INV_CAP_ERR "invalid capacity"
#define FLIGHT_NOT_FOUND_ERR "flight does not exist"

/* Reservation Erors */
#define INV_RES_CODE_ERR "invalid reservation code"
#define DUP_RES_CODE_ERR "flight reservation already used"
#define CAP_RES_ERR "too many reservations"
#define INV_PAS_ERR "invalid passenger number"

const int daysMonth[] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* STRUCTS */

typedef struct Date {

    int day;
    int month;
    int year;

} Date;

typedef struct Hour {

    int hour;
    int minute;

} Hour;


typedef struct Airport {

    char id[IDAPMAX + OFFSET]; 
    char country[COUNTRYMAX + OFFSET]; 
    char city[CITYMAX + OFFSET];
    int flights;

} Airport;


typedef struct Flight {

    char id[FLIGHTIDMAX + OFFSET];
    char departure[IDAPMAX + OFFSET]; 
    char arrival[IDAPMAX + OFFSET];
    Date departure_date;
    Hour departure_hour;
    Hour duration;
    int departureNum;
    int arrivalNum;
    int passengers;

} Flight;


typedef struct Reservation {
    char *reservationCode;
    int passengerCount;
    int dateNum;
    char *flightid;
    struct Reservation *next;
} Reservation;

/* 
Returns 1 if the given char is uppercase, 0 if not
*/
int check_upper(char letter) {
    if (letter >= 'A' && letter <= 'Z')
        return 1;
    else
        return 0;
}


Date currentdate = {STARTDAY, STARTMONTH, STARTYEAR};

/* 
Changes the system's date, associated to a global variable
*/
void change_date(int day, int month, int year) {
    currentdate.day = day;
    currentdate.month = month;
    currentdate.year = year;
}

/* 
Converts a date to an int, which is the number of days that passed since 01-01-2022
*/
int getDateNum(Date date) {
    return (date.year - STARTYEAR) * 365 + daysMonth[date.month - 1] + date.day - 1;
}

/* 
Converts an hour to an inn, which is the number of minutes since the start of the day
*/
int getHourNum(Hour hour) {
    return ((hour.hour * 60) + hour.minute);
}

/* 
Converts a date and an hour to an int, which is the number of minutes that passed since 01-01-2022
*/
int getDateHourNum(Date date, Hour hour) {
    return getDateNum(date) * 24 * 60 + getHourNum(hour);
}

/* 
Receiving an int that represents an hour, converts it back to an hour
*/
Hour getHour(int hourNum) {
	Hour hour;
	hour.minute = hourNum % 60;
	hour.hour = ((hourNum - hour.minute) / 60) % 24;

	return hour;
}

/* 
Receiving an int htat represents a date, comverts it back to a date
*/
Date getDate(int dateNum) {
	Date date;
	int i = 0;

	dateNum = (dateNum - (dateNum % (24 * 60))) / (24 * 60);
	date.year = (dateNum / 365) + STARTYEAR;
	dateNum = dateNum - ((date.year - STARTYEAR) * 365);
	
    while (i <= 11 && dateNum >= daysMonth[i])
		i++;
	date.month = i;
	date.day = dateNum - daysMonth[i - 1] + 1;

	return date;
}


/* 
It is not possible to change to a date more than one year further or 
to a date before
the current one.
If it is possible to change the date, the function returns 1, else returns 0
*/
int checkDate(Date date) {
    int dateNum = getDateNum(date);
    Date NextYear = currentdate;
    NextYear.year++;

    if (dateNum < getDateNum(currentdate) || dateNum > getDateNum(NextYear))
        return 1;
    
    return 0;
}

/* 
Prints the date in the format dd-mm-yyyy
*/
void printDate(Date date) {
    printf("%02d-%02d-%04d", date.day, date.month, date.year);
}

/* 
Prints the hour in the format hh:mm
*/
void printHour(Hour hour) {
    printf("%02d:%02d", hour.hour, hour.minute);
}

/*
Creates a new airport, adding it to the array airports
*/
void create_airport(Airport airports[], int airport_count, char id[], 
                    char country[], char city[]) {
    strcpy(airports[airport_count].id, id);
    strcpy(airports[airport_count].country, country);
    strcpy(airports[airport_count].city, city);
    airports[airport_count].flights = 0;
}

/*
Checks if an airport exists, returning 1 if so
*/ 
int airport_exists(Airport airports[], int airport_count, char id[]) {    
    int i;

    for (i = 0; i < airport_count; i++) {
        if (strcmp(airports[i].id, id) == 0)
            return 1;
    }
    return 0;
}

/*
Receiving an ID as argument, returns the index of said airport 
in the airports array
*/
int get_index_by_id(Airport airports[], int airport_count, char id[]) {
    int i;

    for (i = 0; i < airport_count; i++) {
        if (strcmp(airports[i].id, id) == 0)
            return i;
    }

    return 0;
}

/*
Returns 0 if the ID is invalid,
Returns 1 if there are already too many airports
Returns 2 if an airport with the given ID already exists,
Returns 3 if it is possible to create an airport
*/
int check_airport(Airport airports[], char id[], int airport_count) {
    int i;

    for (i = 0; i < 3; i++) {
        if (!check_upper(id[i]))
            return 0;
    }

    if (airport_count == AIRPORTSMAX)
        return 1;

    if (airport_exists(airports, airport_count, id))
        return 2 ;
    
    return 3;
}

/*
Using bubble sort, orders the airports aplhabetically from their IDs
*/
void sort_airports(Airport sorted[], Airport airports[], 
                    int airport_count) {
    Airport temp;
    int i, j;
    int same;

    for (i = 0; i < airport_count; i++) {
        create_airport(sorted, i, airports[i].id, airports[i].country, 
                        airports[i].city);
    }

    for (i = 0; i < airport_count; i++) {
        same = 1;
        for (j = 0; j < airport_count - 1 - i; j++) {
            if (strcmp(sorted[j].id, sorted[j+1].id) > 0) {
                temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
                same = 0;
            }
        }
        if (same)
            break;
    }

}

/*
Creates a new flight, adding it to the array flights
*/
void create_flight(Airport airports[], int airport_count, Flight flights[], int flight_count, char id[], 
                    char departure[], char arrival[], Date departure_date, Hour departure_time, 
                    Hour duration, int passengers, int mode) {
    int departureNum = getDateHourNum(departure_date, departure_time);
    int arrivalNum = departureNum + getHourNum(duration);

    strcpy(flights[flight_count].id, id);
    strcpy(flights[flight_count].departure, departure);
    strcpy(flights[flight_count].arrival, arrival);
    flights[flight_count].departure_date = departure_date;
    flights[flight_count].departure_hour = departure_time;
    flights[flight_count].duration = duration;
    flights[flight_count].departureNum = departureNum;
    flights[flight_count].arrivalNum = arrivalNum;
    flights[flight_count].passengers = passengers;

    if (mode) {
        airports[get_index_by_id(airports, airport_count, departure)].flights++;
    }

}

/*
Returns 0 if the ID is invalid
Returns 1 if a flight with the same ID already exists in the same day
Returns 2 if either the departure or arrival airport does not exist
Returns 3 if there are already too many flights
Returns 4 if the date is invalid
Returns 5 if the duration is higher than 12 hours
Returns 6 if the passenger count does not respect the given limtis
Returns 7 if it is possible to create a flight
*/
int check_flight(Airport airports[], 
                int airport_count, Flight flights[], int flight_count, 
                char id[], char departure[], char arrival[], Date departure_date, 
                Hour duration, int passengers) {
    int i = 2;

    if (!(check_upper(id[0]) && check_upper(id[1]) && id[2] != '0'))
        return 0;

    for (i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].id, id) == 0
                && getDateNum(flights[i].departure_date) == getDateNum(departure_date))
            return 1;
    }

    if (airport_exists(airports, airport_count, departure) == 0
            || airport_exists(airports, airport_count, arrival) == 0)
        return 2;

    if (flight_count == FLIGHTSMAX)
        return 3;

    if (checkDate(departure_date))
        return 4;

    if (duration.hour > 12)
        return 5;
    if (duration.hour == 12)
        if (duration.minute > 0)
            return 5;

    if (passengers < MINPASSENGERS || passengers > MAXPASSENGERS)
        return 6;

    return 7;
}

/*
Compares departure dates and times if mode is set to 0, arrival dates and 
times if mode is set to 1
Returns 1 if flight1 arrives after flight2, 0 if not
*/
int compare_flights(Flight flight1, Flight flight2, int mode) {
    if (mode == 0) {
        if (flight1.departureNum > flight2.departureNum)
            return 1;
        else
            return 0;
    }
    else {
        if (flight1.arrivalNum > flight2.arrivalNum)
            return 1;
        else
            return 0;
    }
}

/*
Using bubble sort, orders flights either from their departure date and 
time, if mode is 0.
If mode is 1, orders from arrival date and time.
From oldest to most recent.
*/
void sort_flights(Airport airports[], int airport_count, Flight sorted[], Flight flights[], 
                    int flight_count, int mode) {
    Flight temp;
    int i, j, same;

    for (i = 0; i < flight_count; i++) {
        create_flight(airports, airport_count, sorted, i, flights[i].id,
        flights[i].departure, flights[i].arrival, flights[i].departure_date, flights[i].departure_hour,
        flights[i].duration, flights[i].passengers, 0);
    }

    for (i = 0; i < flight_count; i++) {
        same = 1;
        for (j = 0; j < flight_count - 1 - i; j++) {
            if (compare_flights(sorted[j], sorted[j+1], mode)) {
                temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
                same = 0;
            }
        }
        if (same)
            break;
    }
}

int getFlightIndex(Flight flights[], int flight_count, char id[]) {
    int i;

    for (i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].id, id) == 0)
            return i;
    }

    return -1;
}

// Alterar p adicionar a head em vez de susbstutuir
Reservation* createReservation(Reservation* head, char flightid[], Date date, char* reservationCode, int passengerCount){
    Reservation* reservation = malloc(sizeof(Reservation));
    reservation->reservationCode = malloc(sizeof(char)*strlen(reservationCode) + 1);
    strcpy(reservation->reservationCode, reservationCode);
    reservation->flightid = malloc(sizeof(char)*strlen(flightid) + 1);
    strcpy(reservation->flightid, flightid);
    reservation->passengerCount = passengerCount;
    reservation->dateNum = getDateNum(date);
    reservation->next = head;
    return reservation;
}

/*
Returns 0 if the reservation code is invalid
Returns 1 if the given flight does not exists
Returns 2 if the reservation is already being used
Returns 3 if, when creating a reservation, it exceeds the flight capacity
Returns 4 if the date is invalid
Returns 5 passengerCount is not an integrer higher than 0
Returns 6 if it is possible to create the given reservation
*/
int checkReservation(Reservation *reservation, Flight flights[], int flight_count, char flightid[], Date date, char* reservationCode, int passengerCount){
    int i;
    int index = -1;

    if (strlen(reservationCode) < 10)
        return 0;

    for (i = 0; reservationCode[i] != '\0'; i++) {
        if (!(check_upper(reservationCode[i])) && (0 < reservationCode[i] > 9))
            return 0;
    }

    index = getFlightIndex(flights, flight_count, flightid);
  
    if (index == -1)
        return 1;

    while (reservation != NULL) {
        if (strcmp(reservation->reservationCode, reservationCode) == 0)
            return 2;
        reservation = reservation->next;
    }

    if (flights[index].passengers < passengerCount)
        return 3;

    if (checkDate(date))
        return 4;

    if (passengerCount < 1)
        return 5;

    return 6;

}

void printReservations(Reservation *reservation, char flightid[], Date date){
    while (reservation != NULL) {
        if (strcmp(reservation->flightid, flightid) == 0 && reservation->dateNum == getDateNum(date))
            printf("%s %d\n", reservation->reservationCode, reservation->passengerCount);

        reservation = reservation->next;
    }
}

/*
Main function, containing the loop for the continuous commands and
where all error handling and function calling takes place
*/
int main() {
    Airport airports[AIRPORTSMAX];
    Airport sorted_airports[AIRPORTSMAX];
    Flight flights[FLIGHTSMAX];
    Flight sorted_flights[FLIGHTSMAX];

    int airport_count = 0;
    int flight_count = 0;
    int check = 0;
    int i;
    int len;

    char input[INPUTLEN + OFFSET];
    char command;

    char* element;
    int index_id;

    char id[IDAPMAX + OFFSET];
    
    char country[COUNTRYMAX + OFFSET];
    char city[CITYMAX + OFFSET];

    char flightid[FLIGHTIDMAX + OFFSET];
    char departureid[IDAPMAX + OFFSET];
    char arrivalid[IDAPMAX + OFFSET];
    Date departuredate;
    Hour departurehour;
    Hour flightduration;
    int passengers;

    Reservation* root = malloc(sizeof(Reservation));

    char* reservationCode = malloc(sizeof(char)*(INPUTLEN - OFFSET));
    int passengerCount;
    

    Date newDate;

    while (1) {
        fgets(input, INPUTLEN + OFFSET, stdin);
        command = input[0];
        switch(command) {
            case 'q':
                exit(0);
                break;
            case 'a':
                sscanf(input, "%*s%s%s %[^\n]", id, country, city);
                check = check_airport(airports, id, airport_count);
                switch(check) {
                    case 0:
                        printf("%s\n", INV_AP_ID_ERR);
                        break;
                    case 1:
                        printf("%s\n", CAP_AP_ERR);
                        break;
                    case 2:
                        printf("%s\n", DUP_AP_ERR);
                        break;
                    case 3:
                        create_airport(airports, airport_count, id, country, 
                                        city);
                        printf("airport %s\n", id);
                        airport_count++;
                        break;
                }
                break;
            case 'l':
                for (len = 0; input[len] != '\0'; len++);
                if (len == 2) {
                    sort_airports(sorted_airports, airports, airport_count);
                    for (i = 0; i < airport_count; i++)
                        printf("%s %s %s %d\n", sorted_airports[i].id, 
                                sorted_airports[i].city, 
                                sorted_airports[i].country, 
                                airports[get_index_by_id(airports, airport_count, sorted_airports[i].id)].flights);
                }
                else{
                    element = strtok(input + 2, " \n");

                    while (element != NULL) {
                        if (airport_exists(airports, airport_count, element)==0)
                            printf("%s: %s\n", element, NO_AP_ID_ERR);

                        else{
                            index_id = get_index_by_id(airports, airport_count, 
                                                        element);
                            printf("%s %s %s %d\n", element, 
                                    airports[index_id].city, 
                                    airports[index_id].country, 
                                    airports[index_id].flights);
                        }

                        element = strtok(NULL, " \n");
                    }
                }
                break;
            case 'v':
                for (len = 0; input[len] != '\0'; len++);
                if (len > 2) {
                    sscanf(input, "%*s %s %s %s %d-%d-%d %d:%d %d:%d %d", flightid, departureid, 
                            arrivalid, &departuredate.day, &departuredate.month, &departuredate.year, &departurehour.hour, &departurehour.minute, 
                            &flightduration.hour, &flightduration.minute, &passengers);
                    
                    check = check_flight(airports, airport_count, 
                                            flights, flight_count, flightid, 
                                            departureid, arrivalid, 
                                            departuredate, flightduration, 
                                            passengers);
                    switch(check) {
                        case 0:
                            printf("%s\n", INV_FLIGHT_ID_ERR);
                            break;
                        case 1:
                            printf("%s\n", DUP_FLIGHT_ERR);
                            break;
                        case 2:
                            if (airport_exists(airports, airport_count, 
                                                departureid))
                                printf("%s: %s\n", arrivalid, NO_AP_ID_ERR);
                            else
                                printf("%s: %s\n", departureid, NO_AP_ID_ERR);
                            break;
                        case 3:
                            printf("%s\n", CAP_FLIGHTS_ERR);
                            break;
                        case 4:
                            printf("%s\n", INV_DATE_ERR);
                            break;
                        case 5:
                            printf("%s\n", INV_DUR_ERR);
                            break;
                        case 6:
                            printf("%s\n", INV_CAP_ERR);
                            break;
                        
                        case 7:
                            create_flight(airports, airport_count, flights, flight_count, flightid, 
                                            departureid, arrivalid, 
                                            departuredate, departurehour, 
                                            flightduration, passengers, 1);
                            flight_count++;
                            break;
                    }
                }
                else
                    for (i = 0; i < flight_count; i++) {
                        printf("%s %s %s ", flights[i].id, flights[i].departure, flights[i].arrival);
                        printDate(flights[i].departure_date);
                        printf(" ");
                        printHour(flights[i].departure_hour);
                        printf("\n");
                    }
                        
                break;
            case 'p':
                sscanf(input, "%*s %s", id);
                if (airport_exists(airports, airport_count, id) == 0)
                    printf("%s: %s\n", id, NO_AP_ID_ERR);
                else{
                    sort_flights(airports, airport_count, sorted_flights, flights, flight_count, 0);
                    for (i = 0; i < flight_count; i++) {
                        if (strcmp(id, sorted_flights[i].departure) == 0) {
                            printf("%s %s ", sorted_flights[i].id, 
                                    sorted_flights[i].arrival);
                            printDate(sorted_flights[i].departure_date);
                            printf(" ");
                            printHour(sorted_flights[i].departure_hour);
                            printf("\n");
                        }
                    }
                }
                break;
            case 'c':
                sscanf(input, "%*s %s", id);
                if (airport_exists(airports, airport_count, id) == 0)
                    printf("%s: %s\n", id, NO_AP_ID_ERR);
                else{
                    sort_flights(airports, airport_count, sorted_flights, flights, flight_count, 1);
                    for (i = 0; i < flight_count; i++) {
                        if (strcmp(id, sorted_flights[i].arrival) == 0) {
                            printf("%s %s ", sorted_flights[i].id, sorted_flights[i].departure);
                            printDate(getDate(sorted_flights[i].arrivalNum));
                            printf(" ");
                            printHour(getHour(sorted_flights[i].arrivalNum));
                            printf("\n");
                        }
                    }
                }

                break;
            case 't':
                sscanf(input, "%*s %02d-%02d-%04d", &newDate.day, &newDate.month, &newDate.year);
                check = checkDate(newDate);
                switch(check) {
                    case 0:
                        currentdate = newDate;
                        printDate(currentdate);
                        printf("\n");
                        break;
                    case 1:
                        printf("%s\n", INV_DATE_ERR);
                        break;
                }
                break;
            case 'r':
                if (strlen(input) > 20) {
                    sscanf(input, "%*s %s %02d-%02d-%04d %s %d", flightid, &newDate.day, &newDate.month, &newDate.year, 
                            reservationCode, &passengerCount);
                
                    reservationCode = realloc(reservationCode, sizeof(char)*strlen(reservationCode) + 1);

                    if (root->reservationCode == NULL)
                        check = checkReservation(NULL, flights, flight_count, flightid, newDate, reservationCode, passengerCount);
                    else
                        check = checkReservation(root, flights, flight_count, flightid, newDate, reservationCode, passengerCount);
                    switch(check) {
                        case 0:
                            printf("%s\n", INV_RES_CODE_ERR);
                            break;
                        case 1:
                            printf("%s: %s\n", flightid, FLIGHT_NOT_FOUND_ERR);
                            break;
                        case 2:
                            printf("%s: %s\n", reservationCode, DUP_RES_CODE_ERR);
                            break;
                        case 3:
                            printf("%s\n", CAP_RES_ERR);
                            break;
                        case 4:
                            printf("%s\n", INV_DATE_ERR);
                            break;
                        case 5:
                            printf("%s\n", INV_PAS_ERR);
                            break;
                        case 6:
                            if (root->reservationCode == NULL){
                                root->reservationCode = malloc(sizeof(char)*strlen(reservationCode) + 1);
                                strcpy(root->reservationCode, reservationCode);
                                root->flightid = malloc(sizeof(char)*strlen(flightid) + 1);
                                strcpy(root->flightid, flightid);
                                root->passengerCount = passengerCount;
                                root->dateNum = getDateNum(newDate);
                                root->next = NULL;
                            }
                            else
                                root = createReservation(root, flightid, newDate, reservationCode, passengerCount);
                            break;   
                    }
                }
                else {
                    sscanf(input, "%*s %s %02d-%02d-%04d", flightid, &newDate.day, &newDate.month, &newDate.year);

                    if (getFlightIndex(flights, flight_count, flightid) == -1)
                        printf("%s: %s\n", flightid, FLIGHT_NOT_FOUND_ERR);
                    else if (checkDate(newDate))
                        printf("%s\n", INV_DATE_ERR);
                    else
                        printReservations(root, flightid, newDate);
                }
            break;
        }
    }

    return(0);
}
