
#ifndef CHAIN_HASH_H
#define CHAIN_HASH_H
#include <iostream>
#include <functional>
#include "forward_list.h"
using namespace std;

const float maxFillFactor = 0.4;
const int maxColision = 10;


template<typename TK, typename TV>
class ChainHash{
private:
    ForwardList<Entry2<TK,TV>>* array;//array de listas
    int capacity;//tamanio del array
    int size;//cantidad total de elementos
    hash<TK> hasher;//hash de string

public:
    ChainHash(int cap = 13){
        this->capacity = cap;
        this->size = 0;
        array = new ForwardList<Entry2<TK,TV>>[capacity]; //sese borra porque se tiene que generar con el fores list de l aclase sha
    }

    void insert(TK key, TV value){
        int index = hasher(key)%capacity;
        if (size*1.0/(maxColision*capacity)>=maxFillFactor) {
            //cout<<"rehashing\n";
            rehashing();}
        array[index].push_front(Entry2<TK,TV>(key,value));

        //array[index].display();
        size++;
    }


    int get_size(){
        return size;
    }

    bool find(TK key){ // busca si existe uno o mas elementos con ese key, en el chain
        int index = hasher(key)%capacity;
        return array[index].find(Entry2<TK,TV>(key,TV())); //ingresa al forwrd list.
    }

    //retorna el id value
    TV search(TK key) {
        if (!find(key)) {
            cout <<"Elemento no encontrado\n";
            return -1;
        }
        int index=hasher(key)%capacity;
        Entry2<TK,TV> entry (key,TV());
        return array[index].top(entry).value; //porque nosostros "si ingresamos un mismo key debe salir el mismo código"--->ESTABILIDAD
    }


    void display(){
        for(int i=0;i<bucket_count();i++){
            array[i].display();
        }
    }

    void remove(TK key, TV value){
        int index = hasher(key)%capacity;
        if (array[index].find(Entry2<TK,TV>(key,value))){
            array[index].remove(Entry2<TK,TV>(key,value));
        }
    }



    int bucket_count(){
        return capacity;
    }

    int bucket_size(int i){
        return array[i].get_size();
    }

    ~ChainHash(){

    }

private: // ver rehashing
    void rehashing(){
        int newCapacity = capacity*2;//se duplica el tamanio del array int newCpacity=capacity*2
        ForwardList<Entry2<TK,TV>>* newArray = new ForwardList<Entry2<TK,TV>>[newCapacity];//se crea un nuevo array con el doble  de tamañio
        for(int i=0; i<bucket_count();i++){ //
            for(int j=0; j<bucket_size(i);j++){
                TK key = array[i][j].key;
                TV value = array[i][j].value;
                int index = hasher(key)%newCapacity;
                newArray[index].push_front(Entry2<TK,TV>(key,value));
            }
        }

        delete [] array;
        this->capacity*=2;
        this->array = new ForwardList<Entry2<TK,TV>>[capacity];

        for(int i=0;i<bucket_count();i++){
            array[i] = newArray[i];
            //array[i].display();
        }
    }


};

#endif
