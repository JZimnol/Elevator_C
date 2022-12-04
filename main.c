/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   main.c
 * @author Zimnol
 * @date   Dec 2022
 * @brief  File containing main function of Embedded Elevator Program.
 *         Currently it is a simulation program, ultimately it may be an
 *         embedded system. The program was created for the purpose of
 *         recruiting to a certain company
 * @ver    0.2
 */

#include "elevator.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void option_get(int *choice) {
    printf("\nWhat do you want to do?\n"
           "\t0. Exit simulation\n"
           "\t1. Pick floor in the elevator\n"
           "\t2. Press button in the corridor\n"
           "\t3. Nothing (next step)\n"
           "Enter value: ");

    char input;
    scanf(" %c", &input);

    if (!isdigit(input)) {
        *choice = -1;
    } else {
        *choice = atoi(&input);
    }
}

int main() {
    elevator_t elevators[NUM_ELEVATORS];

    for (uint8_t i = 0; i < NUM_ELEVATORS; i++) {
        elevator_initialize(&elevators[i], i);
    }

    /*
     * !!! WARNING !!!
     * This simulation does not cover wrong user input; the user only simulates
     * the behavior of the embedded hardware here.
     */

    while (1) {
        elevator_step(elevators);
        elevator_status(elevators);

        int userChoice = 0;
        while (userChoice != 3) {
            int choosen_id = 0;
            int choosen_floor = 0;
            int choosen_direction = 0;

            option_get(&userChoice);

            switch (userChoice) {
            case -1:
                printf("Invalid value provided.\n");
                break;
            case 0:
                return 0;
                break;
            case 1:
                printf("Pick elevator ID (0-%d): ", NUM_ELEVATORS - 1);
                scanf("%d", &choosen_id);
                printf("Pick floor (0-%d): ", NUM_FLOORS - 1);
                scanf("%d", &choosen_floor);
                elevator_floor_choose(&elevators[choosen_id], choosen_floor);
                break;
            case 2:
                printf("Pick floor (0-%d): ", NUM_FLOORS - 1);
                scanf("%d", &choosen_floor);
                printf("Select direction (1-up, 0-down): ");
                scanf("%d", &choosen_direction);
                size_t choice =
                        corridor_button_push(elevators,
                                             choosen_floor,
                                             choosen_direction ? UP : DOWN);
                printf("The elevator with ID %ld will pick you up\n", choice);
                break;
            default:
                break;
            }
        }
    }
}
