#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <iostream>
using namespace std;
#include "json.hpp"
using json = nlohmann::json;

// generic doubly linked list node
template <typename T>
class Node {
private:    // private members
    T data;
    Node<T>* next;
    Node<T>* prev;
public:
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {} // constructoe

    // getters
    T& getData() { return data; }
    Node<T>* getNext() { return next; }
    Node<T>* getPrev() { return prev; }

    // setters
    void setNext(Node<T>* n) { next = n; }
    void setPrev(Node<T>* p) { prev = p; }
    void setData(const T& d) { data = d; }
};

// generic queue using Node
template <typename T>
class Queue {
    Node<T>* head;
    Node<T>* tail;
    int count;
public: 
    Queue() : head(nullptr), tail(nullptr), count(0) {} // constructor
    ~Queue() {  // destructor
        while(!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const { return count == 0; } // check if queue is empty
    int size() const { return count; }  

    void enqueue(const T& data) {   // enqueuing data
        Node<T>* newTail = new Node<T>(data);
        if(isEmpty()) { // if queue is empty
            head = tail = newTail;
        } else {    // otherwise
            tail->setNext(newTail);
            newTail->setPrev(tail);
            tail = newTail;
        }
        count++;
    }

    T dequeue() {   // remove from head
        if(isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        Node<T>* temp = head;
        T removedData = temp->getData();

        if(head == tail) {
            head = tail = nullptr;
        } else {
            head = head->getNext();
            head->setPrev(nullptr);
        }

        delete temp;
        count--;
        return removedData;
    }

    T& peek() { // return the frontmost element
        if(isEmpty()) {
            throw runtime_error("Queue is empty.");
        }
        return head->getData();
    }

    // getters
    Node<T>* getHead() const { return head; }
    Node<T>* getTail() const { return tail; }
    int getCount() const { return count; }
};

// generic stack using Node
template <typename T>
class Stack {
    Node<T>* top;
    int count;
public:
    Stack() : top(nullptr), count(0) {} // default constructor
    ~Stack() {  // destructor
        while(!isEmpty()) {
            Node<T>* temp = top;
            top = top->getNext();
            delete temp;
            count--;
        }
    }
    
    bool isEmpty() { return count == 0; }
    int size() { return count; }

    void push(const T& data) {  // adds to top
        Node<T>* newNode = new Node<T>(data);
        newNode->setNext(top);
        top = newNode;
        count++;
    }

    T pop() {   // removes from top
        if(isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        Node<T>* temp = top;
        T removedData = temp->getData();
        top = top->getNext();
        delete temp;
        count--;

        return removedData;
    }

    T& peek() { // returns top element
        if(isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        return top->getData();
    }

    // getters
    Node<T>* getTop() const { return top; }
    int getCount() const { return count; }
};

// represents a vertex in a graph
template <typename T>
class GraphNode {
public:
    T data;
    GraphNode<T>* next;
    Node<T>* edges;

    GraphNode(T d) : data(d), next(nullptr), edges(nullptr) {}  // constructor
};

// represents an adjacency list
template <typename T>
class Graph {
    GraphNode<T>* vertices;
public:
    Graph() : vertices(nullptr) {}  // constructor

    ~Graph() {
        clear();
    }

    GraphNode<T>* findVertex(const T& value) {  // search for a vertex
        GraphNode<T>* curr = vertices;
        while(curr) {
            if(curr->data == value) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void addVertex(const T& value) {    // insert a vertex if it doesnt exist
        if(findVertex(value)) {
            return;
        }

        GraphNode<T>* newNode = new GraphNode<T>(value);
        newNode->next = vertices;
        vertices = newNode;
    }

    void addEdge(const T& a, const T& b) {  // adds undirected edge between two vertices
        if(a == b) {
            return;
        }

        addVertex(a);
        addVertex(b);

        GraphNode<T>* A = findVertex(a);
        GraphNode<T>* B = findVertex(b);

        // checking if edge already exists
        Node<T>* temp = A->edges;
        while(temp) {
            if(temp->getData() == b) {
                return;
            }
            temp = temp->getNext();
        }

        Node<T>* edge1 = new Node<T>(b);
        edge1->setNext(A->edges);
        A->edges = edge1;

        Node<T>* edge2 = new Node<T>(a);
        edge2->setNext(B->edges);
        B->edges = edge2;
    }

    void display() {    // print all vertices 
        GraphNode<T>* curr = vertices;

        if(!curr) {
            cout << "Graph is empty.\n";
            return;
        }

        while(curr) {
            cout << curr->data << " -> ";
            Node<T>* edge = curr->edges;

            while(edge) {
                cout << edge->getData() << " ";
                edge = edge->getNext();
            }
            cout << endl;
            curr = curr->next;
        }
    }

    json toJSON() const {
        json graphArr = json::array();  // initialise empty json arr
        GraphNode<T>* curr = vertices;
        while(curr) {
            json edges = json::array(); // json arr for edges of current vertex
            Node<T>* edge = curr->edges;
            while(edge) {
                edges.push_back(edge->getData());   // add each connected vertex (edge) to the arr
                edge = edge->getNext();
            }
            graphArr.push_back( {
                {"vertex", curr->data},
                {"edges", edges}
            });
            curr = curr->next;
        }
        return graphArr;
    }

    // helper function to free nodes and edges
    void clear() {
        GraphNode<T>* curr = vertices;  // start at the head of the vertex list
        while(curr) {
            Node<T>* e = curr->edges;   // delete all edges first
            while(e) {  
                Node<T>* tempE = e;
                e = e->getNext();
                delete tempE;
            }
            GraphNode<T>* tempV = curr;
            curr = curr->next;
            delete tempV;   // delete the vertex itself
        }
        vertices = nullptr;
    }

    void fromJSON(const json& j) {
        clear();    // clear any exisitng graph data

        // add all vertices first
        for (const auto& item : j) {
            T vertexVal = item.at("vertex").get<T>();
            addVertex(vertexVal);
        }

        // add edges using 
        for (const auto& item : j) {
            T vertexVal = item.at("vertex").get<T>();
            const json& edges = item.at("edges");

            for (const auto& edge : edges) {
                GraphNode<T>* A = findVertex(vertexVal);
                bool exists = false;
                Node<T>* e = A->edges;

                // check if edge already exists to avoid duplicates
                while(e) {
                    if(e->getData() == edge.get<T>()) { exists = true; break; }
                    e = e->getNext();
                }   

                if(!exists) {
                    addEdge(vertexVal, edge.get<T>());  // add edge if not already present
                }
            }
        }
    }
};

#endif