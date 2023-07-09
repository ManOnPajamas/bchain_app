#ifndef AVL_H
#define AVL_H

#include "nodeavl.h"
//#include <vector>
//#include <stack>
#include "forward_list.h"
#include "circular_array.h"
using namespace std;
template <typename TK, typename TV>
class AVLTree2 {
private:
    NodeT<TK, TV>* root;

public:
    AVLTree2() : root(nullptr) {}
    void insert(TK key, TV value) {
        Entry<TK, TV> entry(key, value);
        insert(this->root, entry);
    }
    bool find(TK value) {
        return find(this->root, value);
    }

    string getInOrder() {
        return getInOrder(this->root);
    }

    int height() {
        return height(this->root);
    }

    TK minValue() {
        return minValue(this->root).key;
    }

    TK maxValue() {
        return maxValue(this->root).key;
    }

    bool isBalanced() {
        return isBalanced(this->root);
    }

    int size() {
        return size(this->root);
    }

    void remove(TK key,TV value) {
        //cout<<"key: "<<key<<" - "<<"value: "<<value<<endl;
        remove(this->root, key, value);
    }

    void displayPretty() {
        displayPretty(this->root, 1);
    }

    TK successor(TK key) {
        Entry<TK, TV> entry(key, TV());
        return successor(root, entry).key;
    }

    TK if_not_found_succesor(TV value) {
        Entry<TK, TV> entry(TK(), value);
        return if_not_found_succesor(root, entry).value;
    }

    TK if_not_found_predecesor(TV value) {
        Entry<TK, TV> entry(TK(), value);
        return if_not_found_predecesor(root, entry).value;
    }


    void search_by_range(CircularArray<TK>& result, TV begin, TV end) {
        result.clear();

        if (!find(begin))
            begin = if_not_found_succesor(begin);
        if (begin>end) return; // para caso en el que no existe en ese rango

        if (!find(end))
            end = if_not_found_predecesor(end);



        if (begin<=end) search_by_range(result,root, begin, end);

    }

    ~AVLTree2() {
        if (this->root != nullptr) {
            this->root->killSelf();
        }
    }

private:
    string getInOrder(NodeT<TK, TV>* node);
    void insert(NodeT<TK, TV>*& node, Entry<TK, TV> entry);
    bool find(NodeT<TK, TV>* node, TV value);
    int height(NodeT<TK, TV>* node);
    bool isBalanced(NodeT<TK, TV>* node);
    Entry<TK, TV> minValue(NodeT<TK, TV>* node);
    Entry<TK, TV> maxValue(NodeT<TK, TV>* node);
    int size(NodeT<TK, TV>* node);
    void remove(NodeT<TK, TV>*& node, TK key, TV value);
    void displayPretty(NodeT<TK, TV>* node, int level);
    Entry<TK, TV> successor(NodeT<TK, TV>* node, Entry<TK, TV> entry);

    /*add for avl*/
    int balancingFactor(NodeT<TK, TV>* node);
    void updateHeight(NodeT<TK, TV>* node);
    void balance(NodeT<TK, TV>*& node);
    void left_rota(NodeT<TK, TV>*& node);
    void right_rota(NodeT<TK, TV>*& node);
    //vector<TK> search_by_range(vector <TK>& vec, NodeT<TK, TV>* node, TV begin, TV end);
    void search_by_range(CircularArray <TK>& vec, NodeT<TK, TV>* node, TV begin, TV end);
    Entry<TK, TV> if_not_found_succesor(NodeT<TK, TV>* node, Entry<TK, TV> entry);
    Entry<TK, TV> if_not_found_predecesor(NodeT<TK, TV>* node, Entry<TK, TV> entry);
};

template <typename TK, typename TV>
bool AVLTree2<TK, TV>::find(NodeT<TK, TV>* node, TV value) {
    if (node == nullptr)
        return false;
    else if (value < node->data.value)
        return find(node->left, value);
    else if (value > node->data.value)
        return find(node->right, value);
    else
        return true;
}

template <typename TK, typename TV>
bool AVLTree2<TK, TV>::isBalanced(NodeT<TK, TV>* node) //O(n^2)
{
    if (node == nullptr)
        return true;
    else
        return abs(height(node->left) - height(node->right)) <= 1 && isBalanced(node->left) && isBalanced(node->right);
}

template <typename TK, typename TV>
Entry<TK, TV> AVLTree2<TK, TV>::minValue(NodeT<TK, TV>* node) {
    if (node == nullptr)
        throw("The tree is empty");
    else if (node->left == nullptr)
        return node->data;
    else
        return minValue(node->left);
}

template <typename TK, typename TV>
Entry<TK, TV> AVLTree2<TK, TV>::maxValue(NodeT<TK, TV>* node) {
    if (node == nullptr)
        throw("The tree is empty");
    else if (node->right == nullptr)
        return node->data;
    else
        return maxValue(node->right);
}

template <typename TK, typename TV>
int AVLTree2<TK, TV>::size(NodeT<TK, TV>* node) {
    if (node == nullptr)
        return 0;
    else
        return 1 + size(node->left) + size(node->right);
}

template <typename TK, typename TV>
string AVLTree2<TK, TV>::getInOrder(NodeT<TK, TV>* node) {
    string result = "";
    if (node != nullptr) {
        result += getInOrder(node->left);
        result += to_string(node->data.key) + " - " + to_string(node->data.value) + ' ';
        result += getInOrder(node->right);
    }
    return result;
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::insert(NodeT<TK, TV>*& node, Entry<TK, TV> entry) {
    if (node == nullptr)
        node = new NodeT<TK, TV>(entry);
    else if (entry.value < node->data.value)
        insert(node->left, entry);
    else
        insert(node->right, entry);

    //regreso de la recursividad
    updateHeight(node);
    balance(node);
}

template <typename TK, typename TV>
int AVLTree2<TK, TV>::height(NodeT<TK, TV>* node) {
    if (node == nullptr) return -1;
    else return node->height;
}

template <typename TK, typename TV>
int AVLTree2<TK, TV>::balancingFactor(NodeT<TK, TV>* node) {
    return height(node->left) - height(node->right);
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::updateHeight(NodeT<TK, TV>* node) {
    node->height = max(height(node->left), height(node->right)) + 1;
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::balance(NodeT<TK, TV>*& node) {
    int bf = balancingFactor(node);
    if (bf >= 2) { //esta cargado a la izquierda
        if (balancingFactor(node->left) < 0) //verifica rotacion doble
            left_rota(node->left);
        right_rota(node);
    }
    else if (bf <= -2) { //esta cargado a la derecha
        if (balancingFactor(node->right) > 0) //verifica rotacion doble
            right_rota(node->right);
        left_rota(node);
    }
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::left_rota(NodeT<TK, TV>*& parent) {
    NodeT<TK, TV>* child = parent->right;
    parent->right = child->left;
    child->left = parent;
    //update height
    updateHeight(parent);
    updateHeight(child);
    parent = child;
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::right_rota(NodeT<TK, TV>*& parent) {
    NodeT<TK, TV>* child = parent->left;
    parent->left = child->right;
    child->right = parent;
    //update height
    updateHeight(parent);
    updateHeight(child);
    parent = child;
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::remove(NodeT<TK, TV>*& node, TK key, TV value) {
    if (node == nullptr)
        return;
    else if (value < node->data.value)
        remove(node->left, key,value);
    else if (value > node->data.value)
        remove(node->right, key,value);
    else if (key==node->data.key && value==node->data.value) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
            return;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            NodeT<TK, TV>* temp = node;
            node = node->left;
            delete temp;
            return;
        }
        else if (node->left == nullptr && node->right != nullptr) { //only right child
            NodeT<TK, TV>* temp = node;
            node = node->right;
            delete temp;
            return;
        }
        else { //two children
            Entry<TK, TV> succesor = minValue(node->right);
            node->data = succesor;
            remove(node->right, succesor.key,succesor.value);
        }

        updateHeight(node);
        balance(node);
    }
    else{
        remove(node->left, key,value);
        remove(node->right, key,value);
    }
}

template <typename TK, typename TV>
void AVLTree2<TK, TV>::displayPretty(NodeT<TK, TV>* node, int level) {
    if (node != nullptr) {
        displayPretty(node->right, level + 1);
        cout << endl;
        if (node == root)
            cout << "Root -> ";
        for (int i = 0; i < level && node != root; i++)
            cout << "        ";
        cout << node->data.key << '|' << node->data.value;
        displayPretty(node->left, level + 1);
    }
}

template <typename TK, typename TV>
Entry<TK, TV> AVLTree2<TK, TV>::successor(NodeT<TK, TV>* node, Entry<TK, TV> entry) {
    if (node == nullptr)
        return entry;
    if (entry.value < node->data.value) {
        entry = node->data;
        return successor(node->left, entry);
    }
    else
        return successor(node->right, entry);
}

template <typename TK, typename TV>
Entry<TK, TV> AVLTree2<TK, TV>::if_not_found_succesor(NodeT<TK, TV>* node, Entry<TK, TV> entry) {
    //stack<Entry<TK,TV>> mstack;
    ForwardList<Entry<TK,TV>> mstack;
    while(node!= nullptr){
        if(node->data.value > entry.value){
            mstack.push_front(node->data);
            node = node -> left;
        }
        else if(node->data.value < entry.value){
            node = node->right;
        }
    }
    return mstack.top();
}

template <typename TK, typename TV>
Entry<TK, TV> AVLTree2<TK, TV>::if_not_found_predecesor(NodeT<TK, TV>* node, Entry<TK, TV> entry) {
    //stack<Entry<TK,TV>> mstack;
    ForwardList<Entry<TK,TV>> mstack;
    while(node!= nullptr){
        if(node->data.value > entry.value){
            node = node -> left;
        }
        else if(node->data.value < entry.value){
            mstack.push_front(node->data);
            node = node->right;
        }
    }
    return mstack.top();
}

template <typename TK, typename TV>
//vector<TK> AVLTree2<TK, TV>::search_by_range(vector<TK>& vec, NodeT<TK, TV>* node, TV begin, TV end) {
void AVLTree2<TK, TV>::search_by_range(CircularArray<TK>& vec, NodeT<TK, TV>* node, TV begin, TV end) {
    if (node != nullptr) {
        if (node->data.value >= begin)
            search_by_range(vec,node->left, begin, end);
        if (node->data.value >= begin && node->data.value <= end){
            if (!vec.find(node->data.key))vec.push_back(node->data.key);
        }
        if (node->data.value <= end)
            search_by_range(vec,node->right, begin, end);
    }
    else{
        return;
    }
}

#endif
