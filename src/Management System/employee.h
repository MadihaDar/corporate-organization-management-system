#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include "json.hpp"
using namespace std;
using nlohmann::json;

// CLASS 01
// abstract class
//______________________________________________________________________________
class Person
{
private:
    string id;
    string name;
    string role; // hierarchy wise: manager (which type) or employee (which type)
    string email;
    string password;
    double salary;

public:
    // constructors
    Person()
    {
        id = "";
        name = "";
        role = "";
        email = "";
        password = "";
        salary = 0.00;
    }
    Person(string id, string name, string role, string email, double salary) : id(id), name(name), role(role), email(email), salary(salary) {}

    // destructor
    virtual ~Person() {}

    // setters and getters

    void setID(const string &id) { this->id = id; }
    void setName(const string &name) { this->name = name; }
    void setRole(const string &role) { this->role = role; }
    void setEmail(const string &email) { this->email = email; }
    void setSalary(double salary) { this->salary = salary; }
    void setPassword(const string &password) { this->password = password; }

    string getID() const { return id; }
    string getName() const { return name; }
    string getRole() const { return role; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    double getSalary() const { return salary; }

    virtual void display() const = 0;

    void setPerson(Person *p)
    {
        this->id = p->id;
        this->name = p->name;
        this->role = p->role;
        this->email = p->email;
        this->salary = p->salary;
        this->password = p->password;
    }
    json toJson() const;
    void fromJson(const json &j);
};
//______________________________________________________________________________

// class 02
//______________________________________________________________________________

class Employee : public Person
{
private:
    string department;

public:
    // constructors
    Employee() : Person(), department("") {}
    Employee(string id, string name, string email, string department, double salary) : Person(id, name, "Employee", email, salary), department(department) {}

    void setDepartment(const string &department) { this->department = department; }
    string getDepartment() const { return department; }

    void display() const override;

    json toJson() const;
    void fromJson(const json &j);

    void setEmployee(Employee *e)
    {
        setPerson(e);
        e->setDepartment(e->getDepartment());
    }
};

struct Product
{
private:
    string productID;
    int sales;
    float price;

public:
    // constructors
    Product() : productID(""), sales(0), price(0.0) {}
    Product(const string &id, int sales, float price)
        : productID(id), sales(sales), price(price) {}

    void setProductID(string productID) { this->productID = productID; }
    string getProductID() const { return productID; }

    void setProductSales(int sales) { this->sales = sales; }
    int getProductSales() const { return sales; }

    void setProductPrice(float price) { this->price = price; }
    float getProductPrice() const { return price; }
};

#endif