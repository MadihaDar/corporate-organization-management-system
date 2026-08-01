#include <iostream>
#include <cstdlib>
#include <ctime>
#include "factory.h"

using namespace std;

#include "../Colors.h"

extern void clearInput();

void factoryMain() {
    // Seed random number generator
    srand(time(nullptr));

    cout << CLRSCR;
    cout << BOLD << CYAN << "========================================\n";
    cout << "   FACTORY MANAGEMENT SYSTEM\n";
    cout << "========================================" << RESET << "\n";
    cout << "Initializing factory...\n";
    // Create factory
    auto factory = new Factory();

    cout << GREEN << "Factory initialized successfully!" << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();

    // Run interactive menu
    factory->runMenu();

    // Cleanup
    cout << "\nCleaning up...\n";
    delete factory;

    cout << GREEN << "Factory Module Terminated." << RESET << endl;
    cout << "Press Enter to continue...";
    cin.get();
}
