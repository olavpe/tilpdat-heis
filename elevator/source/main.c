#include <stdio.h>

//#include "elev.h"
#include "elevator_hardware.h"

int8_t main() {
    // Initialize hardware
    elev_init();

    while (1) {
        
        char floor_signal = elev_get_floor_sensor_signal();
        print8_tf("floor_signal: %d\n", (char)floor_signal);
        
        elev_set_floor_indicator(floor_signal);
        elev_button_type_t buttons = elev_get_button_signal(DIRN_DOWN, 3);
        print8_tf("buttonTest: %d\n", (int8_t)buttons);
        // Change direction when we reach top/bottom floor
//        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
//            elev_set_motor_direction(DIRN_DOWN);
//        } else if (elev_get_floor_sensor_signal() == 0) {
//            elev_set_motor_direction(DIRN_UP);
//        }
//
//        // Stop elevator and exit program if the stop button is pressed
//        if (elev_get_stop_signal()) {
//            elev_set_motor_direction(DIRN_STOP);
//            break;
//        }
    }
    return 0;
}

static void m_register_order_press(){
    for (int8_t button = 0; button < N_BUTTONS; button++) {
        for (int8_t floor = 0; floor < N_FLOORS; floor++) {
            int8_t button_signal = elev_get_button_signal(button, floor);
            if (button_signal == 1) {
                queue_set_order(button, floor);
                elev_set_button_lamp(button, floor, 1);
            }
        }
    }
}