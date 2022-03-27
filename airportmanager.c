#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define OFFSET 1

#define IDAPMAX 3
#define COUNTRYMAX 30
#define CITYMAX 50
#define AIRPORTSMAX 40

#define FLIGHTIDMAX 6
#define MINPASSENGERS 10
#define MAXPASSENGERS 100
#define MAXFLIGHTDURATION 12
#define FLIGHTSMAX 30000

#define DATEMAX 10
#define TIMEMAX 5
#define STARTDAY 1
#define STARTMONTH 1
#define STARTYEAR 2022

#define INPUTLEN 200
#define MAXPASSENGERSLEN 3

/* STRUCTS */

struct date{

    int day;
    int month;
    int year;
};

struct airport{

    char id[IDAPMAX + OFFSET]; /* 3 uppercase letters, these must be unique */
    char country[COUNTRYMAX + OFFSET]; /* Non empty string with 30 max characters, can only contain upper and lower case letters */
    char city[CITYMAX + OFFSET]; /* Non empty string with 50 max characters, white spaces and tabs can occur */
};

struct flight{

    char id[FLIGHTIDMAX + OFFSET]; /* Two flights can exist with the same code as long as they do not have the same departure date
                                      String made of two uppercase letters followed by a number 1-9999, that can not start with 0 */
    char departure[IDAPMAX + OFFSET]; /* Departure airport ID */
    char arrival[IDAPMAX + OFFSET]; /* Arrival airport ID */
    char date[DATEMAX + OFFSET]; /* Departure day */
    char time[TIMEMAX + OFFSET]; /* Departure time */
    char duration[TIMEMAX + OFFSET]; /* Can not be over 12 hours long */
    int passengers; /* Max passengers allowed, can be any int between 10 and 100 (inclusive) */
};

/* DATE */

void change_date(struct date currentdate[], int day, int month, int year){

    currentdate[0].day = day;
    currentdate[0].month = month;
    currentdate[0].year = year;
}

int check_date(struct date date[], int day, int month, int year){

    /* 
        It is not possible to change to a date more than one year further or to a date before
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

void create_airport(struct airport airports[], int airport_count, char id[], char country[], char city[]){

    strcpy(airports[airport_count].id, id);
    strcpy(airports[airport_count].country, country);
    strcpy(airports[airport_count].city, city);
}

int airport_exists(struct airport airports[], int airport_count, char id[]){
    int i;

    for (i = 0; i < airport_count; i++){
        if (strcmp(airports[i].id, id) == 0)
            return(1);
    }
    return (0);
}

int get_index_by_id(struct airport airports[], int airport_count, char id[]){
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

void order_airports(struct airport ordered[], struct airport airports[], int airport_count){
    struct airport temp;
    int i, j;

    for (i = 0; i < airport_count; i++){
        create_airport(ordered, i, airports[i].id, airports[i].country, airports[i].city);
    }

    for (i = 0; i < airport_count; i++){
        for (j = 0; j < airport_count - 1 - i; j++){
            if (strcmp(ordered[j].id, ordered[j+1].id) > 0){
                temp = ordered[j];
                ordered[j] = ordered[j+1];
                ordered[j+1] = temp;
            }
        }
    }
}

/* FLIGHT */

void create_flight(struct flight flights[], int flight_count, char id[], char departure[], char arrival[], char date[], char time[], char duration[], int passengers){
    
    strcpy(flights[flight_count].id, id);
    strcpy(flights[flight_count].departure, departure);
    strcpy(flights[flight_count].arrival, arrival);
    strcpy(flights[flight_count].date, date);
    strcpy(flights[flight_count].time, time);
    strcpy(flights[flight_count].duration, duration);
    flights[flight_count].passengers = passengers;
}

int flight_exists(struct flight flights[], int airport_count, char id[], char date[]){

    int i;

    for (i = 0; i < airport_count; i++){
        if ((strcmp(flights[i].id, id) == 0) && (strcmp(flights[i].date, date) == 0))
            return(1);
    }
    return (0);
}

int check_flight(struct date currentdate[], struct airport airports[], int airport_count, struct flight flights[], int flight_count, char id[], char departure[], char arrival[], char date[], char duration[], int passengers){

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
        if ((strcmp(flights[i].id, id) == 0) && (strcmp(flights[i].date, date) == 0))
            return 1;
    }

    if ((airport_exists(airports, airport_count, departure) == 0) || (airport_exists(airports, airport_count, arrival) == 0))
        return 2;

    if (flight_count == FLIGHTSMAX)
        return 3;

    sscanf(date, "%d-%d-%d", &day, &month, &year);
    if (check_date(currentdate, day, month, year)) /* check_date() returns 1 (considered true) if the date is invalid */
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

/* MULTI */

int departure_count(struct flight flights[], int flight_count, char id[]){ /* Flight + Airport */
    int i;
    int count = 0;

    for (i = 0; i < flight_count; i++)
        if (strcmp(flights[i].departure, id) == 0)
            count++;

    return count;
}

int arrival_time_aux(struct flight flight, int pos){ /* Flight + Date */
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

char arrival_date_n_hour(struct flight flight, int pos){ /* Flight + Date */

    /* 
        Returns the integrer with the value of
        Day if pos is 0,
        Month if pos is 1,
        Year if pos is 2,
        New hour (since when this function is called, for the time, while minutes are correct, hours can exceed possible values) if pos 3
    */

    int day, mo, y, arr_hrs;

    day = parse_date(flight.date, 0);
    mo = parse_date(flight.date, 1);
    y = parse_date(flight.date, 2);

    arr_hrs = arrival_time_aux(flight, 0);

    printf("day: %d\nmo: %d\ny: %d\nhrs: %d\n", day, mo, y, arr_hrs);
    
    if (arr_hrs > 23){
        arr_hrs -= 24;

        /* New day */

        if ((day == 31) && (mo == 12)){
            day = 1;
            mo = 1;
            y++;
        }

        else{
            if ((mo % 2 == 0) && (mo != 2))
                if (day == 30){
                    day = 1;
                    mo++;
                }
                else
                    day++;
            
            else if (mo % 2 == 1)
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

    printf("day: %d\nmo: %d\ny: %d\nhrs: %d\n", day, mo, y, arr_hrs);
    printf("%d-%d-%d %d:10\n", day, mo, y, arr_hrs);

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




int main(){
    
    struct airport airports[AIRPORTSMAX];
    struct airport ordered_airports[AIRPORTSMAX];
    struct flight flights[FLIGHTSMAX];
    /*struct flight ordered_flights[FLIGHTSMAX];*/
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
            c : lista all flight with arrival at an airport
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
                        printf("invalid airport ID\n");
                        break;
                    case 1:
                        printf("too many airports\n");
                        break;
                    case 2:
                        printf("duplicate airport\n");
                        break;
                    case 3:
                        create_airport(airports, airport_count, id, country, city);
                        printf("airport %s\n", id);
                        airport_count++;
                        break;
                }
                break;
            case 'l':
                for (len = 0; input[len] != '\0'; len++);
                if (len == 2){
                    order_airports(ordered_airports, airports, airport_count);
                    for (i = 0; i < airport_count; i++)
                        printf("%s %s %s %d\n", ordered_airports[i].id, ordered_airports[i].city, ordered_airports[i].country, departure_count(flights, flight_count, ordered_airports[i].id));
                }
                else{
                    element = strtok(input + 2, " \n");

                    while (element != NULL){
                        if (airport_exists(airports, airport_count, element) == 0)
                            printf("%s: no such airport ID\n", element);

                        else{
                            index_id = get_index_by_id(airports, airport_count, element);
                            printf("%s %s %s %d\n", element, airports[index_id].city, airports[index_id].country, departure_count(flights, flight_count, element));
                        }

                        element = strtok(NULL, " \n");
                    }
                }
                break;
            case 'v':
                for (len = 0; input[len] != '\0'; len++);
                if (len > 2){
                    sscanf(input, "%*s%s%s%s%s%s%s%d", flightid, departureid, arrivalid, departuredate, departuretime, flightduration, &passengers);
                    check = check_flight(currentdate, airports, airport_count, flights, flight_count, flightid, departureid, arrivalid, departuredate, flightduration, passengers);
                    switch(check){
                        case 0:
                            printf("invalid flight code\n");
                            break;
                        case 1:
                            printf("flight already exists\n");
                            break;
                        case 2:
                            if (airport_exists(airports, airport_count, departureid))
                                printf("%s: no such airport ID\n", arrivalid);
                            else
                                printf("%s: no such airport ID\n", departureid);
                            break;
                        case 3:
                            printf("too many flights\n");
                            break;
                        case 4:
                            printf("invalid date\n");
                            break;
                        case 5:
                            printf("invalid duration\n");
                            break;
                        case 6:
                            printf("invalid capacity\n");
                            break;
                        
                        case 7:
                            create_flight(flights, flight_count, flightid, departureid, arrivalid, departuredate, departuretime, flightduration, passengers);
                            flight_count++;
                            break;
                    }
                }
                else
                    for (i = 0; i < flight_count; i++)
                        printf("%s %s %s %02d-%02d-%d %s\n", flights[i].id, flights[i].departure, flights[i].arrival, parse_date(flights[i].date, 0), parse_date(flights[i].date, 1), parse_date(flights[i].date, 2), flights[i].time);
                break;
            case 'p':
                printf("p");
                break;
            case 'c':
                
                printf("%d-%d-%d %d:10\n", arrival_date_n_hour(flights[0], 0), arrival_date_n_hour(flights[0], 1), arrival_date_n_hour(flights[0], 2), arrival_date_n_hour(flights[0], 3));
                
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
                        printf("invalid date\n");
                        break;
                }
                break;
        }
    }

    return(0);
}
