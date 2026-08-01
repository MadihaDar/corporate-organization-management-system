#include "management.h"
#include "leaveManagement.h"
#include "../Colors.h"

extern void clearInput();

// static vars
Management *Management::instance = nullptr;
Manager *Management::loggedInManager = nullptr;

Person *Management::findEmployee(const string& id)
{
    if (departmentsManager)
    {
        return departmentsManager->findEmployee(id);
    }
    return nullptr;
}

//______________________________________________________________________________________________________

void Management ::savePasswords()
{
    json jPasswords;
    int V = managerGraph.getVertexCount();
    for (int i = 0; i < V; ++i)
    {
        Manager *m = managerGraph.getDataAt(i);
        if (m && !m->getEmail().empty() && !m->getPassword().empty())
        {
            jPasswords[m->getEmail()] = m->getPassword();
        }
    }
    ofstream file("passwords.json");
    if (file.is_open())
    {
        file << jPasswords.dump(4);
        file.close();
    }
}
//______________________________________________________________________________________________________

void Management::loadPasswords()
{
    ifstream file("passwords.json");
    if (!file.is_open())
        return;

    json jPasswords;
    file >> jPasswords;
    file.close();

    int V = managerGraph.getVertexCount();
    for (int i = 0; i < V; ++i)
    {
        Manager *m = managerGraph.getDataAt(i);
        if (m && !m->getEmail().empty())
        {
            if (jPasswords.contains(m->getEmail()))
            {
                string pass = jPasswords[m->getEmail()];
                m->setPassword(pass);
            }
        }
    }
}
//______________________________________________________________________________________________________

Manager *Management ::login() const
{
    if (loggedInManager)
    {
        return loggedInManager;
    }

    cout << "Enter Manager Email: ";
    string enteredEmail;
    cin.ignore();
    getline(cin, enteredEmail);

    cout << "Enter Password: ";
    string enteredPassword;
    getline(cin, enteredPassword);

    // vertex count of graph
    int V = managerGraph.getVertexCount();
    // traversal through graph
    for (int i = 0; i < V; ++i)
    {
        Manager *m = managerGraph.getDataAt(i);
        // matching the required data
        if (m && m->getEmail() == enteredEmail)
        {
            if (m->getPassword() == enteredPassword)
            {
                loggedInManager = m; //set session
                return m;
            }
            else
            {
                cout << "Incorrect Password.\n";
                return nullptr;
            }
        }
    }

    cout << "Manager with email " << enteredEmail << " not found or Access Denied.\n";
    return nullptr;
}

//______________________________________________________________________________________________________
MidLevelManager *Management::getDepartmentManager(const string &dept)
{
    if (dept == "Manufacturing")
        return manufacturingManager;
    if (dept == "Logistics")
        return logisticsManager;
    if (dept == "Franchise")
        return franchiseManager;
    return nullptr;
}

//______________________________________________________________________________________________________
bool Management::isCorrectManager(const string &dept) const
{
    return (dept == "Manufacturing" && manufacturingManager) ||
           (dept == "Logistics" && logisticsManager) ||
           (dept == "Franchise" && franchiseManager);
}
//______________________________________________________________________________________________________

int Management ::addManager(Manager *manager) { return managerGraph.addVertex(manager); }
//______________________________________________________________________________________________________

bool Management ::updateManager(Manager *oldManager, Manager *newManager)
{
    // find manager to be updated
    int index = managerGraph.findVertex(oldManager);
    if (index == -1)
    {
        cout << "\nManager NOT Found.\n";
        return false;
    }

    // if found, update:
    managerGraph.setDataAt(index, newManager);
    delete oldManager;
    return true;
}
//______________________________________________________________________________________________________

bool Management ::updateManagerData(Manager *manager, const Manager &updateData)
{
    // find manager manager's data is to be updated
    int index = managerGraph.findVertex(manager);
    // if not found, return
    if (index == -1)
    {
        cout << "\nManager NOT Found.\n";
        return false;
    }

    Manager *storedManager = managerGraph.getDataAt(index);
    storedManager->updateFrom(updateData);

    return true;
}
//______________________________________________________________________________________________________

void Management ::connectManagers(Manager *managerOne, Manager *managerTwo)
{
    // find vertices
    int vertexOne = managerGraph.findVertex(managerOne);
    int vertexTwo = managerGraph.findVertex(managerTwo);

    // if not found
    if (vertexOne == -1 || vertexTwo == -1)
    {
        cout << "Manager NOT found.\n";
        return;
    }

    // form connection
    managerGraph.addEdge(vertexOne, vertexTwo);
}
//______________________________________________________________________________________________________

MidLevelManager *Management::getMidLevelManagerByID(const string &id)
{
    // Check mid-level managers
    if (manufacturingManager && manufacturingManager->getID() == id)
    {
        return manufacturingManager;
    }
    if (logisticsManager && logisticsManager->getID() == id)
    {
        return logisticsManager;
    }
    if (franchiseManager && franchiseManager->getID() == id)
    {
        return franchiseManager;
    }
    else
    {
        return nullptr;
    }
}

//______________________________________________________________________________________________________
void Management::setUpGraph()
{
    managerGraph.clear(); // reset

    managerGraph.addVertex(ceo);
    managerGraph.addVertex(coo);
    managerGraph.addVertex(cfo);

    managerGraph.addVertex(manufacturingManager);
    managerGraph.addVertex(logisticsManager);
    managerGraph.addVertex(franchiseManager);

    // franchise queue
    // Queue<MidLevelManager *> *tempQueue = franchiseManager;
    // int index = 0;
    // while (index < tempQueue->size())
    //{
    //    MidLevelManager *m = tempQueue->getByIndex(index);
    //    managerGraph.addVertex(m);
    //    index++;
    //}
}
//______________________________________________________________________________________________________

void Management::setUpConnections()
{
    // ceo -> coo & cfo
    connectManagers(ceo, coo);
    connectManagers(ceo, cfo);

    // coo & cfo - > HR and accounting
    // connectManagers(coo,  HRManager);
    // connectManagers(cfo, accountingManager);

    // coo -> mid level managers
    connectManagers(coo, manufacturingManager);
    connectManagers(coo, logisticsManager);
    connectManagers(coo, franchiseManager);
    // connectManagers(coo, accountingManager);

    // cfo -> mid level managers
    connectManagers(cfo, manufacturingManager);
    connectManagers(cfo, logisticsManager);
    connectManagers(cfo, franchiseManager);
    // connectManagers(cfo, accountingManager);

    // Queue<MidLevelManager *> *tempQueue = franchiseManager;
    // int index = 0;
    // while (index < tempQueue->getSize())
    //{
    //     MidLevelManager *m = tempQueue->getByIndex(index);
    //     managerGraph.addVertex(m);
    //     index++;
    //     connectManagers(coo, m);
    //     connectManagers(cfo, m);
    // }
}

//______________________________________________________________________________________________________

int Management::loadAllManagers(Manager **arr)
{
    int count = 0;

    if (ceo)
    {
        arr[count++] = ceo;
    }
    if (coo)
    {
        arr[count++] = coo;
    }
    if (cfo)
    {
        arr[count++] = cfo;
    }

    if (manufacturingManager)
    {
        arr[count++] = manufacturingManager;
    }

    if (logisticsManager)
    {
        arr[count++] = logisticsManager;
    }

    if (franchiseManager)
    {
        arr[count++] = franchiseManager;
    }

    return count;
}
//______________________________________________________________________________________________________

void Management::heapify(Manager **arr, int n, int i)
{
    int largest = i;       // parent
    int left = 2 * i + 1;  // left child
    int right = 2 * i + 2; // right child

    // left max check
    if (left < n && arr[left]->getSalary() > arr[largest]->getSalary())
        largest = left;

    // right check
    if (right < n && arr[right]->getSalary() > arr[largest]->getSalary())
        largest = right;

    if (largest != i)
    {
        // swap
        Manager *temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}
//______________________________________________________________________________________________________

void Management::heapSortManagers(Manager **arr, int n)
{
    // max heap (end to top)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // extract elements
    for (int i = n - 1; i > 0; i--)
    {
        Manager *temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}
//______________________________________________________________________________________________________

void Management::sortAllManagersBySalary()
{
    Manager *arr[6]; // CEO, COO, CFO + 3 mid-level
    int n = loadAllManagers(arr);

    if (n <= 1)
        return;

    heapSortManagers(arr, n);

    // optional: display sorted list
    cout << "\nManagers sorted by salary:\n";
    for (int i = 0; i < n; i++)
    {
        arr[i]->display();
    }
}
//______________________________________________________________________________________________________

void Management::saveGraph(const string &filename)
{
    json jGraph;

    json jManagerArray = json::array();
    // save top-level managers
    if (ceo)
        jManagerArray.push_back(ceo->toJson());
    if (coo)
        jManagerArray.push_back(coo->toJson());
    if (cfo)
        jManagerArray.push_back(cfo->toJson());

    // save mid-level managers
    if (manufacturingManager)
        jManagerArray.push_back(manufacturingManager->toJson());
    if (logisticsManager)
        jManagerArray.push_back(logisticsManager->toJson());
    if (franchiseManager)
        jManagerArray.push_back(franchiseManager->toJson());
    jGraph["managers"] = jManagerArray;
    // jGraph["managers"].push_back(accountingManager->toJson());

    // save franchise managers from queue
    // Queue<MidLevelManager *> *tempQueue = franchiseManager; // copy the exact
    // while (!tempQueue->isEmpty())
    //{
    //    MidLevelManager *m = tempQueue->dequeue();
    //    jGraph["managers"].push_back(m->toJson());
    //}

    // save adjacency matrix
    int V = managerGraph.getVertexCount();
    json jAdj = json::array();
    // traverse through the adj mat
    for (int i = 0; i < V; i++)
    {
        json row = json::array();
        for (int j = 0; j < V; j++)
        {
            row.push_back(managerGraph.hasEdge(i, j) ? 1 : 0);
        }
        jAdj.push_back(row);
    }
    jGraph["adjMatrix"] = jAdj;

    // write to file
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open file: " << filename << "\n";
        return;
    }

    file << jGraph.dump(4);
    file.close();
    cout << "Graph saved to " << filename << "\n";
}
//______________________________________________________________________________________________________

void Management::loadGraph(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open file: " << filename << "\n";
        return;
    }

    json jGraph;
    file >> jGraph;
    file.close();

    // clear the current graph
    managerGraph.clear();

    // make managers
    for (auto &jManager : jGraph["managers"])
    {
        Manager *m = nullptr;
        // check for designation first (Top-Level)
        if (jManager.contains("designation"))
        {
            string designation = jManager["designation"];
            if (designation == "CEO")
            {
                CEO *newCeo = new CEO();
                newCeo->fromJson(jManager);
                ceo = newCeo;
                m = ceo;
            }
            else if (designation == "COO")
            {
                COO *newCoo = new COO();
                newCoo->fromJson(jManager);
                coo = newCoo;
                m = coo;
            }
            else if (designation == "CFO")
            {
                CFO *newCfo = new CFO();
                newCfo->fromJson(jManager);
                cfo = newCfo;
                m = cfo;
            }
        }
        // check for department (Mid-Level)
        else if (jManager.contains("managerLevel") && jManager["managerLevel"] == "Mid-Level")
        {
            MidLevelManager *newMid = new MidLevelManager();
            newMid->fromJson(jManager);

            string dept = newMid->getDepartment();
            if (dept == "Manufacturing")
                manufacturingManager = newMid;
            else if (dept == "Logistics")
                logisticsManager = newMid;
            else if (dept == "Franchise")
                franchiseManager = newMid;

            m = newMid;
        }

        // if generic manager or fallback
        if (m == nullptr)
        {
            m = new Manager();
            m->fromJson(jManager);
        }

        managerGraph.addVertex(m);
    }

    // make adjaceny mat
    int V = managerGraph.getVertexCount();
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (jGraph["adjMatrix"][i][j] == 1)
            {
                managerGraph.addEdge(i, j);
            }
        }
    }

    cout << "Graph loaded from " << filename << "\n";
}
//______________________________________________________________________________________________________

MidLevelManager *Management ::getManagerForDepartment(const string &dept)
{
    // manufacturing dept
    if (dept == "Manufacturing")
    {
        return manufacturingManager;
    }

    // logistics dept
    else if (dept == "Logistics" || dept == "Logisctics") // Keeping typo check just in case
    {
        return logisticsManager;
    }

    else if (dept == "Franchise" || dept == "Financial")
    {
        return franchiseManager;
    }
    else
    {
        cout << "\nDeaprtment: " << dept << " not found.\n";
        return nullptr;
    }
}
//______________________________________________________________________________________________________

void Management::hireMidLevelManager(MidLevelManager *newManager)
{
    if (!newManager)
    {
        cout << "Invalid manager.\n";
        return;
    }

    // add manager to graph
    addManager(newManager);

    // assign to department pointer if department matches one of the predefined ones
    string dept = newManager->getDepartment();
    if (dept == "Manufacturing")
    {
        manufacturingManager = newManager;
    }
    else if (dept == "Logistics")
    {
        logisticsManager = newManager;
    }
    else if (dept == "Franchise")
    {
        franchiseManager = newManager;
    }
    else
    {
        cout << "Manager assigned to department: " << dept << "\n";
    }

    cout << "Manager " << newManager->getName() << " successfully added to the system.\n";
}
//______________________________________________________________________________________________________

void Management::hireCOO(COO *newCoo)
{
    if (!newCoo)
    {
        cout << "Invalid COO.\n";
        return;
    }

    // Connect to graph
    addManager(newCoo);

    // Assign
    coo = newCoo;

    cout << "COO " << newCoo->getName() << " successfully added to the system.\n";
}
//______________________________________________________________________________________________________

void Management::hireCFO(CFO *newCfo)
{
    if (!newCfo)
    {
        cout << "Invalid CFO.\n";
        return;
    }

    // Connect to graph
    addManager(newCfo);

    // Assign
    cfo = newCfo;

    cout << "CFO " << newCfo->getName() << " successfully added to the system.\n";
}
//______________________________________________________________________________________________________

void Management::hireCEO(CEO *newCeo)
{
    if (!newCeo)
    {
        cout << "Invalid CEO.\n";
        return;
    }

    // add CEO to graph
    addManager(newCeo);

    // assign
    ceo = newCeo;

    cout << "CEO " << newCeo->getName() << " successfully added to the system.\n";
}
//______________________________________________________________________________________________________
bool Management::isIDTaken(const string &id) const
{
    // check top level managers
    if (ceo && ceo->getID() == id)
        return true;
    if (coo && coo->getID() == id)
        return true;
    if (cfo && cfo->getID() == id)
        return true;

    // check mid level managers
    if (manufacturingManager && manufacturingManager->getID() == id)
        return true;
    if (logisticsManager && logisticsManager->getID() == id)
        return true;
    if (franchiseManager && franchiseManager->getID() == id)
        return true;

    return false; // not found anywhere
}
//______________________________________________________________________________________________________

void Management ::display()
{
    if (ceo)
        ceo->display();
    else
        cout << "CEO not assigned.\n";

    if (coo)
        coo->display();
    else
        cout << "COO not assigned.\n";

    if (cfo)
        cfo->display();
    else
        cout << "CFO not assigned.\n";

    if (manufacturingManager)
        manufacturingManager->display();
    else
        cout << "Manufacturing Manager not assigned.\n";

    if (logisticsManager)
        logisticsManager->display();
    else
        cout << "Logistics Manager not assigned.\n";

    if (franchiseManager)
        franchiseManager->display();
    else
        cout << "Franchise Manager not assigned.\n";

    cout << "\nManagement Hierarchy Graph Adjecent Matrix.\n";
    managerGraph.display();
}
//______________________________________________________________________________________________________

void Management::updateMidLevelManagerData(Manager *admin)
{
    if (!admin)
        return;

    if (isCFO(admin))
    {
        cout << RED << "\nAccess Denied." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Enter Manager Department: ";
    string dept;
    cin.ignore();
    getline(cin, dept);

    // get manager of that department
    MidLevelManager *deptManager = getDepartmentManager(dept);
    if (!deptManager)
    {
        cout << YELLOW << "Manager for " << dept << " not found. Creating new Manager..." << RESET << "\n";

        string id, name, email, pass;
        double salary;

        cout << "Enter Manager ID: ";
        cin.ignore();
        getline(cin, id);
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin.ignore();
        getline(cin, email);
        cout << "Enter Salary: ";
        cin >> salary;
        cout << "Enter Password: ";
        cin.ignore();
        cin >> pass;
        cin.ignore();

        MidLevelManager *newMgr = new MidLevelManager(id, name, "Manager", email, salary, dept);
        newMgr->setPassword(pass);

        hireMidLevelManager(newMgr);
        save();
        savePasswords();

        deptManager = newMgr; // assign so we can fall through if needed
        cout << GREEN << "Manager Created. Returning to menu." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "Enter the ID of the Manager to update: ";
    string id;
    getline(cin, id);

    // get manager pointer
    MidLevelManager *mgr = getMidLevelManagerByID(id);
    if (!mgr)
    {
        cout << RED << "Manager with ID " << id << " not found." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    int choice = -1;
    while (choice != 0)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Update Manager Menu___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Update Name\n";
        cout << GREEN << "2." << RESET << " Update Department\n";
        cout << GREEN << "3." << RESET << " Update Email\n";
        cout << GREEN << "4." << RESET << " Update Password\n";
        cout << RED << "0. Back" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "Enter New Name: ";
            getline(cin, name);
            mgr->setName(name);
            break;
        }

        case 2:
        {
            string newDept;
            cout << "Enter New Department: ";
            getline(cin, newDept);
            mgr->setDepartment(newDept);
            break;
        }
        case 3:
        {
            string email;
            cout << "Enter New Email: ";
            getline(cin, email);
            mgr->setEmail(email);
            break;
        }
        case 4:
        {
            string pass;
            cout << "Enter New Password: ";
            getline(cin, pass);
            mgr->setPassword(pass);
            break;
        }
        case 0:
            cout << GREEN << "Exiting update menu." << RESET << "\n";
            break;

        default:
            cout << RED << "Invalid choice. Try again." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
    }

    save();
    cout << GREEN << "Manager updated successfully." << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.get();
}
//______________________________________________________________________________________________________

void Management::updateMidLevelManagerSalaries(Manager *admin)
{
    if (!isCFO(admin) && !isCEO(admin))
    {
        cout << RED << "\nAccess Denied. Only CFO & CEO can update Mid-Level Manager salaries." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Enter Manager ID: ";
    string id;
    cin.ignore();
    getline(cin, id);

    MidLevelManager *mgr = getMidLevelManagerByID(id);
    if (!mgr)
    {
        cout << RED << "Manager not found." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    double newSalary;
    cout << "Enter New Salary: ";
    cin >> newSalary;

    mgr->setSalary(newSalary);

    save();
    cout << GREEN << "Salary updated successfully." << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}
//______________________________________________________________________________________________________
void Management::updateTopLevelSalaries(Manager *admin)
{
    if (!isCEO(admin))
    {
        cout << RED << "\nAccess Denied. Only CEO can update Top-Level salaries." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    int choice;
    cout << CLRSCR;
    cout << BOLD << CYAN << "___Update Top-Level Salaries___" << RESET << "\n";
    cout << GREEN << "1." << RESET << " Update CFO Salary\n";
    cout << GREEN << "2." << RESET << " Update COO Salary\n";
    cout << RED << "0. Back" << RESET << "\n";
    cout << "Enter choice: ";
    cin >> choice;
    
    if (cin.fail()) {
         cout << RED << "Invalid input!" << RESET << endl;
         clearInput();
         cout << "Press Enter to continue...";
         cin.get();
         return;
    }

    Manager *mgr = nullptr;

    switch (choice)
    {
    case 1:
        mgr = cfo;
        break;
    case 2:
        mgr = coo;
        break;
    case 0:
        return;
    default:
        cout << RED << "Invalid choice." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    if (!mgr)
    {
        cout << "Selected role does not exist.\n";
        return;
    }

    double newSalary;
    cout << "Enter New Salary: ";
    cin >> newSalary;

    mgr->setSalary(newSalary);

    save();
    cout << "Salary updated successfully.\n";
}

//______________________________________________________________________________________________________
void Management::updateCFOData(Manager *admin)
{
    if (!isCEO(admin) && !isCFO(admin))
    {
        cout << RED << "\nAccess Denied. Only CEO & CFO can update CFO data." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    Manager *mgr = cfo;
    if (!mgr)
    {
        cout << "CFO not found. Creating new CFO...\n";
        string id, name, email, pass;
        double salary;

        cout << "Enter CFO ID: ";
        cin.ignore();
        getline(cin, id);
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin.ignore();
        getline(cin, email);
        cout << "Enter Salary: ";
        cin >> salary;
        cout << "Enter Password: ";
        cin.ignore();
        cin >> pass;

        CFO *newCfo = new CFO(id, name, email, salary);
        newCfo->setPassword(pass);

        hireCFO(newCfo);
        save();
        savePasswords();
        cout << GREEN << "CFO Created." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    int choice = -1;
    while (choice != 0)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Update CFO Menu___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Update Name\n";
        cout << GREEN << "2." << RESET << " Update Email\n";
        cout << GREEN << "3." << RESET << " Update Role\n";
        cout << GREEN << "4." << RESET << " Update Password\n";
        cout << RED << "0. Exit" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "Enter New Name: ";
            getline(cin, name);
            mgr->setName(name);
            break;
        }

        case 2:
        {
            string email;
            cout << "Enter New Email: ";
            getline(cin, email);
            mgr->setEmail(email);
            break;
        }

        case 3:
        {
            string role;
            cout << "Enter New Role: ";
            getline(cin, role);
            mgr->setRole(role);
            break;
        }
        case 4:
        {
            string pass;
            cout << "Enter New Password: ";
            getline(cin, pass);
            mgr->setPassword(pass);
            break;
        }

        case 0:
            cout << GREEN << "Exiting CFO Update Menu." << RESET << "\n";
            break;

        default:
            cout << RED << "Invalid choice." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
    }

    save();
    cout << GREEN << "CFO updated successfully." << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.get();
}
//______________________________________________________________________________________________________

void Management::updateCOOData(Manager *admin)
{
    if (!isCEO(admin) && !isCOO(admin))
    {
        cout << RED << "\nAccess Denied. Only CEO & COO can update COO data." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    Manager *mgr = coo;
    if (!mgr)
    {
        cout << "COO not found. Creating new COO...\n";
        string id, name, email, pass;
        double salary;

        cout << "Enter COO ID: ";
        cin.ignore();
        getline(cin, id);
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin.ignore();
        getline(cin, email);
        cout << "Enter Salary: ";
        cin >> salary;
        cout << "Enter Password: ";
        cin.ignore();
        cin >> pass;

        COO *newCoo = new COO(id, name, email, salary);
        newCoo->setPassword(pass);

        hireCOO(newCoo);
        save();
        savePasswords();
        cout << GREEN << "COO Created." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    int choice = -1;
    while (choice != 0)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Update COO Menu___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Update Name\n";
        cout << GREEN << "2." << RESET << " Update Email\n";
        cout << GREEN << "3." << RESET << " Update Role\n";
        cout << GREEN << "4." << RESET << " Update Password\n";
        cout << RED << "0. Exit" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "Enter New Name: ";
            getline(cin, name);
            mgr->setName(name);
            break;
        }

        case 2:
        {
            string email;
            cout << "Enter New Email: ";
            getline(cin, email);
            mgr->setEmail(email);
            break;
        }

        case 3:
        {
            string role;
            cout << "Enter New Role: ";
            getline(cin, role);
            mgr->setRole(role);
            break;
        }
        case 4:
        {
            string pass;
            cout << "Enter New Password: ";
            getline(cin, pass);
            mgr->setPassword(pass);
            break;
        }

        case 0:
            cout << GREEN << "Exiting COO Update Menu." << RESET << "\n";
            break;

        default:
            cout << RED << "Invalid choice." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
    }

    save();
    cout << GREEN << "COO updated successfully." << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.get();
}
//______________________________________________________________________________________________________

void Management::updateCEOData(Manager *admin)
{
    if (!isCEO(admin))
    {
        cout << RED << "\nAccess Denied. Only CEO can update CEO data." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    Manager *mgr = ceo;

    int choice = -1;
    while (choice != 0)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Update CEO Menu___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Update Name\n";
        cout << GREEN << "2." << RESET << " Update Email\n";
        cout << GREEN << "3." << RESET << " Update Role\n";
        cout << GREEN << "4." << RESET << " Update Password\n";
        cout << RED << "0. Exit" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "Enter New Name: ";
            getline(cin, name);
            mgr->setName(name);
            break;
        }

        case 2:
        {
            string email;
            cout << "Enter New Email: ";
            getline(cin, email);
            mgr->setEmail(email);
            break;
        }

        case 3:
        {
            string role;
            cout << "Enter New Role: ";
            getline(cin, role);
            mgr->setRole(role);
            break;
        }

        case 4:
        {
            string pass;
            cout << "Enter New Password: ";
            getline(cin, pass);
            mgr->setPassword(pass);
            break;
        }

        case 0:
            cout << GREEN << "Exiting CEO Update Menu." << RESET << "\n";
            break;

        default:
            cout << RED << "Invalid choice. Try again." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }
    }

    save();
    cout << GREEN << "CEO updated successfully." << RESET << "\n";
    cout << "Press Enter to continue...";
    cin.get();
}

//______________________________________________________________________________________________________

void Management::managementMenu(Manager *loggedIn)
{
    int choice;
    
    do
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "========================================" << endl;
        cout << "           MANAGEMENT MENU              " << endl;
        cout << "========================================" << RESET << endl;
        cout << GREEN << "1." << RESET << " Hire Mid Level Manager\n";
        cout << GREEN << "2." << RESET << " View Management Hierarchy\n";
        cout << GREEN << "3." << RESET << " View Least To Highest Paid Managers\n";
        cout << GREEN << "4." << RESET << " Update Managers (Restricted)\n";
        cout << GREEN << "5." << RESET << " Update Top-Level Managers Salaries\n";
        cout << GREEN << "6." << RESET << " Update Mid-Level Managers Salaries\n";
        cout << RED << "0. Exit/Return" << RESET << "\n";
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
        case 1:
        {
            cout << CLRSCR;
            if (!isCEO(loggedIn) && !isCOO(loggedIn))
            {
                cout << RED << "Access Denied. Only CEO/COO can hire Managers." << RESET << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }

            cout << BOLD << YELLOW << "___Hire a Manager___" << RESET << "\n";

            string id, name, email, dept, pass;
            double salary;

            cout << "Enter Manager ID: ";
            cin.ignore();
            getline(cin, id);
            while (isIDTaken(id))
            {
                cout << RED << "ID Already Taken. Enter New ID: " << RESET;
                getline(cin, id);
            }

            cout << "Enter Manager Name: ";
            getline(cin, name);

            cout << "Enter Manager Email: ";
            getline(cin, email);

            cout << "Enter Manager Password: ";
            getline(cin, pass);

            cout << "Enter Manager Salary: ";
            cin >> salary;
            cin.ignore();

            cout << "Enter Manager Department: ";
            getline(cin, dept);

            MidLevelManager *newManager =
                new MidLevelManager(id, name, "Manager", email, salary, dept);
            newManager->setPassword(pass);

            // hire manager via system
            this->hireMidLevelManager(newManager);
            this->save();

            cout << GREEN << "Manager " << name << " hired successfully under "
                 << dept << " department." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            break;
        }

        case 2:
        {
            cout << CLRSCR;
            this->display();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 3:
        {
            cout << CLRSCR;
            sortAllManagersBySalary();
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }

        case 4:
        {
            updateManagerMenu(loggedIn);
            break;
        }

        case 5:
        {
            updateTopLevelSalaries(loggedIn);
            break;
        }

        case 6:
        {
            updateMidLevelManagerSalaries(loggedIn);
            break;
        }
        case 0:
            cout << GREEN << "Exiting Management Menu..." << RESET << "\n";
            break;

        default:
            cout << RED << "Invalid choice. Try again." << RESET << "\n";
             cout << "Press Enter to continue...";
             cin.ignore();
             cin.get();
            break;
        }

    } while (choice != 0);
}
//______________________________________________________________________________________________________

void Management ::updateManagerMenu(Manager *admin)
{
    cout << CLRSCR;
    cout << BOLD << CYAN << "========================================" << endl;
    cout << "           UPDATE MENU                 " << endl;
    cout << "========================================" << RESET << endl;
    cout << GREEN << "1." << RESET << " Update Mid-Level Manager\n";
    cout << GREEN << "2." << RESET << " Update COO\n";
    cout << GREEN << "3." << RESET << " Update CFO\n";
    cout << GREEN << "4." << RESET << " Update CEO\n";
    cout << RED << "0. Back" << RESET << "\n";
    cout << "Enter choice: ";
    int uChoice;
    cin >> uChoice;
    
    if (cin.fail()) {
         cout << RED << "Invalid input!" << RESET << endl;
         clearInput();
         cout << "Press Enter to continue...";
         cin.get();
         return;
    }

    if (uChoice == 1)
        updateMidLevelManagerData(admin);
    else if (uChoice == 2)
        updateCOOData(admin);
    else if (uChoice == 3)
        updateCFOData(admin);
    else if (uChoice == 4)
        updateCEOData(admin);
    else if (uChoice == 0)
        return;
    else {
        cout << RED << "Invalid option." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
//_______________________________________________________________________________________________________