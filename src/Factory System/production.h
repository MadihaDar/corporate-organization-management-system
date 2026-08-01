#pragma once
#include <iostream>

using namespace std;


// 1. Machines
class Machine {
    int machineID;
    int remainingLife;

public:
    Machine(int id, int life = 100);

    int getID() const;

    int getLife() const;

    bool shouldFail() const;

    void degrade(int amount = 5);

    void repair();

    bool needsMaintenance() const;

    void display() const;
};

class ProductionStage;

// 2. Production Stage (( A graph implement))

class ProductionStage {
    int stageID;
    char name[50];
    ProductionStage **nextStages;
    int nextCount;
    int capacity;
    Machine *machine;

    void resize();

public:
    ProductionStage(int id, const char *stageName);

    ~ProductionStage();

    int getID() const;

    const char *getName() const;

    Machine *getMachine() const;

    int getNextCount() const;

    void addNext(ProductionStage *stage);

    void assignMachine(Machine *m);

    ProductionStage *getNext(int index) const;

    void display() const;
};


// 3. Production Graph

class ProductionGraph {
    ProductionStage **stages;
    int stageCount;
    int capacity;

    void resize();

public:
    ProductionGraph();

    ~ProductionGraph();

    void addStage(ProductionStage *stage);

    void connect(int fromID, int toID) const;

    ProductionStage *findStage(int id) const;

    ProductionStage *getFirst() const;

    void display() const;
};
