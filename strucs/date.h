#include <iostream>
#include <ctime>
using namespace std;
#include <chrono>
#include <sstream>
#include <iomanip>

time_t convertToUnixTimestamp(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return std::chrono::system_clock::to_time_t(tp);
}

/*
int main() {

    AVLTree2<int,int> avl;
    avl.insert(1, 400);
    avl.insert(2, 300);
    avl.insert(3, 150);
    avl.insert(4, 50);
    avl.insert(5, 250);
    avl.insert(6, 500);
    avl.displayPretty();
    cout << endl;
    cout << avl.getInOrder() << endl;
    cout << avl.search_by_range(56,367) << endl;


    time_t fecha = convertToUnixTimestamp("2003-12-21");
    time_t fecha2 = convertToUnixTimestamp("2003-09-15");
    time_t fecha3 = convertToUnixTimestamp("2022-11-10");
    time_t fecha4 = convertToUnixTimestamp("2016-05-04");
    time_t fecha5 = convertToUnixTimestamp("2008-07-07");

    AVLTree2<int,time_t> avl2;
    avl2.insert(1, fecha);
    avl2.insert(2, fecha2);
    avl2.insert(3, fecha3);
    avl2.insert(4, fecha4);
    avl2.insert(5, fecha5);
    cout << avl2.getInOrder() << endl;
    cout << avl2.search_by_range(1070000000,1462338070);

}
*/
