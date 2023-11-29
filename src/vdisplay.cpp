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
#include "machine.h"

extern Machine vendingMachine;

static const uchar pattern[] =
{
    0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

void
init_vdisplay(void)
{
}

void refresh_vdisplay()
{
    for (int pr_no = 0; pr_no < 8; ++pr_no)
    {
        // Check if pr_no is a valid index in the products array
        if (pr_no < vendingMachine.products.size())
        {
            int stock = vendingMachine.products[pr_no].quantity;
            set_row(pr_no, pattern[stock]);
        }
        else
        {
            // Handle the case where pr_no is out of bounds
            // This could be an error or default value based on your requirements
            int stock = 0; // or any default value
            set_row(pr_no, pattern[stock]);
        }
    }
}




