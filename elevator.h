/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   elevator.h
 * @author Zimnol
 * @date   Feb 2022
 * @brief  File containing structures and declarations of an embedded elevator program;
 *         The program was created for the purpose of recruiting to a certain company
 * @ver    0.1
 */

#ifndef ELEVATOR_ELEVATOR_H
#define ELEVATOR_ELEVATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//******************************************************************************
// Global definitions
//******************************************************************************

#define NUM_FLOORS      (30)
#define NUM_ELEVATORS   (16)
#define BUFF_SIZE       (NUM_FLOORS)

#ifndef TRUE
    #define TRUE        (1)
#endif

#ifndef FALSE
    #define FALSE       (0)
#endif


//******************************************************************************
// Struct declarations
//******************************************************************************

typedef struct {
    uint8_t ID;                    /* unique ID of an elevator */
    int8_t currentFloor;           /* current floor of an elevator */
    int8_t nextFloor;              /* next (destination) floor */
    uint8_t hasOpenDoors;          /* simple flag */
    /* basic buffer, can be changed to linked list */
    struct {
        uint8_t end;               /* end of a buffer */
        int8_t buff[BUFF_SIZE];    /* main buffer */
    } buffer;
} Elevator_t;

//******************************************************************************
// Function declarations
//******************************************************************************

/**-----------------------------------------------------------------------------
 * @brief         perform next step in simulation, run update() function on each
 *                elevator object
 * @param[in/out] elevArr - an array of elevator objects
 */
void Step(Elevator_t *elevArr);

/**-----------------------------------------------------------------------------
 * @brief         initialize elevator object
 * @param[in/out] elevator - elevator object
 * @param[in]     ID - elevator ID
 */
void Initialize(Elevator_t *elevator, uint8_t ID);

/**-----------------------------------------------------------------------------
 * @brief         print elevators' status on screen; later it can be changed to
 *                return whole elevator objects or reduced data (as variables)
 * @param[in/out] elevArr - an array of elevator objects
 */
void Status(Elevator_t *elevArr);

/**-----------------------------------------------------------------------------
 * @brief         update elevator status, change floor etc.
 * @param[in/out] elevator - elevator object
 */
void Update(Elevator_t *elevator);

/**-----------------------------------------------------------------------------
 * @brief         function responsible for "pushing" button e.g. in the corridor
 * @param[in/out] elevArr - an array of elevator objects
 * @param[in]     pickupFloor - floor number (of pushing a button)
 * @param[in]     direction - direction (up or down)
 * @return        ID of elevator to pick me up
 */
uint8_t Pickup(Elevator_t *elevArr, int8_t pickupFloor, uint8_t direction);

/**-----------------------------------------------------------------------------
 * @brief         press button in the elevator, add floor to buffer
 * @param[in/out] elevator - elevator object
 * @param[in]     floorNumber - chosen floor number
 */
void ChooseFloor(Elevator_t *elevator, int8_t floorNumber);

#endif //ELEVATOR_ELEVATOR_H
