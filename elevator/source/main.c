//#include "elev.h"
#include "elevator_hardware.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    elev_init();
//    if (!elev_init()) {
//        printf("Unable to initialize elevator hardware!\n");
//        return 1;
//    }

//    elev_set_motor_direction(DIRN_UP);

    while (1) {
        char floor_signal = elev_get_floor_sensor_signal();
        printf("floor_signal: %d\n", (char)floor_signal);
        
        elev_set_floor_indicator(floor_signal);
        elev_button_type_t buttons = elev_get_button_signal(DIRN_DOWN, 3);
        printf("buttonTest: %d\n", (int)buttons);
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

//int main() {
//    // Initialize hardware
//    if (!elev_init()) {
//        printf("Unable to initialize elevator hardware!\n");
//        return 1;
//    }
//
//    printf("Press STOP button to stop elevator and exit program.\n");
//
//    elev_set_motor_direction(DIRN_UP);
//
//    while (1) {
//        // Change direction when we reach top/bottom floor
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
//    }
//
//    return 0;
//}
//