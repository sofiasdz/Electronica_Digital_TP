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

    void updateFromJSON(const std::string& jsonString);


    void update(const Machine& otherMachine);

    void workingInMachine();

    void readyMachine();

    void deleteMachine();

    void productDelete(const std::string& msg);

    void adjustPrice(const std::string& msg);
    std::vector<std::string> getProductNames() const;
};

#endif  // MACHINE_H
