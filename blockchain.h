#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "Block.h"
#include "chainhash.h"
//#include "../structs/tabla_hash/chainhash.h"
//#include "../structs/avl/date.h"

template <class T>
class BlockChain{
    friend class Block<T>;
private:
    Block<T>* head;//sentinel
    int nodes;
    string* atributos;
    int values; // cantidad de atributos



public:
    BlockChain(int columnas=0){
        this->head=new Block<T>();//Node() es el sentinela
        this->head->next=this->head;
        this->head->prev=this->head;
        this->nodes=0;
        this->values = columnas;
    }

    BlockChain(string* _atributos, int _n){
        this->head=new Block<T>();//Node() es el sentinela
        this->head->next=this->head;
        this->head->prev=this->head;
        this->nodes=0;
        values = _n;
        atributos = new string[_n];
        for (int i = 0; i<values; i++) atributos[i] = _atributos[i];
    }

    void set_atributos(string* _atributos, int _n){
        values = _n;
        atributos = new string[_n];
        for (int i = 0; i<values; i++) atributos[i] = _atributos[i];
    }

    template <class... Typenames>
    void insert_string(int size,T data,Typenames... data_extra){ //al final recibe cantidad de valores
        T* result = new T[size];
        result[0] = data; // agregamos primer elemento
        insert_str(1,size,result, data_extra...);
    }

    template <class... Typenames>
    void insert_str(int indice,int size,T* data,T data1,Typenames... data_extra){ // recibe indice en el que se va a añadir
        data[indice] = data1; // agregamos valor en posicion especificada
        insert_str(indice+1,size,data, data_extra...);
    }

    void insert_str(int indice,int size,T* data, T data1){ // caso cuando solo falta añadir 1
        data[indice] = data1;
        insert(data,size);
    }

    void insert(T* data,int size){
        Block<T>* temp = new Block<T>(data,size,this->nodes);
        temp->next=this->head; //hacemos que el nodo se cuelgue
        temp->prev=this->head->prev; //hacemos que el nodo se cueelgue del prev del head
        this->head->prev->next=temp;//hacemos que el nodo se cuelgue siguiente y atrás
        this->head->prev=temp;
        //aumentamos el contador de nodos
        this->nodes++;

        //actualizar huella del padre
        temp->huella_padre = temp->prev->huella;

        SHA256 sha;
        //uint8_t * digest = sha.digest();

        //actualizando huella y nonce (huella: nro+data+huella_padre)
        //temp->nonce=findNonce((temp->huella+temp->huella_padre),temp->huella);
        temp->nonce=findNonce(temp->huella);
        //delete[] digest;
        temp->hasheado = true; // tiene su codigo hash correcto

    }

    int get_block(string*& result, int ind){
        Block<T>* temp = head->next;
        for (int i=0;i<ind;i++){
            temp = temp->next;
        }

        result = new string[temp->cant_data];
        for (int i=0; i<temp->cant_data; i++) result[i] = temp->data[i];
        return temp->cant_data;
    }

    void get_block(int ind){ // retorna un bloque de acuerdo a un indice que se pasa
        if (ind<0){return;}

        Block<T>* temp = head->next;
        for (int i=0;i<ind;i++){
            temp = temp->next;
        }
        temp->display_block(this->atributos,this->values);
        //int i = 0;
        /*
          temp->hasheado?cout<<"CORRECTO":cout<<"ERROR"; // para imprimir error
          cout<<temp->hasheado?"":"ERROR"; // para imprimir error
          cout<<"\n";
          for (int i=0; i<temp->cant_data; i++) {
            //cout<<temp->data[i]<<"\t";

              cout<<atributos[i%values]<<": "<<temp->data[i]<<"\t";


              if (i%values==values-1){ // ultimo elemento (solo para imprimir el salto de linea, pero ya se hace fuera del for)
                cout<<endl;
              }

          }
          cout<<"\n\tNonce: "<<temp->nonce;
          cout<<"\n\tHuella: "<<temp->huella;
          cout<<"\n\n";
          */
    }

    bool exist_block(int ind){
        return ind<nodes? true:false;
    }

    void print_huellas(){
        Block<T>* temp = head->next;
        while(temp!=head) {
            cout<<"nro: "<<temp->nro<<" - "<<temp->huella;
            cout<<" - Nonce: "<<temp->nonce<<endl;
            temp = temp->next;
        }
    }


    void display(){
        Block<T>* temp = head->next;
        while(temp!=head) {
            temp->display_block(this->atributos,this->values);
            cout<<setfill('-')<<setw(80)<<"";
            cout<<endl;
            temp = temp->next;
            /*
            cout<<"Bloque nro "<<temp->nro<<": \t\t\t";
            temp->hasheado?cout<<"CORRECTO":cout<<"ERROR"; // para imprimir error
            cout<<endl;
            for (int i=0;i<temp->cant_data;i++){//recorriendo cada atributo

              //cout<<temp->data[i]<<"\t"; // impresión simple

              //impresion con atributos
              cout<<atributos[i%values]<<": "<<temp->data[i]<<"\t";
              if (i%values==values-1){ // ultimo elemento
                cout<<endl;
              }

            }

            cout<<"Nonce: "<<temp->nonce<<"\t";
            cout<<"Huella: "<<temp->huella<<endl;
            cout<<" ---------------------------------- "<<endl;
            temp = temp->next;
            */
        }
    }

    void recalculo_cascada(){ // recalculo general
        recalculo_cascada(head->next);
    }

    void recalculo_cascada(Block<T>* temp){ // desde un bloque en especifico
        Block<T>* recorrido = temp;
        while (recorrido!=head){ //se recorre cada bloque
            if (!recorrido->hasheado){ // cuando el bloque fue modificado
                recorrido->huella_padre = recorrido->prev->huella; // huella del anterior
                recorrido->recalculate(); // metodo de clase Block
            }
            recorrido = recorrido->next;
            /*
            recorrido->huella_padre = recorrido->prev->huella; // huella del bloque anterior
            //temp->huella = temp->huella_padre;
            recorrido->recalculate(); // huella = nro + data // notar que huella_padre ya tiene su valor correcto
            recorrido = recorrido->next; //avanzamos a la sig posicion
            */
        }
    }

    bool proof_of_work(){ // verifica si se cumple el proof_of_work
        Block<T>* temp = head->next->next; // empezamos desde segundo
        while (temp!=head){
            if (temp->prev->huella!=temp->huella_padre) return false;
            temp = temp->next;
        }
        return true;
    }

    int get_size(){
        return this->nodes;
    }

    // para proof of work
    void modificar_bloque(int ind, string* _data, int values){
        if (ind>=nodes || ind<0) {cout<<"No existe este bloque"; return; }

        Block<T>* temp = head->next;

        for (int i=0; i<ind; i++){
            temp=temp->next;
        }

        temp->cant_data = values;

        string* t = temp->data;
        temp->data = new string [values]; delete [] t;



        std::string s = temp->nonce;  //es para verificar si la huella sigue siendo correcta (solo lo será se si modifica el bloquecon sus datos anteriores)

        s += to_string(temp->nro); // nro de bloque
        for (int i=0; i<values; i++){
            temp->data[i] = _data[i];
            s += _data[i];
        }
        s += temp->huella_padre; // añadimos la huella del padre

        //verificando si la huella sigue siendo correcta
        SHA256 sha;
        sha.update(s);
        uint8_t * digest = sha.digest();//obtiendo huella de bolque
        //cout<<"Huella: "<<sha.toString(digest)<<endl;
        if(SHA256::toString(digest) == temp->huella) {//si se sigue cumpliendo el proof of work
            delete [] digest;
            return ;
        }
        delete [] digest;
        while (temp!=head){//cuando no se cumple el proof of work
            temp->hasheado = false; // la huella debe cambiar
            temp = temp->next;
        }
        //temp->recalculate(); // recalculo del mismo bloque
        //recalculo_cascada(temp); // (otra opcion: para corregir en cascada ni bien se actualiza) NO BORRAR
    }
    ~BlockChain(){
        if(head) head->killSelf();
        delete [] atributos;
    }
};
#endif
