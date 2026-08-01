#ifndef FRANCHISE_H
#define FRANCHISE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "json.hpp"
#include "template.h"
using json = nlohmann::json;
#include <fstream>

using namespace std;

// MUNIBA'S STRUCT BELOW

struct Product
{
private:
    string productID;
    int sales;
    float price;

public:
    // constructors
    Product() : productID(""), sales(0), price(0.0) {}
    Product(const string &id, int sales, float price)
        : productID(id), sales(sales), price(price) {}

    void setProductID(string productID) { this->productID = productID; }
    string getProductID() const { return productID; }

    void setProductSales(int sales) { this->sales = sales; }
    int getProductSales() const { return sales; }

    void setProductPrice(float price) { this->price = price; }
    float getProductPrice() const { return price; }

    json toJSON() const {
        return {
            {"productID", productID},
            {"quantity", sales},
            {"price", price}
        };
    }

    static Product fromJSON(const json& j) {
        return Product(j.at("productID"), j.at("quantity"), j.at("price"));
    }
};

class Purchase {
    string purchaseID;
    string date;
    Node<Product>* items;  // linked list of products
    float totalPaid;

public:
    Purchase();
    Purchase(const string& id, const string& d);
    Purchase(const Purchase& other);
    ~Purchase();

    string getPurchaseID() const;
    string getDate() const;
    Node<Product>* getItems() const;
    float getTotalPrice() const;
    void addItem(const Product& p, int quantity);
    json toJSON() const;
    static Purchase fromJSON(const json& j);
};

class Customer {
    string customerID;
    Node<Purchase>* purchaseList;

public:
    Customer(string id);
    Customer(const Customer& other);
    ~Customer();
    string getCustomerID() const;
    void setID(const string& id);
    Node<Purchase>* getPurchaseList() const;
    void addPurchase(const Purchase& p);

    json purchasesToJSON() const;
    json toJSON() const;
    static Customer fromJSON(const json& j);
};

class InventoryNode {
    Product* product;
    int quantity;
    int threshold;

    InventoryNode* left;
    InventoryNode* right;
public:
    InventoryNode(Product* p, int q, int thresh);
    
    Product* getProduct();
    int getQuantity();
    int getThreshold();
    InventoryNode* getLeft();
    InventoryNode* getRight();

    void setLeft(InventoryNode* l);
    void setRight(InventoryNode* r);
    void setQuantity(int q);
    void setThreshold(int t);

    bool needsRestock();
    void displayProductDetails();

    ~InventoryNode();
};

class FranchiseInventory {
    InventoryNode* root;
    struct SortNode {   // node for a linked list of products used for sorting by price
        Product* product;
        int quantity;
        int threshold;
        SortNode* next;
        SortNode(Product* p, int q) : product(p), quantity(q), next(nullptr) {}
        SortNode(Product* p, int q, int t) : product(p), quantity(q), threshold(t), next(nullptr) {}
    };
public:
    FranchiseInventory();

    void addProduct(Product* p, int quantity, int threshold);
    InventoryNode* search(const string& productID);
    void updateStock(const string& id, int soldQuantity);

    void displayInventoryHelper(InventoryNode* node);
    void displayInventory();
    void checkLowStockHelper(InventoryNode* node, bool& found);
    void checkLowStock();

    void buildListFromBST(InventoryNode* node, SortNode*& head);
    
    // sorts a linked list of SortNode by product price in ascending order using insertion sort
    SortNode* sortByPrice(SortNode* head);
    void displaySortedProductsByPrice();

    SortNode* splitList(SortNode* head);
    SortNode* mergeBySales(SortNode* a, SortNode* b);
    SortNode* mergeSortBySales(SortNode* head);
    void displaySortedProductsBySales();
    

    SortNode* sortByThreshold(SortNode* head);
    void displaySortedProductsByThreshold();


    void helperFunction(InventoryNode* node, json& arr) const;
    json toJSON() const;
    void fromJSON(const json& j);

    void deleteBST(InventoryNode* node);
    ~FranchiseInventory();
};

class Franchise {
    string franchiseID;
    string location;
    bool isActive;

    FranchiseInventory inv;
    Queue<string> customerQueue;
    Stack<Product> billingStack;
    Node<Purchase>* purchaseHistory;
    Node<Customer>* customersHead;  // for storing customers and their purchases

    Graph<string> purchaseGraph;
public:
    Franchise(string id = "", string loc = "") : franchiseID(id), location(loc), isActive(true), purchaseHistory(nullptr), customersHead(nullptr) {}
    void activate();
    void deactivate();
    bool getStatus();

    void addProduct(Product* p, int quantity, int threshold);
    
    void sellProduct(const string& productID, int quantity);
    void showInventory();

    void enqueueCustomer(const string& customerID);
    void dequeueCustomer();

    void addItemToBill(Product* p, int quantity);

    Customer* getCustomerRecord(const string& customerID);
    Customer* addCustomerifNotPresent(const string& customerID);

    void addPurchaseRecord(const Purchase& p, const string& customerID);
    void displayPurchaseHistory();

    FranchiseInventory& getInventory();
    void displayFranchises();

    void showFrequentlyBoughtWith(const string& productID);
    void displayPurchaseGraph();

    json customersToJSON() const;
    void customersFromJSON(const json& j);
    void loadCustomersFromFile(const string& filename);
    void saveCustomersToFile(const string& filename);

    json toJSON() const;
    void fromJSON(const json& j);
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);

    ~Franchise();
};

#endif
