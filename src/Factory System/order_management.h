#pragma once
#include <utility>

#include "production.h"
#include "monitoring.h"

// testing stuff

struct Product {
private:
    string productID;
    int sales;
    float price;

public:
    // constructors
    Product() : productID(""), sales(0), price(0.0) {
    }

    Product(string id, int sales, float price)
        : productID(std::move(id)), sales(sales), price(price) {
    }

    void setProductID(string productID) { this->productID = std::move(productID); }
    string getProductID() const { return productID; }

    void setProductSales(int sales) { this->sales = sales; }
    int getProductSales() const { return sales; }

    void setProductPrice(float price) { this->price = price; }
    float getProductPrice() const { return price; }
};

class Order {
    int orderID;
    int priority;
    Product *product;

public:
    // Constructors
    Order() {
        orderID = 0;
        priority = 0;
        product = nullptr;
    }

    Order(int id, int prio) : orderID(id), priority(prio) {
    }

    // Getters
    int getOrderID() const { return orderID; }
    int getPriority() const { return priority; }
    void setProduct(Product *prod) { product = prod; }

    Product *getProduct() {
        return product;
    }

    // Setters
    void setOrderID(int id) { orderID = id; }
    void setPriority(int prio) { priority = prio; }
};

// ============================================================================
// 9. ORDER PROCESSOR CLASS
// ============================================================================
class OrderProcessor {
    ProductionGraph *workflow;
    QualityControl *qa;
    ProductionMonitor *monitor;
    ActivityLogger *logger;

public:
    OrderProcessor(ProductionGraph *wf, QualityControl *q,
                   ProductionMonitor *m, ActivityLogger *log);

    int process(Order *order, int orderID) const;

    bool executeProduction(Order *order, int orderID, Machine *machine) const;

    bool executeQA(Order *order, int orderID) const;
};

