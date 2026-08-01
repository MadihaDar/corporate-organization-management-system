#ifndef MANAGERS_H
#define MANAGERS_H

#include "employee.h"
#include "json.hpp"
using nlohmann ::json;

// CLASS 03
// abstract manager class
//______________________________________________________________________________
class Manager : public Person
{
private:
    string managerLevel; // top level or mid level

public:
    // constructors
    Manager() : Person(), managerLevel("") {}
    Manager(string id, string name, string role, string email, double salary, string managerLevel) : Person(id, name, role, email, salary), managerLevel(managerLevel) {}

    void setManagerLevel(const string &managerLevel) { this->managerLevel = managerLevel; }
    string getManagerLevel() const { return managerLevel; }

    // overriding the display function
    virtual void display() const;

    virtual json toJson() const;
    virtual void fromJson(const json &j);

    virtual void updateFrom(const Manager &other);
};
//______________________________________________________________________________

// CLASS 06
//______________________________________________________________________________
class TopLevelManager : public Manager
{
private:
    string designation; // ceo, coo, cfo

public:
    // constructors
    TopLevelManager() : Manager("", "", "Manager", "", 0.0, "Top-Level") {}
    TopLevelManager(string id, string name, string role, string email, double salary, string designation) : Manager(id, name, "Manager", email, salary, "Top-Level"), designation(designation) {}

    string getDesignation() const { return designation; }
    void setDesignation(string designation) { this->designation = designation; }
    // overriding the display function
    void display() const override;

    json toJson() const override;
    void fromJson(const json &j) override;

    void updateFrom(const Manager &other) override;
};

// CLASS 07
//______________________________________________________________________________
class MidLevelManager : public Manager
{
private:
    string department;

public:
    // constructor
    MidLevelManager() : Manager("", "", "Manager", "", 0.0, "Mid-Level"), department("") {}
    MidLevelManager(string id, string name, string role, string email, double salary, string department) : Manager(id, name, "Manager", email, salary, "Mid-Level"), department(department) {}

    string getDepartment() const { return department; }
    void setDepartment(string department) { this->department = department; }

    // overriding the display function
    void display() const override;
    json toJson() const override;
    void fromJson(const json &j) override;

    void updateFrom(const Manager &other) override;
    void setMidLevelManager(MidLevelManager *m)
    {
        setPerson(m);
        m->setDepartment(m->getDepartment());
    }
};

//______________________________________________________________________________

// CLASS 08
//______________________________________________________________________________
class CEO : public TopLevelManager
{
public:
    CEO() : TopLevelManager("", "", "Manager", "", 0.0, "CEO") {}

    CEO(string id, string name, string email, double salary) : TopLevelManager(id, name, "Manager", email, salary, "CEO") {}
    // void manageAllDepartments();
    // void modifyHierarchy();

    void display() const override;
    json toJson() const override;
    void fromJson(const json &j) override;

    void setCEO(CEO *ceo)
    {
        setPerson(ceo);
        ceo->setDesignation("CEO");
    }
};
//______________________________________________________________________________

// CLASS 09
//______________________________________________________________________________
class COO : public TopLevelManager
{
public:
    COO() : TopLevelManager("", "", "Manager", "", 0.0, "COO") {}

    COO(string id, string name, string email, double salary) : TopLevelManager(id, name, "Manager", email, salary, "COO") {}
    void hireEmployee();
    void fireEmployee();
    void display() const override;

    json toJson() const override;
    void fromJson(const json &j) override;

    void setCOO(COO *coo)
    {
        setPerson(coo);
        coo->setDesignation("COO");
    }
};
//______________________________________________________________________________

// CLASS 10
//______________________________________________________________________________
class CFO : public TopLevelManager
{
public:
    CFO() : TopLevelManager("", "", "Manager", "", 0.0, "CFO") {}

    CFO(string id, string name, string email, double salary) : TopLevelManager(id, name, "Manager", email, salary, "CFO") {}
    // void setCompanyAssetCost();
    void updateSalary(Employee &e, double salary) { e.setSalary(salary); }
    void addBonus(Employee &e, double bonus) { e.setSalary(e.getSalary() + bonus); }
    void display() const override;

    json toJson() const override;
    void fromJson(const json &j) override;

    void setCFO(CFO *cfo)
    {
        setPerson(cfo);
        cfo->setDesignation("CFO");
    }
};
//______________________________________________________________________________

//______________________________________________________________________________
#endif