#include <iostream>
#include <string>
#include "FinancialSystem.h"
#include "FeedbackSystem.h"
#include "../Colors.h"
using namespace std;
extern void clearInput();

void handleFinancialMenu(FinancialDatabase* finDB)
{
    int choice;
    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "============================================" << endl;
        cout << " FINANCE MANAGEMENT MODULE" << endl;
        cout << "============================================" << RESET << endl;
        cout << GREEN << "1." << RESET << " Log Transaction (Manual)" << endl;
        cout << GREEN << "2." << RESET << " Approve Pending Operations" << endl;
        cout << GREEN << "3." << RESET << " Undo Last Operation" << endl;
        cout << GREEN << "4." << RESET << " View Module Reports" << endl;
        cout << GREEN << "5." << RESET << " Analyze Supply Chain Costs (Graph)" << endl;
        cout << GREEN << "6." << RESET << " Locate Optimal Supply Chain Route (Graph)" << endl;
        cout << GREEN << "7." << RESET << " Audit Trail (Trace Product by ID)" << endl;
        cout << GREEN << "8." << RESET << " Most Expensive Financial Logs (merge)" << endl;
        cout << GREEN << "9." << RESET << " Cheapest (bubble)" << endl;
        cout << RED << "0. Back to Main Menu" << RESET << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        if (choice == 0) break;

        switch (choice)
        {
        case 1:
            {
                cout << CLRSCR;
                cout << BOLD << YELLOW << "--- Log Transaction ---" << RESET << endl;
                string modStr, desc, type, trace;
                double amt;
                cout << "Enter Module (Management/Warehouse/Franchising/Production/Financial/Feedback): ";
                cin >> modStr;
                cin.ignore();
                cout << "Enter Description: ";
                getline(cin, desc);
                cout << "Enter Amount: ";
                cin >> amt;
                cout << "Enter Type (Revenue/Expense): ";
                cin >> type;
                cout << "Enter Trace ID (Optional): ";
                cin >> trace;

                const ModuleType mod = getModuleType(modStr);
                if (mod != UNKNOWN)
                {
                    finDB->logTransaction(mod, desc, amt, type, trace);
                    cout << GREEN << "Transaction Logged!" << RESET << endl;
                }
                else
                {
                    cout << RED << "Invalid Module Name!" << RESET << endl;
                }
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 2:
            {
                cout << CLRSCR;
                finDB->approveNextOperation();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 3:
            {
                cout << CLRSCR;
                finDB->undoLastOperation();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 4:
            {
                cout << CLRSCR;
                string modStr;
                cout << "Enter Module Name: ";
                cin >> modStr;
                const ModuleType mod = getModuleType(modStr);
                if (mod != UNKNOWN)
                {
                    finDB->generateModuleReport(mod);
                }
                else
                {
                    cout << RED << "Invalid Module Name!" << RESET << endl;
                }
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 5:
            {
                cout << CLRSCR;
                finDB->analyzeSupplyChainCosts();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 6:
            {
                cout << CLRSCR;
                finDB->optimizeLogistics(MANAGEMENT, FRANCHISING);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 7:
            {
                cout << CLRSCR;
                string trace;
                cout << "Enter Product Trace ID (e.g., BATCH-001): ";
                cin >> trace;
                finDB->traceProductLifecycle(trace);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 8:
            {
                cout << CLRSCR;
                finDB->displayMostExpensiveLogs();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 9:
            {
                cout << CLRSCR;
                finDB->displayCheapestLogs();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        default: 
            cout << RED << "Invalid choice." << RESET << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
}

void handleCustomerMenu(FeedbackSystem& fbSys)
{
    int custChoice;
    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "\n--- Customer Portal ---" << RESET << endl;
        cout << GREEN << "1." << RESET << " Submit Feedback" << endl;
        cout << RED << "0. Back to Main Menu" << RESET << endl;
        cout << "Enter choice: ";
        cin >> custChoice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }
        
        if (custChoice == 0) break;

        if (custChoice == 1)
        {
            cout << CLRSCR;
            int rating;
            string name, content, date, cat;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Content: ";
            getline(cin, content);
            cout << "Enter Rating (1-5): ";
            cin >> rating;
            cout << "Enter Date: ";
            cin >> date;
            cin.ignore();
            cout << "Enter Category (Product Quality/Store Service/Delivery/Website): ";
            getline(cin, cat);
            fbSys.submitFeedback(Feedback(name, content, rating, date, cat));
            cout << GREEN << "Feedback Submitted!" << RESET << endl;
            cout << "Press Enter to continue...";
            cin.get(); // cin.ignore() already called above
        }
        else
        {
            cout << RED << "Invalid choice." << RESET << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
}

void handleFeedbackManagementMenu(FeedbackSystem& fbSys)
{
    int fbChoice;
    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "\n--- Feedback & Ticket Management ---" << RESET << endl;
        cout << GREEN << "1." << RESET << " View All Feedback (Categorized)" << endl;
        cout << GREEN << "2." << RESET << " View Top Rated Feedback (Merge Sort)" << endl;
        cout << GREEN << "3." << RESET << " View Critical/Low Rated Feedback (Bubble Sort)" << endl;
        cout << GREEN << "4." << RESET << " View Rating Distribution (Counting Logic)" << endl;
        cout << GREEN << "5." << RESET << " Raise Support Ticket" << endl;
        cout << GREEN << "6." << RESET << " Resolve Next Ticket" << endl;
        cout << GREEN << "7." << RESET << " Undo Ticket Resolution" << endl;
        cout << GREEN << "8." << RESET << " View System Architecture Info" << endl;
        cout << RED << "0. Back to Main Menu" << RESET << endl;
        cout << "Enter choice: ";
        cin >> fbChoice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }
        
        if (fbChoice == 0) break;

        switch (fbChoice)
        {
        case 1: 
            cout << CLRSCR;
            fbSys.displayAllFeedback();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 2:
            cout << CLRSCR;
            fbSys.displayTopRatedFeedback();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 3:
            cout << CLRSCR;
            fbSys.displayLowRatedFeedback();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 4:
            cout << CLRSCR;
            fbSys.displayRatingDistribution();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 5:
            {
                cout << CLRSCR;
                int priority;
                string issue;
                cin.ignore();
                cout << "Enter Issue: ";
                getline(cin, issue);
                cout << "Enter Priority (1-10): ";
                cin >> priority;
                fbSys.raiseTicket(Ticket(issue, priority));
                cout << GREEN << "Ticket Raised!" << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore(); // consume newline
                cin.get(); // wait for enter
                break;
            }
        case 6:
            {
                cout << CLRSCR;
                fbSys.resolveNextTicket();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 7:
            {
                cout << CLRSCR;
                fbSys.undoLastTicketAction();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        case 8:
            {
                cout << CLRSCR;
                FeedbackSystem::displaySystemInfo();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        default:
            {
                cout << RED << "Invalid choice." << RESET << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
        }
    }
}

void testGraphSystem()
{
    FinancialDatabase* sys = FinancialDatabase::getInstance();
    // Management -> Production (100)
    sys->defineSupplyChainLink(MANAGEMENT, PRODUCTION, 100.0f);
    //
    // Production -> Warehouse (50)
    sys->defineSupplyChainLink(PRODUCTION, WAREHOUSE, 50.0f);
    //
    // Warehouse -> Franchising (50)
    sys->defineSupplyChainLink(WAREHOUSE, FRANCHISING, 50.0f);
    //
    // Management -> Warehouse (10)
    sys->defineSupplyChainLink(MANAGEMENT, WAREHOUSE, 10.0f);
    //
    //production -> Franchising (200)
    sys->defineSupplyChainLink(PRODUCTION, FRANCHISING, 200.0f);
    //
    //Warehouse -> Production (20)
    sys->defineSupplyChainLink(WAREHOUSE, PRODUCTION, 20.0f);

    // cout << "Scenario: Finding best route from Management to Franchising." << endl;

    // Expect Management -> Warehouse(10) -> Franchising(50) = Cost 60
    // sys->optimizeLogistics(MANAGEMENT, FRANCHISING);
}

void financialFeedbackMain()
{
    //init
    testGraphSystem(); //graph test

    FinancialDatabase* finDB = FinancialDatabase::getInstance();
    FeedbackSystem fbSys;

    int choice;
    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "=============================================" << endl;
        cout << "   FINANCIAL & FEEDBACK MANAGEMENT SYSTEM    " << endl;
        cout << "=============================================" << RESET << endl;
        cout << GREEN << "1." << RESET << " Financial Management (Central)" << endl;
        cout << GREEN << "2." << RESET << " Customer Portal (Submit Feedback)" << endl;
        cout << GREEN << "3." << RESET << " Feedback & Ticket Management (Staff)" << endl;
        cout << GREEN << "4." << RESET << " Simulation (Testing Audit) (Approve first!)" << endl;
        cout << RED << "0. Exit Module" << RESET << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        switch (choice)
        {
        case 1: handleFinancialMenu(finDB);
            break;
        case 2: handleCustomerMenu(fbSys);
            break;
        case 3: handleFeedbackManagementMenu(fbSys);
            break;
        case 4:
            cout << CLRSCR;
            cout << "Simulation: Moving 'BATCH-001' through supply chain..." << endl;
            finDB->logTransaction(WAREHOUSE, "Received BATCH-001 from Production", 50.0, "Expense", "BATCH-001");
            finDB->logTransaction(FRANCHISING, "Sold BATCH-001 Units", 8000.0, "Revenue", "BATCH-001");

            cout << GREEN << "Simulated!" << RESET << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 0:
            cout << GREEN << "Exiting Financial Module..." << RESET << endl;
            return;
        default:
            cout << RED << "Invalid choice." << RESET << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
}
