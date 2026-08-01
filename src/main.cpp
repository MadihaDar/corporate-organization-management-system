#include <iostream>
#include <limits>
#include "ModuleMains.h"
#include "Colors.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    int choice;
    while (true) {
        cout << CLRSCR;
        cout << BOLD << CYAN << "============================================================\n";
        cout << "  ██████╗██╗  ██╗ █████╗  ██████╗ ████████╗██╗ ██████╗     ██████╗ ██████╗  ██████╗ \n";
        cout << " ██╔════╝██║  ██║██╔══██╗██╔═══██╗╚══██╔══╝██║██╔════╝    ██╔═══██╗██╔══██╗██╔════╝ \n";
        cout << " ██║     ███████║███████║██║   ██║   ██║   ██║██║         ██║   ██║██████╔╝██║  ███╗\n";
        cout << " ██║     ██╔══██║██╔══██║██║   ██║   ██║   ██║██║         ██║   ██║██╔══██╗██║   ██║\n";
        cout << " ╚██████╗██║  ██║██║  ██║╚██████╔╝   ██║   ██║╚██████╗    ╚██████╔╝██║  ██║╚██████╔╝\n";
        cout << "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝ ╚═════╝     ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ \n";
        cout << "============================================================" << RESET << "\n\n";

        cout << BOLD << YELLOW << "MAIN MENU" << RESET << "\n";
        cout << YELLOW << "-------------------------------------------------" << RESET << "\n";
        cout << GREEN << " 1." << RESET << " Financial & Feedback Management " << MAGENTA << "(BSSE24007)" << RESET << "\n";
        cout << GREEN << " 2." << RESET << " Franchise Management            " << MAGENTA << "(BSSE24025)" << RESET << "\n";
        cout << GREEN << " 3." << RESET << " Warehouse Management            " << MAGENTA << "(BSSE24027)" << RESET << "\n";
        cout << GREEN << " 4." << RESET << " Factory Management              " << MAGENTA << "(BSSE24039)" << RESET << "\n";
        cout << GREEN << " 5." << RESET << " Corporate Management            " << MAGENTA << "(BSSE24067)" << RESET << "\n";
        cout << RED << " 0. Exit System" << RESET << "\n";
        cout << YELLOW << "-------------------------------------------------" << RESET << "\n";
        cout << "Enter choice: ";
        
        cin >> choice;
        
        if (cin.fail()) {
            cout << RED << "Invalid input. Please enter a number." << RESET << "\n";
            clearInput();
            cout << "Press Enter to try again...";
            cin.get();
            continue;
        }

        switch (choice) {
            case 1:
                financialFeedbackMain();
                break;
            case 2:
                franchiseMain();
                break;
            case 3:
                warehouseMain();
                break;
            case 4:
                factoryMain();
                break;
            case 5:
                managementMain();
                break;
            case 0:
                cout << BOLD << CYAN << "Exiting Chaotic Org Management System... Goodbye!" << RESET << "\n";
                return 0;
            default:
                cout << RED << "Invalid choice. Please try again." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
        }
    }
    return 0;
}
