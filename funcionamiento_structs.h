
#include "patricia_trie.h"
#include "pedir_datos.h"
#include <unistd.h>
#include <stdio.h>
void console();
void readCSV(const string archivo);

const int columnas = 4;
string* atributos = new string [columnas]; // atributos de cada bloque
BlockChain<string>* cadena_bloques = new BlockChain<string>(); // blockchain

//arboles (2: no string<> date, int)
AVLTree2<int,int>* avl1 = new AVLTree2<int,int>(); // id monto
AVLTree2<int,long>* avl2 = new AVLTree2<int,long>(); //id fechas

//table_hash (4: todas las columnas)
ChainHash<string,int>* string1 = new ChainHash<string,int>(); //primer string
ChainHash<string,int>* string2 = new ChainHash<string,int>(); //segundo string
ChainHash<int,int>* numero = new ChainHash<int,int>(); //para monto id/monto
ChainHash<long,int>* fecha = new ChainHash<long,int>(); //para fecha/id>

//patricia (2: strings)
TriePatricia<int>* inicia_string1 = new TriePatricia<int>();
TriePatricia<int>* inicia_string2 = new TriePatricia<int>();

//boyer (2: strings)

//void begin(){
//    string archivo = "tests/test2.csv";
//    //readCSV(archivo,atributos,columnas,cadena_bloques,avl1,avl2, string1, string2, numero, fecha); // creando estructuras
//    readCSV(archivo);
//    console();
//}

//void readCSV(const string archivo){
//    std::ifstream file(archivo);
//    if (!file.is_open()) {
//        std::cout << "Failed to open the file." << std::endl;
//        return;
//    }
//
//    int count =0;
//    std::string line;
//    std::string item;
//    std::getline(file, line); // recibir atributos
//    std::stringstream ss(line);
//    for (int i=0;i<4;i++){
//        std::getline(ss, item, ','); // primer string
//        atributos[i] = item;
//    }
//    cadena_bloques->set_atributos(atributos,columnas);
//    string* values = new string[4];//vector <string> values;
//    while (std::getline(file, line)) {
//        int i=0;
//        std::stringstream ss(line);
//        std::getline(ss, item, ','); // primer string
//        string1->insert(item,count); // hash
//        //boyer
//        inicia_string1->insert(count,item); //patricia
//        values[i++] = item; //values.push_back(item);
//
//        std::getline(ss, item, ',');  // 2da columna
//        string2->insert(item,count); // hash
//        inicia_string2->insert(count,item); //patricia
//        values[i++] = item; //values.push_back(item);
//
//        std::getline(ss, item, ',');//3era columna: monto
//        int number=stoi(item);
//        avl1->insert(count,number); // key: nro bloque  //  value:monto -- lo ordena por monto(por value)
//        numero->insert(number,count);
//        values[i++] = item; //values.push_back(item);
//
//        std::getline(ss, item, ','); //4ta columna: fecha
//        time_t date = convertToUnixTimestamp(item);
//        avl2->insert(count,date); // key: nro bloque   //  value:fecha -- lo ordena por fecha(por value)
//        fecha->insert(date,count);
//        values[i++] = item; //values.push_back(item);
//
//        count ++; // nro de bloque aumenta
//        cadena_bloques->insert(values,i); // VERIFICAR
//    }
//
//    file.close();
//}
//


void agregar_registro(){
    int size_data =4 ;
    string* data = new string[size_data]; int ind=0;//vector <string> data;
    int opcion = 1;
    string s1,s2;
    int s3;
    string s4;
    int size = cadena_bloques->get_size();
    do {
        if (ind == size_data) { //resize
            string* copia = new string[size_data];
            for (int i = 0; i < size_data; i++)
                copia[i] = data[i];

            delete[] data; // Liberar la memoria del arreglo original

            data = new string[size_data * 2];
            for (int i = 0; i < size_data; i++)
                data[i] = copia[i];

            size_data *= 2;
            delete[] copia;
        }


        s1 = pedir_string(atributos[0]); // string1
        string1->insert(s1,size);
        inicia_string1->insert(size,s1);

        s2 = pedir_string(atributos[1]);  //string2
        string2->insert(s2,size);
        inicia_string2->insert(size,s2);

        s3 = pedir_entero(atributos[2]); // entero(monto)
        numero->insert(s3,size);
        avl1->insert(size,s3);

        s4 = fecha_string(); // fecha
        time_t date = convertToUnixTimestamp(s4);
        fecha->insert(date,size);
        avl2->insert(size,date);

        data[ind++] = s1; //data.push_back(s1);
        data[ind++] = s2; //data.push_back(s2);
        data[ind++] = to_string(s3); //data.push_back(to_string(s3));
        data[ind++] = s4; //data.push_back(s4);

        cout<<"\n\tDesea agregar otro registro? ";
        opcion = pedir_entero("opcion: 1. Si \t 2. No \t");
    } while (opcion==1);
    cadena_bloques->insert(data,ind); // insertamos el dato en blockchain (4 por el nro de columnas)
}

string* nuevos_datos(int& cant_datos, int nro_block){
    string* datos;
    int size_datos = cadena_bloques->get_block(datos,nro_block); // recibe todos los datos

    int ind = -1; // para saber qué dato vamos a eliminar por iteracion (en el caso de +1 fila)

    while (size_datos>0){// eliminamos todo lo relacionado con el anterior bloque

        // eliminamos clientes
        string1->remove(datos[++ind],nro_block); // eliminacion en hash
        cout<<datos[ind]<<endl;
        cout<<"cadenas antes de eliminar: "<<inicia_string1->toString(" ")<<endl;
        inicia_string1->remove(datos[ind]); // eliminacion en patricia
        cout<<"cadenas luego de eliminar: "<<inicia_string1->toString(" ")<<endl;
        //cout<<ind; // ind = 0, 4, 8 , ...

        // eliminamos lugar
        string2->remove(datos[++ind],nro_block); // eliminacion en hash
        inicia_string2->remove(datos[ind]); // eliminacion en patricia
        //cout<<ind; // ind = 1, 5, 9, ...

        // eliminamos monto // ind = 2, 6, 10,...
        numero->remove(stoi(datos[++ind]),nro_block); // eliminacion en hash
        avl1->remove(nro_block,stoi(datos[ind])); // eliminacion en avl
        //cout<<ind;  // ind = 1, 5, 9, ...

        // eliminamos fecha
        fecha->remove(convertToUnixTimestamp(datos[++ind]),nro_block);//eliminacion en hash
        avl2->remove(nro_block,convertToUnixTimestamp(datos[ind])); // eliminacion en avl
        //cout<<ind; // ind = 3, 7, 11,...

        size_datos-=columnas; // eliminamos un registro por iteracion
    }

    // para pedir datos
    int size_data = 4;
    string* data = new string[size_data];
    int opcion = 1;
    string s1,s2;
    int s3;
    string s4;
    int size = cadena_bloques->get_size();
    do {
        if (cant_datos == size_data) { //resize
            string* copia = new string[size_data];
            for (int i = 0; i < size_data; i++)
                copia[i] = data[i];

            delete[] data; // Liberar la memoria del arreglo original

            data = new string[size_data * 2];
            for (int i = 0; i < size_data; i++)
                data[i] = copia[i];

            size_data *= 2;
            delete[] copia;
        }


        s1 = pedir_string(atributos[0]);
        string1->insert(s1,nro_block);
        inicia_string1->insert(nro_block,s1);

        s2 = pedir_string(atributos[1]); string2->insert(s2,nro_block);
        string1->insert(s1,nro_block);
        inicia_string2->insert(nro_block,s2);

        s3 = pedir_entero(atributos[2]);
        numero->insert(s3,nro_block);
        avl1->insert(nro_block,s3);
        s4 = fecha_string();

        time_t date = convertToUnixTimestamp(s4); fecha->insert(date,nro_block); avl2->insert(nro_block,date);

        data[cant_datos++] = s1; //data.push_back(s1);
        data[cant_datos++] = s2; //data.push_back(s2);
        data[cant_datos++] = to_string(s3); //data.push_back(to_string(s3));
        data[cant_datos++] = s4; //data.push_back(s4);

        cout<<"\n¿Desea agregar otro registro? ";
            opcion = pedir_entero("opcion: 1. Si \t 2. No \t");
    } while (opcion==1);
    return data;
}

int pedir_opcion(){
    int alternativa;
    do{
        cout<<"[1]. agregar registro"<<endl;
        cout<<"[2]. buscar por "<<atributos[0]<<endl; // hash
        cout<<"[3]. buscar por "<<atributos[1]<<endl;
        cout<<"[4]. buscar por "<<atributos[2]<<endl;
        cout<<"[5]. buscar por "<<atributos[3]<<endl;
        cout<<"[6]. buscar por rango en "<<atributos[2]<<endl; // avl
        cout<<"[7]. buscar por rango en "<<atributos[3]<<endl;
        cout<<"[8]. buscar minimo por "<<atributos[2]<<endl; // avl
        cout<<"[9]. buscar minimo por "<<atributos[3]<<endl;
        cout<<"[10]. buscar maximo por "<<atributos[2]<<endl; // avl
        cout<<"[11]. buscar maximo por "<<atributos[3]<<endl;
        cout<<"[12]. "<< atributos[0] << " inicia con "<<endl; // patricia
        cout<<"[13]. "<< atributos[1] << " inicia con "<<endl;
        cout<<"[14]. buscar patron en "<<atributos[0] <<endl; // boyer
        cout<<"[15]. buscar patron en "<<atributos[1] <<endl;
        cout<<"[16]. mostrar todos los bloques"<<endl;
        cout<<"[17]. recalculo en cascada"<<endl;
        cout<<"[18]. Modificar bloque"<<endl;
        cout<<"[19]. Salir"<<endl;
        cin>>alternativa;
    }while (alternativa>0 && 19<alternativa); // mientras no se ingrese opcion correcta
    return alternativa;
}

void console(){
    int opc = 0; //para la opcion
    do {
        int opc = pedir_opcion();

        //vector<int> res;
        CircularArray<int> res;
        int n1,n2;
        time_t c1, c2;
        int cant_datos = 0;
        string* new_data = new string[4];
        string data;

        switch (opc){
        case 1:
            agregar_registro(); break;
        case 2:
            cadena_bloques->get_block(string1->search(pedir_string(atributos[0]))); break;
        case 3:
            cadena_bloques->get_block(string2->search(pedir_string(atributos[1]))); break;
        case 4:
            cadena_bloques->get_block(numero->search(pedir_entero(atributos[2]))); break;
        case 5:
            //c1 = pedir_fecha();
            //cout<<"Fecha: "<<c1<<endl;
            cadena_bloques->get_block(fecha->search(pedir_fecha()));
            break;

        case 6:
            n1 = pedir_entero(atributos[2]+" 1");
            n2 = pedir_entero(atributos[2]+" 2");

            avl1->search_by_range(res,n1,n2);

            //cout<<"\tBloques: \n"; for (auto i:res) cadena_bloques->get_block(i); // cout<<i<<" ";
            for (int i=0; i<int(res.size()); i++){
                cadena_bloques->get_block(res[i]);
            }

            break;

        case 7:
            c1 = pedir_fecha();
            c2 = pedir_fecha();
            avl2->search_by_range(res,c1,c2);

            for (int i=0; i<int(res.size()); i++){
                cadena_bloques->get_block(res[i]);
            }

            break;

        case 8:
            cadena_bloques->get_block(avl1->minValue()); break;

        case 9:
            cadena_bloques->get_block(avl2->minValue()); break;

        case 10:
            cadena_bloques->get_block(avl1->maxValue()); break;

        case 11:
            cadena_bloques->get_block(avl2->maxValue()); break;

        case 12:
            data = pedir_string("cadena");

            //cambiamos a minuscula
            for (int i=0; i<int(data.size()); i++) data[i] = tolower(data[i]);

            inicia_string1->start_with(res, data);

            if (int(res.size())==0){
                cout<<"No existen cadenas que inicien con "<<data<<endl;
            }
            else{
                for (int i=0; i<int(res.size()); i++){
                    cadena_bloques->get_block(res[i]);
                }
            }
            //cout<<"cadenas: "<<inicia_string1->toString(" ");
            break;

        case 13:
            data = pedir_string("cadena");

            for (int i=0; i<int(data.size()); i++) data[i] = tolower(data[i]);

            inicia_string2->start_with(res, data);

            if (int(res.size())==0){
                cout<<"No existen cadenas que inicien con "<<data<<endl;
            }
            else{
                for (int i=0; i<int(res.size()); i++){
                    cadena_bloques->get_block(res[i]);
                }
            }

            break;
        case 14:
            cout<<"boyer 1"; break;
            //llamando al algoritmo de boyer moorey que devuelve un vector con los indices de los bloques que contienen el patron
            //res = testBoyerMoore(cadena_bloques->getInOrder(),pedir_string("patron"));
            //cout<<"\tBloques: \n"; for (auto i:res) cadena_bloques->get_block(i); // cout<<i<<" ";

        case 15:
            cout<<"boyer 2"; break;

        case 16:
            cadena_bloques->display(); break;

        case 17:
            cadena_bloques->recalculo_cascada(); break;

        case 18:
            cout<<"Inserte nro de bloque a actualizar: ";
            cin>>n1;

            if (cadena_bloques->exist_block(n1)){
                //avl1->remove(n1);
                //avl2->remove(n1);
                cant_datos = 0;
                new_data = nuevos_datos(cant_datos,n1);
                cadena_bloques->modificar_bloque(n1, new_data,cant_datos);

                delete [] new_data; cant_datos=0; new_data = nullptr;
            }
            else cout<<"Bloque no existe\n";

            break;


        default:
            cout<<"Programa finalizado"<<endl;
            break;

            res.clear();
        }
        if (opc==19) { break;}
        sleep(3);
    }while (opc!=19);

}

