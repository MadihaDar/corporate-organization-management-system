//
// Created by Mahrukh Khan on 12/7/2025.
//

#ifndef SORT_H
#define SORT_H

#include <iostream>
#include "Product.h"
using namespace std;
class Sort {
private:
    Product** array; //pointer to product type data
    int size;        //size of array
    int capacity;

    void resize() {  //if capacity is full then resize
        capacity *= 2;
        Product** newArray = new Product*[capacity];
        for (int i = 0; i < size; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }

    void manualSwap(Product*& a, Product*& b) {
        Product* temp = a;
        a = b;
        b = temp;
    }
    //############################
   //          ###Merge Sort###
   //#################################
    void merge(int start, int mid, int end) {
        Product** temp = new Product*[end - start + 1]; //making temporary array of size when two subarrays are merging back so size of their both
        int left = start, right = mid + 1, index = 0;   //make variables to trace a check on indexes
        //arranging in ascending order until left and right arrays both exist
        while (left <= mid && right <= end) {
            //comparing both side arrays elements and the lesser one come in temp array
            if (array[left]->getQuantity() <= array[right]->getQuantity()) {
                temp[index] = array[left];
                index++;
                left++;
            }
            else {
                temp[index] = array[right];
                index++;
                right++;
            }
        }
        while (left <= mid) {
            //if only left array remains
            temp[index] = array[left];
            index++;
            left++;
        }
        while (right <= end) {
            //if only right array remains
            temp[index] = array[right];
            index++;
            right++;
        }
        //copy back the temp array elements in original array
        for (int i = 0; i < index; i++) {
            array[start + i] = temp[i];
        }

        delete[] temp;
    }

    void mergeSortFunction(int start, int end) {
        //base case
        if (start >= end) {
            return;
        }
        int mid = start + (end - start) / 2; //for finding mid value
        mergeSortFunction(start, mid);   //left side
        mergeSortFunction(mid + 1, end);//right side
        merge(start, mid, end);              //for merging back in sorted order ascending
    }

    //##########################
    //Bubble sort
    //##########################
    void bubbleSortFunction() {
        for (int i = size - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                //comparing elements if they are in wrong order swap
                if (array[j]->getQuantity() > array[j + 1]->getQuantity()) {
                    manualSwap(array[j], array[j + 1]);
                }
            }
        }
    }

    //#############################
    //Quick Sort
    //############################
    int medianOfThree(int start, int end) {
        int mid = start + (end - start) / 2; //middle value
        //comparing to place that theree position elements in right ascending order
        if (array[start]->getQuantity() > array[mid]->getQuantity()) {
            manualSwap(array[start], array[mid]);
        }
        if (array[start]->getQuantity() > array[end]->getQuantity()) {
            manualSwap(array[start], array[end]);
        }
        if (array[mid]->getQuantity() > array[end]->getQuantity()) {
            manualSwap(array[mid], array[end]);
        }

        manualSwap(array[mid], array[end]);
        return end; //pivot index
    }

    int partition(int start, int end) {
        //apprach is this that taking median of three elements (start , mid , end) of array
        //then taking the middle one from the three as median and consider it as pivot then replacing it with the last index
        //then comparing from 1 index to the n-2
        //from left side if element is greater and from right side if elements are smaller swao the elements
        //at last when iterators cross each other ,place the pivot value at that place
        //again recursion calls and array is sorted
        int pivotIndex = medianOfThree(start, end);
        Product* pivot = array[pivotIndex]; //pivot value
        int low = start, high = end - 1;

        while (true) {
            while (low <= high && array[low]->getQuantity() < pivot->getQuantity()) {
                low++;
            }
            while (high >= low && array[high]->getQuantity() > pivot->getQuantity()) {
                high--;
            }
            if (low >= high)
            {break;}
            manualSwap(array[low], array[high]);
            low++;
            high--;
        }
        manualSwap(array[low], array[end]);
        return low;
    }

    void quickSortFunction(int start, int end) {
        //base case
        if (end - start <= 4) {
            insertionSortFunction(start, end);
            return;
        }
        int pivotIndex = partition(start, end);  //1stly finding pivot
        quickSortFunction(start, pivotIndex - 1); //left side
        quickSortFunction(pivotIndex + 1, end); //right side
    }

    void insertionSortFunction(int start, int end) {

        for (int i = start + 1; i <= end; i++) {
            for (int j = i; j > start; j--) {
                //comparing values
                if (array[j]->getQuantity() < array[j - 1]->getQuantity()){
                manualSwap(array[j], array[j - 1]);
            }
            }
        }
    }

public:
    //constructor
    Sort(Product** arr, int siz) : array(arr), size(siz), capacity(siz) {}

    void bubbleSort() { bubbleSortFunction(); }
    void mergeSort() { mergeSortFunction(0, size - 1); }
    void quickSort() { quickSortFunction(0, size - 1); }
};



#endif //SORT_H
