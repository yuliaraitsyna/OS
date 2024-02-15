#include <windows.h>
#include <iostream>
#include <fstream>
#include "employee.h"
#include <conio.h>
#include <string>

void startProcess(char* buffer) {
    STARTUPINFO si; //creation of process
    ZeroMemory(&si, sizeof(STARTUPINFO)); //setting zero value
    si.cb = sizeof(si); //count bytes
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(NULL, buffer, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "\nERROR: Unable to open process\n";
        std::cout << "Press any key to exit...";
        system("pause");
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main() {
    char binaryFileName[256];
    std::cout << "Enter binary file name: ";
    std::cin.getline(binaryFileName, 256);
    if(binaryFileName == "") {
        std::cerr << "File name is empty" << std:: endl;
        return 1;
    }

    int recordsNum = 0;
    std::cout << "Enter the number of records: ";
    std::cin >> recordsNum;
    std::cin.ignore();
    if(recordsNum < 0) {
        std::cerr << "Records' number can't be negative" << std::endl;
        return 1;
    }

    char buffer[1000];
    sprintf_s(buffer, "%s %s %d", "creator.exe", binaryFileName, recordsNum);
    startProcess(buffer);

    char outputFileName[256];
    std::cout << "\n\nEnter output file name: ";
    std::cin.getline(outputFileName, 256);
    if(outputFileName == "") {
        std::cerr << "File name is empty" << std:: endl;
        return 1;
    }

    double hourSalary = 0.0;
    std::cout << "Enter salary value per hour: ";
    std::cin >> hourSalary;
    if(hourSalary < 0) {
        std::cerr << "Salary per hour can't be negative" << std::endl;
        return 1;
    }

    buffer[0] = '\0';
    sprintf_s(buffer, "%s %s %s %lf", "reporter.exe", binaryFileName, outputFileName, hourSalary);
    startProcess(buffer);

    std::ifstream finReport(outputFileName);

    std::cout << std::endl << "Output file info:" << std::endl;
    char info[1000];
    for (int i = 0; i < recordsNum; i++)
    {
        finReport.getline(info, 1000);
        for (int i = 0; info[i] != '\0'; i++)
            printf("%c", info[i]);
        std::cout << std::endl;
    }
    finReport.close();
    return 0;
}
