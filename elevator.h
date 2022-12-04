/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   elevator.h
 * @author Zimnol
 * @date   Dec 2022
 * @brief  File containing structures and declarations of an embedded elevator
 *         program. This program was created for the purpose of recruiting to a
 *         certain company
 * @ver    0.2
 */

#ifndef ELEVATOR_ELEVATOR_H
#define ELEVATOR_ELEVATOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_FLOORS (30)
#define NUM_ELEVATORS (5)
#define BUFF_SIZE (NUM_FLOORS)

typedef enum { UP, DOWN } direction_t;

typedef struct {
    /* unique ID of an elevator */
    uint8_t id;
    /* current floor of an elevator */
    int8_t current_floor;
    /* next (destination) floor */
    uint8_t next_floor;
    /* simple flag */
    bool has_open_doors;
    /* basic buffer, can be changed to linked list */
    struct {
        /* end of a buffer */
        uint8_t end;
        /* main buffer */
        int8_t data[BUFF_SIZE];
    } buff;
} elevator_t;

/**
 * @brief         perform next step in simulation, run update() function on each
 *                elevator object
 * @param[in/out] elevators - an array of elevator objects
 */
void elevator_step(elevator_t *elevators);

/**
 * @brief         initialize elevator object
 * @param[in/out] elevator - elevator object
 * @param[in]     id - elevator ID
 */
void elevator_initialize(elevator_t *elevator, uint8_t id);

/**
 * @brief         print elevators' status on screen; later it can be changed to
 *                return whole elevator objects or reduced data (as variables)
 * @param[in/out] elevators - an array of elevator objects
 */
void elevator_status(elevator_t *elevators);

/**
 * @brief         update elevator status, change floor etc.
 * @param[in/out] elevator - elevator object
 */
void elevator_update(elevator_t *elevator);

/**
 * @brief         function responsible for "pushing" button e.g. in the corridor
 * @param[in/out] elevators - an array of elevator objects
 * @param[in]     pickup_floor - floor number (of pushing a button)
 * @param[in]     direction - direction (up or down)
 * @return        ID of elevator to pick me up
 */
size_t corridor_button_push(elevator_t *elevators,
                             uint8_t pickup_floor,
                             direction_t direction);

/**
 * @brief         press button in the elevator, add floor to buffer
 * @param[in/out] elevator - elevator object
 * @param[in]     floor_number - chosen floor number
 */
void elevator_floor_choose(elevator_t *elevator, uint8_t floor_number);

#endif /* ELEVATOR_ELEVATOR_H */
