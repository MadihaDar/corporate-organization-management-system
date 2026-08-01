#include "Product.h"
#include "common.h"
#include <string>
#include "MinHeap.h"
#include <iostream>


//MinHeap<K,P> it is a template class to run with any data type
//K is key ; it is actual data ; means datatype
//P is priority ; at which comparison is done

//constructor
template<typename K, typename P>
MinHeap<K,P>::MinHeap(int initialCap) : capacity(initialCap), length(0) {
    if (capacity < 4){
    capacity = 4;
}
    arr = new Node[capacity];
}

//destructor
template<typename K, typename P>
MinHeap<K,P>::~MinHeap() {
    delete[] arr;
}


template<typename K, typename P>
void MinHeap<K,P>::resize(int newCap) {
    Node* newArr = new Node[newCap];
    for (int i = 0; i < length; ++i) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    capacity = newCap;
}
//when element is added , it is added at last of array then it needs to heapify up
//to take the element to its correct position to maintain minheap queue by comparing its parent

template<typename K, typename P>
void MinHeap<K,P>::heapify_up(int index) {
    while (index > 0) {
        int parent = (index- 1) / 2;  //to find parent of any index
        if (arr[index].pr < arr[parent].pr) { //parent must be smaller then child otherwise swap
            Node temp = arr[index];
            arr[index] = arr[parent];
            arr[parent] = temp;
            index = parent;  //now again compare with its parent until index is zero
        } else break;
    }
}

//when element is pop , it removes from the start of queue,
//to maintain minheap property in queue
//Extract the start element from array then replaces it with last element in array
//then heapify down that number downwards by comparing its children
template<typename K, typename P>
void MinHeap<K,P>::heapify_down(int index) {
    while (true) {
        int left = 2*index + 1;  //left child of index
        int right = 2*index + 2; //right child of index
        int smallest = index;    //assuming index is the smallest
        //comparing left child with smallest(parent)
        if (left < length && arr[left].pr < arr[smallest].pr) {smallest = left;}
        //comparing right child with smallest(parent)
        if (right < length && arr[right].pr < arr[smallest].pr) {smallest = right;}
        //if smallest is not equal to index , it means swapping will happen,
        //parent that assumed as smallest is swapped from accurate child index that smaller than parent
        if (smallest != index) {
            Node temp = arr[index];
            arr[index] = arr[smallest];
            arr[smallest] = temp;
            index = smallest; //update index to smallest (left or right child index)
                              //again heapify down until loop is true
        } else break;
    }
}

template<typename K, typename P>
void MinHeap<K,P>::push(const K& key, P priority) {
    if (length == capacity) {
        resize(capacity * 2);
    }
    arr[length].key = key;
    arr[length].pr = priority;
    heapify_up(length); //last element index at which value is placed
    length++;
}

template<typename K, typename P>
typename MinHeap<K,P>::Node MinHeap<K,P>::pop() {
    Node emptyNode = Node();
    if (length == 0) {return emptyNode;}
    Node top = arr[0];
    arr[0] = arr[length-1]; //replacing last index to first index
    length--;
    heapify_down(0);
    return top;
}

template<typename K, typename P>
bool MinHeap<K,P>::empty() const {
    return length == 0;
}

template<typename K, typename P>
int MinHeap<K,P>::size() const {
    return length;
}

//Explicit instantiation to tell the compiler datatypes

template class MinHeap<string, double>;
template class MinHeap<int, double>;
