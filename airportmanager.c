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

typedef struct{
    char id[IDAPMAX + OFFSET]; /* 3 uppercase letters, these must be unique */
    char country[COUNTRYMAX + OFFSET]; /* Non empty string with 30 max characters, can only contain upper and lower case letters */
    char city[CITYMAX + OFFSET]; /* Non empty string with 50 max characters, white spaces and tabs can occur */

}airports;

typedef struct
{
    char id[FLIGHTIDMAX + OFFSET]; /* Two flights can exist with the same code as long as they do not have the same departure date
                                      String made of two uppercase letters followed by a number 1-9999, that can not start with 0 */
    char departure[IDAPMAX + OFFSET]; /* Departure airport ID */
    char arrival[IDAPMAX + OFFSET]; /* Arrival airport ID */
    char date[DATEMAX + OFFSET]; /* Departure day */
    char time[TIMEMAX + OFFSET]; /* Departure time */
    char duration[TIMEMAX + OFFSET]; /* Can not be over 12 hours long */
    int passengers; /* Max passengers allowed, can be any int between 10 and 100 (inclusive) */

}flights;

airports airport[AIRPORTSMAX];
flights flight[FLIGHTSMAX];

void create_airport(int airport_count, char id[], char country[], char city[]){
    strcpy(airport[airport_count].id, id);
    strcpy(airport[airport_count].country, country);
    strcpy(airport[airport_count].city, city);
    printf("airport %s", id);
}

void create_flight(int flight_count, char id[], char departure[], char arrival[], char date[], char time[], char duration[], int passengers){
    strcpy(flight[flight_count].id, id);
    strcpy(flight[flight_count].departure, departure);
    strcpy(flight[flight_count].arrival, arrival);
    strcpy(flight[flight_count].date, date);
    strcpy(flight[flight_count].time, time);
    strcpy(flight[flight_count].duration, duration);
    flight[flight_count].passengers = passengers;
}

int airport_exists(char id[]){
    /*
        Returns 1 if an airport with the given ID already exists
        0 if it does not.
    */
    int i;
    for (i=0; i < AIRPORTSMAX; i++){
        if (strcmp(airport[i].id, id) == 0)
            return(1);
    }
    return(0);
}

int main(){
    
    int airport_count, flight_count = 0;

    char input[INPUTLEN];
    char command;

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
                break;
            case 'l':
                break;
            case 'v':
                printf("v");
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
