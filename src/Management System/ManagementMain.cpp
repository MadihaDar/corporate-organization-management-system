#include <iostream>
#include "BSSE24067/management.h"
#include "BSSE24067/department.h"
#include "BSSE24067/leaveManagement.h"
#include "../Colors.h"

using namespace std;
extern void clearInput();

void managementMain()
{
    // Initialize System
    Management *management = Management::getInstance();
    // management->getDepartmentsManager()->loadAll();

    LeaveManagement leaveManagement;
    int choice = 0;
    do
    {
        Manager* loggedIn = Management::getLoggedInManager();
        string status = (loggedIn) ? loggedIn->getName() : "Not Logged In";

        cout << CLRSCR;
        cout << BOLD << CYAN << "========================================" << endl;
        cout << "CORPORATE MANAGEMENT SYSTEM" << endl;
        cout << "========================================" << RESET << endl;
        cout << "User Status: " << ((loggedIn) ? GREEN : RED) << status << RESET << endl;
        cout << YELLOW << "----------------------------------------" << RESET << endl;
        cout << GREEN << "1." << RESET << " Management Menu (Requires Login)" << endl;
        cout << GREEN << "2." << RESET << " Departmental Menu (Requires Login)" << endl;
        cout << GREEN << "3." << RESET << " Leave Management Menu (Managers Only)" << endl;
        cout << GREEN << "4." << RESET << " Apply for Leave (Employee)" << endl;
        if (loggedIn)
            cout << GREEN << "5." << RESET << " Logout" << endl;
        cout << RED << "0. Exit Module" << RESET << endl;
        cout << "Enter Input: ";
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
        case 1: // management menu
        {
            cout << CLRSCR;
            if (!loggedIn)
            {
                loggedIn = management->login();
            }

            if (loggedIn)
            {
                management->managementMenu(loggedIn);
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 2: // departmental menu
        {
            cout << CLRSCR;
             if (!loggedIn)
            {
                loggedIn = management->login();
            }

            if (loggedIn)
            {
                management->getDepartmentsManager()->departmentsMain(management);
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 3: // leave management (Manager Mode)
        {
            cout << CLRSCR;
            if (!loggedIn)
            {
                loggedIn = management->login();
            }
            
            if (loggedIn)
            {
                leaveManagement.leaveMenu(loggedIn);
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 4: // apply for Leave (Employee Mode)
        {
            cout << CLRSCR;
            string id;
            cout << "Enter your Employee ID: ";
            cin.ignore();
            getline(cin, id);

            Person* p = management->findEmployee(id);
            if (p)
            {
                leaveManagement.applyForLeaveMenu(p);
            }
            else
            {
                cout << RED << "Employee with ID " << id << " not found." << RESET << endl;
            }
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
        
        case 5: // logout
            if (loggedIn)
            {
                Management::logout();
                cout << GREEN << "Logged Out Successfully." << RESET << endl;
            }
            else
            {
                cout << RED << "Invalid Input." << RESET << endl;
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;

        case 0: // Exit
            cout << GREEN << "Exiting Corporate Management Module..." << RESET << endl;
            break;

        default:
            cout << RED << "Invalid Input." << RESET << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }
    } while (choice != 0);

    return;
}
