#include <iostream>
#include "franchise.h"
#include "../Colors.h"

extern void clearInput();

int getValidatedInt(int min, int max) {
    int val;
    while(true) {
        cin >> val;
        if(cin.fail() || val < min || val > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input! Enter a number (" << min << "-" << max << "): " << RESET;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

char getYesNo() {
    char c;
    while(true) {
        cin >> c;
        if(c == 'y' || c == 'n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return c;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');;
        cout << RED << "Enter 'y' or 'n': " << RESET;
    }
}

void mainMenu(Franchise& f) {
    int choice;
    do {
        cout << CLRSCR;
        cout << BOLD << MAGENTA << "========================================" << endl;
        cout << "      FRANCHISE MANAGEMENT MENU" << endl;
        cout << "========================================" << RESET << endl;
        cout << GREEN << "1." << RESET << " Inventory Management" << endl;
        cout << GREEN << "2." << RESET << " Customer Service" << endl;
        cout << GREEN << "3." << RESET << " Show Purchase History" << endl;
        cout << GREEN << "4." << RESET << " Show Frequently Bought Together" << endl;
        cout << RED << "0. Exit Module" << RESET << endl;
        cout << "Enter choice: ";
        choice = getValidatedInt(0, 4);

        switch(choice) {
            case 1: { // inventory menu
                int invChoice;
                do {
                    cout << CLRSCR;
                    cout << BOLD << YELLOW << "--- Inventory Menu ---" << RESET << endl;
                    cout << GREEN << "1." << RESET << " Display Inventory" << endl;
                    cout << GREEN << "2." << RESET << " Add Product" << endl;
                    cout << GREEN << "3." << RESET << " Check Low Stock" << endl;
                    cout << GREEN << "4." << RESET << " Display Products Sorted By Price" << endl;
                    cout << GREEN << "5." << RESET << " Display Products Sorted By Threshold" << endl;
                    cout << GREEN << "6." << RESET << " Display Products Sorted By Sales" << endl;
                    cout << RED << "0. Back" << RESET << endl;
                    cout << "Choice: ";
                    invChoice = getValidatedInt(0, 6);

                    switch(invChoice) {
                        case 1:
                            cout << CLRSCR;
                            f.showInventory();
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        case 2: {
                            cout << CLRSCR;
                            string id;
                            cout << "Product ID: "; 
                            cin >> id;

                            // Check if product exists
                            InventoryNode* existing = f.getInventory().search(id);
                            if(existing) {
                                cout << YELLOW << "Product ID " << id << " already exists." << RESET << endl;
                                cout << "Do you want to update quantity and threshold instead? (y/n): ";
                    
                                if(getYesNo() == 'y') {
                                int newQty, newThresh;
                                cout << "Enter new quantity: "; cin >> newQty;
                                cout << "Enter new threshold: "; cin >> newThresh;

                                existing->setQuantity(newQty);
                                existing->setThreshold(newThresh);
                                cout << GREEN << "Product updated successfully." << RESET << endl;
                            } else {
                                cout << "No changes made." << endl;
                            }
                            cout << "Press Enter to continue...";
                            cin.ignore();
                            cin.get();
                            break; // exit this case
                        }

                        // If product doesn't exist, ask for details
                        float price;
                        int qty, thresh;
                        cout << "Price: "; cin >> price;
                        cout << "Quantity: "; cin >> qty;
                        cout << "Threshold: "; cin >> thresh;

                        Product* p = new Product(id, 0, price);
                        f.addProduct(p, qty, thresh);
                        cout << GREEN << "Product Added!" << RESET << endl;
                        cout << "Press Enter to continue...";
                        cin.ignore();
                        cin.get();
                        break;
                    }
                        case 3:
                            cout << CLRSCR;
                            f.getInventory().checkLowStock();
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        
                        case 4: {
                            cout << CLRSCR;
                            f.getInventory().displaySortedProductsByPrice();
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        }
                        case 5: {
                            cout << CLRSCR;
                            f.getInventory().displaySortedProductsByThreshold();
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        }
                        case 6: {
                            cout << CLRSCR;
                            f.getInventory().displaySortedProductsBySales();
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        }
                    }
                } while(invChoice != 0);
                break;
            }

            case 2: { // customer service
                    cout << CLRSCR;
                    string customerID;
                    cout << "Customer ID: ";
                    cin >> customerID;

                    f.enqueueCustomer(customerID);
                    cout << GREEN << "Customer " << customerID << " added to queue." << RESET << endl;

                    // ask for date once per customer checkout
                    string purchaseDate;
                    cout << "Enter purchase date (YYYY-MM-DD): ";
                    cin >> purchaseDate;

                    // create a single purchase for this customer
                    string purchaseID; 
                    cout << "Purchase ID: ";
                    cin >> purchaseID;
                    Purchase purchaseRecord(purchaseID, purchaseDate);

                    // selling loop
                    char more = 'y';
                    while(more == 'y' || more == 'Y') {
                        string prodID;
                        int qty;
                        cout << "Enter product ID to buy: ";
                        cin >> prodID;
                        cout << "Quantity: ";
                        cin >> qty;

                        f.sellProduct(prodID, qty);

                        // add product to purchase
                        InventoryNode* node = f.getInventory().search(prodID);
                        if(node) {
                            Product* prod = node->getProduct();
                            purchaseRecord.addItem(*prod, qty);
                        }

                        cout << "Does the customer want more products? (y/n): ";
                        cin >> more;
                    }

                    //now add the purchase to customer record
                    f.addPurchaseRecord(purchaseRecord, customerID);
                    
                    //save both inventory and customers
                    f.saveToFile("franchise_data.json");
                    f.saveCustomersToFile("customers.json");

                    f.dequeueCustomer();
                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
                break;
            }

            case 3:
                cout << CLRSCR;
                f.displayPurchaseHistory();
                cout << "Press Enter to continue...";
                cin.get();
                break;

            case 4: {
                cout << CLRSCR;
                string pid;
                cout << "Enter Product ID: "; cin >> pid;
                f.showFrequentlyBoughtWith(pid);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        }

    } while(choice != 0);
}

void franchiseMain() {
    Franchise f("F001", "Lahore");

    // load inventory and customer data 
    f.loadFromFile("franchise_data.json");
    f.loadCustomersFromFile("customers.json");

    mainMenu(f);

    // save everything before exit
    f.saveToFile("franchise_data.json");
    f.saveCustomersToFile("customers.json");

    cout << GREEN << "Exiting Franchise Module. Data saved." << RESET << endl;
    return;
}
