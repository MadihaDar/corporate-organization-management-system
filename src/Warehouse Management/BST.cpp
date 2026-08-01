#include "Product.h"
#include "common.h"

#include "BST.h"

//constructor
template<typename T>
BST<T>::BST() : root(nullptr) {}

//destructor
template<typename T>
BST<T>::~BST() {
    destroy(root);
}

//insert node in tree
template<typename T>
typename BST<T>::Node* BST<T>::insert(Node* node, T val) {
    if (!node) {
        return new Node(val); //adress is returnin here that is created
    }
    if (val->getID() < node->data->getID()) {
        node->left = insert(node->left, val);
    }
    else {
        node->right = insert(node->right, val);
    }
    return node;
}

template<typename T>
void BST<T>::insert(T val) {
    root = insert(root, val);
}

//remove node
template<typename T>
typename BST<T>::Node* BST<T>::remove(Node* node, const string& id) {
    //if node==nullptr(no tree) so just return nullptr
    if (!node) {return nullptr;}
    //if id of product is less than the current node of tree,find it in left side
    if (id < node->data->getID())
    {node->left = remove(node->left, id);}
    //else if id is greater , go and find in right side tree
    else if (id > node->data->getID()) {
        node->right = remove(node->right, id);
    }
    else {
        //Node is found, this block only runs when node is found
        if (!node->left) {
            //if no left child exists of the node that we want to delete then
            //replace that node with its right child
            Node* r = node->right;
            delete node;
            return r;
        }
        if (!node->right) {
            //if no right child exists of the node that we want to delete then
            //replace that node with its left child
            Node *l = node->left;
            delete node;
            return l;
        }
        //case: when both children existed then move to the right subtree and then go
        //left side's of node and found the least element then replace this last left node data
        //with the node we want to delete ,but it creates copy of that node so then delete that last node too
        Node* minRight = node->right;// right subtree of the node we want to delete
        while (minRight->left) {
            minRight = minRight->left; //its like inorder traversal LNR, this goes left andleft and found the least value node
            //this approach actually maintains the BST order
        }
        node->data = minRight->data; //replacing the data
        node->right = remove(node->right, minRight->data->getID());//again recursion call to delete that leaf node now
    }
    return node;//it will return the updated tree
}

template<typename T>
void BST<T>::remove(const string& key) {
    root = remove(root, key);
}

//search node
template<typename T>
T BST<T>::search(Node* node, const string& id) const {
    if (!node) {//if node does not exist
        return nullptr;
    }
    if (id == node->data->getID()) {
        return node->data; //if id is found
    }
    if (id < node->data->getID()) {
        return search(node->left, id); // if id is less then current node, go and find in left
    }
    return search(node->right, id);//else go in right
}

template<typename T>
T BST<T>::search(const string& key) const {
    return search(root, key);
}

//inorder traversal
template<typename T>
void BST<T>::inorder(Node* node) const {
    if (!node) {
        return;
    }
    inorder(node->left); //first go left , then prints node , then go right
    cout << node->data->getName() << " (ID:" << node->data->getID() << ")"<<endl;
    inorder(node->right);
}

template<typename T>
void BST<T>::inorder_traverse() const {
    inorder(root);
}

//destroy tree
template<typename T>
void BST<T>::destroy(Node* node) {
    if (!node) {return;}
    destroy(node->left); //go left
    destroy(node->right); //go right
    delete node; //delete
}

//Explicit instantiation for Product* to tell the compiler
template class BST<Product*>;
