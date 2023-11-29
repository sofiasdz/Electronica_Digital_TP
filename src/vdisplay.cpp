/*
 *  vdisplay.cpp
 *
 *      Project 91.vending
 *      Showing stock in matrix display
 */

#include "cutils.h"

#include "matrix.h"
#include "vdisplay.h"
#include "stock.h"

static const uchar pattern[] =
{
    0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

void
init_vdisplay(void)
{
}

void
refresh_vdisplay(void)
{
    int pr_no, stock;

    for( pr_no = 0; pr_no < 8; ++pr_no )
    {
        //stock = stock_state(pr_no+1);
        stock = 2;
        set_row(pr_no, pattern[stock]);
    }
}





