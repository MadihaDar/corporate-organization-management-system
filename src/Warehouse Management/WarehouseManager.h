//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef WAREHOUSEMANAGER_H
#define WAREHOUSEMANAGER_H
#include <string>
#include <iostream>
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;


#include "Product.h"
#include "BST.h"
#include "Truck.h"
#include "Store.h"
#include "ManufacturerArray.h"
#include "RoutePlanner.h"
#include "FinanceReporter.h"
#include"Sort.h"
#include "common.h"
#include <string>

class WarehouseManager {
private:
    ManufacturerArray* incoming;
    BST<Product*> inventory;
    SLinkedList<Truck*> trucks;
    SLinkedList<Store*> stores;
    RoutePlanner router;
    FinanceReporter finance;
    //BST<*HR_Node>employeeTree;


public:
    WarehouseManager(ManufacturerArray* incomingArray = nullptr);
    ~WarehouseManager();

    void registerTruck(Truck* t);
    void registerStore(Store* s);

    void receiveFromManufacturer(ManufacturerArray* arr); //aggregation is happening here
    void indexIncomingToInventory();
    void sortProductsInArray(ManufacturerArray* arr, const string& field, const string& type="quick");

    void loadDeliveries( int perTruckLimit);


    void executeDeliveries();

    void showInventorySummary(ostream& out);
    void showPendingDeliveries(ostream& out);
    //for connection with hr module (optional)
    //void insertEmployeeFromHR(HR_Node * hr_root);

    Truck* getFirstAvailableTruck();
    void addFinanceExpense(const string& date, double amount, const string& desc);
    void showFinanceReport(ostream& out);
    void saveFile(const char* filename);

    void planRoutesManually(int index1, int index2, int weight);
    void showRouteMatrix();
    void showStoreList();
    void runShortestPaths(int source);
    int getSCount() const;
    void saveWarehouse(const string& filename);
    string serializeJSON() const;
    void deserializeJSON(const string& json);
};


#endif //WAREHOUSEMANAGER_H
