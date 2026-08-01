//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H
#include <string>
#include <iostream>
#include "common.h"
#include"Store.h"
using namespace std;

#include <string>
#include "MinHeap.h"
class RoutePlanner {
private:
    int maxStores;
    int storeCount;
    Store** stores;          //dynamic list of store pointers
    double** adjMatrix;      //adjacency matrix by 2D array creating it

public:
    RoutePlanner(int maxS = 50);
    ~RoutePlanner();

    //to add stores , like vertex of graph
    void addStore(Store* s);
     int getStoreCount() const {
         return storeCount;
     }

    void addEdge(int u, int v, double w);//edge is undirected and weighted
    void displayAdjMatrix();


    void dijkstra(int source, double*& outDist, int*& outParent);

    void listAllStores();
    void runAndPrintDijkstra(int source);

};

#endif //ROUTEPLANNER_H
