//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef STORE_H
#define STORE_H
#include <string>
#include <iostream>
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

#include <string>
class Store {
private:
    string storeID;
    string name;


public:
    Store();
    Store(const string& id, const string& n);
    ~Store() {}

    string getID() const;
    string getName() const;



    void receiveDelivery(const string& productID, int qty);


    string serializeJSON() const;
    void deserializeJSON(const string& jsonStr);

};


#endif //STORE_H


