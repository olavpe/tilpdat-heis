#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "timer.h"

time_t timer_set_timestamp(void){
    time_t current_time = 0;
    current_time = time(NULL);
    return current_time;
}

time_t timer_start_timer(){
    time_t start_time = timer_set_timestamp();
    return start_time;
}

bool timer_is_timer_expired(time_t start_time){
    time_t new_timestamp = timer_set_timestamp();
    //printf("end: %ld\n", is_3_sec_elapsed);
    //time_t time_difference = start_time - is_time_elapsed;
    //difftime is a time.h function
    double time_difference = difftime(new_timestamp, start_time);
    //printf("diff: %f\n", time_difference);;
    if (time_difference > 3){
        return true;
    } else {
        return false;
    }
}