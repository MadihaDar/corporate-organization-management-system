#include "Product.h"
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;

#include<fstream>
#include "FinanceReporter.h"
//constructor
FinanceReporter::FinanceReporter() : total_transport_cost(0.0), head(nullptr) {}
//destructor
FinanceReporter::~FinanceReporter() {

    Expense* current = head;
    while (current) {
        Expense* t = current;
        current = current->next;
        delete t;
    }
}
//
void FinanceReporter::addExpense(const string& date, double amount, const string& desc) {

    Expense* e = new Expense(date, amount, desc);
    e->next = head;
    head = e;        //LIFO approach is used here, every time expense object comes at start of Llist.

    total_transport_cost += amount;

}

double FinanceReporter::getTotalTransportCost() const { return total_transport_cost; }

double FinanceReporter::getTotalExpenses() const { return total_transport_cost ;}

void FinanceReporter::generateReport(ostream& out) const {
    out << "=== Finance Report ===\n";
    Expense* current = head;
    while (current) {
        out << current->date << " | " << current->desc << " | " << current->amount << "\n";
       current = current->next;
    }

    out << "Total expenses: " << getTotalExpenses() << "\n";
}


void FinanceReporter::saveFile(const char* filename) const {
    ofstream file(filename);
    if (!file) {
        cout << "File does not open";
        return;
    }

    Expense* temp = head;
    while (temp) {
        file << temp->date << " "
             << temp->amount << " "
             << temp->desc << "\n";
        temp = temp->next;
    }
    file.close();
}







string FinanceReporter::serializeJSON() const {
    json j;
    j["total_transport_cost"] = total_transport_cost;
    j["expenses"] = json::array();

    Expense* current = head;
    while (current) {
        json e;
        e["date"] = current->date;
        e["desc"] = current->desc;
        e["amount"] = current->amount;
        j["expenses"].push_back(e);
        current = current->next;
    }

    return j.dump(4);
}


void FinanceReporter::deserializeJSON(const string& jsonStr) {
    json j = json::parse(jsonStr);
    Expense* current = head;
    while (current) {
        Expense* t = current;
        current = current->next;
        delete t;
    }
    head = nullptr;
    total_transport_cost = 0.0;

    if (j.contains("total_transport_cost")) {
        total_transport_cost = j["total_transport_cost"].get<double>();
    }

    if (j.contains("expenses")) {
        for (auto& e : j["expenses"]) {
            addExpense(
                e["date"].get<string>(),
                e["amount"].get<double>(),
                e["desc"].get<string>()
            );
        }
    }
}
