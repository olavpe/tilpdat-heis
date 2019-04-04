#include "orders.h"

void order_array(){
    //making a 2D array of orders with the dimension 3(row)x4(col)
    int orders[3][4];
    //setting default values in 2D array
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 4; c++){
            orders[r][c] == -1;
        }
    }
}