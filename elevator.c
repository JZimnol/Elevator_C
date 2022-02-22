/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   elevator.c
 * @author Zimnol
 * @date   Feb 2022
 * @brief  File containing definitions of an embedded elevator program;
 *         The program was created for the purpose of recruiting to a certain company
 * @ver    0.1
 */

#include "elevator.h"

//******************************************************************************
// Function definitions
//******************************************************************************

void Step(Elevator_t *elevArr) {
    for( uint8_t i=0; i<NUM_ELEVATORS; i++ )
        Update(&elevArr[i]);
}

void Initialize(Elevator_t *elevator, uint8_t ID) {
    elevator->ID = ID;
    elevator->currentFloor = 0;
    elevator->nextFloor = 0;
    elevator->hasOpenDoors = TRUE;
    elevator->buffer.end = 0;
    for( uint8_t i=0; i<BUFF_SIZE; i++ )
        elevator->buffer.buff[i] = 0;
}

void Status(Elevator_t *elevArr) {
    static int counter = 0;

    printf("\n================================================================================%3d", counter++);

    for( uint8_t i=0; i<NUM_ELEVATORS; i++ ) {
        printf("\nElevator ID: %2d\n", elevArr[i].ID);
        printf("\tCurrent floor: %2d || Next floor: %2d || Has open doors: %d || ",
               elevArr[i].currentFloor, elevArr[i].nextFloor, elevArr[i].hasOpenDoors);
        printf("Floors in queue: ");
        for( uint8_t j=0; j<elevArr[i].buffer.end; j++ )
            printf("%d, ", elevArr[i].buffer.buff[j]);
    }

    printf("\n===================================================================================");
}

void Update(Elevator_t *elevator) {
    /* if elevator wasn't moving */
    if( elevator->currentFloor == elevator->nextFloor ) {
        if( elevator->buffer.end == 0 )
            return;
        else {
            // next destination - first element in the buffer
            elevator->nextFloor = elevator->buffer.buff[0];
            // move elements in the buffer (consider linked list)
            for( uint8_t i=0; i<elevator->buffer.end-1; i++ )
                elevator->buffer.buff[i] = elevator->buffer.buff[i+1];
            elevator->buffer.end--;
        }

    }

    /* if elevator should move down */
    if( elevator->currentFloor > elevator->nextFloor )
        elevator->currentFloor--;
    else  // if elevator should move up
        elevator->currentFloor++;

    /* check destination */
    if( elevator->currentFloor == elevator->nextFloor ) {
        elevator->hasOpenDoors = TRUE;
        // if there are still elements in the buffer
        if( elevator->buffer.end != 0 ) {
            // next destination - first element in the buffer
            elevator->nextFloor = elevator->buffer.buff[0];
            // move elements in the buffer (consider linked list)
            for( uint8_t i=0; i<elevator->buffer.end-1; i++ )
                elevator->buffer.buff[i] = elevator->buffer.buff[i+1];
            elevator->buffer.end--;
        }
    }
    else {
        elevator->hasOpenDoors = FALSE;
        // check if current floor is in the buffer
        for( uint8_t i=0; i<elevator->buffer.end; i++ ) {
            // if current floor is in the buffer
            if( elevator->buffer.buff[i] == elevator->currentFloor ) {
                elevator->hasOpenDoors = TRUE;
                // move elements in the buffer (consider linked list)
                for( uint8_t j=i; j<elevator->buffer.end-1; j++ )
                    elevator->buffer.buff[j] = elevator->buffer.buff[j+1];
                elevator->buffer.end--;
            }
            else { // current floor not in the buffer - do not stop and do not open the door
                elevator->hasOpenDoors = FALSE;
            }
        }
    }
}

uint8_t Pickup(Elevator_t *elevArr, int8_t pickupFloor, uint8_t direction) {
    /* check if any "moving" elevators can quickly pick me up (and goes where I want to go) */
    switch(direction) {
        case 1: // up
            for( uint8_t i=0; i<NUM_ELEVATORS; i++ ) {
                /* check if elevator is "below me" and goes to the highest floor (does not care about buffer length)
                   or if the elevator is "below me" and ends "above me" (and the buffer is empty) */
                if( (elevArr[i].currentFloor <= pickupFloor && elevArr[i].nextFloor == NUM_FLOORS - 1) \
                    || (elevArr[i].currentFloor <= pickupFloor && elevArr[i].nextFloor >= pickupFloor && elevArr[i].buffer.end == 0) ) {
                    // check if requested floor is already in the buffer
                    for( uint8_t j=0; j<elevArr[i].buffer.end; j++ )
                        if( elevArr[i].buffer.buff[j] == pickupFloor )
                            return i;
                    elevArr[i].buffer.buff[elevArr[i].buffer.end] = pickupFloor;
                    elevArr[i].buffer.end++;
                    return i;
                }
            }
            break;
        case 0: // down
            for( uint8_t i=0; i<NUM_ELEVATORS; i++ ) {
                /* check if elevator is "above me" and goes to the lowest floor (does not care about buffer length)
                   or if the elevator is "above me" and ends "below me" (and the buffer is empty) */
                if( (elevArr[i].currentFloor >= pickupFloor && elevArr[i].nextFloor == 0) \
                    || (elevArr[i].currentFloor >= pickupFloor && elevArr[i].nextFloor <= pickupFloor && elevArr[i].buffer.end == 0) ) {
                    // check if requested floor is already in the buffer
                    for( uint8_t j=0; j<elevArr[i].buffer.end; j++ )
                        if( elevArr[i].buffer.buff[j] == pickupFloor )
                            return i;
                    elevArr[i].buffer.buff[elevArr[i].buffer.end] = pickupFloor;
                    elevArr[i].buffer.end++;
                    return i;
                }
            }
            break;
        default: break;
    }

    /* check if any elevator is not moving (closest to me to serve) */
    uint8_t lowestDistance = NUM_FLOORS+1;
    uint8_t chosenElevator = 0;
    for( uint8_t i=0; i<NUM_ELEVATORS; i++ ) {
        if( elevArr[i].currentFloor == elevArr[i].nextFloor ) {
            if( abs(elevArr[i].currentFloor-pickupFloor) < lowestDistance ) {
                lowestDistance = abs(elevArr[i].currentFloor-pickupFloor);
                chosenElevator = i;
            }
        }
    }
    /* check if any elevator is not moving right now */
    if( lowestDistance < NUM_FLOORS+1 ) {
        // next check if requested floor is already in the buffer
        for( uint8_t j=0; j<elevArr[chosenElevator].buffer.end; j++ )
            if( elevArr[chosenElevator].buffer.buff[j] == pickupFloor )
                return chosenElevator;
        elevArr[chosenElevator].buffer.buff[elevArr[chosenElevator].buffer.end] = pickupFloor;
        elevArr[chosenElevator].buffer.end++;
        return chosenElevator;
    }

    /* find elevator with the lowest buffer length */
    uint8_t lowestBuff = BUFF_SIZE;
    chosenElevator = 0;
    for( uint8_t i=0; i<NUM_ELEVATORS; i++ ) {
        if( elevArr[i].buffer.end <= lowestBuff ) {
            lowestBuff = elevArr[i].buffer.end;
            chosenElevator = i;
        }
    }
    /* next check if requested floor is already in the buffer */
    for( uint8_t j=0; j<elevArr[chosenElevator].buffer.end; j++ )
        if( elevArr[chosenElevator].buffer.buff[j] == pickupFloor )
            return chosenElevator;
    elevArr[chosenElevator].buffer.buff[elevArr[chosenElevator].buffer.end] = pickupFloor;
    elevArr[chosenElevator].buffer.end++;
    return chosenElevator;
}

void ChooseFloor(Elevator_t *elevator, int8_t floorNumber) {
    for( uint8_t i=0; i<elevator->buffer.end; i++ ) {
        // if floorNumber is already in the buffer
        if( elevator->buffer.buff[i] == floorNumber )
            return;
    }
    // else add element to the end of a buffer, move buffer end pointer
    elevator->buffer.buff[elevator->buffer.end] = floorNumber;
    elevator->buffer.end++;
}
