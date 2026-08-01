//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef FINANCEREPORTER_H
#define FINANCEREPORTER_H
#include <string>
#include <iostream>
#include "common.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;


#include <string>
#include <iostream>

class FinanceReporter {
private:
    double total_transport_cost;


    struct Expense {
        string date;
        double amount;
        string desc;
        Expense* next;
        Expense(const string& d="", double a=0.0, const string& ds="")
            : date(d), amount(a), desc(ds), next(nullptr) {}
    };
    Expense* head;

public:
    FinanceReporter();
    ~FinanceReporter();

    void addExpense(const string& date, double amount, const string& desc);
    double getTotalTransportCost() const;

    double getTotalExpenses() const;
    void generateReport(ostream& out) const;

    void saveFile(const char* filename) const;
    string serializeJSON() const;
    void deserializeJSON(const string& json);
};

#endif //FINANCEREPORTER_H
