/*
Bernardo Cunha Meireles
ist1103378
Airport and flight management system that allows for their definition, as well
as consulting them
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
#define INPUTLEN 100

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

const int daysMonth[] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* STRUCTS */

typedef struct {

    int day;
    int month;
    int year;

} Date;

typedef struct {

    char id[IDAPMAX + OFFSET]; 
    char country[COUNTRYMAX + OFFSET]; 
    char city[CITYMAX + OFFSET];
    int flights;

} Airport;

typedef struct {

    int hour;
    int minute;

} Hour;

typedef struct {

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

Date currentdate = {STARTDAY, STARTMONTH, STARTYEAR};

/* DATE */

void change_date(int day, int month, int year){
    /* 
    Changes the system's date, associated to a global variable
    */

    currentdate.day = day;
    currentdate.month = month;
    currentdate.year = year;

}

int getDateNum(Date date) {
    return (date.year - STARTYEAR) * 365 + daysMonth[date.month - 1] + date.day - 1;
}

int getHourNum(Hour hour) {
    return ((hour.hour * 60) + hour.minute);
}

int getDateHourNum(Date date, Hour hour) {
    return getDateNum(date) * 24 * 60 + getHourNum(hour);
}

Hour getHour(int hourNum) {
	Hour hour;
	hour.minute = hourNum % 60;
	hour.hour = ((hourNum - hour.minute) / 60) % 24;
	return hour;
}

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


int check_date(Date date){
    /* 
    It is not possible to change to a date more than one year further or 
    to a date before
    the current one.
    If it is possible to change the date, the function returns 1, else returns 0
    */

    int dateNum = getDateNum(date);
    Date NextYear = currentdate;
    NextYear.year++;

    if (dateNum < getDateNum(currentdate) || dateNum > getDateNum(NextYear))
        return 0;
    
    return 1;
    
}

void printDate(Date date){
    /* 
    Prints the date in the format dd/mm/yyyy
    */

    printf("%02d/%02d/%04d", date.day, date.month, date.year);
}

void printHour(Hour hour){
    /* 
    Prints the hour in the format hh:mm
    */

    printf("%02d:%02d", hour.hour, hour.minute);
}

/* AIRPORT */

void create_airport(Airport airports[], int airport_count, char id[], 
                    char country[], char city[]){
    /*
    Creates a new airport, adding it to the array airports
    */

    strcpy(airports[airport_count].id, id);
    strcpy(airports[airport_count].country, country);
    strcpy(airports[airport_count].city, city);
    airports[airport_count].flights = 0;
}

int airport_exists(Airport airports[], int airport_count, char id[]){
    /*
    Checks if an airport exists, returning 1 if so
    */ 

    int i;

    for (i = 0; i < airport_count; i++){
        if (strcmp(airports[i].id, id) == 0)
            return(1);
    }
    return (0);
}

int get_index_by_id(Airport airports[], int airport_count, char id[]){
    /*
    Receiving an ID as argument, returns the index of said airport 
    in the airports array
    */

    int i;

    for (i = 0; i < airport_count; i++){
        if (strcmp(airports[i].id, id) == 0)
            return i;
    }

    return 0;
}

int check_airport(Airport airports[], char id[], int airport_count){
    /*
    Returns 0 if the ID is invalid,
    Returns 1 if there are already too many airports
    Returns 2 if an airport with the given ID already exists,
    Returns 3 if it is possible to create an airport
    */

    int i;

    for (i = 0; i < 3; i++){
        if (!(id[i] >= 'A' && id[i] <= 'Z'))
            return 0;
    }

    if (airport_count == AIRPORTSMAX)
        return(1);

    if (airport_exists(airports, airport_count, id))
        return(2);
    
    return(3);
}

void sort_airports(Airport sorted[], Airport airports[], 
                    int airport_count){
    /*
    Using bubble sort, orders the airports aplhabetically from their IDs
    */

    Airport temp;
    int i, j;

    for (i = 0; i < airport_count; i++){
        create_airport(sorted, i, airports[i].id, airports[i].country, 
                        airports[i].city);
    }

    for (i = 0; i < airport_count; i++){
        for (j = 0; j < airport_count - 1 - i; j++){
            if (strcmp(sorted[j].id, sorted[j+1].id) > 0){
                temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
}

/* FLIGHT */

void create_flight(Airport airports[], int airport_count, Flight flights[], int flight_count, char id[], 
                    char departure[], char arrival[], Date departure_date, Hour departure_time, 
                    Hour duration, int passengers, int mode){
    /*
    Creates a new flight, adding it to the array flights
    */

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

    if (mode)
        airports[get_index_by_id(airports, airport_count, departure)].flights++;

}

int check_flight(Airport airports[], 
                int airport_count, Flight flights[], int flight_count, 
                char id[], char departure[], char arrival[], Date departure_date, Hour departure_hour, 
                Hour duration, int passengers){
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

    int i = 2;
    int day, month, year;
    int hrs, mins;

    if (!(isupper(id[0]) && isupper(id[1]) && id[2] != '0'))
        return 0;
    
    while (id[i] != '\0'){
        if (!isdigit(id[i]))
            return 0;
        
        if (i == FLIGHTIDMAX)
            return 0;
        
        i++;
    
    }

    for (i = 0; i < flight_count; i++){
        if ((strcmp(flights[i].id, id) == 0)
                && (flights[i].departureNum == getDateHourNum(departure_date, 
                                                            departure_hour)))
            return 1;
    }

    if ((airport_exists(airports, airport_count, departure) == 0)
            || (airport_exists(airports, airport_count, arrival) == 0))
        return 2;

    if (flight_count == FLIGHTSMAX)
        return 3;

    if (check_date(departure_date))
        return 4;

    if (duration.hour > 12)
        return 5;
    if (duration.hour == 12)
        if (duration.minute > 0)
            return 5;

    if ((passengers < MINPASSENGERS )|| (passengers > MAXPASSENGERS))
        return 6;

    return 7;
}

int compare_flights(Flight flight1, Flight flight2, int mode){
    /*
    Compares departure dates and times if mode is set to 0, arrival dates and 
    times if mode is set to 1
    Returns 1 if flight1 arrives after flight2, 0 if not
    */

    if (mode == 0){
        if (flight1.departureNum > flight2.departureNum)
            return 1;
        else
            return 0;
    }
    else{
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
void sort_flights(Airport airports, int airport_count, Flight sorted[], Flight flights[], 
                    int flight_count, int mode){
    Flight temp;
    int i, j, same;

    for (i = 0; i < flight_count; i++){
        create_flight(airports, airport_count, sorted, i, flights[i].id,
        flights[i].departure, flights[i].arrival, flights[i].departure_date, flights[i].departure_hour,
        flights[i].duration, flights[i].passengers, 0);
    }

    for (i = 0; i < flight_count; i++){
        same = 1;
        for (j = 0; j < flight_count - 1 - i; j++){
            if (compare_flights(sorted[j], sorted[j+1], mode)){
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
Main function, containing the loop for the continuous commands and
where all error handling and function calling takes place
*/
int main(){

    Airport airports[AIRPORTSMAX];
    Airport sorted_airports[AIRPORTSMAX];
    Flight flights[FLIGHTSMAX];
    Flight sorted_flights[FLIGHTSMAX];

    /* printf("%ld", sizeof(Flight)); -> 44 */

    int airport_count = 0;
    int flight_count = 0;
    int check = 0;
    int i;
    int len;

    char input[INPUTLEN + OFFSET];
    char command;

    char *element;
    int index_id;

    char id[IDAPMAX + OFFSET];
    
    char country[COUNTRYMAX + OFFSET];
    char city[CITYMAX + OFFSET];

    char flightid[FLIGHTIDMAX + OFFSET];
    char departureid[IDAPMAX + OFFSET];
    char arrivalid[IDAPMAX + OFFSET];
    char departuredate[DATEMAX + OFFSET];
    char departuretime[TIMEMAX + OFFSET];
    char flightduration[TIMEMAX + OFFSET];
    int passengers;

    int day;
    int month;
    int year;

    while (1){
        fgets(input, INPUTLEN + OFFSET, stdin);
        command = input[0];

        /*
        q : kills the program
        a : adds a new airport to the system
        l : lists all airports
        v : adds a new flight or shows all existing ones
        p : lists all flights with departure from an airport
        c : lists all flight with arrival at an airport
        t : advances the date of the system
        */ 

        switch(command){
            case 'q':
                exit(0);
                break;
            case 'a':
                sscanf(input, "%*s%s%s %[^\n]", id, country, city);
                check = check_airport(airports, id, airport_count);
                switch(check){
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
                if (len == 2){
                    sort_airports(sorted_airports, airports, airport_count);
                    for (i = 0; i < airport_count; i++)
                        printf("%s %s %s %d\n", sorted_airports[i].id, 
                                sorted_airports[i].city, 
                                sorted_airports[i].country, 
                                departure_count(flights, flight_count, 
                                                    sorted_airports[i].id));
                }
                else{
                    element = strtok(input + 2, " \n");

                    while (element != NULL){
                        if (airport_exists(airports, airport_count, element)==0)
                            printf("%s: %s\n", element, NO_AP_ID_ERR);

                        else{
                            index_id = get_index_by_id(airports, airport_count, 
                                                        element);
                            printf("%s %s %s %d\n", element, 
                                    airports[index_id].city, 
                                    airports[index_id].country, 
                                    departure_count(flights, flight_count, 
                                                        element));
                        }

                        element = strtok(NULL, " \n");
                    }
                }
                break;
            case 'v':
                for (len = 0; input[len] != '\0'; len++);
                if (len > 2){
                    sscanf(input, "%*s%s%s%s%s%s%s%d", flightid, departureid, 
                            arrivalid, departuredate, departuretime, 
                            flightduration, &passengers);
                    check = check_flight(currentdate, airports, airport_count, 
                                            flights, flight_count, flightid, 
                                            departureid, arrivalid, 
                                            departuredate, flightduration, 
                                            passengers);
                    switch(check){
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
                            create_flight(flights, flight_count, flightid, 
                                            departureid, arrivalid, 
                                            departuredate, departuretime, 
                                            flightduration, passengers);
                            flight_count++;
                            break;
                    }
                }
                else
                    for (i = 0; i < flight_count; i++)
                        printf("%s %s %s %02d-%02d-%d %s\n", flights[i].id, 
                                flights[i].departure, flights[i].arrival, 
                                parse_date(flights[i].date, 0), 
                                parse_date(flights[i].date, 1), 
                                parse_date(flights[i].date, 2), 
                                flights[i].time);
                break;
            case 'p':
                sscanf(input, "%*s %s", id);
                if (airport_exists(airports, airport_count, id) == 0)
                    printf("%s: %s\n", id, NO_AP_ID_ERR);
                else{
                    sort_flights(sorted_flights, flights, flight_count, 0);
                    for (i = 0; i < flight_count; i++){
                        if (strcmp(id, sorted_flights[i].departure) == 0)
                            printf("%s %s %s %s\n", sorted_flights[i].id, 
                                    sorted_flights[i].arrival, 
                                    sorted_flights[i].date, 
                                    sorted_flights[i].time);
                    }
                }
                break;
            case 'c':
                sscanf(input, "%*s %s", id);
                if (airport_exists(airports, airport_count, id) == 0)
                    printf("%s: %s\n", id, NO_AP_ID_ERR);
                else{
                    sort_flights(sorted_flights, flights, flight_count, 1);
                    for (i = 0; i < flight_count; i++){
                        if (strcmp(id, sorted_flights[i].arrival) == 0)
                            printf("%s %s %02d-%02d-%d %02d:%02d\n", 
                                    sorted_flights[i].id, 
                                    sorted_flights[i].departure, 
                                    arrival_date_n_hour(sorted_flights[i], 0), 
                                    arrival_date_n_hour(sorted_flights[i], 1), 
                                    arrival_date_n_hour(sorted_flights[i], 2), 
                                    arrival_date_n_hour(sorted_flights[i], 3), 
                                    arrival_time_aux(sorted_flights[i], 1));
                    }
                }

                break;
            case 't':
                sscanf(input, "%*s %d-%d-%d", &day, &month, &year);
                check = check_date(currentdate, day, month, year);
                switch(check){
                    case 0:
                        change_date(day, month, year);
                        printDate(currentdate);
                        break;
                    case 1:
                        printf("%s\n", INV_DATE_ERR);
                        break;
                }
                break;
        }
    }

    return(0);
}
