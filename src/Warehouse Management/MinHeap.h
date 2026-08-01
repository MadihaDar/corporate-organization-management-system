//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef MINHEAP_H
#define MINHEAP_H
#include <string>
#include <iostream>
#include "common.h"

using namespace std;


#include <string>

template<typename K, typename P>
class MinHeap {
private:
    struct Node {
        K key;
        P pr;
    };
    Node* arr;
    int capacity;
    int length;

    void resize(int newCap);
    void heapify_up(int idx);
    void heapify_down(int idx);

public:
    MinHeap(int initial_cap = 64);
    ~MinHeap();

    void push(const K& key, P priority); //in queue value pushes at last of array
    Node pop(); //returns smallest because this is min heap and in queue , value retrieves from start
    bool empty() const;
    int size() const;
};

#endif //MINHEAP_H
