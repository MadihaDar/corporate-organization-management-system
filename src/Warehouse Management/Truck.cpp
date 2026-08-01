#include "Product.h"
#include "common.h"
#include <string>
#include "Product.h"
#include "common.h"
#include "Truck.h"
#include "Truck.h"
#include "json.hpp"

using json = nlohmann::json;

#include <iostream>

//constructors
Truck::Truck(){}
Truck::Truck(const string& id, int maxU, double maxW)
    : truckID(id), max_units(maxU), max_weight(maxW), current_units(0), current_weight(0.0){}
//destructor
Truck::~Truck() { clearCargo(); }

bool Truck::load(Product* p, int qty) {
    if (!p || qty <= 0) {
        return false;
    }

    double addWeight = p->getWeight() * qty;//it gives weight of all products of that 1 type
    if (current_units + qty > max_units) {
        return false; //if quantity increases beyond limit of truck max_units
    }
    if (current_weight + addWeight > max_weight) {
        return false;//if weight increases beyond limit of truck max_weight
    }

    CargoItem* item = new CargoItem(p, qty);  //make a object of cargoitem type
    cargo.append(item);                       //add it in linked list
    current_units += qty;                     //update current units
    current_weight += addWeight;              //update current weight
    return true;
}


//this function unloads the products from the truck
bool Truck::unload(const string& targetProductID, int removeQuantity) {
    if (removeQuantity <= 0) { //check if quantity is invalid
        return false;
    }
    //start checking from the 1st cargo item in truck
    SingleNode<CargoItem*>* currentNode = cargo.get_head();
    //to check through all cargo item nodes
    while (currentNode) {
        CargoItem* currentItem= currentNode->getData();//extract data
        if (currentItem->prod->getID() == targetProductID) { //target id is found
            if (currentItem->qty <removeQuantity) {
                return false;  //check its quantity , if we dont have then return false
            }
            currentItem->qty -=removeQuantity; //update quantity from truck and cargo item
            current_units -= removeQuantity;

            return true; // when everything is done return true
        }
        currentNode= currentNode->getNext(); //then mover to the next node of cargo item
    }

    return false; //if id not found,return false
}


void Truck::clearCargo() {
    while (!cargo.isEmpty()) {
        CargoItem* trav = cargo.get_head()->getData();
       delete trav;
        cargo.deleteFromStart();
    }
    current_units = 0;
    current_weight = 0.0;
}

int Truck::get_current_units() const { return current_units; }
double Truck::get_current_weight() const { return current_weight; }
string Truck::getID() const { return truckID; }

void Truck::printCargo() const {
    SingleNode<CargoItem*>* n = cargo.get_head();
    while (n) {
        CargoItem* trav = n->getData();
        if (trav && trav->prod)
            cout << "Product: " << trav->prod->getID() << " Qty: " << trav->qty << endl;
        n = n->getNext();
    }
}







string Truck::serializeJSON() const {
    json j;

    j["truckID"] = truckID;
    j["max_units"] = max_units;
    j["max_weight"] = max_weight;
    j["current_units"] = current_units;
    j["current_weight"] = current_weight;


    j["cargo"] = json::array();
    SingleNode<CargoItem*>* node = cargo.get_head();
    while (node) {
        CargoItem* item = node->getData();
        if (item && item->prod) {
            j["cargo"].push_back({
                {"productID", item->prod->getID()},
                {"qty", item->qty}
            });
        }
        node = node->getNext();
    }

    return j.dump(4);
}


void Truck::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);

    truckID = j.value("truckID", "");
    max_units = j.value("max_units", 0);
    max_weight = j.value("max_weight", 0.0);
    current_units = j.value("current_units", 0);
    current_weight = j.value("current_weight", 0.0);
    clearCargo();
    for (auto& c : j["cargo"]) {
        string pid = c.value("productID", "");
        int qty = c.value("qty", 0);


        CargoItem* item = new CargoItem(nullptr, qty);
        cargo.append(item);
    }
}
