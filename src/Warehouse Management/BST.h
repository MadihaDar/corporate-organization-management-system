//
// Created by Mahrukh Khan on 12/3/2025.
//

#ifndef INVENTORYBST_H
#define INVENTORYBST_H
#include <string>
#include <iostream>
#include "common.h"

using namespace std;


#include "Product.h"
#include <iostream>
#include <string>

template<typename T>
class BST {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(T d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, T val);
    Node* remove(Node* node, const string& id);
    T search(Node* node, const string& id) const;
    void inorder(Node* node) const;
    void destroy(Node* node);
    int countNodes(Node* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
    void inorderToArray(Node* node, T* arr, int& index) const {
        if (!node) return;
        inorderToArray(node->left, arr, index);
        arr[index++] = node->data;
        inorderToArray(node->right, arr, index);
    }
public:
    BST();
    ~BST();

    void insert(T val);
    void remove(const string& key);
    T search(const string& key) const;
    void inorder_traverse() const;
    void inorder_traverse_to_array(T* arr, int& index) const {
        inorderToArray(root, arr, index);
    }
    int getSize() const {
        return countNodes(root);
    }
};

#endif //INVENTORYBST_H
