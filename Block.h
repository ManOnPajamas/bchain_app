#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <string>
#include <iomanip>
//#include <vector>
#include "SHA256.h"
using namespace std;


string findNonce(string& huella){
    uint32_t nonce = 0;
    std::string targetPrefix = "0000";
    string data = huella;

    while (true) {
        SHA256 sha;
        sha.update(std::to_string(nonce) + data);
        uint8_t * digestResult = sha.digest();
        std::string hash = SHA256::toString(digestResult);

        if (hash.substr(0, 4) == targetPrefix) {
            //std::cout<<toString(digestResult);
            huella = SHA256::toString(digestResult);
            //std::cout<<"Huella todo: "<<std::to_string(nonce)+data<<std::endl;
            //std::cout<<"Huella: "<<huella<<std::endl<<std::endl;
            //cout<<"Nonce: "<<nonce<<endl;
            return to_string(nonce);
        }

        delete[] digestResult;

        nonce++;
    }
}


template <class T>
struct Block
{
    //vector <T> data; // SHA256 solo funciona con strings
    T* data;
    int cant_data; // nro de valores por bloque
    int nro;
    string huella; // hash(huellapadre+datos) = huella
    string huella_padre;
    string nonce;
    Block* next;
    Block* prev;
    bool hasheado; // atributo para saber si está bien hasheado

public:
    void killSelf(){
        delete next;
        delete prev;
    }

    Block(){
        huella_padre = huella = nonce = "";
    }

    Block(T* _data,int size_data,int _nro_bloque) {
        nro = _nro_bloque;
        nonce = "";
        next = prev = nullptr;

        //para valores que almacenará el bloque
        cant_data = size_data;
        data = new T[cant_data];

        huella = to_string(nro);

        for (int i=0; i<cant_data; i++) {
            data[i] = _data[i];
            huella += _data[i];
        } //huella tiene valor concatenado de todos los elementos (orden: nro, data)
        huella_padre="";
    }

    void recalculate(){
        huella = "";
        huella = to_string(nro); // nro de bloque

        for (int i=0; i<cant_data; i++){
            huella += data[i];
        }


        //(huella: nro+data+huella_padre)

        huella += huella_padre;
        //actualizando huella y nonce
        //nonce=findNonce((huella+huella_padre),huella); //aquí se envia huella del padre
        //delete[] digest;
        nonce=findNonce(huella);
        this->hasheado = true; //el codigo hash es correcto

    }

    void print_huella(){
        for (int i=0; i<cant_data; i++) cout<<data[i]<<" ";
        cout<<" Huella: "<<huella<<endl;
    }

    T* get_data(){
        return data;
    }

    void display_block(string* atributos, int values){
        cout<<"Bloque nro "<<this->nro<<": \t\t\t\t";
        this->hasheado?cout<<"CORRECTO":cout<<"ERROR"; // para imprimir error
        cout<<endl;
        for (int i=0;i<this->cant_data;i++){//recorriendo cada atributo
            //impresion con atributos:
            //       atributo           :     valor
            //cout<<atributos[i%values]<<": "<<data[i];
            cout<<"| "<<atributos[i%values]<<": "<<data[i]<<"\t";
            if (i%values==values-1)cout<<"|"<<endl; // ultimo elemento (por registro)
        }
        cout<<"Nonce: "<<this->nonce<<"\t";
        cout<<"Huella: "<<this->huella<<endl<<endl;
    }

    bool verificar_hash(){
        // hash = nonce + nro + data + huella_padre
        std::string s = this->nonce;  //es para verificar si la huella sigue siendo correcta (solo lo será se si modifica el bloquecon sus datos anteriores)
        s += to_string(this->nro); // nro de bloque

        for (int i=0; i<this->cant_data;i++) s+= this->data[i];
        s += this->huella_padre; // añadimos la huella del padre

        SHA256 sha;
        sha.update(s);
        uint8_t * digest = sha.digest();//obtiendo huella de bolque
        //cout<<"Huella: "<<sha.toString(digest)<<endl;
        /*
        if(SHA256::toString(digest) == this->huella) {//si se sigue cumpliendo el proof of work
            delete [] digest;
            return ;
        }
        */
        string result = SHA256::toString(digest);
        delete [] digest;
        return  result == this->huella;
    }

};

#endif
