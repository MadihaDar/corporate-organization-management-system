#include "Product.h"
#include "common.h"  
#include"JSONHandler.h"
#include <sstream>
#include <fstream>
#include "WarehouseManager.h"
#include <iostream>
#include"Sort.h"
#include "json.hpp"
using json = nlohmann::json;
#include"FinanceReporter.h"

WarehouseManager::WarehouseManager(ManufacturerArray* incomingArray)
    : incoming(incomingArray),router(20) {
    if (!incoming) incoming = new ManufacturerArray();
}

WarehouseManager::~WarehouseManager() {

    if (incoming) {
        incoming->clear();
        delete incoming;
        incoming = nullptr;
    }
    //deleting trucks and stores
    while (!trucks.isEmpty()) {
        Truck* t = trucks.get_head()->getData();
        delete t;
        trucks.deleteFromStart();
    }
    while (!stores.isEmpty()) {
        Store* s = stores.get_head()->getData();
        delete s;
        stores.deleteFromStart();
    }
}

void WarehouseManager::registerTruck(Truck* t) {
    trucks.append(t);
}

void WarehouseManager::registerStore(Store* s) {
    stores.append(s);
    router.addStore(s);
}


void WarehouseManager::receiveFromManufacturer(ManufacturerArray* arr) {
    if (!arr) {
        return;
    }
    for (int i = 0; i < arr->size(); ++i) {
        Product* p = arr->at(i);
        incoming->push(p);
    }
}

void WarehouseManager::indexIncomingToInventory() {
    for (int i = 0; i < incoming->size(); ++i) {
        Product* p = incoming->at(i);
        Product* found = inventory.search(p->getID());
        if (found) {found->increaseQuantity(p->getQuantity());}
        else {inventory.insert(p);}
    }
}



Truck* WarehouseManager::getFirstAvailableTruck() {
    SingleNode<Truck*>* n = trucks.get_head();

    while (n) {
        Truck* t = n->getData();
        if (t->get_current_units() < t->get_max_units()) {
            return t;
        }
        n = n->getNext();
    }
    return nullptr;
}

 //load deliveries function in truck for stores

void WarehouseManager::loadDeliveries(int perTruckLimit) {
    //if there are no stores ,no delivery
    if (stores.isEmpty()) {
        cout << "No stores registered so delivery is not possible\n";
        return;
    }

    int n = inventory.getSize(); //size of inventrory that has products
    if (n == 0) { //if inventory is empty then
        cout << "Inventory is empty so cannot schedule deliveries"<<endl;
        return;
    }

    // ###Converting BST tree to array for sorting#####
    //as products are in binary tree form so converting into in order array
    Product** prodArray = new Product*[n]; //pointer of product array
    int index = 0;
    inventory.inorder_traverse_to_array(prodArray, index);

    //user can select by their own desire which sorting algorithm to apply
    cout << "Choose sorting algorithm for scheduling:"<<endl;
    cout << "1. Bubble Sort"<<endl;
    cout << "2. Merge Sort"<<endl;
    cout << "3. Quick Sort"<<endl;
    cout << "Enter choice: "<<endl;
    int sortChoice;
    cin >> sortChoice;


    Sort sorter(prodArray, n); //array, size is passing
    switch (sortChoice) {
        case 1: sorter.bubbleSort(); break;
        case 2: sorter.mergeSort(); break;
        case 3: sorter.quickSort(); break;
        default: cout << "Invalid choice, default set to Bubble Sort.\n";
            sorter.bubbleSort(); break;
    }

    cout << "\nSorted Inventory for delivery:\n";
    for (int i = 0; i < n; i++) {
        if (!prodArray[i]) continue; //if any product is empty at some index so just skip that iteration
        cout << prodArray[i]->getID() << " | "
             << prodArray[i]->getName() << " | Qty: "
             << prodArray[i]->getQuantity() << "\n";
    }
    cout << "*******************************\n";
    cout << "*******************************\n";



    SingleNode<Store*>* sNode = stores.get_head(); //make a pointer to store
    while (sNode) {
        Store* store = sNode->getData(); //get store node data
        Truck* t = getFirstAvailableTruck(); //take 1st available truck
        if (!t) { //if no trucks are available
            cout << "No available trucks for store so delivery is not possible" << store->getID() << "\n";
            sNode = sNode->getNext();
            continue;
        }

        int loadedUnits = 0; //units that are loading on truck to check how much units are loadded
        for (int i = 0; i < n && loadedUnits < perTruckLimit; i++) { //this loop will load products until truck reaches its limit
            Product* p = prodArray[i];  //pointer to product array
            if (!p || p->getQuantity() <= 0) {
                continue; //skip if product is empty
            }

            int qtyToLoad;
            int remainingSpace = perTruckLimit - loadedUnits;
            if (p->getQuantity() < remainingSpace) {
                qtyToLoad = p->getQuantity();   //to load product's whole quantity
            } else {
                qtyToLoad = remainingSpace;     //to load remaining capacity of truck
            }
            //if product is load
            if(t->load(p, qtyToLoad)) {
                p->decreaseQuantity(qtyToLoad);
                loadedUnits += qtyToLoad;

                cout << "Loaded " << qtyToLoad << " of " << p->getID()
                     << " onto truck " << t->getID()
                     << " for store " << store->getID() << "\n";
            }

        }

        sNode = sNode->getNext(); //moving to the next store
    }

    delete[] prodArray;
}

   //plan routes functions
void WarehouseManager::planRoutesManually(int index1, int index2, int weight) {
    router.addEdge(index1, index2, weight);
}
void WarehouseManager::showRouteMatrix() {
    router.displayAdjMatrix();
}

void WarehouseManager::showStoreList() {
    router.listAllStores();
}
void WarehouseManager::runShortestPaths(int source) {
    router.runAndPrintDijkstra(source);
}

int WarehouseManager::getSCount() const {
   return  router.getStoreCount();
}

//this function is used to execute deliveries products to stores
//1st truck unloads to 1st store
//2nd truck unloads to 2nd truck and so on...

void WarehouseManager::executeDeliveries() {
    SingleNode<Truck*>* truckNode = trucks.get_head(); //starting from 1st truck node
    SingleNode<Store*>* storeNode = stores.get_head(); //starting from 1st store node

    while (truckNode && storeNode) {              //until both truck and store exists
        Truck* truck = truckNode->getData();      //taking out current truck data
        Store* store = storeNode->getData();      //taking out current store data

        cout << "Truck " << truck->getID() << " delivering to store "
             << store->getID() << " (" << store->getName() << ")\n";

        truck->printCargo(); //printing the items inside the truck before delivery

        //items linkedlist(truck) in cargoNode
        SingleNode<Truck::CargoItem*>* cargoNode = truck->cargo.get_head();
        while (cargoNode) {
            Truck::CargoItem* item = cargoNode->getData(); //this is product  data
            if (item && item->qty > 0) {
                int deliveredQty = item->qty; //delivered full quantity of this item
                //unloading product and updating its quantity too inside this function
                truck->unload(item->prod->getID(), deliveredQty);
                //taking confirmation from store that delivery is received
                store->receiveDelivery(item->prod->getID(), deliveredQty);
            }
            cargoNode = cargoNode->getNext(); //moving to next node in cargo list
        }

        cout << "Truck " << truck->getID() << " deliveries completed for store "
             << store->getID() << "\n\n";

        //move to next truck and next store
        truckNode = truckNode->getNext();
        storeNode = storeNode->getNext();
    }

    if (!truckNode && storeNode) {
        cout << "Trucks are not available.\n";
    } else if (truckNode && !storeNode) {
        cout << "Stores are finished.\n";
    }

    //deleting CargoItems from all trucks after deliveries
    truckNode = trucks.get_head();//iterating at trucks linked list
    while (truckNode) {
        Truck* truck = truckNode->getData();
        SingleNode<Truck::CargoItem*>* traverse_node = truck->cargo.get_head(); //iterate at cargo list in truck
        while (traverse_node) {
            SingleNode<Truck::CargoItem*>* nextNode = traverse_node->getNext();//saving next node address
            Truck::CargoItem* item = traverse_node->getData(); //this is product type data in this traverse node (traverse node - cargo - product)
            if (item) {
                delete item;
            } //deleting this cargo item
            delete traverse_node;          //deleting linked list node
            traverse_node = nextNode;
        }
        truckNode = truckNode->getNext(); //moving to next truck in list
    }
}


void WarehouseManager::showInventorySummary(ostream& out) {
    out << "=== Warehouse Inventory ===\n";
    inventory.inorder_traverse();
}

void WarehouseManager::showPendingDeliveries(ostream& out) {
    out << "=== Pending Deliveries ===\n";

    SingleNode<Truck*>* t = trucks.get_head();
    while (t) {
        Truck* tr = t->getData();
        out << "Truck " << tr->getID() << " units: " << tr->get_current_units() << "\n";
        t = t->getNext();
    }
}

     /*
void WarehouseManager::insertEmployeeFromHR(HR_Node * hr_root){
if(hr_root ==nullptr){
return;
}
insertEmployeeFromHR(hr_root->left);
employeeTree.insert(hr_root->employee);
insertEmployeeFromHR(hr_root->right);
}


*/

void WarehouseManager::addFinanceExpense(const string& date, double amount, const string& desc) {
    finance.addExpense(date, amount, desc);
}

void WarehouseManager::showFinanceReport(ostream& out) {
    finance.generateReport(out);
}

void WarehouseManager::saveFile(const char* filename) {
finance.saveFile(filename);

}























string WarehouseManager::serializeJSON() const {
    json j;

    //Trucks
    j["trucks"] = json::array();
    SingleNode<Truck*>* tNode = trucks.get_head();
    while (tNode) {
        j["trucks"].push_back(json::parse(tNode->getData()->serializeJSON()));
        tNode = tNode->getNext();
    }

    //Stores
    j["stores"] = json::array();
    SingleNode<Store*>* sNode = stores.get_head();
    while (sNode) {
        j["stores"].push_back(json::parse(sNode->getData()->serializeJSON()));
        sNode = sNode->getNext();
    }

    //Incoming Products
    j["incoming"] = json::array();
    for (int i = 0; i < incoming->size(); ++i) {
        j["incoming"].push_back(json::parse(incoming->at(i)->serializeJSON()));
    }

    //Finance
    j["finance"] = json::parse(finance.serializeJSON());

    return j.dump(4);
}
void WarehouseManager::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);
    trucks.clear();
    stores.clear();
    incoming->clear();

    //Trucks
    for (auto& t : j["trucks"]) {
        Truck* truck = new Truck();
        truck->deserializeJSON(t.dump());
        trucks.append(truck);
    }

    //Stores
    for (auto& s : j["stores"]) {
        Store* store = new Store();
        store->deserializeJSON(s.dump());
        stores.append(store);
    }

    //Incoming Products
    for (auto& p : j["incoming"]) {
        Product* prod = new Product();
        prod->deserializeJSON(p.dump());
        incoming->push(prod);
    }

    //Finance
    if (j.contains("finance")) {
        finance.deserializeJSON(j["finance"].dump());
    }
}
