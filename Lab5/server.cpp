#include <process.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <conio.h>
#include <algorithm>
#include <string>
#include "employee.h"

int employeesCount;
employee *employees;
HANDLE *handleReadyEvents;
CRITICAL_SECTION employeesCriticalSection;
bool *employeesIsModifying;
int employeeSize = sizeof(employee);

const std::string PIPE_NAME = "\\\\.\\pipe\\pipe_name";
const int BUFF_LENGTH = 10;
const int MESSAGE_LENGTH = 10;
char buff[BUFF_LENGTH];

int empCmp(const void* p1, const void* p2) {
    return ((employee*)p1)->num - ((employee*)p2)->num;
}

int generateClientCount() {
    srand(time(0));
    return (rand() / 5 + 3) % 5 + 2;
}

employee *findEmployee(const int ID) {
    employee key;
    key.num = ID;
    return (employee *) bsearch(&key, employees, employeesCount, employeeSize, empCmp);
}

void sortEmployees() {
    qsort(employees, employeesCount, employeeSize, empCmp);
}

void writeDataToFile(std::string fileName) {
    std::fstream file(fileName.c_str(), std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char *>(employees), employeeSize * employeesCount);

    std::cout << "Data has been written.\n";
    file.close();
}

void readDataFromSTD() {
    employees = new employee[employeesCount];

    std::cout << "Input ID, name and working hours of each employee: ";
    for (int i = 0; i < employeesCount; ++i) {
        std::cout << "Employee " << i + 1 << ": ";
        std::cin >> employees[i].num >> employees[i].name >> employees[i].hours;
    }
}

void startProcesses(const int COUNT) {
    for (int i = 0; i < COUNT; ++i) {
        std::string cmdArgs = "client.exe ";
        std::string eventName = "READY_EVENT_";
        itoa(i + 1, buff, BUFF_LENGTH);

        eventName += buff;
        cmdArgs += eventName;

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        si.cb = sizeof(STARTUPINFO);
        ZeroMemory(&si, sizeof(STARTUPINFO));

        handleReadyEvents[i] = CreateEvent(NULL, TRUE,
                                           FALSE, eventName.c_str());

        char tempArg[60] = "";
        strcat(tempArg, cmdArgs.c_str());

        if (!CreateProcess(NULL, tempArg,
                           NULL, NULL,
                           FALSE, CREATE_NEW_CONSOLE,
                           NULL, NULL,
                           &si, &pi)) {
            std::cout << "Creation process error.\n";

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

DWORD WINAPI handleMessages(LPVOID p) {
employee *errorEmp = new employee;
errorEmp->num = -1;

HANDLE hPipe = (HANDLE) p;
while (true) {
DWORD readBytes;
char message[MESSAGE_LENGTH];

bool isRead = ReadFile(hPipe, message, MESSAGE_LENGTH, &readBytes, NULL);

if (!isRead) {
if (ERROR_BROKEN_PIPE == GetLastError()) {
std::cout << "Client disconnected." << std::endl;
} else {
std::cerr << "Error in reading a message." << std::endl;
}
break;
}

if (strlen(message) != 0) {
char command = message[0];
message[0] = ' ';
int id = atoi(message);

DWORD bytesWritten;
EnterCriticalSection(&employeesCriticalSection);
employee *empToSend = findEmployee(id);
LeaveCriticalSection(&employeesCriticalSection);

if (NULL == empToSend) {
empToSend = errorEmp;
} else {
int ind = empToSend - employees;

if (employeesIsModifying[ind]) {
empToSend = errorEmp;
} else {
if (command == 'r') {
std::cout << "Requested to read ID " << id << ".";
} else if (command == 'w') {
std::cout << "Requested to modify ID " << id << ".";
employeesIsModifying[ind] = true;
} else {
std::cout << "Unknown request. ";
}
}
}

bool isSent = WriteFile(hPipe, empToSend, employeeSize, &bytesWritten, NULL);
if (isSent) {
std::cout << "Answer is sent.\n";
} else {
std::cout << "Error in sending answer.\n";
}

if (empToSend != errorEmp && 'w' == command) {
isRead = ReadFile(hPipe, empToSend, employeeSize, &readBytes, NULL);
if (!isRead) {
std::cerr << "Error in reading a message." << std::endl;
break;
} else {
std::cout << "Employee record changed.\n";

employeesIsModifying[empToSend - employees] = false;
EnterCriticalSection(&employeesCriticalSection);
sortEmployees();
LeaveCriticalSection(&employeesCriticalSection);
}
}
}
}

FlushFileBuffers(hPipe);
DisconnectNamedPipe(hPipe);
CloseHandle(hPipe);
delete errorEmp;
return 0;
}

void openPipes(int clientCount) {
    HANDLE hPipe;
    HANDLE * hThreads = new HANDLE[clientCount];
    for (int i = 0; i < clientCount; ++i) {
        hPipe = CreateNamedPipe(PIPE_NAME.c_str(), PIPE_ACCESS_DUPLEX,
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);
        if (!ConnectNamedPipe(hPipe, NULL)) {
            std::cout << "No connected clients.\n";
            break;
        }
        if (INVALID_HANDLE_VALUE == hPipe) {
            std::cerr << "Create named pipe failed.\n";
            getch();
            return;
        }
        hThreads[i] = CreateThread(NULL, 0, handleMessages, (LPVOID) hPipe, 0, NULL);
    }
    std::cout << "Clients connected to pipe.\n";
    WaitForMultipleObjects(clientCount, hThreads, TRUE, INFINITE);
    std::cout << "All clients are disconnected.\n";
    delete[] hThreads;
}

int main() {
    std::string filename;
    std::cout << "Input name of file: ";
    std::cin >> filename;
    std::cout << "Input count of employees: ";
    std::cin >> employeesCount;

    readDataFromSTD();
    writeDataToFile(filename);
    sortEmployees();

    int countOfClient = generateClientCount();
    employeesIsModifying = new bool[employeesCount];

    for (int i = 0; i < employeesCount; ++i) {
        employeesIsModifying[i] = false;
    }

    InitializeCriticalSection(&employeesCriticalSection);
    HANDLE handleStartAll = CreateEvent(NULL, TRUE, FALSE, "START_ALL");

    handleReadyEvents = new HANDLE[countOfClient];
    startProcesses(countOfClient);

    WaitForMultipleObjects(countOfClient, handleReadyEvents, TRUE, INFINITE);
    std::cout << "All processes are ready. Starting.\n";

    SetEvent(handleStartAll);

    openPipes(countOfClient);

    for (int i = 0; i < employeesCount; ++i) {
        employees[i].print();
    }

    std::cout << "Press any key to exit...\n";
    getch();
    DeleteCriticalSection(&employeesCriticalSection);

    delete[] employeesIsModifying;
    delete[] handleReadyEvents;
    delete[] employees;

    return 0;
}