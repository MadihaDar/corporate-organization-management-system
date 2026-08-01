#pragma once

#include "production.h"
#include "monitoring.h"
#include "order_management.h"
#include "templates.h"


// 10. Factory (Main Class)
class Factory {
    // Data structures
    OrderQueue<Order>*normalQ;
    PriorityOrderQueue<Order>*priorityQ;
    ReworkStack<Order>*reworkStack;
    FinishedProductArray<Product>*finished;

    // Business classes
    ProductionGraph *workflow;
    QualityControl *qa;
    MaintenanceManager *maintenance;
    ProductionMonitor *monitor;
    ProductionReport *report;
    ActivityLogger *logger;
    OrderProcessor *processor;

    Machine **machines;
    int machineCount;
    int machineCapacity;

    int nextOrderID;

    void setupWorkflow();

    void resizeMachines();

public:
    Factory();

    ~Factory();

    void receiveOrder(Order *order, int priority);

    Order *getNextOrder(int &orderID) const;

    void processOrder(Order *order, int orderID) const;

    void handleProductionFailure(Order *order, int orderID) const;

    void handleQAFailure(Order *order, int orderID) const;

    void completeOrder(Order *order, int orderID) const;

    void checkMachines() const;

    void performMaintenance() const;

    void processRework() const;

    void addMachine(Machine *m);

    void displayWorkflow() const;

    void displayStats() const;

    void displayStatus() const;

    Product **getFinishedProducts(int &count) const;

    void showRecentActivity(int count = 10) const;

    void saveLogReport(const char *filename = "factory_log.txt") const;

    void processBatch(int count) const;

    void runMenu();
};
