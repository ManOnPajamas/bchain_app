#ifndef NODE_H
#define NODE_H
#include <iostream>
using namespace std;

template <typename TK, typename TV>
struct Entry {
    TK key;
    TV value;

    Entry() {
        key = TK();
        value = TV();
    }

    Entry(TK k, TV v) {
        this->key = k;
        this->value = v;
    }

    bool same_key(Entry a) {
        return a.key == this->key;
    }

    friend ostream& operator<<(ostream& salida, Entry<TK, TV> e) {
        salida << e.key << " " << e.value;
        return salida;
    }
};

template <typename TK, typename TV>
struct NodeT { //
    Entry<TK, TV> data;
    NodeT* left;
    NodeT* right;
    int height;
    NodeT() : left(nullptr), right(nullptr), height(0) {}
    NodeT(Entry<TK, TV> value) : data(value), left(nullptr), right(nullptr), height(0) {}

    void killSelf() {
        if (left != nullptr) left->killSelf();
        if (right != nullptr) right->killSelf();
        delete this;
    }

};

#endif

/*
template <typename TK, typename TV>
struct Node{
    Entry<TK, TV>* data;
    Node<TK, TV>** children;
    int count;
    bool leaf;

    Node(): data(nullptr), children(nullptr), count(0), leaf(true){}

    Node(int M, bool _leaf = true){
        count = 0;
        leaf = _leaf;
        data = new Entry<TK, TV>[M-1];
        children = new Node<TK, TV>*[M];
        for(int i=0;i<M;i++) children[i] = nullptr;
    }

    void display(){
        for (int i = 0; i < count; ++i)
            cout<<data[i]<<", ";
        cout<<endl;
    }

    ~Node(){
        delete[] data;
        delete[] children;
    }
*/
