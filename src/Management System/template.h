#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <iostream>
using namespace std;
//_____________________________________________________________________________

// TEMPLATE CLASS 01
//_____________________________________________________________________________
template <typename T>
class Node
{
private:
    T data;
    Node<T> *next;
    Node<T> *prev;

public:
    Node(const T &d) : data(d), next(nullptr), prev(nullptr) {}

    T &getData() { return data; }
    Node<T> *getNext() const { return next; }
    void setNext(Node<T> *n) { next = n; }

    Node<T> *getPrev() const { return prev; }
    void setPrev(Node<T> *prev) { this->prev = prev; }
};
//______________________________________________________________________________

// TEMPLATE CLASS 02
//______________________________________________________________________________
template <typename T>
// fifo
class Queue
{
private:
    Node<T> *head;
    Node<T> *tail;
    int count;

public:
    Queue() : head(nullptr), tail(nullptr), count(0) {}
    // copy constructor
    Queue(const Queue &other)
    {
        this->head = this->tail = nullptr;

        Node<T> *temp = other.head;
        while (temp != nullptr)
        {
            this->enqueue(temp->getData());
            temp = temp->getNext();
        }
    }
    ~Queue()
    {
        while (head != nullptr)
        {
            Node<T> *temp = head;
            head = head->getNext();
            delete temp;
        }
        // setting the whole queue as for it be empty
        tail = nullptr;
        count = 0;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    //______________________________________________________________________________

    void enqueue(const T &data)
    {
        Node<T> *newTail = new Node<T>(data);
        // empty check (only one element in the queue after adding)
        if (isEmpty())
        {
            head = tail = newTail;
        }

        // if not empty
        else
        {
            // insert at end and update tail
            tail->setNext(newTail);
            newTail->setPrev(tail);
            tail = newTail;
        }
        // update count
        count++;
    }
    //______________________________________________________________________________
    int getSize() const { return count; }

    //______________________________________________________________________________

    T dequeue()
    {
        // empty check
        if (isEmpty())
        {
            throw runtime_error("Queue is Empty.\n");
        }
        Node<T> *temp = head;
        T fronVal = temp->getData();

        // only one node? delete it
        if (head == tail)
        {
            head = tail = nullptr;
        }

        else
        {
            head = head->getNext();
            head->setPrev(nullptr);
        }

        // update accordingly
        delete temp;
        count--;
        return fronVal;
    }
    //______________________________________________________________________________
    T &peek()
    {
        if (isEmpty())
        {
            throw runtime_error("Queue is Empty.\n");
        }
        return head->getData();
    }

    //______________________________________________________________________________
    Node<T> *searchByID(const string &id)
    {
        Node<T> *temp = head;

        while (temp != nullptr)
        {
            if (temp->getData().getID() == id)
            {
                return temp;
            }
            temp = temp->getNext();
        }
        return nullptr;
    }
    //______________________________________________________________________________

    bool deleteAtEnd()
    {
        // empty check
        if (isEmpty())
        {
            cout << "Queue is Empty.\n";
            return false;
        }

        // if there's only one node (delete that)
        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
            count--;
            return true;
        }

        // otherwise delete the end node
        Node<T> *temp = tail;
        tail = tail->getPrev();
        tail->setNext(nullptr);

        // update accordingly
        delete temp;
        count--;
        return true;
    }
    //______________________________________________________________________________

    bool deleteByID(const string &id)
    {
        Node<T> *searchNode = searchByID(id);

        if (!searchNode)
        {
            return false;
        }

        // deletion at start
        if (searchNode == head)
        {
            dequeue();
            return true;
        }

        // deletion at end
        if (searchNode == tail)
        {
            deleteAtEnd();
            return true;
        }

        Node<T> *newPrev = searchNode->getPrev();
        Node<T> *newNext = searchNode->getNext();

        newPrev->setNext(newNext);
        newNext->setPrev(newPrev);

        delete searchNode;
        count--;
        return true;
    }

    //______________________________________________________________________________
    T &getByIndex(int index)
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Index out of bounds");
        }

        Node<T> *tempent = head;
        for (int i = 0; i < index; i++)
        {
            tempent = tempent->getNext();
        }

        return tempent->getData();
    }
    //______________________________________________________________________________
    void display() const
    {
        if (isEmpty())
        {
            cout << "No Employees in the Team.\n";
            return;
        }

        Node<T> *temp = head;

        // traverse through the queue and display
        while (temp != nullptr)
        {
            // calling display fromt he employee class
            temp->getData().display();
            cout << endl;
            temp = temp->getNext();
        }
    }
};

//________________________________________________________________________________________

// TEMPLATE CLASS 03
//______________________________________________________________________________________________________
template <typename T>
class TreeNode
{
private:
    T *data;                  // pointer to employee/manager
    TreeNode<T> *nextSibling; // next employee in same list

public:
    TreeNode(T *d)
        : data(d), nextSibling(nullptr) {}

    // getters and setters
    void setData(T *data) { this->data = data; }
    T *getData() { return data; }
    TreeNode<T> *getNextSibling() { return nextSibling; }
    void setNextSibling(TreeNode<T> *n) { nextSibling = n; }
};

//______________________________________________________________________________________________________

// TEMPLATE CLASS 04
//______________________________________________________________________________________________________
template <typename T>
class Tree
{
private:
    TreeNode<T> *root;       // manager pointer wrapped in a node
    TreeNode<T> *firstChild; // first employee (acts as the head of the filo linked list of employee)

public:
    // construtor and destructor
    Tree() : root(nullptr), firstChild(nullptr) {}

    ~Tree()
    {
        clear();
    }
    //______________________________________________________________________________________________________
    // setter and getter
    void setRoot(T *managerPtr)
    {
        root = new TreeNode<T>(managerPtr);
    }

    TreeNode<T> *getRoot() const { return root; }

    //______________________________________________________________________________________________________
    void insertNode(T *employeePtr)
    {
        TreeNode<T> *newNode = new TreeNode<T>(employeePtr);

        // if first child is empty, insert there
        if (!firstChild)
        {
            firstChild = newNode;
            return;
        }

        // check forward the first child and add to the sibling
        TreeNode<T> *temp = firstChild;
        while (temp->getNextSibling() != nullptr)
        {
            temp = temp->getNextSibling();
        }

        temp->setNextSibling(newNode);
    }

    //______________________________________________________________________________________________________
    bool deleteNode(T *val)
    {
        if (!firstChild)
            return false;

        TreeNode<T> *temp = firstChild;
        TreeNode<T> *prev = nullptr;

        while (temp != nullptr)
        {
            if (temp->getData() == val)
            {

                if (!prev)
                    firstChild = temp->getNextSibling(); // first child deletion
                else
                    prev->setNextSibling(temp->getNextSibling()); // delete the sibling

                delete temp; // only delete node (NOT the employee object)
                return true;
            }

            prev = temp;
            temp = temp->getNextSibling();
        }

        return false;
    }
    //______________________________________________________________________________________________________
    TreeNode<T> *findNodeByID(const string &id)
    {
        TreeNode<T> *temp = firstChild;
        while (temp)
        {
            if (temp->getData()->getID() == id)
                return temp;

            temp = temp->getNextSibling();
        }
        return nullptr;
    }

    //______________________________________________________________________________________________________
    void display()
    {
        if (!root)
        {
            cout << "No Manager Assigned.\n";
            return;
        }

        cout << "Manager: " << root->getData()->getName() << "\n";
        cout << "Employees: ";

        TreeNode<T> *temp = firstChild;
        while (temp)
        {
            cout << temp->getData()->getName() << " ";
            temp = temp->getNextSibling();
        }
        cout << endl;
    }

    //______________________________________________________________________________________________________
    TreeNode<T> *getFirstChild() const { return firstChild; }
    //______________________________________________________________________________________________________

    void clear()
    {
        TreeNode<T> *temp = firstChild;
        while (temp)
        {
            TreeNode<T> *next = temp->getNextSibling();
            delete temp; // delete children
            temp = next;
        }

        delete root; // delete root
        root = nullptr;
    }
};

//______________________________________________________________________________________________________

// TEMPLATE CLASS 05
//______________________________________________________________________________________________________

template <typename T>
class Graph
{
    int V; // no of nodes
    T **data;
    int **adjMat; // array of managers
    int capacity;

public:
    // constructor
    Graph(int initial_size = 10)
    {
        V = 0;
        capacity = initial_size;

        data = new T *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            data[i] = nullptr;
        }
        //____________________________________________________

        adjMat = new int *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            adjMat[i] = new int[capacity];
            for (int j = 0; j < capacity; j++)
            {
                adjMat[i][j] = 0;
            }
        }
    }

    // sinkructor
    ~Graph()
    {
        for (int i = 0; i < capacity; i++)
        {
            delete[] adjMat[i];
        }
        delete[] adjMat;
        delete[] data;
    }

    void resize()
    {
        // double capacity of array
        int newCapacity = capacity * 2;

        // resize data array
        T **newData = new T *[newCapacity];

        for (int i = 0; i < newCapacity; i++)
        {
            newData[i] = (i < V ? data[i] : nullptr);
        }
        // copy back to original array
        delete[] data;
        data = newData;

        //___________________________________________

        // resize adjacency matrix
        int **newAdj = new int *[newCapacity];
        for (int i = 0; i < newCapacity; i++)
        {
            // traverse through the two d matrix
            newAdj[i] = new int[newCapacity];
            for (int j = 0; j < newCapacity; j++)
            {
                // copy into new array
                if (i < V && j < V)
                {
                    newAdj[i][j] = adjMat[i][j];
                }
                else
                {
                    newAdj[i][j] = 0;
                }
            }
        }
        // delete old matrix
        for (int i = 0; i < capacity; i++)
        {
            delete[] adjMat[i];
        }
        delete[] adjMat;

        // copy back into the original arrya
        adjMat = newAdj;
        capacity = newCapacity;
    }
    //______________________________________________________________________________________________________

    int addVertex(T *obj)
    {
        if (V == capacity)
        {
            resize();
        }
        // add vertex
        data[V] = obj;
        return V++;
    }

    //______________________________________________________________________________________________________
    void addEdge(int source, int sink)
    {
        // validity check
        if (source < 0 || sink >= V || source < 0 || sink >= V)
        {
            cout << "Invalid Vertices.\n";
            return;
        }
        // addition of edge
        adjMat[source][sink] = 1;
    }

    //______________________________________________________________________________________________________
    void deleteEdge(int source, int sink)
    {
        if (source < 0 || sink < 0 || source >= V || sink >= V)
        {
            return;
        }

        // update to zero
        adjMat[source][sink] = 0;
    }

    //______________________________________________________________________________________________________
    void display() // display function
    {
        cout << "Managerial Hierarchy Gprah:\n";
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                cout << adjMat[i][j] << " ";
            }
            cout << endl;
        }
    }

    //______________________________________________________________________________________________________
    int findVertex(T *obj)
    {
        for (int i = 0; i < V; i++)
        {
            if (data[i] == obj)
            {
                return i;
            }
        }
        return -1;
    }

    //______________________________________________________________________________________________________
    void clear()
    {

        // clear adjacency matrix
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                adjMat[i][j] = 0;
            }
        }

        // reset vertex count
        V = 0;
    }

    //______________________________________________________________________________________________________
    int getVertexCount() const { return V; }
    T *getDataAt(int index) const { return data[index]; }
    T &getDataReferenceAt(int index) const { return data[index]; }
    void setDataAt(int index, T *value) { data[index] = value; }
    bool hasEdge(int source, int sink) const { return (adjMat[source][sink] == 1); }
};
//________________________________________________________________________________________

// TEMPLATE CLASS 06
//________________________________________________________________________________________
// stack node class
template <typename T>
class StackNode
{
public:
    T data;
    StackNode *next;

    StackNode(const T &d) : data(d), next(nullptr) {}
};
//________________________________________________________________________________________

// TEMPLATE CLASS 07
//________________________________________________________________________________________
template <typename T>
class Stack
{
private:
    StackNode<T> *head;
    int size;

public:
    // constructor
    Stack() : head(nullptr), size(0) {}
    // destructor
    ~Stack()
    {
        while (!isEmpty())
        {
            pop();
        }
    }
    //________________________________________________________________________________________
    // check if stack is empty
    bool isEmpty() const
    {
        return head == nullptr;
    }
    //________________________________________________________________________________________
    int getSize() const
    {
        return size;
    }
    //________________________________________________________________________________________

    void push(const T &item)
    {
        StackNode<T> *node = new StackNode<T>(item); // new node
        // insert node
        node->next = head;
        head = node;
        size++;
    }
    //________________________________________________________________________________________

    T pop()
    {
        // empty check
        if (isEmpty())
        {
            return T();
        }
        StackNode<T> *temp = head;
        T val = head->data;
        head = head->next;
        // pop front
        delete temp;
        size--;
        return val;
    }
    //________________________________________________________________________________________
    T getHeadData()
    {
        if (isEmpty())
        {
            return T();
        }
        // head data reterival
        return head->data;
    }
};
//________________________________________________________________________________________
#endif
