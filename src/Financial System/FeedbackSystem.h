#ifndef FEEDBACKSYSTEM_H
#define FEEDBACKSYSTEM_H

#include "templatedatastructures.h"
#include <string>
#include <iostream>
#include <utility>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

//
// Feedback Class
//
class Feedback
{
public:
    static int nextId;
    int id;
    string customerName;
    string content;
    int rating; //1-5 stars
    string date;
    string category;

    explicit Feedback(string name = "", string content = "", int rating = 0, string date = "", string cat = "");

    bool operator==(const Feedback& other) const;
    bool operator<(const Feedback& other) const; //sorting stuff
    bool operator>(const Feedback& other) const;
    friend ostream& operator<<(ostream& os, const Feedback& fb);

    friend void to_json(json& j, const Feedback& f);
    friend void from_json(const json& j, Feedback& f);
};

//
// FeedbackCategory Class
//
class FeedbackCategory
{
public:
    string name;
    LinkedList<Feedback> feedbackList;

    explicit FeedbackCategory(string n = "");

    bool operator<(const FeedbackCategory& other) const;
    bool operator>(const FeedbackCategory& other) const;
    bool operator==(const FeedbackCategory& other) const;
    friend ostream& operator<<(ostream& os, const FeedbackCategory& cat);
};

//
// Ticket Class
//
class Ticket
{
    //fix constructor bs
public:
    static int nextTicketId;
    int ticketId;
    string issue;
    int priority;
    string status;

    Ticket()
    {
    }

    Ticket(string issue, const int priority);

    bool operator==(const Ticket& other) const;
    friend ostream& operator<<(ostream& os, const Ticket& t);

    friend void to_json(json& j, const Ticket& t);
    friend void from_json(const json& j, Ticket& t);
};

//
// FeedbackSystem Class
//
class FeedbackSystem
{
private:
    Tree<FeedbackCategory> categoryTree;
    PriorityQueue<Ticket> ticketQueue;
    Stack<Ticket> ticketHistory;

    void collectAllFeedback(const TreeNode<FeedbackCategory>* node, DynamicArray<Feedback>& arr);

public:
    FeedbackSystem();

    void submitFeedback(const Feedback& fb);

    void displayAllFeedback() const;
    void displayTopRatedFeedback();
    void displayLowRatedFeedback();
    void displayRatingDistribution();

    void raiseTicket(const Ticket& t);
    void resolveNextTicket();
    void undoLastTicketAction();

    static void displaySystemInfo();

    void saveData();
    void loadData();
};

#endif
