#ifndef MACHINE_H
#define MACHINE_H

#include <iostream>
#include <vector>

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
    Machine(bool works, bool beingRepaired, double income, const std::vector<ProductQuantity>& products, int customId);

    // Method to initialize a machine from JSON string
    void initializeFromJSON(const std::string& jsonString);

    void update(const Machine& otherMachine);

};

#endif  // MACHINE_H
