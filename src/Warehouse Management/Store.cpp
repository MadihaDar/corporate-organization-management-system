#include "Product.h"
#include "common.h"  

#include "json.hpp"
using json = nlohmann::json;

#include "Store.h"
#include <iostream>


//default constructor
Store::Store() : storeID(""), name("") {}
//parametrised constructor
Store::Store(const string& id, const string& n)
    : storeID(id), name(n) {}
//getters
string Store::getID() const { return storeID; }
string Store::getName() const { return name; }



void Store::receiveDelivery(const string& productID, int qty) {
    cout << "Delivered " << qty << " units of Product " << productID
         << " to Store " << storeID << " (" << name << ")"<<endl;
}



string Store::serializeJSON() const {
    json j;
    j["storeID"] = storeID;
    j["name"] = name;
    return j.dump();
}


void Store::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);
    if (j.contains("storeID")) storeID = j["storeID"].get<string>();
    if (j.contains("name")) name = j["name"].get<string>();
}
