#ifndef LEAVE_H
#define LEAVE_H

#include "management.h"
#include "../Colors.h"

extern void clearInput();

using namespace std;

class LeaveRequest
{
private:
    string requesterID;
    string requesterRole;
    string reason;
    int days;
    int priority; // 1 = high, 2 = medium, 3 = low

public:
    // constructors
    LeaveRequest() : days(0), priority(3) {}

    LeaveRequest(string id, string role, string reason, int days, int priority)
        : requesterID(id), requesterRole(role), reason(reason), days(days), priority(priority) {}

    // setters and getters
    void setRequesterID(const string &id) { requesterID = id; }
    void setRequesterRole(const string &role) { requesterRole = role; }
    void setDays(int d) { days = d; }
    void setPriority(int p) { priority = p; }
    void setReason(const string &r) { reason = r; }

    string getRequestedID() const { return requesterID; }
    string getRequesterRole() const { return requesterRole; }
    string getReason() const { return reason; }
    int getDays() const { return days; }
    int getPriority() const { return priority; }

    //___________________________________________________________________________________________________
    void display() const
    {
        cout << "ID: " << requesterID
             << " | Role: " << requesterRole
             << " | Days: " << days
             << " | Priority: " << priority
             << " | Reason: " << reason << endl;
    }
    //___________________________________________________________________________________________________

    json toJson() const
    {
        return {
            {"id", requesterID},
            {"role", requesterRole},
            {"days", days},
            {"priority", priority},
            {"reason", reason}};
    }
    //___________________________________________________________________________________________________

    void fromJson(const json &j)
    {
        setRequesterID(j["id"]);
        setRequesterRole(j["role"]);
        setDays(j["days"]);
        setPriority(j["priority"]);
        setReason(j["reason"]);
    }
    //___________________________________________________________________________________________________

    void input(Person *p) // input requests
    {
        requesterID = p->getID();
        
        // role determine via dynamic cast
        Manager *m = dynamic_cast<Manager*>(p);
        if (m)
        {
            if (m->getManagerLevel() == "Top-Level")
            {
                TopLevelManager *t = dynamic_cast<TopLevelManager*>(m);
                if (t) requesterRole = t->getDesignation(); // CEO, COO, CFO
                else requesterRole = "Top-Level Manager";
            }
            else
            {
                requesterRole = "Mid-Level Manager";
            }
        }
        else
        {
            requesterRole = p->getRole();
        }

        cout << "Enter days: ";
        cin >> days;

        cout << "\n1. For Emergency Leave.\n";
        cout << "2. For Medical Leave.\n";
        cout << "3. For Casual Leave.\n";
        cout << "Priority (1-high, 2-medium, 3-low): ";
        cin >> priority;
        // priority validity check
        if (getPriority() < 1 && getPriority() > 3)
        {
            cout << "Invalid Priority.\n";
            return;
        }

        cout << "Reason: ";
        cin.ignore();
        getline(cin, reason);
    }
};

//___________________________________________________________________________________________________
class LeaveManagement
{
private:
    Queue<LeaveRequest> highPriority;
    Queue<LeaveRequest> mediumPriority;
    Queue<LeaveRequest> lowPriority;

    // aproval permission
    bool canApprove(Person *admin, const LeaveRequest &req) const
    {
        string adminRole = admin->getRole();
        string adminLevel;
        string adminDesignation;

        Manager *m = dynamic_cast<Manager*>(admin);
        if (m) //if it is a manager
        {
            adminLevel = m->getManagerLevel(); //get the managers level
            TopLevelManager *t = dynamic_cast<TopLevelManager*>(m); //if they are a top level manager
            if (t) adminDesignation = t->getDesignation(); //then get their designation like CEO/COO
        }

        // no self approval!!
        if (admin->getID() == req.getRequestedID())
        {
             cout << "Cannot approve your own leave request.\n";
             return false;
        }

        const string reqRole = req.getRequesterRole();

        bool isEmployee = (reqRole != "Mid-Level Manager" && reqRole != "CEO" && reqRole != "COO" && reqRole != "CFO" && reqRole != "Top-Level Manager");
        if (isEmployee)
        {
             if (adminDesignation == "CEO" || adminDesignation == "COO") return true;
             if (adminLevel == "Mid-Level") return true; //dept manager based approval
        }


        if (reqRole == "Mid-Level Manager")
        {
            if (adminDesignation == "CEO" || adminDesignation == "COO") return true;
        }


        if (reqRole == "COO" || reqRole == "CFO")
        {
            if (adminDesignation == "CEO") return true;
        }

        return false;
    }

public:
    // enqueue requests based off of priority
    void applyLeave(const LeaveRequest &req)
    {
        if (req.getPriority() == 1)
        {
            highPriority.enqueue(req);
        }
        else if (req.getPriority() == 2)
        {
            mediumPriority.enqueue(req);
        }
        else
        {
            lowPriority.enqueue(req);
        }
    }

    // getting leaves from the top of the queues
    bool processLeave(Manager *admin)
    {
        LeaveRequest req;

        if (!highPriority.isEmpty())
        {
            req = highPriority.peek();
        }
        else if (!mediumPriority.isEmpty())
        {
            req = mediumPriority.peek();
        }
        else if (!lowPriority.isEmpty())
        {
            req = lowPriority.peek();
        }
        else
        {
            cout << "No pending leaves.\n";
            return false;
        }

        // approval check
        if (!canApprove(admin, req))
        {
            cout << "Access denied.\n";
            return false;
        }

        // dequeue requests to show approved
        if (!highPriority.isEmpty())
        {
            highPriority.dequeue();
        }
        else if (!mediumPriority.isEmpty())
        {
            mediumPriority.dequeue();
        }
        else
        {
            lowPriority.dequeue();
        }

        cout << "Leave approved:\n";
        req.display();
        return true;
    }

    bool rejectLeave(Manager *admin)
    {
        LeaveRequest req;

        // dequeue requests to show rejected
        if (!highPriority.isEmpty())
        {
            req = highPriority.dequeue();
        }
        else if (!mediumPriority.isEmpty())
        {
            req = mediumPriority.dequeue();
        }
        else if (!lowPriority.isEmpty())
        {
            req = lowPriority.dequeue();
        }
        else
        {
            cout << "No pending leaves.\n";
            return false;
        }

        if (!canApprove(admin, req))
        {
            cout << "Access denied.\n";
            return false;
        }

        cout << "Leave rejected:\n";
        req.display();
        return true;
    }

    void displayAll() const
    {
        cout << "\n___High Priority___\n";
        highPriority.display();

        cout << "\n___Medium Priority___\n";
        mediumPriority.display();

        cout << "\n___Low Priority___\n";
        lowPriority.display();
    }

    void saveToFile(const string &filename)
    {
        json j;
        j["leaves"] = json::array();

        while (!highPriority.isEmpty())
            j["leaves"].push_back(highPriority.dequeue().toJson());
        while (!mediumPriority.isEmpty())
            j["leaves"].push_back(mediumPriority.dequeue().toJson());
        while (!lowPriority.isEmpty())
            j["leaves"].push_back(lowPriority.dequeue().toJson());

        ofstream out(filename);
        out << j.dump(4);
        out.close();
    }

    void
    loadFromFile(const string &filename)
    {
        ifstream in(filename);
        if (!in.is_open())
            return;

        json j;
        in >> j;
        in.close();

        highPriority = Queue<LeaveRequest>();
        mediumPriority = Queue<LeaveRequest>();
        lowPriority = Queue<LeaveRequest>();

        for (auto &item : j["leaves"])
        {
            LeaveRequest r;
            r.fromJson(item);
            applyLeave(r);
        }
    }
    void leaveMenu(Manager *loggedIn)
    {
        if (!loggedIn)
        {
            cout << RED << "Access Denied. Please Login First." << RESET << "\n";
            cout << "Press Enter to continue...";
             cin.ignore();
             cin.get();
            return;
        }

        int choice = -1;

        while (choice != 0)
        {
            cout << CLRSCR;
            cout << BOLD << CYAN << "______Leave Management (Manager View)______" << RESET << "\n";
            cout << GREEN << "1." << RESET << " Apply for Leave\n";
            cout << GREEN << "2." << RESET << " Approve Leave\n";
            cout << GREEN << "3." << RESET << " Reject Leave\n";
            cout << GREEN << "4." << RESET << " View Pending Leaves\n";
            cout << RED << "0. Back" << RESET << "\n";
            cout << "Choice: ";
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

            this->loadFromFile("leaves.json");

            switch (choice)
            {
            case 1:
            {
        
                LeaveRequest r;
                r.input(loggedIn);
                this->applyLeave(r);
                this->saveToFile("leaves.json");
                cout << GREEN << "Leave Request Submitted." << RESET << "\n";
                break;
            }

            case 2:
            {
              
                if (this->processLeave(loggedIn))
                    this->saveToFile("leaves.json");
                break;
            }

            case 3:
            {
                if (this->rejectLeave(loggedIn))
                    this->saveToFile("leaves.json");
                break;
            }

            case 4:
                cout << CLRSCR;
                this->displayAll();
                break;

            default:
                cout << RED << "Invalid choice." << RESET << "\n";
                break;
            }
             cout << "Press Enter to continue...";
             cin.ignore();
             cin.get();
        }
    }

    void applyForLeaveMenu(Person *p)
    {
        if (!p) return;
        
        cout << "\nApplying for leave as: " << p->getName() << " (" << p->getRole() << ")\n";
        LeaveRequest req;
        req.input(p);
        this->loadFromFile("leaves.json");
        this->applyLeave(req);
        this->saveToFile("leaves.json");
        cout << "Leave Request Submitted Successfully.\n";
    }
};

#endif
