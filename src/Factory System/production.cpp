#include "production.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// ============================================================================
// MACHINE
// Represents a single production machine with wear and failure chance
// ============================================================================

// Create machine with ID and starting life
Machine::Machine(int id, int life) : machineID(id), remainingLife(life) {
}

// Machine identifier
int Machine::getID() const {
    return machineID;
}

// Current machine health
int Machine::getLife() const {
    return remainingLife;
}

// Determines whether the machine fails this run
// Lower life = higher chance of failure
bool Machine::shouldFail() const {
    int failChance = 100 - remainingLife;
    return (rand() % 100) < failChance;
}

// Reduce machine life after use
void Machine::degrade(int amount) {
    remainingLife -= amount;
    if (remainingLife < 0) remainingLife = 0;
}

// Fully repair the machine
void Machine::repair() {
    remainingLife = 100;
}

// Check if machine is close to breaking down
bool Machine::needsMaintenance() const {
    return remainingLife < 20;
}

// Print machine status
void Machine::display() const {
    cout << "Machine #" << machineID << " [Life: " << remainingLife << "%]";
}


// ============================================================================
// PRODUCTION STAGE
// One step in the production process
// ============================================================================

// Create stage with ID and name
ProductionStage::ProductionStage(int id, const char *stageName) {
    stageID = id;

    // Copy name safely
    strncpy(name, stageName, 49);
    name[49] = '\0';

    // Setup next-stage list
    capacity = 2;
    nextStages = new ProductionStage *[capacity];
    nextCount = 0;

    // No machine assigned initially
    machine = nullptr;
}

// Cleanup dynamic memory
ProductionStage::~ProductionStage() {
    delete[] nextStages;
}

// Expand next-stage list
void ProductionStage::resize() {
    capacity *= 2;
    auto newArray = new ProductionStage *[capacity];

    for (int i = 0; i < nextCount; i++) {
        newArray[i] = nextStages[i];
    }

    delete[] nextStages;
    nextStages = newArray;
}

// Link another stage after this one
void ProductionStage::addNext(ProductionStage *stage) {
    if (nextCount >= capacity) {
        resize();
    }
    nextStages[nextCount++] = stage;
}

// Get connected stage by index
ProductionStage *ProductionStage::getNext(int index) const {
    if (index < 0 || index >= nextCount) return nullptr;
    return nextStages[index];
}

// Print stage info and connections
void ProductionStage::display() const {
    cout << "Stage " << stageID << ": " << name;

    if (machine) {
        cout << " [Machine #" << machine->getID() << "]";
    }

    cout << " -> ";
    for (int i = 0; i < nextCount; i++) {
        cout << nextStages[i]->getID();
        if (i < nextCount - 1) cout << ", ";
    }
    cout << endl;
}

// Accessors
int ProductionStage::getID() const { return stageID; }
const char *ProductionStage::getName() const { return name; }
Machine *ProductionStage::getMachine() const { return machine; }
int ProductionStage::getNextCount() const { return nextCount; }

// Assign machine to this stage
void ProductionStage::assignMachine(Machine *m) {
    machine = m;
}


// ============================================================================
// PRODUCTION GRAPH
// Holds all stages and their connections
// ============================================================================

// Create empty graph
ProductionGraph::ProductionGraph() {
    capacity = 5;
    stages = new ProductionStage *[capacity];
    stageCount = 0;
}

// Cleanup all stages
ProductionGraph::~ProductionGraph() {
    for (int i = 0; i < stageCount; i++) {
        delete stages[i];
    }
    delete[] stages;
}

// Expand stage list
void ProductionGraph::resize() {
    capacity *= 2;
    auto newArray = new ProductionStage *[capacity];

    for (int i = 0; i < stageCount; i++) {
        newArray[i] = stages[i];
    }

    delete[] stages;
    stages = newArray;
}

// Add a new stage to the graph
void ProductionGraph::addStage(ProductionStage *stage) {
    if (stageCount >= capacity) {
        resize();
    }
    stages[stageCount++] = stage;
}

// Connect two stages by ID
void ProductionGraph::connect(int fromID, int toID) const {
    ProductionStage *from = findStage(fromID);
    ProductionStage *to = findStage(toID);

    if (from && to) {
        from->addNext(to);
    }
}

// Find stage by ID
ProductionStage *ProductionGraph::findStage(int id) const {
    for (int i = 0; i < stageCount; i++) {
        if (stages[i]->getID() == id) {
            return stages[i];
        }
    }
    return nullptr;
}

// First stage in the workflow
ProductionStage *ProductionGraph::getFirst() const {
    return stageCount > 0 ? stages[0] : nullptr;
}

// Print full production flow
void ProductionGraph::display() const {
    cout << "\n=== PRODUCTION WORKFLOW ===" << endl;
    for (int i = 0; i < stageCount; i++) {
        stages[i]->display();
    }
    cout << "===========================\n" << endl;
}
