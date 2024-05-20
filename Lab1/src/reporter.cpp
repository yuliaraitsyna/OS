#include <fstream>
#include "employee.h"

int main(int argc, char *argv[])
{
    std::ifstream fin(argv[1], std::ios::binary);
    std::ofstream fout(argv[2]);
    double hourSalary = atof(argv[3]);

    int recordsNum;
    fin.read((char *)&recordsNum, sizeof(int));
    employee *records = new employee[recordsNum];

    for (int i = 0; i < recordsNum; i++)
    {
        fin.read((char *)&records[i], sizeof(employee));
    }

    for (int i = 0; i < recordsNum; i++)
    {
        fout << "Number: " << records[i].num << " Name: " << records[i].name << " Hours: " << records[i].hours << " Total salary: " << records[i].hours * hourSalary << std::endl;
    }
    fin.close();
    fout.close();

    delete[] records;

    return 0;
}
