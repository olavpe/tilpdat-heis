#pragma once
#include <stdio.h>
#include <stdbool.h>

#include "elevator_hardware.h"
#include "fsm.h"

void queue_reset_queue();
void queue_delete_order(position_t current_position);
int queue_get_order(elev_button_type_t button, position_t floor);
void queue_set_order(elev_button_type_t button, position_t floor);
bool queue_is_queue_empty();
elev_motor_direction_t queue_get_next_direction(position_t current_position, elev_motor_direction_t last_direction);
