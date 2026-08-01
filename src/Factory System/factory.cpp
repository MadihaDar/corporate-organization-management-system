#include "factory.h"
#include <iostream>
#include "../Colors.h"

extern void clearInput();

using namespace std;

// Factory Implementation

Factory::Factory() {
    // Initialize data structures
    normalQ = new OrderQueue<Order>();
    priorityQ = new PriorityOrderQueue<Order>();
    reworkStack = new ReworkStack<Order>();
    finished = new FinishedProductArray<Product>();

    // Initialize business classes
    workflow = new ProductionGraph();
    qa = new QualityControl();
    maintenance = new MaintenanceManager();
    monitor = new ProductionMonitor();
    report = new ProductionReport();
    logger = new ActivityLogger();
    processor = new OrderProcessor(workflow, qa, monitor, logger);

    // Initialize machine array
    machineCapacity = 5;
    machines = new Machine *[machineCapacity];
    machineCount = 0;

    nextOrderID = 1;

    // Setup workflow
    setupWorkflow();
}

Factory::~Factory() {
    delete normalQ;
    delete priorityQ;
    delete reworkStack;
    delete finished;

    delete workflow;
    delete qa;
    delete maintenance;
    delete monitor;
    delete report;
    delete logger;
    delete processor;

    for (int i = 0; i < machineCount; i++) {
        delete machines[i];
    }
    delete[] machines;
}

void Factory::setupWorkflow() {
    // Basic Workflow:
    // Receive Order/Commence Production->Testing->Finishing.


    // Create production stages
    auto stage1 = new ProductionStage(0, "Assembly");
    auto *stage2 = new ProductionStage(1, "Testing");
    auto stage3 = new ProductionStage(2, "Packaging");

    // Add stages to workflow
    workflow->addStage(stage1);
    workflow->addStage(stage2);
    workflow->addStage(stage3);

    // Connect stages
    workflow->connect(0, 1);
    workflow->connect(1, 2);

    // Create and assign machines
    auto m1 = new Machine(1, 100);
    auto m2 = new Machine(2, 100);
    auto m3 = new Machine(3, 100);


    stage1->assignMachine(m1);
    stage2->assignMachine(m2);
    stage3->assignMachine(m3);

    addMachine(m1);
    addMachine(m2);
    addMachine(m3);
}

void Factory::resizeMachines() {
    machineCapacity *= 2;
    auto newArray = new Machine *[machineCapacity];
    for (int i = 0; i < machineCount; i++) {
        newArray[i] = machines[i];
    }
    delete[] machines;
    machines = newArray;
}

void Factory::addMachine(Machine *m) {
    if (machineCount >= machineCapacity) {
        resizeMachines();
    }
    machines[machineCount++] = m;
}

void Factory::receiveOrder(Order *order, int priority) {
    // two queues to handle two kinds of orders normal priority and Priority Orders
    if (priority > 0) {
        priorityQ->insert(order, nextOrderID, priority);
    } else {
        normalQ->enqueue(order, nextOrderID);
    }
    logger->logOrderReceived(nextOrderID, priority);
    nextOrderID++;
}

Order *Factory::getNextOrder(int &orderID) const {
    // Check priority queue first
    if (!priorityQ->isEmpty()) {
        return priorityQ->extractMin(orderID);
    }

    // Then check normal queue
    if (!normalQ->isEmpty()) {
        return normalQ->dequeue(orderID);
    }

    return nullptr;
}

void Factory::processOrder(Order *order, int orderID) const {
    monitor->orderStart();

    int result = processor->process(order, orderID);

    if (result == 0) {
        // Success
        completeOrder(order, orderID);
    } else if (result == 1) {
        // Production failure
        handleProductionFailure(order, orderID);
    } else {
        // QA failure
        handleQAFailure(order, orderID);
    }

    monitor->orderComplete();
}

void Factory::handleProductionFailure(Order *order, int orderID) const {
    report->recordFail();
    // Don't add to rework for production failures - order is lost
}

void Factory::handleQAFailure(Order *order, int orderID) const {
    // send the failed products to a stack to handle them later.
    reworkStack->push(order, orderID);
    // add the failure to the log.
    logger->logQAFail(orderID);
}

void Factory::completeOrder(Order *order, int orderID) const {
    finished->add(order->getProduct(), orderID);
    report->recordComplete();
    logger->logOrderComplete(orderID);
}

void Factory::checkMachines() const {
    for (int i = 0; i < machineCount; i++) {
        // check and handle priority for machine maintenance.
        if (machines[i]->needsMaintenance()) {
            int priority = 100 - machines[i]->getLife();
            maintenance->schedule(machines[i], priority);
            logger->logMachineFailure(machines[i]->getID(), machines[i]->getLife());
        }
    }
}

void Factory::performMaintenance() const {
    // perform maintenance on machines
    Machine *m = maintenance->getNext();
    if (m) {
        MaintenanceManager::repair(m);
        logger->logMachineRepair(m->getID());
        cout << "Repaired Machine #" << m->getID() << endl;
    } else {
        cout << "No machines in maintenance queue." << endl;
    }
}

void Factory::processRework() const {
    // commence production of failed products.
    if (reworkStack->isEmpty()) {
        cout << "No items in rework stack." << endl;
        return;
    }

    int orderID;
    Order *order = reworkStack->pop(orderID);
    logger->logRework(orderID);
    processOrder(order, orderID);
}

void Factory::displayWorkflow() const {
    workflow->display();
}

void Factory::displayStats() const {
    qa->display();
    report->display();
    monitor->display();
    maintenance->display();
}

void Factory::displayStatus() const {
    cout << "\n========== FACTORY STATUS ==========" << endl;
    cout << "Orders in Normal Queue: " << normalQ->getSize() << endl;
    cout << "Orders in Priority Queue: " << priorityQ->getSize() << endl;
    cout << "Orders in Rework Stack: " << reworkStack->getSize() << endl;
    cout << "Finished Products: " << finished->getCount() << endl;
    cout << "Active Orders: " << monitor->getActive() << endl;
    cout << "\nMachine Status:" << endl;
    for (int i = 0; i < machineCount; i++) {
        cout << "  ";
        machines[i]->display();
        cout << endl;
    }
    cout << "====================================\n" << endl;
}

Product **Factory::getFinishedProducts(int &count) const {
    // return finshed prod count
    count = finished->getCount();
    if (count == 0) return nullptr;

    auto **products = new Product *[count];
    for (int i = 0; i < count; i++) {
        int dummy;
        products[i] = finished->get(i, dummy);
    }
    return products;
}

void Factory::showRecentActivity(int count) const {
    // displays user selected amount of logs
    logger->displayRecent(count);
}

void Factory::saveLogReport(const char *filename) const {
    // dump the log to Json
    logger->dumpToJSON(filename);
}

void Factory::processBatch(int count) const {
    cout << "\nProcessing batch of " << count << " orders..." << endl;

    for (int i = 0; i < count; i++) {
        int orderID;
        Order *order = getNextOrder(orderID);

        if (order) {
            processOrder(order, orderID);
        } else {
            cout << "No more orders to process." << endl;
            break;
        }
    }

    // Check machines after batch
    checkMachines();

    cout << "Batch processing complete!" << endl;
}

void Factory::runMenu() {
    int choice;

    while (true) {
        cout << CLRSCR;
        cout << BOLD << MAGENTA << "========== FACTORY MANAGEMENT SYSTEM ==========" << RESET << endl;
        cout << GREEN << "1." << RESET << "  Receive New Order (Normal Priority)" << endl;
        cout << GREEN << "2." << RESET << "  Receive New Order (High Priority)" << endl;
        cout << GREEN << "3." << RESET << "  Process Next Order" << endl;
        cout << GREEN << "4." << RESET << "  Process Batch of Orders" << endl;
        cout << GREEN << "5." << RESET << "  Process Rework Item" << endl;
        cout << GREEN << "6." << RESET << "  Check Machine Status" << endl;
        cout << GREEN << "7." << RESET << "  Perform Maintenance" << endl;
        cout << GREEN << "8." << RESET << "  Display Workflow" << endl;
        cout << GREEN << "9." << RESET << "  Display Factory Status" << endl;
        cout << GREEN << "10." << RESET << " Display Statistics" << endl;
        cout << GREEN << "11." << RESET << " Show Recent Activity" << endl;
        cout << GREEN << "12." << RESET << " Save Log Report" << endl;
        cout << RED << "0.   Exit Module" << RESET << endl;
        cout << "===============================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        switch (choice) {
            case 1: {
                cout << CLRSCR;
                auto order = new Order(nextOrderID, 0);
                receiveOrder(order, 0);
                cout << GREEN << "Order added to normal queue." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                cout << CLRSCR;
                int priority;
                cout << "Enter priority (1-10, lower = higher priority): ";
                cin >> priority;
                auto order = new Order(nextOrderID, priority);
                receiveOrder(order, priority);
                cout << GREEN << "Order added to priority queue." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 3: {
                cout << CLRSCR;
                int orderID;
                Order *order = getNextOrder(orderID);
                if (order) {
                    cout << CYAN << "Processing Order #" << orderID << "..." << RESET << endl;
                    processOrder(order, orderID);
                } else {
                    cout << YELLOW << "No orders to process." << RESET << endl;
                }
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                cout << CLRSCR;
                int count;
                cout << "Enter number of orders to process: ";
                cin >> count;
                processBatch(count);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 5: {
                cout << CLRSCR;
                processRework();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 6: {
                cout << CLRSCR;
                checkMachines();
                cout << GREEN << "Machine check complete. Check maintenance queue for repairs." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 7: {
                cout << CLRSCR;
                performMaintenance();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 8: {
                cout << CLRSCR;
                displayWorkflow();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 9: {
                cout << CLRSCR;
                displayStatus();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 10: {
                cout << CLRSCR;
                displayStats();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 11: {
                cout << CLRSCR;
                int count;
                cout << "How many recent events to show? ";
                cin >> count;
                showRecentActivity(count);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 12: {
                saveLogReport("factory_logs.json");
                cout << GREEN << "Log report saved." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 0: {
                cout << GREEN << "Exiting Factory Management System..." << RESET << endl;
                return;
            }
            default: {
                cout << RED << "Invalid choice. Please try again." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
}
