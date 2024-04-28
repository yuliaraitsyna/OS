#include <iostream>

struct employee {
    int num;
    char name[10];
    double hours;
    void print() {
        std::cout << "ID: " 
                  << num 
                  << "\tName: " 
                  << name 
                  << "\tHours: " 
                  << hours 
                  << '\n';
    }
};