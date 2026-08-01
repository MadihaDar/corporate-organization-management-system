#include "Product.h"
#include "common.h"  

#include "json.hpp"
using json = nlohmann::json;
#include "ManufacturerArray.h"
#include <iostream>

//constructor
ManufacturerArray::ManufacturerArray(int initialCap) : capacity(initialCap), length(0) {
    arr = new Product*[capacity];
}
//destructor
ManufacturerArray::~ManufacturerArray() {
    clear();
    delete[] arr;
}

void ManufacturerArray::resize(int newCap) {
    //if array size is full ,then this function resizes array
    Product** newArr = new Product*[newCap];
    for(int i=0;i<length;i++) {
        newArr[i]=arr[i];
    }
    delete[] arr;
    arr=newArr;
    capacity=newCap;
}

void ManufacturerArray::push(Product* p) {
    //it pushes product type object into array
    if(length==capacity) {
        resize(capacity*2);
    }
    arr[length++]=p;
}

Product* ManufacturerArray::at(int index) const {
    if(index<0 || index>=length) {
        return nullptr;
    }
    //it returns product address of particular index
    return arr[index];
}

Product** ManufacturerArray:: getArray() {
    return arr; //it returns address of arr
}


int ManufacturerArray::size() const {
    return length; //it returns size of array
}
void ManufacturerArray::clear() {
    length=0; //it clears array
}












string ManufacturerArray::serializeJSON() const {
    json j;
    j["count"] = size();
    j["products"] = json::array();

    for (int i = 0; i < size(); ++i) {
        j["products"].push_back(json::parse(arr[i]->serializeJSON()));
    }

    return j.dump(4);
}


void ManufacturerArray::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);
    clear();

    if (!j.contains("products")) return;

    for (auto& item : j["products"]) {
        Product* p = new Product();
        p->deserializeJSON(item.dump());
        push(p);
    }
}