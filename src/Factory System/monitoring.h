#pragma once
#include <iostream>
#include "production.h"

using namespace std;

// 4. Quality Control
class QualityControl {
    int passed;
    int failed;

public:
    QualityControl();

    bool inspect();

    int getPassed() const;

    int getFailed() const;

    float getPassRate() const;

    void display() const;
};


// 5. Maintenance Manager
class MaintenanceManager {
    struct MaintenanceItem {
        Machine *machine;
        int priority;
    };

    MaintenanceItem *queue;
    int count;
    int capacity;

    void resize();

    void sortByPriority() const;

public:
    explicit MaintenanceManager(int cap = 10);

    ~MaintenanceManager();

    void schedule(Machine *m, int priority);

    Machine *getNext();

    static void repair(Machine *m);

    int getCount() const;

    void display() const;
};


// 6. Production Monitor

class ProductionMonitor {
    int activeOrders;
    int productionFailures;
    int qaFailures;

public:
    ProductionMonitor();

    void orderStart();

    void orderComplete();

    void recordProductionFail();

    void recordQAFail();

    int getActive() const;

    int getTotalFailures() const;

    void display() const;
};


// 7. Production Report

class ProductionReport {
    int totalCompleted;
    int totalFailed;

public:
    ProductionReport();

    void recordComplete();

    void recordFail();

    int getCompleted() const;

    int getFailed() const;

    float getSuccessRate() const;

    void display() const;
};

// 8. Activity Logger

class ActivityLogger {
    struct LogEntry {
        char message[200];
        int eventType;
        LogEntry *next;

        LogEntry(const char *msg, int type);
    };

    LogEntry *top;
    int logCount;
    int maxLogs;

public:
    ActivityLogger(int max = 1000);

    ~ActivityLogger();

    void logOrderReceived(int orderID, int priority);

    void logProductionStart(int orderID);

    void logProductionComplete(int orderID);

    void logProductionFail(int orderID, int machineID);

    void logQAPass(int orderID);

    void logQAFail(int orderID);

    void logMachineFailure(int machineID, int life);

    void logMachineRepair(int machineID);

    void logOrderComplete(int orderID);

    void logRework(int orderID);

    void log(const char *message, int eventType = 0);

    void dumpToFile(const char *filename) const;

    void displayRecent(int count) const;

    void clear();

    int getLogCount() const;

    void dumpToJSON(const char *filename) const;
};
