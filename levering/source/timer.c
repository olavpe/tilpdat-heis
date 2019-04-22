/**
 * @file
 * @brief Implementation of the functions in timer.h
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "timer.h"

time_t timer_start_timer(){
    time_t start_timestamp = time(NULL);
    return start_timestamp;
}

bool timer_is_timer_expired(time_t start_timestamp){
    time_t current_timestamp = time(NULL);
    // difftime() returns the difference between two time_t values.
    double time_difference = difftime(current_timestamp, start_timestamp);
    if (time_difference >= 3){
        return true;
    } else {
        return false;
    }
}