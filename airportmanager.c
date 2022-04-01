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

/* STRUCTS */

struct date{

    int day;
    int month;
    int year;
};

struct airport{

    char id[IDAPMAX + OFFSET]; 
    char country[COUNTRYMAX + OFFSET]; 
    char city[CITYMAX + OFFSET];
};

struct flight{

    char id[FLIGHTIDMAX + OFFSET];
    char departure[IDAPMAX + OFFSET]; 
    char arrival[IDAPMAX + OFFSET];
    char date[DATEMAX + OFFSET]; 
    char time[TIMEMAX + OFFSET]; 
    char duration[TIMEMAX + OFFSET]; 
    int passengers; 
};

/* DATE */

void change_date(struct date currentdate[], int day, int month, int year){
    /* 
    Changes the system's date, which is passed by reference,
    not needing a variable global to accomplish it
    */

    currentdate[0].day = day;
    currentdate[0].month = month;
    currentdate[0].year = year;
}

int check_date(struct date date[], int day, int month, int year){
    /* 
    It is not possible to change to a date more than one year further or 
    to a date before
    the current one.
    If it is possible to change the date, the function returns 0, else returns 1
    */

    /* More than a year in the future */

    if (year > date[0].year + 1)
        return 1;

    if (year > date[0].year){
        if (month > date[0].month)
            return 1;
        
        if (month == date[0].month)
            if (day > date[0].month)
                return 1;
    }

    /* In the past */

    if (year < date[0].year)
        return 1;

    if (year == date[0].year){
        if (month < date[0].month)
            return 1;
        if (month == date[0].month)
            if (day < date[0].day)
                return 1;
    }
    
   return 0;
}

int parse_date(char date[], int pos){
    /* 
    Returns the integrer with the value of
    Day if pos is 0,
    Month if pos is 1,
    Year if pos is 2
    */

    int day, month, year;

    sscanf(date, "%d-%d-%d", &day, &month, &year);

    switch(pos){
        case 0:
            return day;
        case 1:
            return month;
        case 2:
            return year;
    }

    return 0;
}

int parse_time(char time[], int pos){
    /* 
    Returns the integrer with the value of
    Hours if pos is 0,
    Minutes if pos is 1
    */

    int hours, minutes;

    sscanf(time, "%d:%d", &hours, &minutes);

    if (pos == 0)
        return hours;
    else
        return minutes;

    return 0;
}

/* AIRPORT */

void create_airport(struct airport airports[], int airport_count, char id[], 
                    char country[], char city[]){
    /*
    Creates a new airport, adding it to the array airports
    */

    strcpy(airports[airport_count].id, id);
    strcpy(airports[airport_count].country, country);
    strcpy(airports[airport_count].city, city);
}

int airport_exists(struct airport airports[], int airport_count, char id[]){
    /*
    Checks if an airport exists, returning  1 if so
    */ 

    int i;

    for (i = 0; i < airport_count; i++){
        if (strcmp(airports[i].id, id) == 0)
            return(1);
    }
    return (0);
}

int get_index_by_id(struct airport airports[], int airport_count, char id[]){
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

int check_airport(struct airport airports[], char id[], int airport_count){
    /*
    Returns 0 if the ID is invalid,
    Returns 1 if there are already too many airports
    Returns 2 if an airport with the given ID already exists,
    Returns 3 if it is possible to create an airport
    */

    int i;

    for (i = 0; i < 3; i++){
        if (!isupper(id[i]))
            return 0;
    }

    if (airport_count == AIRPORTSMAX)
        return(1);

    if (airport_exists(airports, airport_count, id))
        return(2);
    
    return(3);
}

void sort_airports(struct airport sorted[], struct airport airports[], 
                    int airport_count){
    /*
    Using bubble sort, orders the airports aplhabetically from their IDs
    */

    struct airport temp;
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

void create_flight(struct flight flights[], int flight_count, char id[], 
                    char departure[], char arrival[], char date[], char time[], 
                    char duration[], int passengers){
    /*
    Creates a new flight, adding it to the array flights
    */

    strcpy(flights[flight_count].id, id);
    strcpy(flights[flight_count].departure, departure);
    strcpy(flights[flight_count].arrival, arrival);
    strcpy(flights[flight_count].date, date);
    strcpy(flights[flight_count].time, time);
    strcpy(flights[flight_count].duration, duration);
    flights[flight_count].passengers = passengers;
}

int flight_exists(struct flight flights[], int airport_count, char id[], 
                    char date[]){
    /*
    Checks if a flight exists, returning  1 if so
    */ 

    int i;

    for (i = 0; i < airport_count; i++){
        if ((strcmp(flights[i].id, id) == 0) 
                && (strcmp(flights[i].date, date) == 0))
            return(1);
    }
    return (0);
}

int check_flight(struct date currentdate[], struct airport airports[], 
                int airport_count, struct flight flights[], int flight_count, 
                char id[], char departure[], char arrival[], char date[], 
                char duration[], int passengers){
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
                && (strcmp(flights[i].date, date) == 0))
            return 1;
    }

    if ((airport_exists(airports, airport_count, departure) == 0)
            || (airport_exists(airports, airport_count, arrival) == 0))
        return 2;

    if (flight_count == FLIGHTSMAX)
        return 3;

    sscanf(date, "%d-%d-%d", &day, &month, &year);
    if (check_date(currentdate, day, month, year))
        /* check_date() returns 1 (considered true) if the date is invalid */
        return 4;

    sscanf(duration, "%d:%d", &hrs, &mins);
    if (hrs > 12)
        return 5;
    if (hrs == 12)
        if (mins > 0)
            return 5;

    if ((passengers < MINPASSENGERS )|| (passengers > MAXPASSENGERS))
        return 6;

    return 7;
}

int departure_count(struct flight flights[], int flight_count, char id[]){
    /*
    Receiving an airport ID, returns the amount of flights that have a 
    departure on it
    */

    int i;
    int count = 0;

    for (i = 0; i < flight_count; i++)
        if (strcmp(flights[i].departure, id) == 0)
            count++;

    return count;
}

int arrival_time_aux(struct flight flight, int pos){ 
    /* 
    Will calculate the minutes of an arrival hours, while hours may
    be higher than 24, in case it advances a day, which is later 
    fed to the function arrival_date_n_hour that indeed correctly 
    calculates the day changes

    if pos is 0, returns the hours. Returns the minutes if pos is 1.
    */

    int arr_hrs, arr_mins;

    arr_hrs = parse_time(flight.time, 0) + parse_time(flight.duration, 0);
    arr_mins = parse_time(flight.time, 1) + parse_time(flight.duration, 1);


    if (arr_mins == 60){
        arr_mins = 0;
        arr_hrs++;
    }

    if (arr_mins > 60){
        arr_mins -= 60;
        arr_hrs++;
    }

    if (pos == 0)
        return arr_hrs;
    else
        return arr_mins;
}

int arrival_date_n_hour(struct flight flight, int pos){ 
    /* 
    Returns the integrer with the value of
    Day if pos is 0,
    Month if pos is 1,
    Year if pos is 2,
    New hour (since when this function is called, for the time, while minutes 
    are correct, hours can exceed possible values) if pos 3
    */

    int day, mo, y, arr_hrs;

    day = parse_date(flight.date, 0);
    mo = parse_date(flight.date, 1);
    y = parse_date(flight.date, 2);

    arr_hrs = arrival_time_aux(flight, 0);
    
    if (arr_hrs > 23){
        arr_hrs -= 24;

        /* New day */

        if ((day == 31) && (mo == 12)){
            day = 1;
            mo = 1;
            y++;
        }

        else{
            if ((mo == 4) || (mo == 6) || (mo == 9) || (mo == 11))
                if (day == 30){
                    day = 1;
                    mo++;
                }
                else
                    day++;
            
            else if ((mo == 1) || (mo == 3) || (mo == 5) || (mo == 7) 
                        || (mo == 8) || (mo == 10) || (mo == 12))
                if (day == 31){
                    day = 1;
                    mo++;
                }
                else
                    day++;

            else if ((mo == 2) && (day == 28)){
                day = 1;
                mo++;
            }
        }
    
    }

    switch(pos){
        case 0:
            return day;
        case 1:
            return mo;
        case 2:
            return y;
        case 3:
            return arr_hrs;
    }
    
    return 0;
}

int compare_flights(struct flight flight1, struct flight flight2, int mode){
    /*
    Compares departure dates and times if mode is set to 0, arrival dates and 
    times if mode is set to 1
    Returns 1 if flight1 arrives after flight2, 0 if not
    */

    int day1, mo1, y1, hrs1, mins1;
    int day2, mo2, y2, hrs2, mins2;

    if (mode == 1){

        y1 = arrival_date_n_hour(flight1, 2);
        y2 = arrival_date_n_hour(flight2, 2);
    }

    else{
        y1 = parse_date(flight1.date, 2);
        y2 = parse_date(flight2.date, 2);
    }



    if (y1 > y2)
        return 1;
    if (y1 < y2)
        return 0;


    if (mode == 1){
        mo1 = arrival_date_n_hour(flight1, 1);
        mo2 = arrival_date_n_hour(flight2, 1);
    }

    else{
        mo1 = parse_date(flight1.date, 1);
        mo2 = parse_date(flight2.date, 1);
    }



    if ((y1 == y2) && (mo1 > mo2))
        return 1;

    if ((y1 == y2) && (mo1 < mo2))
        return 0;



    if (mode == 1){

        day1 = arrival_date_n_hour(flight1, 0);
        day2 = arrival_date_n_hour(flight2, 0);
    }

    else{
        day1 = parse_date(flight1.date, 0);
        day2 = parse_date(flight2.date, 0);
    }



    if ((mo1 == mo2) && (day1 > day2))
        return 1;

    if ((mo1 == mo2) && (day1 < day2))
        return 0;



    if (mode == 1){
        hrs1 = arrival_date_n_hour(flight1, 3);
        hrs2 = arrival_date_n_hour(flight2, 3);
    }

    else{
        hrs1 = parse_time(flight1.time, 0);
        hrs2 = parse_time(flight2.time, 0);
    }



    if ((day1 == day2) && (hrs1 > hrs2))
        return 1;
    
    if ((day1 == day2) && (hrs1 < hrs2))
        return 0;


    if (mode == 1){
        mins1 = arrival_time_aux(flight1, 1);
        mins2 = arrival_time_aux(flight2, 1);
    }
    else{
        mins1 = parse_time(flight1.time, 1);
        mins2 = parse_time(flight2.time, 1);
    }



    if ((hrs1 == hrs2) && (mins1 > mins2))
        return 1;

    if ((hrs1 == hrs2) && (mins1 < mins2))
        return 0;

    return 0;
}

void sort_flights(struct flight sorted[], struct flight flights[], 
                    int flight_count, int mode){
    /*
    Using bubble sort, orders flights either from their departure date and 
    time, if mode is 0.
    If mode is 1, orders from arrival date and time.
    From oldest to most recent.
    */

    struct flight temp;
    int i, j;

    for (i = 0; i < flight_count; i++){
        create_flight(sorted, i, flights[i].id, flights[i].departure, 
                        flights[i].arrival, flights[i].date, flights[i].time, 
                        flights[i].duration, flights[i].passengers);
    }

    for (i = 0; i < flight_count; i++){
        for (j = 0; j < flight_count - 1 - i; j++){
            if (compare_flights(sorted[j], sorted[j + 1], mode)){
                temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
    
}

int main(){
    /*
    Main function, containing the loop for the continuous commands and
    where all error handling and function calling takes place
    */
    
    struct airport airports[AIRPORTSMAX];
    struct airport sorted_airports[AIRPORTSMAX];
    struct flight flights[FLIGHTSMAX];
    struct flight sorted_flights[FLIGHTSMAX];
    struct date currentdate[1];

    /* printf("%ld", sizeof(struct flight)); -> 44 */

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

    currentdate[0].day = STARTDAY;
    currentdate[0].month = STARTMONTH;
    currentdate[0].year = STARTYEAR;

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
                        change_date(currentdate, day, month, year);
                        printf("%02d-%02d-%d\n", day, month, year);
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
