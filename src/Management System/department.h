#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "managers.h"
#include "template.h"

// forward declaration
class Management;

// CLASS 11
//______________________________________________________________________________

// BASE CLASS FOR ALL DEPARTMENTS
//______________________________________________________________________________
class Department
{
protected:
    string departmentName;
    MidLevelManager *manager; // unified manager pointer
    Tree<Person> *teamTree;   // every department has its own tree

    double cost;

public:
    Department(const string &name)
        : departmentName(name),
          manager(nullptr),
          cost(0) {}

    virtual ~Department() {}

    // setter + tree root setter
    void setManager(MidLevelManager *m)
    {
        manager = m;
        teamTree->setRoot(m); // root of tree is the manager
    }

    MidLevelManager *getManager() const { return manager; }

    void setCost(double c) { cost = c; }
    double getCost() const { return cost; }

    string getDepartmentName() const { return departmentName; }

    // functions
    virtual void displayTeam() const
    {
        cout << "Department Name: " << departmentName << "\n";
        teamTree->display();
    }

    void hireEmployee(Employee *e);
    bool fireEmployee(Employee *e);

    // abstracted menu for each department to implement
    virtual void menu(Management *mgmt) = 0;

    json toJson() const;
    void fromJson(const json &j);
    void saveToFile(const string &filename) const;
    void loadFromFile(const string &filename);

    Person *findEmployee(const string &id) const
    {
        if (teamTree)
        {
            TreeNode<Person> *node = teamTree->findNodeByID(id);
            if (node)
                return node->getData();
        }
        return nullptr;
    }

    int countEmployees(Tree<Person> &tree);
    void treeToArray(Tree<Person> &tree, Person **arr);
    void arrayToTree(Tree<Person> &tree, Person **arr, int n);

    // sort funcs
    void selectionSort(Tree<Person> &tree);                 // O(n^2)
    void merge(Person **arr, int left, int mid, int right); // O(nlogn)
    void mergeSortArray(Person **arr, int left, int right);
    void mergeSort(Tree<Person> &tree);
};

//______________________________________________________________________________

class Manufacturing : public Department
{
private:
    Tree<Person> manufacturingTree;

public:
    Manufacturing()
        : Department("Manufacturing")
    {
        teamTree = &manufacturingTree;
    }

    void menu(Management *mgmt) override;
};

//______________________________________________________________________________

class Logistics : public Department
{
private:
    Tree<Person> logisticsTree;

public:
    Logistics() : Department("Logistics")
    {
        teamTree = &logisticsTree;
    }

    void menu(Management *mgmt) override;
};

//______________________________________________________________________________

class Franchise : public Department
{
private:
    Tree<Person> franchiseTree;

public:
    Franchise()
        : Department("Franchise")
    {
        teamTree = &franchiseTree;
    }

    void menu(Management *mgmt) override;
};

//______________________________________________________________________________

class DepartmentsManager
{
private:
    Manufacturing manufacturing;
    Logistics logistics;
    Franchise franchise;

public:
    DepartmentsManager() = default;

    void departmentsMain(Management *mgmt);

    Department *getDepartment(const string &name);

    bool isValidDepartment(const string &dept) const;

    void saveAll();
    void loadAll();

    Person *findEmployee(const string &id);
};

#endif