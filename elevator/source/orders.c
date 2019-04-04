#include "orders.h"
#include "elev.h"


//making a 2D array of orders with dimension 3(row)x4(col)
int order_array[N_BUTTONS][N_FLOORS];

void reset_orders(){
    //setting default values in 2D array
    int i, j;
    for(i = 0; i < N_BUTTONS ; i++){
        for(j = 0; j < N_FLOORS; j++){
            order_array[i][j] == -1;
        }
    }
}