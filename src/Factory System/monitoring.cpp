#include "monitoring.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// ============================================================================
// QUALITY CONTROL
// Keeps track of QA checks and pass/fail stats
// ============================================================================

// Start with zero results
QualityControl::QualityControl() : passed(0), failed(0) {
}

// Run a QA check
// ~90% chance to pass
bool QualityControl::inspect() {
    bool pass = (rand() % 100) < 90;

    if (pass) {
        passed++;
    } else {
        failed++;
    }

    return pass;
}

// Percentage of orders that passed QA
float QualityControl::getPassRate() const {
    int total = passed + failed;
    if (total == 0) return 100.0f;

    return static_cast<float>(passed) / static_cast<float>(total) * 100.0f;
}

// Print QA summary
void QualityControl::display() const {
    cout << "\n=== QUALITY CONTROL ===" << endl;
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Pass Rate: " << getPassRate() << "%" << endl;
    cout << "=======================\n" << endl;
}

// Accessors
int QualityControl::getPassed() const { return passed; }
int QualityControl::getFailed() const { return failed; }


// ============================================================================
// MAINTENANCE MANAGER
// Simple priority list for machines needing repair
// ============================================================================

// Create empty maintenance queue
MaintenanceManager::MaintenanceManager(int cap) {
    capacity = cap;
    queue = new MaintenanceItem[capacity];
    count = 0;
}

// Cleanup
MaintenanceManager::~MaintenanceManager() {
    delete[] queue;
}

// Grow queue when full
void MaintenanceManager::resize() {
    capacity *= 2;
    auto newQueue = new MaintenanceItem[capacity];

    for (int i = 0; i < count; i++) {
        newQueue[i] = queue[i];
    }

    delete[] queue;
    queue = newQueue;
}

// Sort machines so the highest priority comes first
void MaintenanceManager::sortByPriority() const {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (queue[j].priority > queue[j + 1].priority) {
                MaintenanceItem temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
        }
    }
}

// Add a machine to the repair list
void MaintenanceManager::schedule(Machine *m, int priority) {
    if (count >= capacity) {
        resize();
    }

    queue[count].machine = m;
    queue[count].priority = priority;
    count++;

    // Keep list ordered
    sortByPriority();
}

// Get next machine to repair
Machine *MaintenanceManager::getNext() {
    if (count == 0) return nullptr;

    Machine *m = queue[0].machine;

    // Shift remaining machines forward
    for (int i = 0; i < count - 1; i++) {
        queue[i] = queue[i + 1];
    }
    count--;

    return m;
}

// Repair a machine
void MaintenanceManager::repair(Machine *m) {
    if (m) {
        m->repair();
    }
}

// Show maintenance queue
void MaintenanceManager::display() const {
    cout << "\n=== MAINTENANCE QUEUE ===" << endl;
    cout << "Machines waiting: " << count << endl;

    for (int i = 0; i < count; i++) {
        cout << "  Priority " << queue[i].priority << ": ";
        queue[i].machine->display();
        cout << endl;
    }

    cout << "=========================\n" << endl;
}

// Number of machines waiting
int MaintenanceManager::getCount() const { return count; }


// ============================================================================
// PRODUCTION MONITOR
// Tracks live system status and failures
// ============================================================================

// Start with clean counters
ProductionMonitor::ProductionMonitor()
    : activeOrders(0), productionFailures(0), qaFailures(0) {
}

// Print current system state
void ProductionMonitor::display() const {
    cout << "\n=== PRODUCTION MONITOR ===" << endl;
    cout << "Active Orders: " << activeOrders << endl;
    cout << "Production Failures: " << productionFailures << endl;
    cout << "QA Failures: " << qaFailures << endl;
    cout << "Total Failures: " << getTotalFailures() << endl;
    cout << "==========================\n" << endl;
}

// Order entered the system
void ProductionMonitor::orderStart() { activeOrders++; }

// Order finished processing
void ProductionMonitor::orderComplete() { activeOrders--; }

// Failure tracking
void ProductionMonitor::recordProductionFail() { productionFailures++; }
void ProductionMonitor::recordQAFail() { qaFailures++; }

// Accessors
int ProductionMonitor::getActive() const { return activeOrders; }
int ProductionMonitor::getTotalFailures() const {
    return productionFailures + qaFailures;
}


// ============================================================================
// PRODUCTION REPORT
// Final numbers for completed vs failed orders
// ============================================================================

// Start empty
ProductionReport::ProductionReport() : totalCompleted(0), totalFailed(0) {
}

// Overall success percentage
float ProductionReport::getSuccessRate() const {
    int total = totalCompleted + totalFailed;
    if (total == 0) return 100.0f;

    return static_cast<float>(totalCompleted) /
           static_cast<float>(total) * 100.0f;
}

// Print report
void ProductionReport::display() const {
    cout << "\n=== PRODUCTION REPORT ===" << endl;
    cout << "Completed: " << totalCompleted << endl;
    cout << "Failed: " << totalFailed << endl;
    cout << "Success Rate: " << getSuccessRate() << "%" << endl;
    cout << "==========================\n" << endl;
}

// Update counters
void ProductionReport::recordComplete() { totalCompleted++; }
void ProductionReport::recordFail() { totalFailed++; }

// Accessors
int ProductionReport::getCompleted() const { return totalCompleted; }
int ProductionReport::getFailed() const { return totalFailed; }


// ============================================================================
// ACTIVITY LOGGER
// Stores a history of events during runtime
// ============================================================================

// Single log entry
ActivityLogger::LogEntry::LogEntry(const char *msg, int type)
    : eventType(type), next(nullptr) {
    strncpy(message, msg, 199);
    message[199] = '\0';
}

// Create logger with max size
ActivityLogger::ActivityLogger(int max)
    : top(nullptr), logCount(0), maxLogs(max) {
}

// Cleanup all logs
ActivityLogger::~ActivityLogger() {
    clear();
}

// Add a raw log entry
void ActivityLogger::log(const char *message, int eventType) {
    if (logCount >= maxLogs) return;

    auto newEntry = new LogEntry(message, eventType);
    newEntry->next = top;
    top = newEntry;
    logCount++;
}

// Convenience logging helpers
void ActivityLogger::logOrderReceived(int orderID, int priority) {
    char msg[200];
    sprintf(msg, "[ORDER] Order #%d received (Priority: %d)", orderID, priority);
    log(msg, 0);
}

void ActivityLogger::logProductionStart(int orderID) {
    char msg[200];
    sprintf(msg, "[PRODUCTION] Order #%d started production", orderID);
    log(msg, 0);
}

void ActivityLogger::logProductionComplete(int orderID) {
    char msg[200];
    sprintf(msg, "[PRODUCTION] Order #%d completed production stage", orderID);
    log(msg, 1);
}

void ActivityLogger::logProductionFail(int orderID, int machineID) {
    char msg[200];
    sprintf(msg, "[FAILURE] Order #%d failed - Machine #%d broke down", orderID, machineID);
    log(msg, 2);
}

void ActivityLogger::logQAPass(int orderID) {
    char msg[200];
    sprintf(msg, "[QA] Order #%d PASSED quality inspection", orderID);
    log(msg, 1);
}

void ActivityLogger::logQAFail(int orderID) {
    char msg[200];
    sprintf(msg, "[QA] Order #%d FAILED quality inspection - sent to rework", orderID);
    log(msg, 2);
}

void ActivityLogger::logMachineFailure(int machineID, int life) {
    char msg[200];
    sprintf(msg, "[MAINTENANCE] Machine #%d failed (Life: %d%%) - sent for repair", machineID, life);
    log(msg, 3);
}

void ActivityLogger::logMachineRepair(int machineID) {
    char msg[200];
    sprintf(msg, "[MAINTENANCE] Machine #%d repaired successfully", machineID);
    log(msg, 1);
}

void ActivityLogger::logOrderComplete(int orderID) {
    char msg[200];
    sprintf(msg, "[COMPLETE] Order #%d finished and ready for shipment", orderID);
    log(msg, 1);
}

void ActivityLogger::logRework(int orderID) {
    char msg[200];
    sprintf(msg, "[REWORK] Order #%d pulled from rework stack for reprocessing", orderID);
    log(msg, 0);
}

//Write logs to a text file
void ActivityLogger::dumpToFile(const char *filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open log file!" << endl;
        return;
    }

    time_t now = time(nullptr);
    char *dt = ctime(&now);

    file << "========================================\n";
    file << "   FACTORY PRODUCTION LOG\n";
    file << "========================================\n";
    file << "Generated: " << dt;
    file << "Total Events: " << logCount << "\n";
    file << "========================================\n\n";

    if (logCount == 0) {
        file << "No events logged.\n";
    } else {
        // Reverse stack to print oldest first
        auto entries = new LogEntry *[logCount];
        LogEntry *current = top;
        int i = 0;

        while (current) {
            entries[i++] = current;
            current = current->next;
        }

        for (int j = logCount - 1; j >= 0; j--) {
            const char *type;
            switch (entries[j]->eventType) {
                case 0: type = "INFO"; break;
                case 1: type = "SUCCESS"; break;
                case 2: type = "FAILURE"; break;
                case 3: type = "MAINTENANCE"; break;
                default: type = "UNKNOWN";
            }

            file << "[" << type << "] " << entries[j]->message << "\n";
        }

        delete[] entries;
    }

    file << "\n========================================\n";
    file << "End of Log\n";
    file << "========================================\n";

    file.close();
    cout << "Log dumped to '" << filename << "' successfully!" << endl;
}

// Show last N events on screen
void ActivityLogger::displayRecent(int count) const {
    cout << "\n=== RECENT ACTIVITY (Last " << count << " events) ===" << endl;

    LogEntry *current = top;
    int displayed = 0;

    while (current && displayed < count) {
        const char *type;
        switch (current->eventType) {
            case 0: type = "INFO"; break;
            case 1: type = "✓"; break;
            case 2: type = "✗"; break;
            case 3: type = "🔧"; break;
            default: type = "?";
        }

        cout << type << " " << current->message << endl;
        current = current->next;
        displayed++;
    }

    cout << "========================================\n";
}

// Remove all log entries
void ActivityLogger::clear() {
    while (top) {
        LogEntry *temp = top;
        top = top->next;
        delete temp;
    }
    logCount = 0;
}

// Number of stored logs
int ActivityLogger::getLogCount() const { return logCount; }

// Export logs as JSON
void ActivityLogger::dumpToJSON(const char *filename) const {
    json j;
    j["logCount"] = logCount;
    j["entries"] = json::array();

    // Collect entries oldest to newest
    auto entries = new LogEntry *[logCount];
    LogEntry *current = top;
    int i = 0;

    while (current) {
        entries[i++] = current;
        current = current->next;
    }

    for (int idx = logCount - 1; idx >= 0; --idx) {
        json entry;
        entry["eventType"] = entries[idx]->eventType;
        entry["message"] = entries[idx]->message;
        j["entries"].push_back(entry);
    }

    delete[] entries;

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open JSON file!" << endl;
        return;
    }

    file << j.dump(4);
    file.close();

    cout << "JSON log dumped to '" << filename << "' successfully!" << endl;
}
