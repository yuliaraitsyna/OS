#include <iostream>
#include <fstream>
#include "employee.h"

int main(int argc, char* argv[]){

    std::ofstream fout(argv[1], std::ios::binary);

    int recordsNum = atoi(argv[2]);
    employee* records = new employee[recordsNum];

    std::cout << "Type information about " << recordsNum << " employees:" << std::endl;
    for (int i = 0; i < recordsNum; i++)
    {
        std::cout << "Employee " << (i + 1) << ": " << std::endl;
        std::cout << "Enter employee's number (ID): " << std::endl;
        std::cin >> records[i].num;
        if (records[i].num < 0) {
            std::cerr << "Employee's number can't be negative" << std::endl;
        }
        std:: cout << "Enter employee's name: " << std::endl;
        std::cin >> records[i].name;
        if (records[i].name == "") {
            std::cerr << "Employee's name is empty" << std::endl;
        }
        std::cout << "Enter employee's working hours: " << std::endl;
        std::cin >> records[i].hours;
        if (records[i].hours < 0) {
            std::cerr << "Number of working hours can't be negative" << std::endl;
        }
    }

    fout.write((char*)&recordsNum, sizeof(int));
    for (int i = 0; i < recordsNum; i++) {
        fout.write((char*)&records[i], sizeof(employee));
    }

    fout.close();

    delete[] records;

    return 0;
}
