#include "Machine.h"
#include <string>

bool sell_product(Machine& machine, const std::string& product_name)
{
    for(auto& product_quantity : machine.products)
    {
        if(product_quantity.product.name == product_name && product_quantity.quantity > 0)
        {
            product_quantity.quantity--;
            machine.income += product_quantity.product.price;
            return true;
        }
    }
    return false;
}

//Despues hacer un publish soldProduct