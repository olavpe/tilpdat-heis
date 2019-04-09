#include <stdio.h>
#include <stdbool.h>
#include <time.h>

time_t timer_set_timestamp(void);
time_t timer_start_timer();
bool timer_is_timer_expired(time_t start_time);