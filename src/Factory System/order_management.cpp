#include "order_management.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// ============================================================================
// ORDER PROCESSOR IMPLEMENTATION
// Handles the full lifecycle of an order:
// Production -> Quality Assurance -> Final status
// ============================================================================

// Constructor
// Wires together workflow, QA, monitoring, and logging systems
OrderProcessor::OrderProcessor(ProductionGraph *wf, QualityControl *q,
                               ProductionMonitor *m, ActivityLogger *log)
    : workflow(wf), qa(q), monitor(m), logger(log) {
}

// Main order processing pipeline
// Returns:
// 0 = success
// 1 = production failure
// 2 = QA or setup failure
int OrderProcessor::process(Order *order, int orderID) const {
    // -----------------------------
    // Stage 1: Production
    // -----------------------------

    // Always start from stage 0 in the production graph
    ProductionStage *prodStage = workflow->findStage(0);
    if (!prodStage) return 2; // Invalid workflow

    // Retrieve machine assigned to this stage
    Machine *machine = prodStage->getMachine();
    if (!machine) return 2; // No machine available

    // Execute production step
    if (!executeProduction(order, orderID, machine)) {
        return 1; // Production failure
    }

    // -----------------------------
    // Stage 2: Quality Assurance
    // -----------------------------
    if (!executeQA(order, orderID)) {
        return 2; // QA failure
    }

    return 0; // Order completed successfully
}

// Executes the production phase for a single order
bool OrderProcessor::executeProduction(Order *order, int orderID, Machine *machine) const {
    // Log production start
    logger->logProductionStart(orderID);

    // Randomized failure check based on machine condition
    if (machine->shouldFail()) {
        logger->logProductionFail(orderID, machine->getID());
        monitor->recordProductionFail();
        return false;
    }

    // Degrade machine condition after use
    machine->degrade(rand() % 10 + 5);

    // Log successful production
    logger->logProductionComplete(orderID);
    return true;
}

// Executes QA inspection for a produced order
bool OrderProcessor::executeQA(Order *order, int orderID) const {
    // Inspection failed
    if (!qa->inspect()) {
        logger->logQAFail(orderID);
        monitor->recordQAFail();
        return false;
    }

    // Inspection passed
    logger->logQAPass(orderID);
    return true;
}
