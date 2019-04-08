#include <stdio.h>

#include "elevator_hardware.h"

void queue_reset_queue();
void queue_delete_order(position_t current_position);
int queue_get_order(elev_button_type_t button, position_t floor);
void queue_set_order(elev_button_type_t button, position_t floor);

elev_motor_direction_t queue_get_next_direction(position_t current_position, elev_motor_direction_t last_direction);
void assert_buttons();
elev_motor_direction_t choose_direction_based_on_priority(elev_motor_direction_t , int8_t orders_above, int8_t orders_below);
