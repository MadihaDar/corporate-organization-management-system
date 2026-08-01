#include "FinancialSystem.h"

#include <utility>

//
// Helper Functions
//
string getModuleName(const ModuleType type)
{
    switch (type)
    {
    case MANAGEMENT: return "Management";
    case WAREHOUSE: return "Warehouse";
    case FRANCHISING: return "Franchising";
    case PRODUCTION: return "Production";
    case FINANCIAL: return "Financial";
    case FEEDBACK: return "Feedback";
    default: return "Unknown";
    }
}

ModuleType getModuleType(const string& name)
{
    if (name == "Management") return MANAGEMENT;
    if (name == "Warehouse") return WAREHOUSE;
    if (name == "Franchising") return FRANCHISING;
    if (name == "Production") return PRODUCTION;
    if (name == "Financial") return FINANCIAL;
    if (name == "Feedback") return FEEDBACK;
    return UNKNOWN;
}

//
// FinancialLog Implementation
//
int FinancialLog::nextId = 1000;

FinancialLog::FinancialLog(string desc, const double amt, string date, string type, const ModuleType mod, string trace)
    : id(nextId++), description(std::move(desc)), amount(amt), date(std::move(date)), type(std::move(type)),
      module(mod), traceId(std::move(trace))
{
}

bool FinancialLog::operator==(const FinancialLog& other) const
{
    return id == other.id;
}

bool FinancialLog::operator<(const FinancialLog& other) const
{
    return amount < other.amount;
}

bool FinancialLog::operator>(const FinancialLog& other) const
{
    return amount > other.amount;
}

ostream& operator<<(ostream& os, const FinancialLog& log)
{
    os << "[ID:" << log.id << " | " << getModuleName(log.module) << "] " << log.description << " | $" << log.amount <<
        " (" << log.type << ")";
    if (!log.traceId.empty())
    {
        os << " [TraceID: " << log.traceId << "]";
    }
    return os;
}

void to_json(json& j, const FinancialLog& l)
{
    j = json{
        {"id", l.id},
        {"description", l.description},
        {"amount", l.amount},
        {"date", l.date},
        {"type", l.type},
        {"module", l.module},
        {"traceId", l.traceId}
    };
}

void from_json(const json& j, FinancialLog& l)
{
    j.at("id").get_to(l.id);
    j.at("description").get_to(l.description);
    j.at("amount").get_to(l.amount);
    j.at("date").get_to(l.date);
    j.at("type").get_to(l.type);
    j.at("module").get_to(l.module);
    if (j.contains("traceId")) j.at("traceId").get_to(l.traceId);
}

//
// FinancialLogCategory Implementation
//
FinancialLogCategory::FinancialLogCategory(const ModuleType mod) : module(mod)
{
}

bool FinancialLogCategory::operator<(const FinancialLogCategory& other) const
{
    return module < other.module;
}

bool FinancialLogCategory::operator>(const FinancialLogCategory& other) const
{
    return module > other.module;
}

bool FinancialLogCategory::operator==(const FinancialLogCategory& other) const
{
    return module == other.module;
}

ostream& operator<<(ostream& os, const FinancialLogCategory& cat)
{
    os << "Module: " << getModuleName(cat.module);
    return os;
}

//
// ModuleNode Implementation
//
ModuleNode::ModuleNode(const ModuleType mod) : module(mod)
{
}

bool ModuleNode::operator==(const ModuleNode& other) const
{
    return module == other.module;
}

ostream& operator<<(ostream& os, const ModuleNode& node)
{
    os << getModuleName(node.module);
    return os;
}

//
// FinancialDatabase Implementation
//
FinancialDatabase* FinancialDatabase::instance = nullptr;

FinancialDatabase::FinancialDatabase()
{
    initializeModules();
    loadData();
}

FinancialDatabase* FinancialDatabase::getInstance()
{
    if (!instance)
    {
        instance = new FinancialDatabase();
    }
    return instance;
}

void FinancialDatabase::initializeModules()
{
    ModuleType modules[] = {MANAGEMENT, WAREHOUSE, FRANCHISING, PRODUCTION, FINANCIAL, FEEDBACK};

    for (const ModuleType& mod : modules)
    {
        logTree.insert(FinancialLogCategory(mod));
        supplyChainGraph.addNode(ModuleNode(mod));
    }
}

void FinancialDatabase::logTransaction(const ModuleType module, const string& description, const double amount,
                                       const string& type, const string& traceId)
{
    //create the log and add it to the pending queue
    //it wont be official until an admin approves it
    const FinancialLog newLog(description, amount, "2025-12-07", type, module, traceId);
    pendingOperations.enqueue(newLog);
    cout << "Transaction logged for " << getModuleName(module) << ". TraceID: " << (traceId.empty() ? "None" : traceId)
        << endl;
    saveData();
}

void FinancialDatabase::approveNextOperation()
{
    if (pendingOperations.isEmpty())
    {
        cout << "No pending operations." << endl;
        return;
    }

    //get the oldest pending log
    const FinancialLog log = pendingOperations.dequeue();

    //find the correct module (category) in the tree
    const FinancialLogCategory searchKey(log.module);
    TreeNode<FinancialLogCategory>* node = logTree.search(searchKey);

    if (node)
    {
        //add log to the module's ledger
        node->data.logs.insert(log);

        //record this action in the history stack for potential undo
        operationHistory.push(log);
        cout << "Approved: " << log << endl;
        saveData();
    }
    else
    {
        cout << "Error: Module not found in system." << endl;
    }
}

void FinancialDatabase::undoLastOperation()
{
    if (operationHistory.isEmpty())
    {
        cout << "Nothing to undo." << endl;
        return;
    }

    //pop the last approved log from the history stack
    FinancialLog log = operationHistory.pop();
    cout << "Undoing transaction: " << log.description << endl;
    saveData();
}

void FinancialDatabase::defineSupplyChainLink(const ModuleType from, const ModuleType to, const float costImpact)
{
    supplyChainGraph.addEdge(ModuleNode(from), ModuleNode(to), costImpact);
    saveData();
}

void FinancialDatabase::generateModuleReport(const ModuleType module)
{
    const FinancialLogCategory searchKey(module);
    TreeNode<FinancialLogCategory>* node = logTree.search(searchKey);

    if (node)
    {
        cout << "\n--- Financial Report: " << getModuleName(module) << " ---" << endl;
        node->data.logs.display();
    }
    else
    {
        cout << "Module not found." << endl;
    }
}

void FinancialDatabase::analyzeSupplyChainCosts()
{
    cout << "\n--- Supply Chain Cost Flow ---" << endl;
    supplyChainGraph.display();
}

float FinancialDatabase::calculateRouteCost(const ModuleType start, const ModuleType end)
{
    return supplyChainGraph.getEdgeWeight(ModuleNode(start), ModuleNode(end));
}

void FinancialDatabase::optimizeLogistics(const ModuleType start, const ModuleType end)
{
    //use dijkstra's algorithm to find the cheapest path through the graph
    auto result = supplyChainGraph.getShortestPath(ModuleNode(start), ModuleNode(end));

    cout << "\n--- Supply Chain Optimization ---" << endl;
    cout << "Path: " << getModuleName(start) << " -> " << getModuleName(end) << endl;

    if (result.totalCost < 0)
    {
        cout << "Status: No viable path found." << endl;
    }
    else
    {
        cout << "Status: Optimal Route Found" << endl;
        cout << "Route Sequence: ";
        for (int i = 0; i < result.path.size(); i++)
        {
            cout << getModuleName(result.path[i].module);
            if (i < result.path.size() - 1) cout << " -> ";
        }
        cout << endl;
        cout << "Total Cost Index: " << result.totalCost << endl;
    }
}

void FinancialDatabase::displayAllModules() const
{
    cout << "\n--- System Modules ---" << endl;
    logTree.inOrder(logTree.root);
    cout << endl;
}

//
// Audit Trail Logic
//
void FinancialDatabase::collectLogsForTrace(const TreeNode<FinancialLogCategory>* node, const string& traceId,
                                            DynamicArray<FinancialLog>& results)
{
    if (!node) return;
    collectLogsForTrace(node->left, traceId, results);

    // Check logs in this category
    const Node<FinancialLog>* current = node->data.logs.head;
    while (current)
    {
        if (current->data.traceId == traceId)
        {
            results.push_back(current->data);
        }
        current = current->next;
    }

    collectLogsForTrace(node->right, traceId, results);
}

void FinancialDatabase::traceProductLifecycle(const string& traceId)
{
    cout << "\n=============================================" << endl;
    cout << "   Audit Trail: " << traceId << "\n\n";

    DynamicArray<FinancialLog> trail;
    collectLogsForTrace(logTree.root, traceId, trail);

    if (trail.isEmpty())
    {
        cout << "No records found for Trace ID: " << traceId << endl;
        return;
    }
    double totalCost = 0;
    double totalRevenue = 0;

    for (int i = 0; i < trail.size(); i++)
    {
        FinancialLog& log = trail[i];
        cout << log << endl;

        if (log.type == "Expense") totalCost += log.amount;
        else if (log.type == "Revenue") totalRevenue += log.amount;
    }

    cout << "\n";
    cout << "Total Revenue: $" << totalRevenue << endl;
    cout << "Total Cost:    $" << totalCost << endl;
    cout << "Net Profit:    $" << (totalRevenue - totalCost) << endl;
    cout << "=============================================" << endl;
}

void FinancialDatabase::collectLogs(const TreeNode<FinancialLogCategory>* node, DynamicArray<FinancialLog>& results)
{
    if (!node) return;
    collectLogs(node->left, results);

    const Node<FinancialLog>* current = node->data.logs.head;
    while (current)
    {
        results.push_back(current->data);
        current = current->next;
    }

    collectLogs(node->right, results);
}

void FinancialDatabase::displayMostExpensiveLogs()
{
    DynamicArray<FinancialLog> allLogs;
    collectLogs(logTree.root, allLogs);

    if (allLogs.isEmpty())
    {
        cout << "No logs found." << endl;
        return;
    }

    Sorter<FinancialLog>::mergeSort(allLogs, 0, allLogs.size() - 1);

    cout << "\n--- Most Expensive Financial Logs (Merge Sort) ---" << endl;
    for (int i = 0; i < allLogs.size(); i++)
    {
        cout << allLogs[i] << endl;
    }
}

void FinancialDatabase::displayCheapestLogs()
{
    DynamicArray<FinancialLog> allLogs;
    collectLogs(logTree.root, allLogs);

    if (allLogs.isEmpty())
    {
        cout << "No logs found." << endl;
        return;
    }

    Sorter<FinancialLog>::bubbleSort(allLogs);

    cout << "\n--- Cheapest Financial Logs (Bubble Sort) ---" << endl;
    for (int i = allLogs.size() - 1; i >= 0; i--)
    {
        cout << allLogs[i] << endl;
    }
}


void collect(const TreeNode<FinancialLogCategory>* node, json& jRef)
{
    if (!node) return;
    collect(node->left, jRef);

    const Node<FinancialLog>* current = node->data.logs.head;
    while (current)
    {
        jRef.push_back(current->data);
        current = current->next;
    }

    collect(node->right, jRef);
}

void FinancialDatabase::saveData()
{
    json j;

    //save logs via tree traversal
    j["logs"] = json::array();
    collect(logTree.root, j["logs"]);

    //save the pending operations
    j["pending"] = json::array();
    DynamicArray<FinancialLog> tempPending;
    while (!pendingOperations.isEmpty())
    {
        tempPending.push_back(pendingOperations.dequeue());
    }
    for (int i = 0; i < tempPending.size(); i++)
    {
        j["pending"].push_back(tempPending[i]);
        pendingOperations.enqueue(tempPending[i]); //restore old arr.
    }

    //save history for undoing (stack)
    j["history"] = json::array();
    Stack<FinancialLog> tempStack;
    while (!operationHistory.isEmpty())
    {
        tempStack.push(operationHistory.pop());
    }
    while (!tempStack.isEmpty())
    {
        FinancialLog l = tempStack.pop();
        j["history"].push_back(l);
        operationHistory.push(l); //restore old stack
    }

    //save all edges
    j["edges"] = json::array();
    auto edges = supplyChainGraph.getAllEdges();
    for (int i = 0; i < edges.size(); i++)
    {
        j["edges"].push_back({
            {"src", edges[i].src.module},
            {"dest", edges[i].dest.module},
            {"weight", edges[i].weight}
        });
    }

    //next log id!!
    j["next_log_id"] = FinancialLog::nextId;

    ofstream file("financial_system.json");
    if (file.is_open())
    {
        file << j.dump(4);
        file << endl;
    }
}

void FinancialDatabase::loadData()
{
    ifstream file("financial_system.json");
    if (!file.is_open()) return;

    json j;
    file >> j;

    //log id
    if (j.contains("next_log_id")) FinancialLog::nextId = j["next_log_id"];

    //logs
    if (j.contains("logs"))
    {
        for (const auto& item : j["logs"])
        {
            FinancialLog log = item;
            const FinancialLogCategory searchKey(log.module);
            TreeNode<FinancialLogCategory>* node = logTree.search(searchKey);
            if (node)
            {
                node->data.logs.insert(log);
            }
        }
    }

    //Pending
    if (j.contains("pending"))
    {
        for (const auto& item : j["pending"])
        {
            FinancialLog log = item;
            pendingOperations.enqueue(log);
        }
    }

    //history
    if (j.contains("history"))
    {
        for (const auto& item : j["history"])
        {
            FinancialLog log = item;
            operationHistory.push(log);
        }
    }

    // edges
    if (j.contains("edges"))
    {
        for (const auto& item : j["edges"])
        {
            ModuleType src = item["src"];
            ModuleType dest = item["dest"];
            float weight = item["weight"];
            supplyChainGraph.addEdge(ModuleNode(src), ModuleNode(dest), weight);
        }
    }
}
