#include "FeedbackSystem.h"
#include <fstream>


#include <utility>

//
// Feedback Implementation
//
int Feedback::nextId = 1;

Feedback::Feedback(string name, string content, const int rating, string date, string cat)
    : id(nextId++), customerName(move(name)), content(move(content)), rating(rating), date(move(date)),
      category(move(cat))
{
}

bool Feedback::operator==(const Feedback& other) const { return id == other.id; }
bool Feedback::operator<(const Feedback& other) const { return rating < other.rating; }
bool Feedback::operator>(const Feedback& other) const { return rating > other.rating; }

ostream& operator<<(ostream& os, const Feedback& fb)
{
    os << "[ID:" << fb.id << " | " << fb.rating << "*] " << fb.customerName << ": " << fb.content << " (" << fb.category
        << ")";
    return os;
}

void to_json(json& j, const Feedback& f)
{
    j = json{
        {"id", f.id},
        {"customerName", f.customerName},
        {"content", f.content},
        {"rating", f.rating},
        {"date", f.date},
        {"category", f.category}
    };
}

void from_json(const json& j, Feedback& f)
{
    j.at("id").get_to(f.id);
    j.at("customerName").get_to(f.customerName);
    j.at("content").get_to(f.content);
    j.at("rating").get_to(f.rating);
    j.at("date").get_to(f.date);
    j.at("category").get_to(f.category);
}

//
// FeedbackCategory Implementation
//
FeedbackCategory::FeedbackCategory(string n) : name(move(n))
{
}

bool FeedbackCategory::operator<(const FeedbackCategory& other) const { return name < other.name; }
bool FeedbackCategory::operator>(const FeedbackCategory& other) const { return name > other.name; }
bool FeedbackCategory::operator==(const FeedbackCategory& other) const { return name == other.name; }

ostream& operator<<(ostream& os, const FeedbackCategory& cat)
{
    os << "Category: " << cat.name;
    return os;
}

//
// Ticket Implementation
//
int Ticket::nextTicketId = 500;

Ticket::Ticket(string issue, const int priority)
    : ticketId(nextTicketId++), issue(move(issue)), priority(priority), status("Open")
{
}

bool Ticket::operator==(const Ticket& other) const { return ticketId == other.ticketId; }

ostream& operator<<(ostream& os, const Ticket& t)
{
    os << "[Ticket #" << t.ticketId << "] Priority " << t.priority << ": " << t.issue << " [" << t.status << "]";
    return os;
}

void to_json(json& j, const Ticket& t)
{
    j = json{
        {"ticketId", t.ticketId},
        {"issue", t.issue},
        {"priority", t.priority},
        {"status", t.status}
    };
}

void from_json(const json& j, Ticket& t)
{
    j.at("ticketId").get_to(t.ticketId);
    j.at("issue").get_to(t.issue);
    j.at("priority").get_to(t.priority);
    j.at("status").get_to(t.status);
}

//
// FeedbackSystem Implementation
//
FeedbackSystem::FeedbackSystem()
{
    //default categories
    categoryTree.insert(FeedbackCategory("Product Quality"));
    categoryTree.insert(FeedbackCategory("Store Service"));
    categoryTree.insert(FeedbackCategory("Delivery"));
    categoryTree.insert(FeedbackCategory("Website"));

    loadData();
}

void FeedbackSystem::submitFeedback(const Feedback& fb)
{
    const FeedbackCategory searchKey(fb.category);
    TreeNode<FeedbackCategory>* node = categoryTree.search(searchKey);

    if (node)
    {
        node->data.feedbackList.insert(fb);
        cout << "Feedback submitted successfully (ID: " << fb.id << ")!" << endl;
    }
    else
    {
        FeedbackCategory newCat(fb.category);
        newCat.feedbackList.insert(fb);
        categoryTree.insert(newCat);
        cout << "New Category created. Feedback submitted (ID: " << fb.id << ")." << endl;
    }
    saveData();
}

void FeedbackSystem::collectAllFeedback(const TreeNode<FeedbackCategory>* node, DynamicArray<Feedback>& arr) //push to
//array for usage
{
    if (!node) return;
    collectAllFeedback(node->left, arr);

    const Node<Feedback>* current = node->data.feedbackList.head;
    while (current)
    {
        arr.push_back(current->data);
        current = current->next;
    }

    collectAllFeedback(node->right, arr);
}

void FeedbackSystem::displayAllFeedback() const
{
    cout << "\n--- All Feedback (Categorized) ---" << endl;
    struct Printer
    {
        static void print(const TreeNode<FeedbackCategory>* node)
        {
            if (!node) return;
            print(node->left);
            cout << "\nCategory: " << node->data.name << endl;
            node->data.feedbackList.display();
            print(node->right);
        }
    };
    Printer::print(categoryTree.root);
}

void FeedbackSystem::displayTopRatedFeedback() //sort the ratings via mergeSort
{
    DynamicArray<Feedback> allFb;
    collectAllFeedback(categoryTree.root, allFb);

    if (allFb.isEmpty())
    {
        cout << "No feedback found." << endl;
        return;
    }

    Sorter<Feedback>::mergeSort(allFb, 0, allFb.size() - 1);

    cout << "\n--- Top Rated Feedback (Merge Sort) ---" << endl;
    for (int i = 0; i < allFb.size(); i++)
    {
        cout << allFb[i] << endl;
    }
}

void FeedbackSystem::displayLowRatedFeedback()
{
    DynamicArray<Feedback> allFb;
    collectAllFeedback(categoryTree.root, allFb);

    if (allFb.isEmpty())
    {
        cout << "No feedback found." << endl;
        return;
    }

    Sorter<Feedback>::bubbleSort(allFb);

    cout << "\n--- Critical Feedback (Low Ratings) ---" << endl;
    for (int i = allFb.size() - 1; i >= 0; i--)
    {
        cout << allFb[i] << endl;
    }
}

void FeedbackSystem::displayRatingDistribution() // COUNT SORT HERE <<
{
    int counts[6] = {};

    DynamicArray<Feedback> allFb;
    collectAllFeedback(categoryTree.root, allFb);

    for (int i = 0; i < allFb.size(); i++)
    {
        const int r = allFb[i].rating;
        if (r >= 1 && r <= 5)
        {
            counts[r]++;
        }
    }

    cout << "\n--- Ratings (Count Sort) ---" << endl;
    for (int i = 5; i >= 1; i--)
    {
        cout << i << " Stars: ";
        for (int j = 0; j < counts[i]; j++) cout << "*";
        cout << " (" << counts[i] << ")" << endl;
    }
}

void FeedbackSystem::raiseTicket(const Ticket& t)
{
    ticketQueue.enqueue(t, t.priority);
    cout << "Ticket raised: " << t.issue << " (Priority: " << t.priority << ", ID: " << t.ticketId << ")" << endl;
    saveData();
}

void FeedbackSystem::resolveNextTicket() //queue system: mark next ticket as resolved
{
    if (ticketQueue.isEmpty())
    {
        cout << "No tickets to resolve." << endl;
        return;
    }

    Ticket t = ticketQueue.dequeue();
    t.status = "Resolved";
    ticketHistory.push(t);
    cout << "Resolved Ticket: " << t.issue << endl;
    saveData();
}

void FeedbackSystem::undoLastTicketAction()
{
    if (ticketHistory.isEmpty())
    {
        cout << "No ticket actions to undo." << endl;
        return;
    }

    Ticket t = ticketHistory.pop();
    t.status = "Open";
    ticketQueue.enqueue(t, t.priority); //shift from stack to queue
    cout << "Undid resolution for Ticket #" << t.ticketId << ". It is now Open again." << endl;
    saveData();
}

void FeedbackSystem::displaySystemInfo() //list of concepts i implemented
{
    cout
        << "\n=============================================" << endl
        << "   Feedback System Architecture" << endl
        << "=============================================" << endl
        << "Data Structures:" << endl
        << "1. Tree: Stores Feedback Categories." << endl
        << "2. LinkedList: Stores Feedback within each Category." << endl
        << "3. PriorityQueue: Manages Support Tickets (High Priority first)." << endl
        << "4. Stack: Manages Undo History for Ticket Resolutions." << endl
        << "5. DynamicArray: Used for collecting data for sorting." << endl
        << "\nAlgorithms:" << endl
        << "1. Merge Sort (n*logn): Used for 'Top Rated' reports." << endl
        << "2. Bubble Sort (n^2): Used for 'Critical/Low Rated' reports." << endl
        << "3. Counting Logic (n): Used for Rating Distribution histograms." << endl
        << "=============================================" << endl;
}

void FeedbackSystem::saveData()
{
    json j;

    // Save Feedback
    DynamicArray<Feedback> allFb;
    collectAllFeedback(categoryTree.root, allFb);
    j["feedbacks"] = json::array();
    for (int i = 0; i < allFb.size(); i++)
    {
        j["feedbacks"].push_back(allFb[i]);
    }

    //(copy over)
    DynamicArray<Ticket> tempTickets;
    while (!ticketQueue.isEmpty())
    {
        tempTickets.push_back(ticketQueue.dequeue());
    }


    j["tickets"] = json::array();
    for (int i = 0; i < tempTickets.size(); i++)
    {
        j["tickets"].push_back(tempTickets[i]);
        ticketQueue.enqueue(tempTickets[i], tempTickets[i].priority); //restoration
    }

    Stack<Ticket> tempStack;
    j["resolved_tickets"] = json::array();

    while (!ticketHistory.isEmpty())
    {
        Ticket t = ticketHistory.pop();
        tempStack.push(t);
    }

    while (!tempStack.isEmpty())
    {
        Ticket t = tempStack.pop();
        j["resolved_tickets"].push_back(t);
        ticketHistory.push(t);
    }

    j["next_ticket_id"] = Ticket::nextTicketId;
    j["next_feedback_id"] = Feedback::nextId;

    ofstream file("feedback_system.json");
    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

void FeedbackSystem::loadData()
{
    ifstream file("feedback_system.json");
    if (!file.is_open()) return;

    json j;
    file >> j;

    if (j.contains("next_ticket_id")) Ticket::nextTicketId = j["next_ticket_id"]; //track next unique id of ticket
    if (j.contains("next_feedback_id")) Feedback::nextId = j["next_feedback_id"]; // ''

    if (j.contains("feedbacks"))
    {
        for (const auto& item : j["feedbacks"])
        {
            Feedback f = item;
            const FeedbackCategory searchKey(f.category);
            TreeNode<FeedbackCategory>* node = categoryTree.search(searchKey);

            if (node)
            {
                node->data.feedbackList.insert(f);
            }
            else
            {
                FeedbackCategory newCategory(f.category);
                newCategory.feedbackList.insert(f);
                categoryTree.insert(newCategory);
            }
        }
    }

    if (j.contains("tickets"))
    {
        for (const auto& item : j["tickets"])
        {
            Ticket t = item;
            ticketQueue.enqueue(t, t.priority);
        }
    }

    if (j.contains("resolved_tickets")) //for "undoing"
    {
        json& arr = j["resolved_tickets"];
        for (auto it = arr.rbegin(); it != arr.rend(); ++it)
        {
            Ticket t;
            from_json(*it, t);
            ticketHistory.push(t);
        }
    }
}
