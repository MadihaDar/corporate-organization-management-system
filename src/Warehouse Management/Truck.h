//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef TRUCK_H
#define TRUCK_H
#include <string>
#include <iostream>
#include "common.h"
using namespace std;
#include <string>
#include "Product.h"
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;


class Truck {
private:
    string truckID;
    int max_units;
    double max_weight;
    int current_units;
    double current_weight;
public:
    struct CargoItem {
        Product* prod;
        int qty;
        CargoItem(Product* p=nullptr,int q=0): prod(p), qty(q) {}
    };

    SLinkedList<CargoItem*> cargo;
public:
    //constructors and destructor
    Truck();
    Truck(const string& id, int maxU, double maxW);
    ~Truck();

    bool load(Product* p, int qty);
    bool unload(const string& productID, int qty); //unloading  by productID
    void clearCargo();
     //getters
    int get_current_units() const;
    double get_current_weight() const;
    string getID() const;
    int get_max_units(){return max_units;}
    void printCargo() const;
    SLinkedList<CargoItem*>& getCargo() {
        return cargo;
    }



    string serializeJSON() const;
    void deserializeJSON(const string& json);
};

#endif //TRUCK_H
