//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <iostream>
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

#include <string>

class Product {
private:
    //data members
    string productID;
    string name;
    int quantity;
    double weight;
    double price;
public:
    //constructors
    Product() : productID(""), name(""), quantity(0), weight(0), price(0) {}
    Product(const string& id, const string& n, int q, double w, double p)
        : productID(id), name(n), quantity(q), weight(w), price(p) {}
    //destructor
    ~Product() {}

    string getID() const { return productID; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getWeight() const { return weight; }
    double getPrice() const { return price; }
    void setQuantity(int q) { quantity = q; }
    void increaseQuantity(int q) { quantity += q; }
    void decreaseQuantity(int q) { quantity -= q; }

    string serializeJSON() const;
    void deserializeJSON(const string& jsonStr);

};



#endif //PRODUCT_H
