#include "criterios_busqueda.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
using namespace std;

//time_t convertToUnixTimestamp(const std::string& date);
time_t pedir_fecha();
string fecha_string();
string pedir_string(string atributo);
int pedir_entero(string atributo);
/*
time_t convertToUnixTimestamp(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, '%Y-%m-%d');

    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return std::chrono::system_clock::to_time_t(tp);
}
*/
time_t pedir_fecha(){
    string dia, mes, anho;
    //cout<<'Ingrese fecha-> '; cout<<'Dia: '; cin>>dia;
    //cout<<'Mes: '; cin>>mes;
    //cout<<'Anho: '; cin>>anho;
    return convertToUnixTimestamp(anho+'-'+mes+'-'+dia);
}

string fecha_string(){
    string dia, mes, anho;
    //cout<<'Ingrese fecha-> '; cout<<'Dia: '; cin>>dia;
    if (dia.length()==1) dia = '0'+dia;

    //cout<<'Mes: '; cin>>mes;
    if (mes.length()==1) mes = '0'+mes;

    //cout<<'Anho: '; cin>>anho;
    return anho+'-'+mes+'-'+dia;
}

string pedir_string(string atributo){
    //cout<<'Ingrese '<<atributo<<': ';
    string atr; cin>>atr;
    atr[0] = toupper(atr[0]);
    return atr;
}

int pedir_entero(string atributo){
    //cout<<'Ingrese '<<atributo<<': '; int num; cin>>num; return num;
}
