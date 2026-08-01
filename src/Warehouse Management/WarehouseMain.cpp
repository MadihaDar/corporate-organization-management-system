#include <iostream>
#include <string>
#include <fstream>
#include "WarehouseManager.h"
#include "Truck.h"
#include "Store.h"
#include "ManufacturerArray.h"
#include "Product.h"
#include "RoutePlanner.h"
#include "FinanceReporter.h"
#include "JSONHandler.h"
#include "common.h"
#include "../Colors.h"
#include "json.hpp"
using json = nlohmann::json;
#include "Sort.h"

using namespace std;
extern void clearInput();

void showMainMenu() {
    cout << CLRSCR;
    cout << BOLD << CYAN << "=================================================\n";
    cout << "          WAREHOUSE MANAGEMENT SYSTEM          \n";
    cout << "=================================================" << RESET << "\n";
    cout << GREEN << "1. " << RESET << "Register Truck\n";
    cout << GREEN << "2. " << RESET << "Register Store\n";
    cout << GREEN << "3. " << RESET << "Receive Products from Manufacturer\n";
    cout << GREEN << "4. " << RESET << "Transfering Incoming Products to Inventory\n";
    cout << GREEN << "5. " << RESET << "Show Inventory Summary\n";
    cout << GREEN << "6. " << RESET << "Load Deliveries in Trucks\n";
    cout << GREEN << "7. " << RESET << "Add Edge between Two stores\n";
    cout << GREEN << "8. " << RESET << "Show Route Adjacency Matrix\n";
    cout << GREEN << "9. " << RESET << "Show Stores List\n";
    cout << GREEN << "10." << RESET << "Run Dijkstra To Find Shortest Paths from WareHouse\n";
    cout << GREEN << "11." << RESET << "Execute Deliveries\n";
    cout << GREEN << "12." << RESET << "Save Warehouse State\n";
    cout << GREEN << "13." << RESET << "Load Warehouse State\n";
    cout << GREEN << "14." << RESET << "Add Finance Expense\n";
    cout << GREEN << "15." << RESET << "Show Finance Report\n";
    cout << GREEN << "16." << RESET << "Save Finance File\n";
    cout << "\n" << RED << "0. Exit Module" << RESET << "\n";
    cout << "Enter choice: ";
}

void warehouseMain() {

    WarehouseManager warehouse;
    ManufacturerArray incomingProducts;

    int choice;
    do {
        showMainMenu();
        cin >> choice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        cin.ignore();

        switch(choice) {
            case 1: {
                cout << CLRSCR;
                string truck_id;
                int maxU; double max_Weight;
                cout << "Enter Truck ID: "; cin >> truck_id;
                cout << "Enter Max Units: "; cin >> maxU;
                cout << "Enter Max Weight: "; cin >>max_Weight;
                Truck* t = new Truck(truck_id, maxU, max_Weight);
                warehouse.registerTruck(t);
                cout << GREEN << "Truck registered successfully." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                cout << CLRSCR;
                string store_id, store_name;
                cout << "Enter Store ID: ";
                cin >> store_id;
                cout << "Enter Store Name: ";
                cin.ignore();
                getline(cin, store_name);

                Store* s = new Store(store_id, store_name);
                warehouse.registerStore(s);
                cout << GREEN << "Store registered successfully." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 3: {
                cout << CLRSCR;
                int n;
                cout << "Enter number of products from manufacturer: ";
                cin >> n;
                for(int i=0;i<n;i++){
                    string product_id, product_name; int qty; double weight, price;
                    cout << "Product ID: ";
                    cin >>product_id;
                    cout << "Product Name: ";
                    cin.ignore();
                    getline(cin,product_name);
                    cout << "Quantity: ";
                    cin >> qty;
                    cout << "Weight: ";
                    cin >> weight;
                    cout << "Price: ";
                    cin >> price;
                    Product* p = new Product(product_id, product_name, qty, weight, price);
                    incomingProducts.push(p);
                }
                warehouse.receiveFromManufacturer(&incomingProducts);
                cout << GREEN << "Products received successfully." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                cout << CLRSCR;
                warehouse.indexIncomingToInventory();
                cout << GREEN << "Inventory indexed." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 5: {
                cout << CLRSCR;
                warehouse.showInventorySummary(cout);
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 6: {
                 cout << CLRSCR;
                 int perTruck;

                cout << "Enter max products per truck: ";
                cin >> perTruck;
                warehouse.loadDeliveries(perTruck);
                cout << GREEN << "Deliveries loaded in truck." << RESET << "\n";

                //expense adding
                string date, description; double amount;
                cout << "Enter date: ";
                cin.ignore();
                getline(cin, date);
                cout << "Enter description: ";
                getline(cin, description);
                cout << "Enter amount: ";
                cin >> amount;
                warehouse.addFinanceExpense(date, amount, description);
                cout << GREEN << "Expense added." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            case 7: {
                cout << CLRSCR;
                int u, v, w;
                int count = warehouse.getSCount();
                if (count < 2) {
                    cout << RED << "At least TWO stores required to add an edge." << RESET << "\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                cout << "You currently have " << count << " stores.\n";
                cout << "Valid store indices: 0 to " << count - 1 << "\n";
                int maxEdges = (count * (count - 1)) / 2;
                cout << "Maximum possible undirected edges = " << maxEdges << "\n";
                cout << "Enter only valid edges using these indices.\n\n";

                cout << "Enter Source Store Index: ";
                cin >> u;
                cout << "Enter Destination Store Index: ";
                cin >> v;
                cout << "Enter Distance (km): ";
                cin >> w;
                warehouse.planRoutesManually(u, v, w);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 8: { //show adjacency matrix
                cout << CLRSCR;
                warehouse.showRouteMatrix();
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 9: { //show store list
                cout << CLRSCR;
                warehouse.showStoreList();
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 10: { //run Dijkstra to tell the shortst path from warehouse to all stores
                cout << CLRSCR;
                int source;
                cout << "Enter Source Store Index for shortest paths: ";
                cin >> source;
                warehouse.runShortestPaths(source);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            case 11: {
                cout << CLRSCR;
                warehouse.executeDeliveries();
                cout << GREEN << "Deliveries executed." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 12: {
                JSONHandler::save("src/BSSE24027/warehouse.json", warehouse);
                cout << GREEN << "Warehouse state saved." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 13: {
               JSONHandler::load("src/BSSE24027/warehouse.json", warehouse);
                cout << GREEN << "Warehouse state loaded." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 14: {
                cout << CLRSCR;
                string date, description; double amount;
                cout << "Enter date: ";
                cin.ignore();
                getline(cin, date);
                cout << "Enter description: ";
                getline(cin, description);
                cout << "Enter amount: ";
                cin >> amount;
                warehouse.addFinanceExpense(date, amount, description);
                cout << GREEN << "Expense added." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 15: {
                cout << CLRSCR;
                warehouse.showFinanceReport(cout);
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 16: {
                warehouse.saveFile("finance_report.txt");
                cout << GREEN << "Finance report saved to file." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 0:
                cout << GREEN << "Exiting Warehouse Module..." << RESET << "\n";
                break;
            default:
                cout << RED << "Invalid choice!" << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.get();
        }
        cout << "\n";
    } while(choice != 0);

    return;
}