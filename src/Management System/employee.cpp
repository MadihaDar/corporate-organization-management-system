#include "employee.h"
//______________________________________________________________________________
void Employee ::display() const
{
    cout << "________________________________\n"
         << "Name: " << getName() << "\n"
         << "Role: " << getRole() << "\n"
         << "Email: " << getEmail() << "\n"
         << "Strategic Department: " << getDepartment() << "\n";
}
//______________________________________________________________________________


json Person::toJson() const
{
    return {
        {"id", getID()},
        {"name", getName()},
        {"role", getRole()},
        {"email", getEmail()},
        {"salary", getSalary()}};
}


void Person::fromJson(const json &j)
{
    setID(j["id"]);
    setName(j["name"]);
    setRole(j["role"]);
    setEmail(j["email"]);
    setSalary(j["salary"]);
}

json Employee::toJson() const
{
    return {
        {"id", getID()},
        {"name", getName()},
        {"role", getRole()},
        {"email", getEmail()},
        {"salary", getSalary()}};
}

void Employee::fromJson(const json &j)
{
    setID(j["id"]);
    setName(j["name"]);
    setRole(j["role"]);
    setEmail(j["email"]);
    setSalary(j["salary"]);
}

