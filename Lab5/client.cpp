#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "employee.h"

const char* PIPE_NAME = "\\\\.\\pipe\\named_pipe";
const int PIPE_WAIT_DELAY = 200000;
const char* IP_NAME = "START_ALL";

void readRecord(HANDLE handlePipe) {
    DWORD bytesRead;
    employee emp;

    if (ReadFile(handlePipe, &emp, sizeof(emp), &bytesRead, NULL)) {
        if (bytesRead == 0) {
            std::cout << "No records available" << std::endl;
        } else {
            std::cout << "Employee ID: " << emp.num << std::endl;
            std::cout << "Employee Name: " << emp.name << std::endl;
            std::cout << "Employee Hours: " << emp.hours << std::endl;
        }
    } else {
        std::cout << "Error: failed to read file" << std::endl;
    }
}

void modifyRecord(HANDLE handlePipe) {
    employee emp;
    DWORD bytesWritten;
    int id;

    std::cout << "Input ID of employee to modify: ";
    std::cin >> id;

    if (!WriteFile(handlePipe, &id, sizeof(id), &bytesWritten, NULL)) {
        std::cout << "Error: failed to send ID to server" << std::endl;
        return;
    }

    if (ReadFile(handlePipe, &emp, sizeof(emp), NULL, NULL)) {
        std::cout << "Current record:" << std::endl;
        std::cout << "Employee ID: " << emp.num << std::endl;
        std::cout << "Employee Name: " << emp.name << std::endl;
        std::cout << "Employee Hours: " << emp.hours << std::endl;
    } else {
        std::cout << "Error: failed to read record from server" << std::endl;
        return;
    }

    std::cout << "Input new name of employee: ";
    std::cin >> emp.name;
    std::cout << "Input new working hours of employee: ";
    std::cin >> emp.hours;

    if (!WriteFile(handlePipe, &emp, sizeof(emp), &bytesWritten, NULL)) {
        std::cout << "Error: failed to send modified record to server" << std::endl;
        return;
    }

    std::cout << "Record was modified successfully!" << std::endl;
}


int main(int argc, char **argv) {

    HANDLE handlePipe;
    DWORD read;
    DWORD mode;
    DWORD bytesWritten;

    while (true) {
        WaitNamedPipeA(PIPE_NAME, INFINITE);
        handlePipe = CreateFileA(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (handlePipe != INVALID_HANDLE_VALUE)
            break;

        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::cout << "Could not open pipe" << std::endl;
            return 1;
        }

        if (!WaitNamedPipeA(PIPE_NAME, PIPE_WAIT_DELAY)) {
            std::cout << "Could not open pipe" << std::endl;
            return 1;
        }
    }

    mode = PIPE_READMODE_MESSAGE;
    bool pipeSuccess = SetNamedPipeHandleState(handlePipe, &mode, NULL, NULL);

    if (!pipeSuccess) {
        std::cout << "Error: failed to set named pipe handle state" << std::endl;
        return 1;
    }

    HANDLE handleReadyEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE handleStartEvent = OpenEventA(SYNCHRONIZE, FALSE, IP_NAME);

    if (!handleReadyEvent || !handleStartEvent) {
        std::cout << "Error: failed to create event" << std::endl;
        return 1;
    }

    SetEvent(handleReadyEvent);
    WaitForSingleObject(handleStartEvent, INFINITE);

    while (true) {
        std::cout << "1 - write the record" << std::endl;
        std::cout << "2 - read the record" << std::endl;
        std::cout << "3 - exit program" << std::endl;

        std::cout << "Choose option: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
                modifyRecord(handlePipe);
                break;
            case 2:
                readRecord(handlePipe);
                break;
            case 3:
                CloseHandle(handlePipe);
                return 0;
            default:
                std::cout << "Error: invalid choice" << std::endl;
                return 1;
        }
    }
}
