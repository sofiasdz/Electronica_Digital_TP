#include <Arduino.h>
#include <ArduinoJson.h>

// Tu respuesta en formato JSON como una cadena
const char* jsonResponse = "{\"works\":true,\"beingRepaired\":false,\"income\":1000,\"products\":[{\"product\":{\"name\":\"Product 1\",\"price\":20,\"customId\":1},\"quantity\":5},{\"product\":{\"name\":\"Product 2\",\"price\":30,\"customId\":2},\"quantity\":3}],\"customId\":123}";

void parseJsonResponse(const char* jsonResponse) {
    const size_t capacity = JSON_OBJECT_SIZE(5) + JSON_ARRAY_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 180;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, jsonResponse);

    if (error) {
        Serial.print(F("Error al parsear JSON: "));
        Serial.println(error.c_str());
        return;
    }

    // Acceder a los valores del documento JSON
    bool works = doc["works"];
    bool beingRepaired = doc["beingRepaired"];
    int income = doc["income"];
    int customId = doc["customId"];

    Serial.println("Valores extraídos del JSON:");
    Serial.println("Works: " + String(works));
    Serial.println("Being Repaired: " + String(beingRepaired));
    Serial.println("Income: " + String(income));
    Serial.println("Custom ID: " + String(customId));

    // Acceder a la matriz de productos
    JsonArray products = doc["products"];
    Serial.println("Lista de productos:");
    for (JsonObject product : products) {
        const char* productName = product["product"]["name"];
        int productPrice = product["product"]["price"];
        int productCustomId = product["product"]["customId"];
        int productQuantity = product["quantity"];

        Serial.println("Nombre: " + String(productName));
        Serial.println("Precio: " + String(productPrice));
        Serial.println("Custom ID del Producto: " + String(productCustomId));
        Serial.println("Cantidad: " + String(productQuantity));
        Serial.println("------");
    }
}





