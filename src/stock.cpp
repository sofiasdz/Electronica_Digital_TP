#include "Machine.h"
#include <map>
#include <string>

std::map<std::string, int> stock;

void fill_stock(Machine& machine)
{
    for(auto& product_quantity : machine.products)
    {
        stock[product_quantity.product.name] = product_quantity.quantity;
    }
}

int stock_state(const std::string& product_name)
{
    if(stock.find(product_name) != stock.end())
    {
        return stock[product_name];
    }
    else
    {
        return -1;
    }
}