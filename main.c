/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   main.c
 * @author Zimnol
 * @date   Feb 2022
 * @brief  File containing main function of Embedded Elevator Program;
 *         currently it is a simulation program, ultimately it may be an
 *         embedded system. The program was created for the purpose of
 *         recruiting to a certain company
 * @ver    0.1
 */

#include <stdio.h>
#include "elevator.h"

int main() {
    Elevator_t elevators[NUM_ELEVATORS];

    for(uint8_t i=0; i < NUM_ELEVATORS; i++ )
        Initialize(&elevators[i], i);

    /*
     * !!! WARNING !!!
     * This simulation does not cover wrong user input; the user only simulates
     * the behavior of the embedded hardware here.
     */

    while( 1 ) {
        Step(elevators);
        Status(elevators);

        int userChoice = 0;
        while( userChoice != 3 ) {
            int elevatorID = 0;
            int selectedFloor = 0;
            int selectedDirection = 0;

            printf("\nWhat do you want to do?\n"
                   "\t0. Exit simulation\n"
                   "\t1. Pick floor in the elevator\n"
                   "\t2. Press button in the corridor\n"
                   "\t3. Nothing (next step)\n");
            scanf("%d", &userChoice);

            switch(userChoice) {
                case 0:
                    return 0;
                    break;
                case 1:
                    printf("Pick elevator ID (0-%d): ", NUM_ELEVATORS-1);
                    scanf("%d", &elevatorID);
                    printf("Pick floor (0-%d): ", NUM_FLOORS-1);
                    scanf("%d", &selectedFloor);
                    ChooseFloor(&elevators[elevatorID], selectedFloor);
                    break;
                case 2:
                    printf("Pick floor (1-%d): ", NUM_FLOORS);
                    scanf("%d", &selectedFloor);
                    printf("Select direction (1-up, 0-down): ");
                    scanf("%d", &selectedDirection);
                    uint8_t choice = Pickup(elevators, selectedFloor, selectedDirection);
                    printf("The elevator with ID %d will pick you up\n", choice);
                    break;
                default:
                    break;
            }
        }
    }
}
