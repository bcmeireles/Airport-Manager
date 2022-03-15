#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define OFFSET 1
#define IDAPMAX 3
#define COUNTRYMAX 30
#define CITYMAX 50
#define DATEMAX 10
#define TIMEMAX 5
#define FLIGHTIDMAX 6
#define AIRPORTSMAX 40
#define FLIGHTSMAX 300000
#define MAXFLIGHTDURATION 12
#define MAXPASSENGERSLEN 3

typedef struct{
    char id[IDAPMAX + OFFSET]; /* 3 uppercase letters, these must be unique */
    char country[COUNTRYMAX + OFFSET]; /* Non empty string with 30 max characters, can only contain upper and lower case letters */
    char city[CITYMAX + OFFSET]; /* Non empty string with 50 max characters, white spaces and tabs can occur */

}Airport;

typedef struct
{
    char id[FLIGHTIDMAX + OFFSET]; /* Two flights can exist with the same code as long as they do not have the same departure date
                                      String made of two uppercase letters followed by a number 1-9999, that can not start with 0 */
    Airport departure;
    Airport arrival;
    char date[DATEMAX + OFFSET]; /* Departure day */
    char time[TIMEMAX + OFFSET]; /* Departure time */
    char duration[TIMEMAX + OFFSET]; /* Can not be over 12 hours long */
    int passengers; /* Max passengers allowed, can be any int between 10 and 100 (inclusive) */

}Flight;



int main(){

    /* Commands:
        q : kills the program
        a : adds a new airport to the system
        l : lists all airports
        v : adds a new flight or shows all existing ones
        p : lists all flights with departure from an airport
        c : lista all flight with arrival at an airport
        t : advances the data of the system

    */ 

}