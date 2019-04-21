#include <stdio.h>

#include "elevator_hardware.h"
#include "fsm.h"

int main() {
    // Initialize hardware
    elev_init();

    while (1) {
        fsm();
    }
    return 0;
}