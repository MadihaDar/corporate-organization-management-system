#ifndef TEMPLATEDATASTRUCTURES_H
#define TEMPLATEDATASTRUCTURES_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

//
// DynamicArray Class
//
template <typename T>
class DynamicArray
{
private:
    T* data;
    int capacity;
    int count;

    void resize()
    {
        //double size if cap is reached
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i < count; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    explicit DynamicArray(const int initCap = 10) : capacity(initCap), count(0)
    {
        data = new T[capacity];
    }

    ~DynamicArray()
    {
        delete[] data;
        capacity = 0;
        count = 0;
    }

    void push_back(T val)
    {
        if (count == capacity)
        {
            resize();
        }
        data[count++] = val;
    }

    T& operator[](int index)
    {
        if (index < 0 || index >= count) return data[0];
        return data[index];
    }

    const T& operator[](int index) const
    {
        if (index < 0 || index >= count) return data[0];
        return data[index];
    }

    [[nodiscard]] int size() const { return count; }

    [[nodiscard]] bool isEmpty() const { return count == 0; }
};

//
// Node Class
//
template <typename T>
class Node
{
public:
    T data;
    Node* next;
    Node* prev;
    int priority;

    explicit Node(T val, const int p = 0) : data(val), next(nullptr), prev(nullptr), priority(p)
    {
    }
};

//
// LinkedList Class
//
template <typename T>
class LinkedList
{
public:
    Node<T>* head;
    Node<T>* tail;
    int size;

    LinkedList() : head(nullptr), tail(nullptr), size(0)
    {
    }

    ~LinkedList() { clear(); }

    void clear()
    {
        //"destructor"
        Node<T>* current = head;
        while (current)
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    void insert(T data)
    {
        auto* newNode = new Node<T>(data);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    bool remove(T data)
    {
        Node<T>* current = head;
        while (current)
        {
            if (current->data == data)
            {
                if (current == head) head = head->next; //delete head
                if (current == tail) tail = tail->prev; //delete tail
                if (current->next) current->next->prev = current->prev;
                if (current->prev) current->prev->next = current->next;
                delete current;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void display() const
    {
        Node<T>* current = head;
        while (current)
        {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }

    [[nodiscard]] bool isEmpty() const { return size == 0; }
};

//
// Stack Class
//
template <typename T>
class Stack
{
private:
    Node<T>* top;

public:
    Stack() : top(nullptr)
    {
    }

    ~Stack()
    {
        while (!isEmpty())
        {
            Node<T>* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(T data)
    {
        auto* newNode = new Node<T>(data);
        newNode->next = top;
        top = newNode;
    }

    T pop()
    {
        if (isEmpty()) throw runtime_error("Stack Underflow");
        Node<T>* temp = top;
        T data = temp->data;
        top = top->next;
        delete temp;
        return data;
    }

    T peek() const
    {
        if (isEmpty()) throw runtime_error("Stack is Empty");
        return top->data;
    }

    [[nodiscard]] bool isEmpty() const { return top == nullptr; }
};

//
// Queue Class
//
template <typename T>
class Queue
{
private:
    Node<T>* front;
    Node<T>* rear;

public:
    Queue() : front(nullptr), rear(nullptr)
    {
    }

    ~Queue()
    {
        while (!isEmpty())
        {
            Node<T>* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        front = nullptr;
    }

    void enqueue(T data)
    {
        auto* newNode = new Node<T>(data);
        if (!rear)
        {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    T dequeue()
    {
        if (isEmpty()) throw runtime_error("Queue Underflow");
        Node<T>* temp = front;
        T data = temp->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return data;
    }

    T peek() const
    {
        if (isEmpty()) throw runtime_error("Queue is Empty");
        return front->data;
    }

    [[nodiscard]] bool isEmpty() const { return front == nullptr; }
};

//
// PriorityQueue Class
//
template <typename T>
class PriorityQueue
{
private:
    Node<T>* head;

public:
    PriorityQueue() : head(nullptr)
    {
    }

    ~PriorityQueue()
    {
        while (!isEmpty())
        {
            const Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        head = nullptr;
    }

    void enqueue(T data, int priority)
    {
        //insert at correct spot acc. to priority
        auto* newNode = new Node<T>(data, priority);
        if (!head || head->priority < priority)
        {
            //can insert at head
            newNode->next = head;
            head = newNode;
        }
        else
        {
            //cant insert at head! find correct location.
            Node<T>* current = head;
            while (current->next && current->next->priority >= priority)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    T dequeue()
    {
        //usual deque
        if (isEmpty()) throw runtime_error("PriorityQueue Underflow");
        Node<T>* temp = head;
        T data = temp->data;
        head = head->next;
        delete temp;
        return data;
    }

    T peek() const
    {
        if (isEmpty()) throw runtime_error("PriorityQueue is Empty");
        return head->data;
    }

    [[nodiscard]] bool isEmpty() const { return head == nullptr; }
};

//
//bst tree
//orders items based on the < and > operators
//
template <typename T>
class TreeNode
{
public:
    T data;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(T val) : data(val), left(nullptr), right(nullptr)
    {
    }
};

template <typename T>
class Tree
{
public:
    TreeNode<T>* root;

    Tree() : root(nullptr)
    {
    }

    ~Tree()
    {
        destroyTree(root);
    }

    void destroyTree(TreeNode<T>* node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void insert(T data)
    {
        root = insertRec(root, data);
    }

    TreeNode<T>* insertRec(TreeNode<T>* node, T data)
    {
        //BST impl.
        if (!node) return new TreeNode<T>(data);
        if (data < node->data) node->left = insertRec(node->left, data);
        else if (data > node->data) node->right = insertRec(node->right, data);
        return node;
    }

    void inOrder(TreeNode<T>* node) const
    {
        if (!node) return;
        inOrder(node->left);
        cout << node->data << " "; //in order: data between left, right
        inOrder(node->right);
    }

    TreeNode<T>* search(T key)
    {
        return searchRec(root, key);
    }

    TreeNode<T>* searchRec(TreeNode<T>* node, T key)
    {
        if (node == nullptr || node->data == key) return node;
        if (key < node->data) return searchRec(node->left, key); //search left side
        return searchRec(node->right, key); //search right side
    }
};

//
// Path Result Struct
//
template <typename T>
struct PathResult
{
    DynamicArray<T> path;
    float totalCost;

    PathResult() : totalCost(-1.0f)
    {
    }
};

//
//graph
//
template <typename T>
struct Edge
{
    T src;
    T dest;
    float weight;
};

//
// Graph Class
//
template <typename T>
class Graph
{
private:
    int V;
    int capacity;
    T* vertices;
    float** adjMat;

    void resize()
    {
        const int newCapacity = capacity * 2;

        T* newVertices = new T[newCapacity];
        for (int i = 0; i < V; i++)
        {
            newVertices[i] = vertices[i];
        }
        delete[] vertices;
        vertices = newVertices;

        auto** newAdj = new float*[newCapacity];
        for (int i = 0; i < newCapacity; i++)
        {
            newAdj[i] = new float[newCapacity];
            for (int j = 0; j < newCapacity; j++)
            {
                if (i < capacity && j < capacity)
                {
                    newAdj[i][j] = adjMat[i][j];
                }
                else
                {
                    newAdj[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < capacity; i++)
        {
            delete[] adjMat[i];
        }
        delete[] adjMat;

        adjMat = newAdj;
        capacity = newCapacity;
    }

    int getIndex(const T& data) const
    {
        for (int i = 0; i < V; i++)
        {
            if (vertices[i] == data) return i;
        }
        return -1;
    }

public:
    explicit Graph(const int init = 10)
    {
        V = 0;
        capacity = init;
        vertices = new T[capacity];

        adjMat = new float*[capacity];
        for (int i = 0; i < capacity; i++)
        {
            adjMat[i] = new float[capacity];
            for (int j = 0; j < capacity; j++)
            {
                adjMat[i][j] = 0;
            }
        }
    }

    ~Graph()
    {
        delete[] vertices;
        if (adjMat)
        {
            for (int i = 0; i < capacity; i++)
            {
                delete[] adjMat[i];
            }
            delete[] adjMat;
        }
    }

    void addNode(T data)
    {
        if (getIndex(data) != -1) return;

        if (V == capacity)
        {
            resize();
        }
        vertices[V++] = data;
    }

    void addEdge(T src, T dest, const float weight)
    {
        const int srcIdx = getIndex(src);
        const int destIdx = getIndex(dest);

        if (srcIdx == -1 || destIdx == -1)
        {
            cout << "Invalid Vertices!" << endl;
            return;
        }
        adjMat[srcIdx][destIdx] = weight;
    }

    float getEdgeWeight(T src, T dest)
    {
        const int srcIdx = getIndex(src);
        const int destIdx = getIndex(dest);

        if (srcIdx != -1 && destIdx != -1)
        {
            return adjMat[srcIdx][destIdx];
        }
        return -1.0f;
    }

    void display()
    {
        cout << "Adjacency Matrix:" << endl;
        cout << "\t";
        for (int i = 0; i < V; i++)
        {
            cout << vertices[i] << "\t";
        }
        cout << endl;

        for (int i = 0; i < V; i++)
        {
            cout << vertices[i] << "\t";
            for (int j = 0; j < V; j++)
            {
                cout << adjMat[i][j] << "\t\t";
            }
            cout << endl;
        }
    }

    DynamicArray<Edge<T>> getAllEdges() const
    {
        DynamicArray<Edge<T>> edges;
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (adjMat[i][j] != 0)
                {
                    Edge<T> e;
                    e.src = vertices[i];
                    e.dest = vertices[j];
                    e.weight = adjMat[i][j];
                    edges.push_back(e);
                }
            }
        }
        return edges;
    }

    PathResult<T> getShortestPath(T start, T end)
    {
        PathResult<T> result;
        const int startIdx = getIndex(start);
        const int endIdx = getIndex(end);

        if (startIdx == -1 || endIdx == -1)
        {
            return result; // Invalid
        }

        //dijkstra time. equivalent to a map!
        float* dist = new float[V];
        bool* visited = new bool[V];
        int* parent = new int[V];

        for (int i = 0; i < V; i++)
        {
            dist[i] = numeric_limits<float>::infinity();
            visited[i] = false;
            parent[i] = -1;
        }

        dist[startIdx] = 0;

        for (int count = 0; count < V - 1; count++)
        {
            //minimum distance
            int u = -1;
            float minVal = numeric_limits<float>::infinity();

            for (int v = 0; v < V; v++)
            {
                if (!visited[v] && dist[v] <= minVal)
                {
                    minVal = dist[v];
                    u = v;
                }
            }

            if (u == -1) break;

            visited[u] = true;

            for (int v = 0; v < V; v++)
            {
                if (!visited[v] && adjMat[u][v] != 0 && dist[u] != numeric_limits<float>::infinity() &&
                    dist[u] + adjMat[u][v] < dist[v])
                {
                    dist[v] = dist[u] + adjMat[u][v];
                    parent[v] = u;
                }
            }
        }

        //find path
        if (dist[endIdx] != numeric_limits<float>::infinity())
        {
            result.totalCost = dist[endIdx];

            int curr = endIdx;
            int pathLen = 0;
            while (curr != -1)
            {
                pathLen++;
                curr = parent[curr];
            }

            //store indices
            int* pathIndices = new int[pathLen];
            curr = endIdx;
            for (int i = pathLen - 1; i >= 0; i--)
            {
                pathIndices[i] = curr;
                curr = parent[curr];
            }

            for (int i = 0; i < pathLen; i++)
            {
                result.path.push_back(vertices[pathIndices[i]]);
            }
            delete[] pathIndices;
        }

        delete[] dist;
        delete[] visited;
        delete[] parent;

        return result;
    }
};

//
// Sorter Class
//
template <typename T> //static class for sorting on DynamicArrays (assuming "T" has comparator overloads.)
class Sorter
{
public:
    // O(n^2)
    static void bubbleSort(DynamicArray<T>& arr)
    {
        const int n = arr.size();
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (arr[j] < arr[j + 1])
                    swap(arr[j], arr[j + 1]);
    }

    // O(n*log(n))
    static void mergeSort(DynamicArray<T>& arr, const int left, const int right)
    {
        if (left < right)
        {
            const int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

private:
    static void merge(DynamicArray<T>& arr, const int left, const int mid, const int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        DynamicArray<T> L(n1), R(n2);

        for (int i = 0; i < n1; i++) L.push_back(arr[left + i]);
        for (int j = 0; j < n2; j++) R.push_back(arr[mid + 1 + j]);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {
            if (L[i] > R[j]) arr[k++] = L[i++];
            else arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    static void swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
};

#endif
