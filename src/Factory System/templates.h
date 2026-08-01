//
// Created by Huzaifa on 12/16/2025.
//

#ifndef TEMPLATES_H
#define TEMPLATES_H
// ============================================================================
// TEMPLATE DATA STRUCTURES IMPLEMENTATION
// Custom implementations for queues, stacks, heaps, and arrays
// ============================================================================

// ============================================================================
// ORDER QUEUE (FIFO) - TEMPLATED
// Used for normal order intake
// ============================================================================

template<typename T>
class OrderQueue {
    struct Node {
        T *data;
        int dataID;
        Node *next;
        Node(T *d, int id) : data(d), dataID(id), next(nullptr) {}
    };
    Node *front;
    Node *rear;
    int size;

public:
    OrderQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~OrderQueue() {
        while (!isEmpty()) {
            int dummy;
            dequeue(dummy);
        }
    }

    void enqueue(T *data, int dataID) {
        Node *newNode = new Node(data, dataID);

        // Empty queue case
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }

        size++;
    }

    T *dequeue(int &dataID) {
        if (isEmpty()) return nullptr;

        Node *temp = front;
        T *data = temp->data;
        dataID = temp->dataID;

        front = front->next;

        // Queue became empty
        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
        size--;
        return data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return size;
    }
};


// ============================================================================
// PRIORITY QUEUE (MIN-HEAP) - TEMPLATED
// Lower priority value = higher priority
// Used for urgent orders
// ============================================================================

template<typename T>
class PriorityOrderQueue {
    struct PriorityItem {
        T *data;
        int dataID;
        int priority;
    };
    PriorityItem **heap;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        auto newHeap = new PriorityItem *[capacity];

        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }

        delete[] heap;
        heap = newHeap;
    }

    void heapifyUp(int index) {
        while (index > 0 && heap[parent(index)]->priority > heap[index]->priority) {
            PriorityItem *temp = heap[index];
            heap[index] = heap[parent(index)];
            heap[parent(index)] = temp;
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int smallest = index;
        int leftChild = left(index);
        int rightChild = right(index);

        if (leftChild < size && heap[leftChild]->priority < heap[smallest]->priority) {
            smallest = leftChild;
        }

        if (rightChild < size && heap[rightChild]->priority < heap[smallest]->priority) {
            smallest = rightChild;
        }

        if (smallest != index) {
            PriorityItem *temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

public:
    PriorityOrderQueue(int cap = 10) {
        capacity = cap;
        heap = new PriorityItem *[capacity];
        size = 0;
    }

    ~PriorityOrderQueue() {
        for (int i = 0; i < size; i++) {
            delete heap[i];
        }
        delete[] heap;
    }

    void insert(T *data, int dataID, int priority) {
        if (size >= capacity) {
            resize();
        }

        auto newItem = new PriorityItem();
        newItem->data = data;
        newItem->dataID = dataID;
        newItem->priority = priority;

        heap[size] = newItem;
        heapifyUp(size);
        size++;
    }

    T *extractMin(int &dataID) {
        if (isEmpty()) return nullptr;

        PriorityItem *minItem = heap[0];
        T *data = minItem->data;
        dataID = minItem->dataID;

        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);

        delete minItem;
        return data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }
};


// ============================================================================
// REWORK STACK (LIFO) - TEMPLATED
// Used for orders that fail QA and need reprocessing
// ============================================================================

template<typename T>
class ReworkStack {
    struct Node {
        T *data;
        int dataID;
        Node *next;
        Node(T *d, int id) : data(d), dataID(id), next(nullptr) {}
    };
    Node *top;
    int size;

public:
    ReworkStack() : top(nullptr), size(0) {}

    ~ReworkStack() {
        while (!isEmpty()) {
            int dummy;
            pop(dummy);
        }
    }

    void push(T *data, int dataID) {
        Node *newNode = new Node(data, dataID);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T *pop(int &dataID) {
        if (isEmpty()) return nullptr;

        Node *temp = top;
        T *data = temp->data;
        dataID = temp->dataID;

        top = top->next;
        delete temp;
        size--;
        return data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }
};


// ============================================================================
// FINISHED PRODUCT ARRAY (DYNAMIC ARRAY) - TEMPLATED
// Stores completed products
// ============================================================================

template<typename T>
class FinishedProductArray {
    struct FinishedItem {
        T *data;
        int dataID;
    };
    FinishedItem *items;
    int count;
    int capacity;

    void resize() {
        capacity *= 2;
        auto newArray = new FinishedItem[capacity];

        for (int i = 0; i < count; i++) {
            newArray[i] = items[i];
        }

        delete[] items;
        items = newArray;
    }

public:
    FinishedProductArray(int cap = 10) {
        capacity = cap;
        items = new FinishedItem[capacity];
        count = 0;
    }

    ~FinishedProductArray() {
        delete[] items;
    }

    void add(T *data, int dataID) {
        if (count >= capacity) {
            resize();
        }

        items[count].data = data;
        items[count].dataID = dataID;
        count++;
    }

    T *get(int index, int &dataID) const {
        if (index < 0 || index >= count) return nullptr;

        dataID = items[index].dataID;
        return items[index].data;
    }

    int getCount() const {
        return count;
    }
};
#endif //TEMPLATES_H
