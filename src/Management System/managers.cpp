#include "managers.h"

//______________________________________________________________________________
void Manager ::display() const
{
    cout << "________________________________\n"
         << "Name: " << getName() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Role: " << getRole() << "\n"
         << "Manager Level: " << getManagerLevel() << "\n";
}
//______________________________________________________________________________
json Manager::toJson() const
{
    return {
        {"id", getID()},
        {"name", getName()},
        {"role", getRole()},
        {"email", getEmail()},
        {"salary", getSalary()},
        {"managerLevel", getManagerLevel()}};
}

//______________________________________________________________________________
void Manager::fromJson(const json &j)
{

    setID(j["id"]);
    setName(j["name"]);
    setRole(j["role"]);
    setEmail(j["email"]);
    setSalary(j["salary"]);
    setManagerLevel(j["managerLevel"]);
}

//______________________________________________________________________________
void Manager ::updateFrom(const Manager &other)
{
    setName(other.getName());
    setEmail(other.getEmail());
    setSalary(other.getSalary());
    setRole(other.getRole());
    setManagerLevel(other.getManagerLevel());
}

//______________________________________________________________________________
void MidLevelManager::display() const
{
    cout << "________________________________\n";
    cout << "\n______Mid Level Manager Info______\n"
         << "Name: " << getName() << "\n"
         << "Role: " << getRole() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Salary: " << getSalary() << "\n"
         << "Department: " << getDepartment() << "\n"; // backward, forward, stability, renewal
    cout << "________________________________\n";
}
//______________________________________________________________________________

void MidLevelManager ::fromJson(const json &j)
{

    Manager::fromJson(j);
    setDepartment(j["department"]);
}

//______________________________________________________________________________

json MidLevelManager ::toJson() const
{
    // basic attributes of manager
    json j = Manager::toJson();
    // add mid level department
    j["department"] = getDepartment();
    return j;
}

//______________________________________________________________________________
void MidLevelManager ::updateFrom(const Manager &other)
{
    // update attributes function (generic)
    Manager ::updateFrom(other);
    // check if the manager passed as parameter is mid level and set department
    if (auto mid = dynamic_cast<const MidLevelManager *>(&other))
    {
        setDepartment(mid->getDepartment());
    }
}

//______________________________________________________________________________

void TopLevelManager::display() const
{
    cout << "________________________________\n";
    cout << "\n______Top Level Manager______\n"
         << "Role: " << getRole() << "\n";
    cout << "________________________________\n";
}
//______________________________________________________________________________

json TopLevelManager::toJson() const
{
    // basic attributes of manager
    json j = Manager::toJson();
    // add top level designation
    j["designation"] = getDesignation();
    return j;
}

//______________________________________________________________________________
void TopLevelManager ::updateFrom(const Manager &other) {
    // update attributes function (generic)
    Manager::updateFrom(other);
    // check if the manager passed as parameter is top level and set department
    if(auto top = dynamic_cast<const TopLevelManager*>(&other)) {
        setDesignation(top->getDesignation());
    }
}

    //______________________________________________________________________________
    void TopLevelManager::fromJson(const json &j)
{
    Manager::fromJson(j);
    setDesignation(j["designation"]);
}

//______________________________________________________________________________

void CEO::display() const
{
    cout << "________________________________\n";
    cout << "\n____________CEO____________\n"
         << "Name: " << getName() << "\n"
         << "Role: " << getRole() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Salary: " << getSalary() << "\n"
         << "Designation: " << getDesignation() << "\n";
    cout << "________________________________\n";
}
//______________________________________________________________________________
json CEO::toJson() const
{
    json j = TopLevelManager::toJson();
    j["designation"] = "CEO";
    return j;
}

//______________________________________________________________________________
void CEO::fromJson(const json &j)
{
    TopLevelManager::fromJson(j);
}

//______________________________________________________________________________
void COO::display() const
{
    cout << "________________________________\n";
    cout << "\n____________COO____________\n"
         << "Name: " << getName() << "\n"
         << "Role: " << getRole() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Salary: " << getSalary() << "\n"
         << "Designation: " << getDesignation() << "\n";
    cout << "________________________________\n";
}
//______________________________________________________________________________
json COO::toJson() const
{
    json j = TopLevelManager::toJson();
    j["designation"] = "COO";
    return j;
}

//______________________________________________________________________________

void COO::fromJson(const json &j)
{
    TopLevelManager ::fromJson(j);
}

//______________________________________________________________________________
void CFO::display() const
{
    cout << "________________________________\n";
    cout << "\n____________CFO____________\n"
         << "Name: " << getName() << "\n"
         << "Role: " << getRole() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Salary: " << getSalary() << "\n"
         << "Designation: " << getDesignation() << "\n";
    cout << "________________________________\n";
}
//______________________________________________________________________________
json CFO::toJson() const
{
    json j = TopLevelManager::toJson();
    j["designation"] = "CFO";
    return j;
}

//______________________________________________________________________________
void CFO::fromJson(const json &j)
{
    TopLevelManager::fromJson(j);
}

//______________________________________________________________________________