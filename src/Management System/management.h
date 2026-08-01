#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "department.h"
#include "template.h"
#include "fstream"
#include "json.hpp"
#include "managers.h"
#include "leaveManagement.h"
using nlohmann::json;

class LeaveManagement;
// CLASS 12
//______________________________________________________________________________
class Management
{
private:
    CEO *ceo;
    COO *coo;
    CFO *cfo;

    MidLevelManager *manufacturingManager;
    MidLevelManager *logisticsManager;
    MidLevelManager *franchiseManager;

    // aggregation
    DepartmentsManager *departmentsManager;
    //LeaveManagement leaveManager;

    Graph<Manager> managerGraph;

    static Management *instance;
    static Manager *loggedInManager;

public:
    static Management *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Management();
        }
        return instance;
    }

    static Manager *getLoggedInManager() { return loggedInManager; }
    static void setLoggedInManager(Manager *m) { loggedInManager = m; }
    static void logout() { loggedInManager = nullptr; }

    Person *findEmployee(const string& id); //find employee across departments

    // constructor (setting up the departments and managers)
    Management()
    {
        departmentsManager = new DepartmentsManager();

        ceo = nullptr;
        coo = nullptr;
        cfo = nullptr;

        manufacturingManager = nullptr;
        logisticsManager = nullptr;
        franchiseManager = nullptr;

        loadGraph("management.json");
        loadPasswords();
        departmentsManager->loadAll();

        if (ceo == nullptr)
        {
            cout << "\nNo CEO found. Please create a CEO profile.\n";

            string id, name, email, password;
            double salary;

            cout << "Enter CEO ID: ";
            getline(cin, id);

            cout << "Enter CEO Name: ";
            getline(cin, name);

            cout << "Enter CEO Email: ";
            getline(cin, email);

            cout << "Enter CEO Salary: ";
            cin >> salary;

            cout << "Enter CEO Password: ";
            cin >> password;

            CEO *newCeo = new CEO(id, name, email, salary);
            newCeo->setPassword(password);

            hireCEO(newCeo);
            save();
            savePasswords();

            cout << "CEO Created and System Initialized.\n";
            cout << "___________________________________\n";
        }
    }

    ~Management()
    {
        if (departmentsManager)
            delete departmentsManager;
    }

    DepartmentsManager *getDepartmentsManager() const { return departmentsManager; }

    TopLevelManager *getCEO() const { return ceo; }

    //LeaveManagement &getLeaveManager() { return leaveManager; }

    void savePasswords();
    void loadPasswords();
    Manager *login() const; // it returns who logged in

    void display();

    // check if ceo
    bool isCEO(Manager *m) const { return m == ceo; }
    bool isCOO(Manager *m) const { return m == coo; }
    bool isCFO(Manager *m) const { return m == cfo; }

    void setCEO(CEO *newCeo) { ceo->setCEO(newCeo); }
    void setCOO(COO *newCoo) { coo->setCOO(newCoo); }
    void setCFO(CFO *newCfo) { cfo->setCFO(newCfo); }

    // get manager based off of department
    MidLevelManager *getManagerForDepartment(const string &dept);

    // check for correct manager
    MidLevelManager *getDepartmentManager(const string &dept);

    int addManager(Manager *manager);
    bool updateManager(Manager *oldManager, Manager *newManager);
    bool updateManagerData(Manager *manager, const Manager &updatedManager);
    void updateMidLevelManagerSalaries(Manager *admin);
    void updateTopLevelSalaries(Manager *admin);
    void connectManagers(Manager *managerOne, Manager *managerTwo);

    void saveGraph(const string &filename);
    void loadGraph(const string &filename);

    void setUpGraph();       // vertices
    void setUpConnections(); // edges

    void hireMidLevelManager(MidLevelManager *newManager);
    void hireCOO(COO *newCoo);
    void hireCFO(CFO *newCfo);
    void hireCEO(CEO *newCeo);
    void updateMidLevelManagerData(Manager *admin);

    bool isCorrectManager(const string &dept) const;
    MidLevelManager *getMidLevelManagerByID(const string &id);
    void managementMenu(Manager* loggedIn);
    void updateCOOData(Manager *admin);
    void updateCFOData(Manager *admin);
    void updateCEOData(Manager *admin);
    void updateManagerMenu(Manager *admin);

    int loadAllManagers(Manager **arr);

    void heapify(Manager **arr, int n, int i);
    void heapSortManagers(Manager **arr, int n);
    void sortAllManagersBySalary();

    void save()
    {
        setUpGraph();
        setUpConnections();
        saveGraph("management.json");
    }

    bool isIDTaken(const string &id) const;
};

///______________________________________________________________________________

/*template <typename T>
class GraphVertex
{
private:
    T data; // manager employee pointer (whoever)
    // two d pointer arrays because managers have two parent on one level
    GraphVertex<T> **parents;
    GraphVertex<T> **children;

    // count of current elements in array
    int parentCount;
    int childCount;

    // to track capacity
    int parentCapacity;
    int childCapacity;

public:
    GraphVertex(T data)
    {
        this->data = data;
        parentCount = 0;
        childCount = 0;
        parentCount = 2;
        childCount = 2;
        parents = new GraphVertex<T> *[parentCapacity];
        children = new GraphVertex<T> *[childCapacity];
    }

    T getData() const { return data; }

    // getters
    GraphVertex<T> **getParents() const { return parents; }
    GraphVertex<T> **getChidlren() const { return children; }

    int getParentCount() const { return parentCount; }
    int getChildrenCount() const { return childCount; }

    int getParentCapacity() const { return parentCapacity; }
    int getChildCapacity() const { return childCapacity; }

    // setters
    void setParentCount(int x) { parentCount = x; }
    void setChildCount(int x) { childCount = x; }

    void setParentCapacity(int c) { parentCapacity = c; }
    void setChildCapacity(int c) { childCapacity = c; }

    void setParents(GraphVertex<T> **p) { parents = p; }
    void setChildren(GraphVertex<T> **c) { children = c; }

    void resizeChildren();
};
*/
//__________________________________________________________________________
/*
template <typename T>
class Graph
{
private:
    GraphVertex<T> **vertex;
    int count;
    int capacity;

public:
    // constructor and destructor
    Graph()
    {
        capacity = 10;
        count = 0;
        vertex = new GraphVertex<T> *[capacity];
    }
    ~Graph()
    {
        for (int i = 0; i < count; i++)
        {
            delete vertex[i];
        }
        delete[] vertex;
    }

    void resize();
    GraphVertex<T> *addVertex(const T &data);
    GraphVertex<T> *addEdge(GraphVertex<T> *parent, GraphVertex<T> *child);
    void printGraph();
    bool updateNode(T oldData, T updateData);
    // void saveToJson();
};
//__________________________________________________________________________

template <typename T>
class StackNode
{
private:
    T data;
    StackNode<T> *next;

public:
    StackNode() { next = nullptr; }
    StackNode(T val)
    {
        data = val;
        next = nullptr;
    }

    ~StackNode() { next = nullptr; }

    void setNext(StackNode<T> *n) { next = n; }
    StackNode<T> *getNext() { return next; }

    void setData(T d) { data = d; }
    T getData() { return data; }
};

template <typename T>
class Stack
{
private:
    Node<T> *head;
    int size;

public:
    // constructors and destructors
    Stack()
    {
        head = nullptr;
        size = 0;
    }

    ~Stack()
    {
        while (!isEmpty())
        {
            Node<T> *temp = head;
            head = head->getNext();
            delete temp;
        }
    }

    // empty validity check
    bool isEmpty() { return head == nullptr; }

    // adding node from the start
    void push(T data)
    {
        Node<T> *temp = new Node<T>(data);
        temp->setNext(head);
        head = temp;
        size++;
    }

    // popping from the start (lifo)
    T pop()
    {
        if (isEmpty())
            return T();

        T topData = head->getData();
        Node<T> *temp = head;
        head = head->getNext();
        delete temp;
        size--;
        return topData;
    }

    // getting hte data of the first node
    T seek()
    {
        if (isEmpty())
            return T();
        return head->getData();
    }

    int getSize() { return size; }
};
*/

#endif
