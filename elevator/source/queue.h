/**
 * @file
 * @brief A queue system that helps the finite state machine (fsm)
 *  to carry out the orders received from the elevator hardware.
 */ 

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>

#include "elevator_hardware.h"
#include "fsm.h"


/**
 * @brief Deletes all orders in the queue by setting
 * all the order options to the initial value 0.
 * 
 * @param[out] queue_array Queue table
 */
void queue_reset_queue();

/**
 * @brief Deletes an order in the queue.
 * 
 * @param[in] current_position Position of the elevator
 * @param[out] queue_array Queue table
 */
void queue_delete_order(floor_t floor);

/**
 * @brief Sets an order in the queue.
 * 
 * @param[in] button Hardware buttons
 * @param[in] floor At a floor
 * @param[out] queue_array Queue table
 */
void queue_set_order(elev_button_type_t button, floor_t floor);

/**
 * @brief Checks whether the queue has any orders.
 * 
 * @return 1 if the queue is empty, 0 if not.
 */
bool queue_is_queue_empty();

/**
 * @brief Gets the next direction of the elevator based 
 * on the requested priorities.
 * 
 * @param[in] current_position At a floor or between floors
 * @param[in] last_direction The last direction of the elevator
 * 
 * @return next direction of the elevator.
 */
elev_motor_direction_t queue_get_next_direction(position_t current_position, elev_motor_direction_t last_direction);

/**
 * @brief Checks if the elevator should stop when arriving at a floor,
 *  based on orders in the queue array.
 * This function will inform the elevator that it should stop if:
 *  - There are no further orders in the direction.
 *  - There are cab or hall calls in the direction of travel.
 *
 * @param[in] fsm_position The current position of the elevator. 
 * @param[in] fsm_floor The current floor of the elevator.
 * @param[in] fsm_direction The direction of travel the elevator.
 * 
 * @return 1 of the elevator should stop, 0 if not.
 */
bool queue_should_stop(position_t fsm_position, floor_t fsm_floor, elev_motor_direction_t fsm_direction);

#endif