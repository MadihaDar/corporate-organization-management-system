#include "Product.h"
#include "common.h"
#include "RoutePlanner.h"
#include <string>
#include "MinHeap.h"
#include "RoutePlanner.h"
#include <limits>
#include <iostream>
//constructor
RoutePlanner::RoutePlanner(int maxS) {
    maxStores = maxS;
    storeCount = 0;
    stores = new Store*[maxStores];

    //making adjacency matrix by 2-D array
    adjMatrix = new double*[maxStores]; //allocate pointers of array
    for (int i = 0; i < maxStores; i++) {
        adjMatrix[i] = new double[maxStores];//assigning columns to every index of pointers array
        for (int j = 0; j < maxStores; j++)
            adjMatrix[i][j] = -1;   //-1 indicates that there will be no connection between stores
    }
}
//destructor
RoutePlanner::~RoutePlanner() {
    for (int i = 0; i < maxStores; i++) {
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
    delete[] stores;
}


//Add a store, it behaves just like adding a vertex of store in graph visualization
void RoutePlanner::addStore(Store* s) {
    if (storeCount < maxStores) {
        stores[storeCount] = s;
        storeCount++;
    }
}

//adding undirected  weighted edge
void RoutePlanner::addEdge(int u, int v, double w) {
    if (u < 0 || v < 0 || u >= storeCount || v >= storeCount) {
        cout << " Invalid edge (" << u << ", " << v
             << "). Store index out of range.\n";
        return;
    }

        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;
    cout<<"Edge Added Successfully"<<endl;

}


//display Adjacency matrix function
void RoutePlanner::displayAdjMatrix() {
    cout << "\nAdjacency Matrix (Distances Between Stores)\n\n";
    //Column headings
    cout << "     ";
    for (int j = 0; j < storeCount; j++) {
        cout << j << "\t";
    }
    cout << "\n";
    cout << "    ";
    for (int j = 0; j < storeCount; j++) {
        cout << "--------";
    }
    cout << "\n";
    //Rows
    for (int i = 0; i < storeCount; i++) {
        cout << i << " |  ";
        for (int j = 0; j < storeCount; j++) {
            cout << adjMatrix[i][j] << "\t";
        }
        cout << "\n";
    }
}


//List of all stores
void RoutePlanner::listAllStores() {
    cout << "\nStores:\n";
    for (int i = 0; i < storeCount; i++) {
        cout << i << " -> " <<stores[i]->getName()
             << " (" << stores[i]->getID() << ")"<<endl;
    }
}


//Dijkstra implementation using priority queue min heap
//this is for undirected weighted graph
void RoutePlanner::dijkstra(int source, double*& shortestDistance, int*& previousStore) {
    shortestDistance = new double[storeCount]; //it will store shortest distance of every Store
   previousStore = new int[storeCount];//this will store parent of shortest path
    bool* Explored = new bool[storeCount];//check which store is explored or which is not

    //initializing everything
    //iterrate all stores
    for (int i = 0; i < storeCount; i++) {
        shortestDistance[i] = 1e12; //we dont know path initially of any store from source so assuming it a very large value
        previousStore[i] = -1; //Parent of store is unknown so it is set to -1 by default
        Explored[i] = false;// no store is explored yet so set to false
    }
     //making a priority queue here based on minHeap
    // <int,double> == <key,priority> ; key is store index ; priority is current shortest index
    MinHeap<int,double> priority_queue;

   shortestDistance[source] = 0;//as source distance is zero from itself
   priority_queue.push(source, 0);//pushing the source in priority queue

    while (!priority_queue.empty()) { //until queue is not empty
        auto top = priority_queue.pop(); //extracting store
        int currentStore= top.key; //store that we want to see

        if (Explored[currentStore]) {
            continue; // if already explored then this loop is skipped
        }
       Explored[currentStore] = true; //otherwise marked true

        //exploring neighbors now
        //every store will check it's edge with all other stores either it is existed or not
        for (int neighborStore = 0; neighborStore < storeCount; neighborStore++) {
            if (adjMatrix[currentStore][neighborStore] != -1) {    //it means edge exists
                //storing weight of edge in edgeWeight
                double edgeWeight= adjMatrix[currentStore][neighborStore];
                //this is relaxation here, if vertex is not visited yet and a new path from current store to neighbour store
                //that is less than before then update the path
                if (!Explored[neighborStore] && shortestDistance[currentStore] + edgeWeight < shortestDistance[neighborStore]) {
                    shortestDistance[neighborStore] =shortestDistance[currentStore] + edgeWeight;//updated new shotest distan in array
                    previousStore[neighborStore] = currentStore;//update parent store
                    priority_queue.push(neighborStore, shortestDistance[neighborStore]);//inserting updated distance in queue
                }
            }
        }
    }

    delete[] Explored;
}

void RoutePlanner::runAndPrintDijkstra(int source) {
    double* distance = nullptr;
    int* parent = nullptr;

    dijkstra(source, distance, parent);

    cout << "\n====**##SHORTEST ROUTES FROM STORE##** " << source << " ====\n";

    for (int i = 0; i < storeCount; i++) {
        cout << "To Store " << i
             << "("<< stores[i]->getName() << "): Distance = "
             << distance[i] << " km";

        cout << " | Path: ";
        int temp = i;
        while (temp != -1) {
            cout<<temp;
            temp=parent[temp];
            if(temp != -1)
                cout << " <- ";
        }
        cout <<endl;
    }

    delete[] distance;
    delete[] parent;
}
