#include <iostream>
#include <vector>
#include "machine.h"

class Product {
public:
    std::string name;
    double price;
    int customId;
};

class ProductQuantity {
public:
    Product product;
    int quantity;
};

class Machine {
public:
    bool works;
    bool beingRepaired;
    double income;
    std::vector<ProductQuantity> products;
    int customId;

    // Constructor that initializes all fields
    Machine(bool works, bool beingRepaired, double income, const std::vector<ProductQuantity>& products, int customId)
        : works(works), beingRepaired(beingRepaired), income(income), products(products), customId(customId) {}

    // Method to update machine with values from another machine
    void update(const Machine& otherMachine) {
        works = otherMachine.works;
        beingRepaired = otherMachine.beingRepaired;
        income = otherMachine.income;
        products = otherMachine.products;  // This assumes a simple overwrite; adjust as needed
        // customId is not updated to prevent accidental uniqueness violation
    }
};