#include "machine.h"
#include <ArduinoJson.h>
// ...

Machine::Machine(bool works, bool beingRepaired, double income, const std::vector<ProductQuantity>& products, int customId)
    : works(works), beingRepaired(beingRepaired), income(income), products(products), customId(customId) {}

void Machine::initializeFromJSON(const std::string& jsonString)
{
    StaticJsonDocument<512> doc; // Adjust the size based on your needs

    DeserializationError error = deserializeJson(doc, jsonString);
    if (error)
    {
        Serial.println(F("Failed to read file, using default configuration"));
        return;
    }

    works = doc["works"];
    beingRepaired = doc["beingRepaired"];
    income = doc["income"];
    customId = doc["customId"];

    products.clear();
    JsonArray productsArray = doc["products"];
    for (JsonObject productObject : productsArray)
    {
        ProductQuantity pq;
        pq.product.name = std::string(productObject["product"]["name"].as<const char*>());
        pq.product.price = productObject["product"]["price"];
        pq.product.customId = productObject["product"]["customId"];
        pq.quantity = productObject["quantity"];
        products.push_back(pq);
    }
}
std::vector<std::string> Machine::getProductNames() const
{
    std::vector<std::string> productNames;
    for (const ProductQuantity& pq : products)
    {
        productNames.push_back(pq.product.name);
    }
    return productNames;
}

// Implement the update method
void Machine::update(const Machine& otherMachine) {
    works = otherMachine.works;
    beingRepaired = otherMachine.beingRepaired;
    income = otherMachine.income;
    products = otherMachine.products;  // This assumes a simple overwrite; adjust as needed
    // customId is not updated to prevent accidental uniqueness violation
}