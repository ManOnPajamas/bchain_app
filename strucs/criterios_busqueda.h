
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "blockchain.h"
#include "chainhash.h"
#include "date.h"
#include "boyer_moree.h"
#include "patricia_trie.h"
#include "avl.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QDebug>
using namespace std;
void readCSV(const string archivo, string* atributos,int col, BlockChain<string>* cadena_bloques, AVLTree2<int,int>* avl1, AVLTree2<int,long>* avl2, ChainHash<string,int>* string1, ChainHash<string,int>* string2, ChainHash<int,int>* numero, ChainHash<long,int>* fecha );

template <class TK, class TV>
TV search(ChainHash<TK,TV>*, TK);

template <class TK,class TV>
TK max_value(AVLTree2<TK,TV>* arbol);

template <class TK,class TV>
TK min_value(AVLTree2<TK,TV>* arbol);

template <class TK,class TV>
vector <TK> range_search(AVLTree2<TK,TV>* arbol, TV begin, TV end);


void readCSV(const string archivo, string* atributos,int col, BlockChain<string>* cadena_bloques, AVLTree2<int,int>* avl1, AVLTree2<int,long>* avl2, ChainHash<string,int>* string1, ChainHash<string,int>* string2, ChainHash<int,int>* numero, ChainHash<long,int>* fecha){

}


template <class TK, class TV>
TV search(ChainHash<TK,TV>* tabla, TK key) {
    return tabla->search(key);
}

template <class TK,class TV>
vector <TK> range_search(AVLTree2<TK,TV>* arbol, TV begin, TV end){
    return arbol->search_by_range(begin,end);
}


template <class TK,class TV>
TK max_value(AVLTree2<TK,TV>* arbol){
    return arbol->maxValue().key;
}

template <class TK,class TV>
TK min_value(AVLTree2<TK,TV>* arbol){
    return arbol->minValue().key;
}


//llamando a alk funcion del boyer moore
vector<int> boyer_moore(string Texto, string Patron){
    return testBoyerMoore(Texto,Patron);
}
