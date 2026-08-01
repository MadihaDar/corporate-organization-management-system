//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef MANUFACTURERARRAY_H
#define MANUFACTURERARRAY_H
#include <string>
#include <iostream>
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
#include "Product.h"

class ManufacturerArray {
private:
    Product** arr;
    int capacity;
    int length;
    void resize(int newCap);
public:
    ManufacturerArray(int initialCap = 16);
    ~ManufacturerArray();
    void push(Product* p);
    Product* at(int index) const;
    Product** getArray() ;
    int size() const;
    void clear();
    string serializeJSON() const;
    void deserializeJSON(const string& json);
};

#endif //MANUFACTURERARRAY_H
