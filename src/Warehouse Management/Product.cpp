#include "Product.h"
#include "common.h"
#include "JSONHandler.h"
//Product.cpp
#include "Product.h"
#include "json.hpp"

using json = nlohmann::json;


string Product::serializeJSON() const {
    json j;
    j["productID"] = productID;
    j["name"] = name;
    j["quantity"] = quantity;
    j["weight"] = weight;
    j["price"] = price;

    return j.dump(4);
}

void Product::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);

    if (j.contains("productID")) j["productID"].get_to(productID);
    if (j.contains("name")) j["name"].get_to(name);
    if (j.contains("quantity")) j["quantity"].get_to(quantity);
    if (j.contains("weight")) j["weight"].get_to(weight);
    if (j.contains("price")) j["price"].get_to(price);
}