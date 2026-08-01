#ifndef FINANCIALSYSTEM_H
#define FINANCIALSYSTEM_H

#include "templatedatastructures.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

//
// ModuleType Enum
//
enum ModuleType
{
    MANAGEMENT,
    WAREHOUSE,
    FRANCHISING,
    PRODUCTION,
    FINANCIAL,
    FEEDBACK,
    UNKNOWN
};

string getModuleName(ModuleType type);
ModuleType getModuleType(const string& name);

//
// FinancialLog Class
//
class FinancialLog
{
public:
    static int nextId;
    int id;
    string description;
    double amount;
    string date;
    string type;
    ModuleType module;
    string traceId;

    explicit FinancialLog(string desc = "", double amt = 0.0, string date = "", string type = "",
                          ModuleType mod = UNKNOWN, string trace = "");

    bool operator==(const FinancialLog& other) const;
    friend ostream& operator<<(ostream& os, const FinancialLog& log);
    bool operator<(const FinancialLog& other) const;
    bool operator>(const FinancialLog& other) const;
    friend void to_json(json& j, const FinancialLog& l);
    friend void from_json(const json& j, FinancialLog& l);
};

//
// FinancialLogCategory Class
//
class FinancialLogCategory
{
public:
    ModuleType module;
    LinkedList<FinancialLog> logs;

    explicit FinancialLogCategory(ModuleType mod = UNKNOWN);

    bool operator<(const FinancialLogCategory& other) const;
    bool operator>(const FinancialLogCategory& other) const;
    bool operator==(const FinancialLogCategory& other) const;
    friend ostream& operator<<(ostream& os, const FinancialLogCategory& cat);
};

//
// ModuleNode Class
//
class ModuleNode
{
public:
    ModuleType module;

    explicit ModuleNode(ModuleType mod = UNKNOWN);

    bool operator==(const ModuleNode& other) const;
    friend ostream& operator<<(ostream& os, const ModuleNode& node);
};

//
// FinancialDatabase Class (Singleton)
//
class FinancialDatabase
{
private:
    static FinancialDatabase* instance;

    Tree<FinancialLogCategory> logTree;
    Graph<ModuleNode> supplyChainGraph;
    Queue<FinancialLog> pendingOperations;
    Stack<FinancialLog> operationHistory;

    FinancialDatabase();
    void collectLogs(const TreeNode<FinancialLogCategory>* node, DynamicArray<FinancialLog>& results);
    void collectLogsForTrace(const TreeNode<FinancialLogCategory>* node, const string& traceId,
                             DynamicArray<FinancialLog>& results);

public:
    static FinancialDatabase* getInstance();

    void displayMostExpensiveLogs();
    void displayCheapestLogs();

    void logTransaction(ModuleType module, const string& description, double amount, const string& type,
                        const string& traceId = "");

    void approveNextOperation();
    void undoLastOperation();

    void initializeModules();
    void defineSupplyChainLink(ModuleType from, ModuleType to, float costImpact);

    void generateModuleReport(ModuleType module);
    void analyzeSupplyChainCosts();
    float calculateRouteCost(ModuleType start, ModuleType end);
    void optimizeLogistics(ModuleType start, ModuleType end);
    void displayAllModules() const;

    void traceProductLifecycle(const string& traceId);

    void saveData();
    void loadData();
};

#endif
