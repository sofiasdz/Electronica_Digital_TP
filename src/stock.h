/*
 *  stock.h
 */

#define MIN_PRODUCT 1
#define MAX_PRODUCT 8

/*
 *  init_stock
 */

void init_stock(void);

/*
 *  stock_state:
 *      Receives: product number
 *      Return: stock qty
 */

int stock_state(int pr_no);

/*
 *  change_stock
 *      Receives:
 *          pr_no: product number
 *          add: items to add to stock (negative substacts from it)
 *      Return:
 *          remaining stock
 */

int change_stock(int pr_no, int add);

/*
 *  fill_stock:
 *      Fills to max items on all stock
 *
 */

void fill_stock(void);

