/*
 *  stock.cpp
 */

#include "cutils.h"
#include "stock.h"

#define NUM_ITEMS   ((MAX_PRODUCT-MIN_PRODUCT)+1)
#define MAX_STOCK   8


static int stock[NUM_ITEMS];

/*
 *  init_stock
 */

void
init_stock(void)
{
    fill_stock();
}


/*
 *  stock_state:
 *      Receives: product number
 *      Return: stock qty
 */

int
stock_state(int pr_no)
{
    return stock[pr_no-1];
}


/*
 *  change_stock
 *      Receives:
 *          pr_no: product number
 *          add: items to add to stock (negative substacts from it)
 *      Return:
 *          remaining stock
 */

int
change_stock(int pr_no, int add)
{
    int num, new_stock;

    num = stock[pr_no-1];
    new_stock = num + add;
    if( new_stock > MAX_STOCK )
        new_stock = MAX_STOCK;
    else if( new_stock < 0 )
        new_stock = 0;
    return stock[pr_no-1] = new_stock;
}


/*
 *  fill_stock:
 *      Fills to max items on all stock
 *
 */

void
fill_stock(void)
{
    int i;
    int *p;

    for(i = 0, p = stock; i < NUM_ITEMS; ++i, ++p )
        *p = MAX_STOCK;
}


