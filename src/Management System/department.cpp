#include "department.h"
#include "management.h"
#include "../Colors.h"

extern void clearInput();

// hire: insert under manager (manager is already be set)
void Department::hireEmployee(Employee *e)
{
    if (!manager)
    {
        cout << "cannot hire: department has no manager assigned\n";
        return;
    }

    // attach emeloyee under manager (root)
    teamTree->insertNode(e);
}
//______________________________________________________________________________________________________

// fire/remove emeloyee by pointer
bool Department::fireEmployee(Employee *e)
{
    if (!manager)
    {
        cout << "no manager assigned\n";
        return false;
    }
    bool deleteNode = teamTree->deleteNode(e);
    if (!deleteNode)
        cout << "emeloyee not found in this deeartment\n";
    return deleteNode;
}
//______________________________________________________________________________________________________

int Department::countEmployees(Tree<Person> &tree)
{
    int count = 0;
    // for traversal
    TreeNode<Person> *t = tree.getFirstChild();
    while (t)
    {
        // update count
        count++;
        t = t->getNextSibling();
    }
    return count;
}
//______________________________________________________________________________________________________

void Department::treeToArray(Tree<Person> &tree, Person **arr)
{
    // traversal
    TreeNode<Person> *t = tree.getFirstChild();
    int i = 0;
    while (t)
    {
        // tree to array
        arr[i++] = t->getData();
        t = t->getNextSibling();
    }
}
//______________________________________________________________________________________________________

void Department::arrayToTree(Tree<Person> &tree, Person **arr, int n)
{
    // for traversal
    TreeNode<Person> *temp = tree.getFirstChild();
    int i = 0;
    // traverse through the emp array
    while (temp && i < n)
    {
        // array to tree
        temp->setData(arr[i++]);
        temp = temp->getNextSibling();
    }
}
//______________________________________________________________________________________________________

void Department::selectionSort(Tree<Person> &tree)
{
    // count of emp
    int n = countEmployees(tree);
    // if no emp return
    if (n <= 1)
        return;

    // array for emp
    Person **arr = new Person *[n];
    treeToArray(tree, arr); // conversion

    // traversal
    for (int i = 0; i < n - 1; i++)
    {
        // selection sort implementation
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
        {
            // comparison
            if (arr[j]->getSalary() < arr[minIdx]->getSalary())
                minIdx = j;
        }
        if (minIdx != i)
            swap(arr[i], arr[minIdx]);
    }

    // back to tree
    arrayToTree(tree, arr, n);
    delete[] arr;
}
//______________________________________________________________________________________________________

void Department::merge(Person **arr, int left, int mid, int right)
{
    // sizes of divided arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // arrays
    Person **leftArr = new Person *[n1];
    Person **rightArr = new Person *[n2];

    // separate elements
    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        rightArr[j] = arr[mid + 1 + j];
    }

    // for sort
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        // comparison
        if (leftArr[i]->getSalary() <= rightArr[j]->getSalary())
        {
            arr[k++] = leftArr[i++];
        }
        else
        {
            arr[k++] = rightArr[j++];
        }
    }

    // condition of the exhausted array
    while (i < n1)
    {
        arr[k++] = leftArr[i++];
    }
    while (j < n2)
    {
        arr[k++] = rightArr[j++];
    }

    // freeing memory :)
    delete[] leftArr;
    delete[] rightArr;
}
//______________________________________________________________________________________________________

void Department::mergeSortArray(Person **arr, int left, int right)
{
    // calling the merge function
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSortArray(arr, left, mid);
    mergeSortArray(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
//______________________________________________________________________________________________________

void Department::mergeSort(Tree<Person> &tree)
{
    // count employees
    int n = countEmployees(tree);
    if (n <= 1)
        return;

    // create array
    Person **arr = new Person *[n];

    treeToArray(tree, arr);

    // apply merge sort
    mergeSortArray(arr, 0, n - 1);

    arrayToTree(tree, arr, n);

    // freeing memory :)
    delete[] arr;
}

//______________________________________________________________________________________________________

void Manufacturing::menu(Management *mgmt)
{
    int choice;

    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Manufacturing Department___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Assign Manager\n";
        cout << GREEN << "2." << RESET << " Hire Employee\n";
        cout << GREEN << "3." << RESET << " Fire Employee (by ID)\n";
        cout << GREEN << "4." << RESET << " Display Team Tree\n";
        cout << RED << "0. Back" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        if (choice == 0)
            return;

        switch (choice)
        {
        case 1:
        {
            if (!mgmt)
            {
                cout << RED << "Management system error." << RESET << "\n";
                break;
            }
            MidLevelManager *mid = mgmt->getManagerForDepartment("Manufacturing");

            if (mid == nullptr)
            {
                cout << RED << "Manager not found in Management!" << RESET << "\n";
            }
            else
            {
                this->setManager(mid);
                cout << GREEN << "Manager assigned successfully." << RESET << "\n";
                saveToFile("manufacturing.json");
            }
            break;
        }

        case 2:
        {
            string name, id, email;
            double salary;

            cout << "Enter Employee Name: ";
            cin >> name;
            cout << "Enter Employee ID: ";
            cin >> id;
            cout << "Enter Employee Email: ";
            cin >> email;
            cout << "Enter Salary: ";
            cin >> salary;

            Employee *emp = new Employee(id, name, email, "Manufacturing", salary);
            this->hireEmployee(emp);

            selectionSort(manufacturingTree); // sort

            saveToFile("manufacturing.json");
            cout << GREEN << "\nEmployee hired!" << RESET << "\n";
            break;
        }

        case 3:
        {
            string id;
            cout << "Enter Employee ID to fire: ";
            cin.ignore();
            getline(cin, id);

            TreeNode<Person> *node = this->teamTree->findNodeByID(id);
            if (!node)
            {
                cout << RED << "Employee not found." << RESET << "\n";
            }
            else
            {
                Person *empPtr = node->getData();
                this->teamTree->deleteNode(empPtr);
                saveToFile("manufacturing.json");
                cout << GREEN << "Employee removed." << RESET << "\n";
            }
            break;
        }

        case 4:
            // loadFromFile("manufacturing.json");
            cout << CLRSCR;
            this->displayTeam();
            break;

        default:
            cout << RED << "Invalid choice!" << RESET << "\n";
            break;
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
//______________________________________________________________________________________________________

void Logistics::menu(Management *mgmt)
{
    int choice;

    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Logistics Department___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Assign Manager\n";
        cout << GREEN << "2." << RESET << " Hire Employee\n";
        cout << GREEN << "3." << RESET << " Fire Employee (by ID)\n";
        cout << GREEN << "4." << RESET << " Display Team Tree\n";
        cout << RED << "0. Back" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        if (choice == 0)
            return;

        switch (choice)
        {
        case 1:
        {
            if (!mgmt)
            {
                cout << RED << "Management system error." << RESET << "\n";
                break;
            }
            MidLevelManager *mid = mgmt->getManagerForDepartment("Logistics");

            if (!mid)
                cout << RED << "Manager not found!" << RESET << "\n";
            else
            {
                this->setManager(mid);
                cout << GREEN << "Manager assigned successfully." << RESET << "\n";
                saveToFile("logistics.json");
            }
            break;
        }

        case 2:
        {
            string name, id, email;
            double salary;

            cout << "Enter Employee Name: ";
            cin >> name;
            cout << "Enter Employee ID: ";
            cin >> id;
            cout << "Enter Employee Email: ";
            cin >> email;
            cout << "Enter Salary: ";
            cin >> salary;

            Employee *emp = new Employee(id, name, email, "Logistics", salary);
            this->hireEmployee(emp);

            mergeSort(logisticsTree); // sort

            saveToFile("logistics.json");
            cout << GREEN << "Employee hired!" << RESET << "\n";
            break;
        }

        case 3:
        {
            string id;
            cin.ignore();
            cout << "Enter Employee ID to fire: ";
            getline(cin, id);

            TreeNode<Person> *node = this->teamTree->findNodeByID(id);
            if (!node)
                cout << RED << "Employee not found." << RESET << "\n";
            else
            {
                Person *empPtr = node->getData();
                this->teamTree->deleteNode(empPtr);
                saveToFile("logistics.json");
                cout << GREEN << "Employee removed." << RESET << "\n";
            }
            break;
        }

        case 4:
            // loadFromFile("logistics.json");
            cout << CLRSCR;
            this->displayTeam();
            break;

        default:
            cout << RED << "Invalid choice!" << RESET << "\n";
            break;
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
//______________________________________________________________________________________________________

void Franchise::menu(Management *mgmt)
{
    int choice;

    while (true)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "___Franchise Department___" << RESET << "\n";
        cout << GREEN << "1." << RESET << " Assign Manager\n";
        cout << GREEN << "2." << RESET << " Hire Employee\n";
        cout << GREEN << "3." << RESET << " Fire Employee (by ID)\n";
        cout << GREEN << "4." << RESET << " Display Team Tree\n";
        cout << RED << "0. Back" << RESET << "\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
             cout << RED << "Invalid input!" << RESET << endl;
             clearInput();
             cout << "Press Enter to continue...";
             cin.get();
             continue;
        }

        if (choice == 0)
            return;

        switch (choice)
        {
        case 1:
        {
            if (!mgmt)
            {
                cout << RED << "Management system error." << RESET << "\n";
                break;
            }

            MidLevelManager *mid = mgmt->getManagerForDepartment("Franchise");

            if (!mid)
            {
                cout << RED << "Manager not found!" << RESET << "\n";
            }
            else
            {
                this->setManager(mid);
                cout << GREEN << "Manager assigned successfully." << RESET << "\n";
                saveToFile("franchise.json");
            }
            break;
        }

        case 2:
        {
            string name, id, email;
            double salary;

            cout << "Enter Employee Name: ";
            cin >> name;
            cout << "Enter Employee ID: ";
            cin >> id;
            cout << "Enter Employee Email: ";
            cin >> email;
            cout << "Enter Salary: ";
            cin >> salary;

            Employee *emp = new Employee(id, name, email, "Franchise", salary);
            this->hireEmployee(emp);

            cout << GREEN << "Employee hired!" << RESET << "\n";

            mergeSort(franchiseTree);

            saveToFile("franchise.json");
            break;
        }

        case 3:
        {
            string id;
            cin.ignore();
            cout << "Enter Employee ID to fire: ";
            getline(cin, id);

            TreeNode<Person> *node = this->teamTree->findNodeByID(id);
            if (!node)
                cout << RED << "Employee not found." << RESET << "\n";
            else
            {
                Person *empPtr = node->getData();
                this->teamTree->deleteNode(empPtr);
                saveToFile("franchise.json");
                cout << GREEN << "Employee removed." << RESET << "\n";
            }
            break;
        }

        case 4:
            // loadFromFile("franchise.json");
            cout << CLRSCR;
            this->displayTeam();
            break;

        default:
            cout << RED << "Invalid choice!" << RESET << "\n";
            break;
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
//______________________________________________________________________________________________________

json Department::toJson() const
{
    json j;

    j["department"] = departmentName;

    // manager
    if (manager)
        j["manager"] = manager->toJson();
    else
        j["manager"] = nullptr;

    // employees
    json empArr = json::array();
    TreeNode<Person> *child = teamTree->getRoot() ? teamTree->getFirstChild() : nullptr;

    while (child != nullptr)
    {
        empArr.push_back(child->getData()->toJson());
        child = child->getNextSibling();
    }

    j["employees"] = empArr;

    return j;
}
//______________________________________________________________________________________________________

void Department::fromJson(const json &j)
{
    departmentName = j["department"];

    teamTree->clear();
    // restore manager
    if (!j["manager"].is_null())
    {
        MidLevelManager *mid = new MidLevelManager();
        mid->fromJson(j["manager"]);
        setManager(mid);
    }

    // restore employees
    for (auto &e : j["employees"])
    {
        Employee *emp = new Employee();
        emp->fromJson(e);
        hireEmployee(emp);
    }
}
//______________________________________________________________________________________________________

void Department::saveToFile(const string &filename) const
{
    json j = toJson();

    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file " << filename << endl;
        return;
    }

    file << j.dump(4);
    file.close();

    cout << "Saved department -> " << filename << endl;
}
//______________________________________________________________________________________________________

void Department::loadFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file " << filename << endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    fromJson(j);

    cout << "Loaded department <- " << filename << endl;
}
//______________________________________________________________________________________________________

void DepartmentsManager::departmentsMain(Management *mgmt)
{
    int choice = -1;

    while (choice != 0)
    {
        cout << CLRSCR;
        cout << BOLD << CYAN << "========================================" << endl;
        cout << "           DEPARTMENT MENU              " << endl;
        cout << "========================================" << RESET << endl;
        cout << GREEN << "1." << RESET << " Manufacturing Department\n";
        cout << GREEN << "2." << RESET << " Logistics Department\n";
        cout << GREEN << "3." << RESET << " Franchise Department\n";
        cout << RED << "0. Exit" << RESET << "\n";
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
            manufacturing.menu(mgmt);
            break;

        case 2:
            logistics.menu(mgmt);
            break;

        case 3:
            franchise.menu(mgmt);
            break;

        case 0:
            cout << GREEN << "Exiting Department Menu." << RESET << "\n";
            return;

        default:
            cout << RED << "Invalid choice!" << RESET << "\n";
             cout << "Press Enter to continue...";
             cin.ignore();
             cin.get();
            break;
        }
    }
}

Department *DepartmentsManager::getDepartment(const string &name)
{
    if (name == "Manufacturing")
        return &manufacturing;
    if (name == "Logistics")
        return &logistics;
    if (name == "Franchise")
        return &franchise;
    return nullptr;
}

bool DepartmentsManager::isValidDepartment(const string &dept) const
{
    return dept == "Manufacturing" ||
           dept == "Logistics" ||
           dept == "Franchise";
}

void DepartmentsManager::saveAll()
{
    manufacturing.saveToFile("manufacturing.json");
    logistics.saveToFile("logistics.json");
    franchise.saveToFile("franchise.json");
}

void DepartmentsManager::loadAll()
{
    manufacturing.loadFromFile("manufacturing.json");
    logistics.loadFromFile("logistics.json");
    franchise.loadFromFile("franchise.json");
}

Person *DepartmentsManager::findEmployee(const string &id)
{
    Person *person = nullptr;

    person = manufacturing.findEmployee(id);
    if (person)
        return person;

    person = logistics.findEmployee(id);
    if (person)
        return person;

    person = franchise.findEmployee(id);
    if (person)
        return person;

    return nullptr;
}