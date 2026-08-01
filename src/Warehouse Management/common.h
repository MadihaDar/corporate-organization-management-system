

#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <iostream>
using namespace std;
//This file "common.h" has classes for single linked list , doubly linked list , and linked queue
//these all classes are template based
//all other classes used these classes
     //==============================================================
               //Single linked list template
     //==============================================================

     template<typename T>
     class SingleNode{
private:
      //Data Members
      T data;
      SingleNode* next;
public:

      //parameterised constructor
         SingleNode(T valuee){
          data = valuee;
          next = nullptr;
         }
      //destructor
        ~SingleNode(){}

    //Sets the next pointer to the provided node
     void setNext(SingleNode* valueAddress){
            next = valueAddress;
         }
    //Returns the next node pointer
    SingleNode* getNext(){
            return next;
         }
    void setData(T valuee){
     data = valuee;
         }
    T getData(){
    return data ;
         }
};


//Singly linked List
template <typename T>
class SLinkedList{
private:
       SingleNode<T>* head;
       SingleNode<T>* tail;
       int count = 0 ;
public:


//constructor
SLinkedList(){
head = nullptr;
tail = nullptr;
count = 0;

}
//destructor
~SLinkedList(){

SingleNode<T>* current = head;
while(current != nullptr){
SingleNode<T> * temp = current ;
current = current -> getNext();
delete temp;
}
}
 void clear() {
 while (!isEmpty()) {
  deleteFromStart();
 }
}

bool isEmpty(){
   if (head==nullptr){
        return true;
     }
   else {return false ;}

}

    void append(T data){
     SingleNode<T> * oneNode = new SingleNode<T>(data);
     if(isEmpty()){
          head = oneNode;
          tail = oneNode;
       }
     else{
           tail ->setNext(oneNode);
           tail = oneNode;
       }
     count++;
    }
    void prepend(T data){
 SingleNode<T> * oneNode = new SingleNode<T>(data);
 if(isEmpty()){
  head = oneNode;
  tail = oneNode;
 }
else{
   oneNode-> setNext(head);
    head = oneNode;
  }
  count ++;

    }
    void deleteFromEnd(){
 if(isEmpty()) {
  cout<<"List is empty"<<endl;
 }

 if(head ==tail){ //if only one node exists
                 delete head ;
                 head = nullptr;
                 tail = nullptr;
 }
else {
    SingleNode<T> * recent = head;
    while(recent -> getNext() != tail){
       recent = recent ->getNext();
        }
    delete tail;
    tail = recent;
    tail -> setNext(nullptr);
     }
count --;
}
    void deleteFromStart(){
      if(isEmpty()){cout<<"List is empty"<<endl;}
       SingleNode<T>* tentative = head;
       head = head -> getNext();
       delete tentative;
       if(head == nullptr){
        tail =nullptr;

       }
     count--;
     }
   SingleNode<T>* getHead( ){
       return head;
      }
    SingleNode<T>* getTail( ){

       return tail;
      }
    void printList( ) {
 if(isEmpty()){
  cout<<"List is empty"<<endl;
  return;
 }
 cout<<"=========="<<endl;
 SingleNode<T> * currentNode = head;
 while(currentNode != nullptr){

  cout<<currentNode -> getData();
  currentNode = currentNode-> getNext();

  cout<<endl;
 }
}
 int get_Count()const{
 return count;
}


 SingleNode<T>* get_head() const { return head; }
 SingleNode<T>* get_tail() const { return tail; }
};
//==============================================================
      // doubly linked list template
//==============================================================
      //node class
template<typename T>
//Node class
class DListNode{
 //data members
private:
 T data;
 DListNode *next;
 DListNode *previous;
public:
 //constructor
 DListNode(T value){
  data = value;
  next= nullptr;
  previous= nullptr;
 }
 //destructor
 ~DListNode(){}

 //Sets the next pointer to the provided node
 void setNext(DListNode *val){
  next = val;
 }
 // Returns the next node pointer
 DListNode* getNext(){
  return next;
 }
 //Sets the previous pointer to the provided node
 void setPrevious(DListNode* val) {
  previous = val;
 }
 // Returns the previous node pointer.
 DListNode* getPrevious(){
  return previous;
 }
 // Sets the data of the node
 void setData(T data){
  this->data = data;
 }
 // Returns the data of the node
 T getData(){
  return data;
 }
};
//      list

template<typename T>
//List class
class List{
private:
DListNode<T> *head;  //Pointer to the first node in the list
DListNode<T> *tail;  //Pointer to the last node in the list.
int count;   //The number of elements in the list
public:
//constructor
List(){
head = nullptr;
tail = nullptr;
count = 0;
}
//destructor
~List(){
     DListNode<T> *current = head;
    while(current!=nullptr){
          DListNode<T> * temp = current;
          current = current->getNext();
          delete temp;
    }
}
bool isEmpty(){
   if(head==nullptr){
     return true;
    }
   return false;
 }

    void append(T data){
   if(isEmpty()){
     DListNode<T>*NewNode = new DListNode<T>(data);
     head = NewNode;
     tail = NewNode;
     count++;
   }
else{
  DListNode<T> *NewNode = new DListNode<T>(data);
  tail->setNext(NewNode);
  NewNode->setPrevious(tail);
  tail = NewNode;
  count++;
}
   }

    void prepend(T data){
    if(isEmpty()){
        DListNode<T> *NewNode = new DListNode<T>(data);
        head = NewNode;
        tail = NewNode;
        count++;
    }
    else{
   DListNode<T> * NewNode = new DListNode<T>(data);
   head->setPrevious(NewNode);
   NewNode->setNext(head);
   head = NewNode;
   count++;
}
}

    void insertAtIndex(T data, int index){
        if(index<0){cout<<"invalid index"<<endl; return;}
        if(isEmpty()){
        prepend(data);
        return;
       }
   else if(index==count){ //if want to insert at last
     append(data);
     return;
  }
  else if(index==0){//if insert at start
         prepend(data);
         return;
  }
   else{
     DListNode<T> * NewNode = new DListNode<T>(data);
     DListNode<T> *temp = head;
     //indexing is zero based
     while(index--){
      temp = temp->getNext();
      }
       //after this while loop temp is point to the specific given index

       temp->getPrevious()->setNext(NewNode); // temp's previous node "next " is set to new node that is created
       NewNode->setPrevious(temp->getPrevious());// New node previous is set to temp's previous
       NewNode->setNext(temp);//then newNode's next is pointing to temp
       temp->setPrevious(NewNode);
      count++;
     }
    }

    void deleteFromEnd(){
        if(isEmpty()){
            cout<<"List is empty"<<endl;
        }
         //if only one node is existed in list
         else if(head->getNext()==nullptr){
          delete head;
          head = nullptr;
          tail = nullptr;
          count--;
         }
        else{  //more than 1 node
        DListNode<T>*temp = tail;
        temp->getPrevious()->setNext(nullptr);
         tail = tail->getPrevious();
         delete temp;
         count--;
       }
     }

    void deleteFromStart(){
     if(isEmpty()){
      cout<<"List is empty"<<endl;
     }
     //if only one node is existed in list
    else if(head->getNext()==nullptr){
      delete head;
      head = nullptr;
      tail = nullptr;
      count--;
     }
    else{ //if more than 1 node is existed
       DListNode<T> *temp = head;
       head = head->getNext();
       delete temp;
       head->setPrevious(nullptr);
       count--;
      }
    }

    void deleteFromIndex(int index){
  if(index<0 || index>=count){
  cout<<"invalid index"<<endl;
  return;
}

//if delete from start edgecase
if(index==0){
 if(isEmpty()){
  cout<<"List is empty"<<endl;
 }
 //if only one node is existed in list
 else if(head->getNext()==nullptr){
  delete head;
  head = nullptr;
  tail = nullptr;
  count--;
 }
 else{ //if more than 1 node is existed
  DListNode<T> *temp = head;
  head = head->getNext();
  delete temp;
  head->setPrevious(nullptr);
  count--;
 }
}
//if delete from end edgecase
else if(index==count-1){
 if(isEmpty()){
  cout<<"List is empty"<<endl;
 }
 //if only one node is existed in list
  else if(head->getNext()==nullptr){
  delete head;
  head = nullptr;
  tail = nullptr;
  count--;
 }
 else{  //more than 1 node
 DListNode<T>*temp = tail;
  temp->getPrevious()->setNext(nullptr);
  tail = tail->getPrevious();
  delete temp;
  count--;
 }
}

//if delete from middle
else{
   DListNode<T>*temp = head;
while(index--){
    temp=temp->getNext();

}
    temp->getPrevious()->setNext(temp->getNext());
    temp->getNext()->setPrevious(temp->getPrevious());
delete temp;
count--;
}

     }

DListNode<T> *getHead(){
    return head;
  }

DListNode<T> *getTail(){
   return tail;
  }
void printList(){
  DListNode<T> *trav= head;
  while(trav){
    cout<<trav->getData();
    trav = trav->getNext();
   }
  }
};
//==============================================================
//               LINKED QUEUE
//==============================================================
// Implement the LinkedQueue Class for Queue Implementation
template<typename T>

class LinkedQueue{
private:
 SingleNode<T> *front;
 SingleNode<T> *rear;
 int size;

public:
 //constructor
 LinkedQueue(){
  front = nullptr;
  rear = nullptr;
  size= 0;
 }
 //destructor
 ~LinkedQueue(){
  clear();

 }

 bool isEmpty(){
  if(front == nullptr){
   return true;}
  else{
   return false;
  }
  return false;
 }
 void enqueue(T data){
  SingleNode<T> *newNode = new SingleNode<T>(data);
  if(isEmpty()){

   front = newNode;
   rear = newNode;

  }
  else{
   rear->setNext(newNode);
   rear = rear -> getNext();

  }
  size++;
 }


 int dequeue(){
  if(isEmpty()){
   cout<<" queue is empty"<<endl;
   return -1;
  }
  T temp_storage = front->getData();
 SingleNode<T> * tempNode = front;
  front = front -> getNext();

  if(front == nullptr){
   rear  = nullptr;
  }
  delete tempNode;
  size--;

  return temp_storage;
 }
 int peek(){

  if(isEmpty()){
   cout<<" queue is empty"<<endl;
   return -1;
  }
  return front->getData();
 }
 T getSize(){
  return size;
 }
 void clear(){
  while(!isEmpty()){

   dequeue();

  }
 }
 void printQueue(){

  if(isEmpty()){
   cout<<" queue is empty"<<endl;
  }
 SingleNode<T>* oneNewNode = front;
  while(oneNewNode != nullptr){
   cout <<  oneNewNode->getData()<<" ";
   oneNewNode = oneNewNode->getNext();
  }
 }
};
#endif //COMMON_H


