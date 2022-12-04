/******************************************************************************
 * This file is a part of the Embedded Elevator Program
 ******************************************************************************/

/**
 * @file   elevator.c
 * @author Zimnol
 * @date   Dec 2022
 * @brief  File containing structures and declarations of an embedded elevator
 *         program. This program was created for the purpose of recruiting to a
 *         certain company
 * @ver    0.2
 */

#include "elevator.h"
#include <string.h>

static void line_print(size_t length, size_t counter) {
    printf("\n");
    for (size_t i = 0; i < length; i++) {
        printf("=");
    }
    printf(" [counter: %ld]\n", counter);
}

static bool is_not_moving(elevator_t *elevator) {
    return (elevator->next_floor == elevator->current_floor);
}

static bool has_empty_buffer(elevator_t *elevator) {
    return (elevator->buff.end == 0);
}

static void buffer_update(elevator_t *elevator) {
    /* consider linked list */
    for (size_t i = 0; i < elevator->buff.end - 1; i++) {
        elevator->buff.data[i] = elevator->buff.data[i + 1];
    }
}

static bool should_move_down(elevator_t *elevator) {
    return (elevator->current_floor > elevator->next_floor);
}

static bool is_below(elevator_t *elevator, uint8_t floor) {
    return (elevator->current_floor <= floor);
}

static bool is_above(elevator_t *elevator, uint8_t floor) {
    return (elevator->current_floor >= floor);
}

static bool goes_to_the_top(elevator_t *elevator) {
    return (elevator->next_floor == NUM_FLOORS - 1);
}

static bool goes_to_the_bottom(elevator_t *elevator) {
    return (elevator->next_floor == 0);
}

static bool goes_above(elevator_t *elevator, uint8_t floor) {
    return (elevator->next_floor >= floor);
}

static bool goes_below(elevator_t *elevator, uint8_t floor) {
    return (elevator->next_floor <= floor);
}

static void buffer_append(elevator_t *elevator, uint8_t floor) {
    elevator->buff.data[elevator->buff.end] = floor;
    elevator->buff.end++;
}

static bool has_floor_in_buffer(elevator_t *elevator, uint8_t floor) {
    for (size_t i = 0; i < elevator->buff.end; i++) {
        if (elevator->buff.data[i] == floor) {
            return true;
        }
    }
    return false;
}

void elevator_step(elevator_t *elevators) {
    for (size_t i = 0; i < NUM_ELEVATORS; i++) {
        elevator_update(&elevators[i]);
    }
}

void elevator_initialize(elevator_t *elevator, uint8_t id) {
    elevator->id = id;
    elevator->current_floor = 0;
    elevator->next_floor = 0;
    elevator->has_open_doors = true;
    elevator->buff.end = 0;
    memset(elevator->buff.data, 0, BUFF_SIZE);
}

void elevator_status(elevator_t *elevators) {
    static int counter = 0;

    line_print(84, counter);

    for (size_t i = 0; i < NUM_ELEVATORS; i++) {
        printf("Elevator ID: %2d\n", elevators[i].id);
        printf("\tCurrent floor: %2d || Next floor: %2d || Has open doors: %d "
               "|| ",
               elevators[i].current_floor, elevators[i].next_floor,
               elevators[i].has_open_doors);
        printf("Floors in queue: ");
        for (size_t j = 0; j < elevators[i].buff.end; j++) {
            printf("%d, ", elevators[i].buff.data[j]);
        }
        if (i != NUM_ELEVATORS - 1) {
            printf("\n");
        }
    }

    line_print(84, counter++);
}

void elevator_update(elevator_t *elevator) {
    if (is_not_moving(elevator)) {
        if (has_empty_buffer(elevator)) {
            return;
        } else {
            /* next destination - first element in the buffer */
            elevator->next_floor = elevator->buff.data[0];
            buffer_update(elevator);
            elevator->buff.end--;
        }
    }

    if (should_move_down(elevator)) {
        elevator->current_floor--;
    } else {
        elevator->current_floor++;
    }

    /* check destination */
    if (is_not_moving(elevator)) {
        elevator->has_open_doors = true;
        if (!has_empty_buffer(elevator)) {
            /* next destination - first element in the buffer */
            elevator->next_floor = elevator->buff.data[0];
            buffer_update(elevator);
            elevator->buff.end--;
        }
    } else {
        elevator->has_open_doors = false;
        for (size_t i = 0; i < elevator->buff.end; i++) {
            if (elevator->buff.data[i] == elevator->current_floor) {
                elevator->has_open_doors = true;
                for (size_t j = i; j < elevator->buff.end - 1; j++) {
                    elevator->buff.data[j] = elevator->buff.data[j + 1];
                }
                elevator->buff.end--;
            }
        }
    }
}

size_t corridor_button_push(elevator_t *elevators,
                            uint8_t pickup_floor,
                            direction_t direction) {
    /* check if any "moving" elevators can quickly pick me up
       (and goes where I want to go) */
    switch (direction) {
    case UP:
        for (size_t i = 0; i < NUM_ELEVATORS; i++) {
            /* check if elevator is "below me" and goes to the highest floor
               (does not care about buffer length)
               or if the elevator is "below me" and ends "above me" (and the
               buffer is empty) */
            if ((is_below(&elevators[i], pickup_floor)
                 && goes_to_the_top(&elevators[i]))
                    || (is_below(&elevators[i], pickup_floor)
                        && goes_above(&elevators[i], pickup_floor)
                        && has_empty_buffer(&elevators[i]))) {
                if (has_floor_in_buffer(&elevators[i], pickup_floor)) {
                    return i;
                }
                buffer_append(&elevators[i], pickup_floor);
                return i;
            }
        }
        break;
    case DOWN:
        for (size_t i = 0; i < NUM_ELEVATORS; i++) {
            /* check if elevator is "above me" and goes to the lowest floor
               (does not care about buffer length)
               or if the elevator is "above me" and ends "below me" (and the
               buffer is empty) */
            if ((is_above(&elevators[i], pickup_floor)
                 && goes_to_the_bottom(&elevators[i]))
                    || (is_above(&elevators[i], pickup_floor)
                        && goes_below(&elevators[i], pickup_floor)
                        && has_empty_buffer(&elevators[i]))) {
                if (has_floor_in_buffer(&elevators[i], pickup_floor)) {
                    return i;
                }
                buffer_append(&elevators[i], pickup_floor);
                return i;
            }
        }
        break;
    }

    /* check if any elevator is not moving (closest to me to serve) */
    uint8_t lowest_distance = NUM_FLOORS + 1;
    uint8_t chosen_elevator = 0;
    for (size_t i = 0; i < NUM_ELEVATORS; i++) {
        if (is_not_moving(&elevators[i])) {
            if (abs(elevators[i].current_floor - pickup_floor)
                    < lowest_distance) {
                lowest_distance =
                        abs(elevators[i].current_floor - pickup_floor);
                chosen_elevator = i;
            }
        }
    }

    /* check if any elevator is not moving right now */
    if (lowest_distance < NUM_FLOORS + 1) {
        if (has_floor_in_buffer(&elevators[chosen_elevator], pickup_floor)) {
            return chosen_elevator;
        }
        buffer_append(&elevators[chosen_elevator], pickup_floor);
        return chosen_elevator;
    }

    /* find elevator with the lowest buffer length */
    uint8_t lowest_buff = BUFF_SIZE;
    chosen_elevator = 0;
    for (size_t i = 0; i < NUM_ELEVATORS; i++) {
        if (elevators[i].buff.end <= lowest_buff) {
            lowest_buff = elevators[i].buff.end;
            chosen_elevator = i;
        }
    }
    if (has_floor_in_buffer(&elevators[chosen_elevator], pickup_floor)) {
        return chosen_elevator;
    }
    buffer_append(&elevators[chosen_elevator], pickup_floor);
    return chosen_elevator;
}

void elevator_floor_choose(elevator_t *elevator, uint8_t floor_number) {
    if (!has_floor_in_buffer(elevator, floor_number)) {
        buffer_append(elevator, floor_number);
    }
}
