#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define OFFSET 1

#define IDAPMAX 3
#define COUNTRYMAX 30
#define CITYMAX 50
#define AIRPORTSMAX 40

#define DATEMAX 10
#define TIMEMAX 5

#define FLIGHTIDMAX 6
#define MINPASSENGERS 10
#define MAXPASSENGERS 100
#define MAXFLIGHTDURATION 12
#define FLIGHTSMAX 300000

#define INPUTLEN 128

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

void create_airport(struct airport airports[], int airport_count, char id[], char country[], char city[]){
    strcpy(airports[airport_count].id, id);
    strcpy(airports[airport_count].country, country);
    strcpy(airports[airport_count].city, city);
    printf("airport %s\n", id);
}

void create_flight(struct flight flights[], int flight_count, char id[], char departure[], char arrival[], char date[], char time[], char duration[], int passengers){
    strcpy(flights[flight_count].id, id);
    strcpy(flights[flight_count].departure, departure);
    strcpy(flights[flight_count].arrival, arrival);
    strcpy(flights[flight_count].date, date);
    strcpy(flights[flight_count].time, time);
    strcpy(flights[flight_count].duration, duration);
    flights[flight_count].passengers = passengers;
    printf("%s %s %s %s %s", id, departure, arrival, date, time);
}


int airport_exists(struct airport airports[], char id[]){
    int i;

    for (i=0; i < AIRPORTSMAX; i++){
        if (strcmp(airports[i].id, id) == 0)
            return(1);
    }
    return (0);
}

int flight_exists(struct flight flights[], char id[], char date[]){
    int i;

    for (i=0; i < FLIGHTSMAX; i++){
        if ((strcmp(flights[i].id, id) == 0) && (strcmp(flights[i].date, date) == 0))
            return(1);
    }
    return (0);
}

int check_airport(struct airport airports[], char id[], int airport_count){
    /*
        Returns 0 if the ID is invalid,
        Returns 1 if there are already too many airports
        Returns 2 if an airport with the given ID already exists,
        Returns 3 if it is possible to create an airport
    */
    int i;
    for (i=0; i<3; i++){
        if (!isupper(id[i]))
            return 0;
    }

    if (airport_count == AIRPORTSMAX)
        return(1);

    if (airport_exists(airports, id))
        return(2);
    
    return(3);
}

int check_flight(struct flight flights[], char id[], int airport_count[], char date[], int duration[]){
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
}

int main(){
    struct airport airports[AIRPORTSMAX];
    struct flight flights[FLIGHTSMAX];

    int airport_count = 0;
    int id_status = 0;
    int i;
    int len;

    char input[INPUTLEN];
    char command;

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

    while (1){
        fgets(input, INPUTLEN, stdin);
        command = input[0];

        /*
            q : kills the program
            a : adds a new airport to the system
            l : lists all airports
            v : adds a new flight or shows all existing ones
            p : lists all flights with departure from an airport
            c : lista all flight with arrival at an airport
            t : advances the data of the system
        */ 

        switch(command){
            case 'q':
                exit(0);
                break;
            case 'a':
                sscanf(input, "%*s%s%s %[^\n]", id, country, city);
                id_status = check_airport(airports, id, airport_count);
                switch(id_status){
                    case 0:
                        printf("invalid airport ID");
                        break;
                    case 1:
                        printf("too many airports");
                        break;
                    case 2:
                        printf("duplicate airport");
                        break;
                    case 3:
                        create_airport(airports, airport_count, id, country, city);
                        airport_count++;
                        break;
                }
                break;
            case 'l':
                /* flight count missing
                aplhabetic ordering missing too */
                for(len = 0; input[len] != '\0'; len++);
                if (len == 2)
                    for(i = 0; i < airport_count; i++)
                        printf("%s %s %s\n", airports[i].id, airports[i].city, airports[i].country);
                break;
            case 'v':
                sscanf(input, "%*s%s%s%s%s%s%s%d", flightid, departureid, arrivalid, departuredate, departuretime, flightduration, &passengers);
                
                break;
            case 'p':
                printf("p");
                break;
            case 'c':
                printf("c");
                break;
            case 't':
                printf("t");
                break;
        }
    }

    return(0);
}
