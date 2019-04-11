#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "timer.h"

//time_t timer_set_timestamp(void){
//    time_t current_time = 0;
//    current_time = time(NULL);
//    return current_time;
//}
//
//time_t timer_start_timer(){
//    time_t start_time = timer_set_timestamp();
//    return start_time;
//}

time_t timer_start_timer(){
    time_t start_timestamp = time(NULL);
    return start_timestamp;
}

bool timer_is_timer_expired(time_t start_timestamp){
    time_t current_timestamp = time(NULL);
    printf("end: %ld\n", current_timestamp);
    //time_t time_difference = start_timestamp - current_timestamp;
    //difftime is a time.h function
    double time_difference = difftime(current_timestamp, start_timestamp);
    printf("diff: %f\n", time_difference);;
    if (time_difference > 3){
        return true;
    } else {
        return false;
    }
}