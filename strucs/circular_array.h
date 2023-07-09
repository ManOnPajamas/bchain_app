
#ifndef CIRCULAR_ARRAY_H
#define CIRCULAR_ARRAY_H

#include <iostream>
using namespace std;

template <class T>//clase generica, se puede usar cualquier tipo de dato
class CircularArray{ //clase circular array
private:
    T *array; //array de tipo T (puede ser cualquier tipo de dato) , el array es un puntero que apunta a la direccion de memoria del primer elemento porque es un array dinamico
    int capacity;
    int back, front;

public:
    CircularArray(int _capacity = 10);
    virtual ~CircularArray(){
        delete array;
    }
    void push_front(T data){
        if (is_full()){ //en caso esté lleno el array
            resize();
        }

        if (back==-1) back =0;//cuando se ingresa el primer elemento
        front = prev(front); //retrocede una posicion

        array[front] = data; //se agrega el elemento

    }

    void push_back(T data){
        if (is_full()){
            resize();
        }

        if (front==-1) front=0; //en caso sea el primer elemento
        back = next(back);
        array[back] = data;

    }

    void insert(T data, int pos){
        if (is_full()){
            resize();//en caso esté lleno el array
        }

        //creamos un nuevo array
        T *temp = new T[capacity];

        for (int i =0;i<capacity;i++){
            if (i==pos) temp[i]=data;
            else if(i<pos){
                temp[i]=array[front+i];
            }
            else {
                temp[i]=array[front+i-1];
            }
        }

        delete [] array;

        array = temp;
        front = 0;
        back=size();//avanzamos una posicion

    }

    bool find(T data){ // determina si ya se encuentra el elemento o no
        int ind = front;

        for (int i=0; i<size(); i++){
            if(array[ind]==data) return true;
            ind = next(ind);
        }
        return false;
    }

    T pop_front(){
        /*si array está vacío y yo le envio un string
        if (is_empty()) return "error";

        */
        if (is_empty()) throw "error";

        T temp=array[front];
        front = next(front);
        return temp;

    }

    T pop_back(){
        //si el array está vacío, creamos una excepción
        if (is_empty()) throw "error";
        T temp=array[back];
        back = prev(back);
        return temp;
    }

    bool is_full(){
        if (size()==capacity) return true;
        return false;
    }

    bool is_empty(){ //
        if (back==front) return true;
        return false;
    }

    int size(){
        if (back==front && front==-1) return 0;
        else if (front>back) return (back+1) + (capacity-front);
        else return  back - front + 1;
    }

    void clear(){
        front = back = -1;
    }

    T& operator[](int val){//Los corchetes en general se aplican a los objetos.
        return array[(front+ val)%capacity];
    }




    //es para imprimir el array con un separador O(n)
    string to_string(string sep=" "){
        //como convertir cualquier tipo de dato a string, en este caso T a string
        string temp="";
        int index=front;
        for (int i=0;i<size();i++) {
            temp += std::to_string(array[index]) + sep;
            index = next(index);
        }
        return temp;
    }



private:
    void resize(){
        T *temp = new int [capacity +1]; //nuevo array con un espacio más
        int index_temp=front;
        for (int i=0;i<size();i++) {
            temp[i] = array [index_temp];
            index_temp = next (index_temp);
        }
        capacity++;

        back = size()-1;

        delete [] array;

        array = temp;
    }


    int next(int val){
        val = (val+1) % capacity;
        return val;
    }

    int prev(int val){
        val = (val==0)? capacity-1 : val-1;
        return val;
    }
};

template <class T>
CircularArray<T>::CircularArray(int _capacity)
{
    this->capacity = _capacity;
    this->array = new T[_capacity];
    this->front = this->back = -1;//empty
}

#endif
