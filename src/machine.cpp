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

// Implement the update method
void Machine::update(const Machine& otherMachine) {
    works = otherMachine.works;
    beingRepaired = otherMachine.beingRepaired;
    income = otherMachine.income;
    products = otherMachine.products;  // This assumes a simple overwrite; adjust as needed
    // customId is not updated to prevent accidental uniqueness violation
}

void Machine::workingInMachine(){
    works = false;
    beingRepaired = true;
}

void Machine::readyMachine(){
    works = true;
    beingRepaired = false;
}

void Machine::deleteMachine(){
    products.clear();

    // Reset other variables to default values
    works = false;
    beingRepaired = false;
    income = 0.0;
    customId = 0;  // You may set it to a specific default value

}

void Machine::productDelete(const std::string& msg){
        // Parse the JSON message to get the productId
    DynamicJsonDocument doc(256); // Adjust the size based on your needs
    deserializeJson(doc, msg);

    int productId = doc["productId"];

    // Use std::remove_if to remove the product with the matching productId
    products.erase(
        std::remove_if(products.begin(), products.end(),
                       [productId](const ProductQuantity& pq) {
                           return pq.product.customId == productId;
                       }),
        products.end()
    );
}

void Machine::adjustPrice(const std::string& msg){
    // Parse the JSON message to get the new product details
    DynamicJsonDocument doc(256); // Adjust the size based on your needs
    deserializeJson(doc, msg);

    Product newProduct;
    newProduct.name = doc["name"].as<std::string>();
    newProduct.price = doc["price"];
    newProduct.customId = doc["customId"];

    // Find the iterator for the product with the matching productId
    auto it = std::find_if(products.begin(), products.end(),
                           [newProduct](const ProductQuantity& pq) {
                               return pq.product.customId == newProduct.customId;
                           });

    if (it != products.end()) {
        // Replace the existing product with the new one
        it->product = newProduct;
    } else {
        // Handle the case where the product with the specified customId is not found
        // You may choose to add the new product to the products vector in this case
        // or take any other appropriate action.
        std::cout << "Product with customId " << newProduct.customId << " not found." << std::endl;
    }
}

