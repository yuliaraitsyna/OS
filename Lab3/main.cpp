#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_THREADS = 10;
const int MAX_ARRAY_SIZE = 100;

int* arr;
int arraySize;
HANDLE mutex;
HANDLE main_cond;
HANDLE marker_cond[MAX_THREADS];

struct MarkerThreadInfo {
    int threadID;
    bool marked;
};

DWORD WINAPI markerThread(LPVOID arg) {
    MarkerThreadInfo* info = (MarkerThreadInfo*)arg;
    WaitForSingleObject(main_cond, INFINITE);
    srand(GetCurrentThreadId() + info->threadID);
    while (true) {
        int randomNumber = rand();
        int index = randomNumber % arraySize;
        WaitForSingleObject(mutex, INFINITE);
        if (arr[index] == 0) {
            Sleep(5);
            arr[index] = info->threadID;
            ReleaseMutex(mutex);
            Sleep(5);
        } else {
            cout << "Thread " << info->threadID << ": ";
            cout << "Number of marked elements: " << index + 1 << endl;
            cout << "Cannot mark element at index " << index << endl;
            info->marked = true;
            SetEvent(main_cond);
            ReleaseMutex(mutex); // Release mutex before waiting for signal
            WaitForSingleObject(marker_cond[info->threadID], INFINITE); // Wait for main thread's signal before retrying
            continue; // Retry marking after receiving signal from main thread
        }
        ReleaseMutex(mutex); // Release mutex after accessing array
        break; // Exit loop if the array element was successfully marked
    }
    return 0;
}

int main() {
    cout << "Enter array size: ";
    cin >> arraySize;
    arr = new int[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        arr[i] = 0;
    }
    mutex = CreateMutex(NULL, FALSE, NULL);
    main_cond = CreateEvent(NULL, TRUE, FALSE, NULL);
    for (int i = 0; i < MAX_THREADS; ++i) {
        marker_cond[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    int numThreads;
    cout << "Enter number of marker threads: ";
    cin >> numThreads;
    HANDLE markerThreads[MAX_THREADS];
    MarkerThreadInfo markerInfo[MAX_THREADS];
    for (int i = 0; i < numThreads; ++i) {
        markerInfo[i].threadID = i;
        markerThreads[i] = CreateThread(NULL, 0, markerThread, &markerInfo[i], 0, NULL);
    }
    SetEvent(main_cond);
    while (true) {
        cout << "Array:" << endl;
        for (int i = 0; i < arraySize; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;

        int markerID;
        cout << "Enter id of marker thread to end (or -1 to exit): ";
        cin >> markerID;

        if (markerID == -1)
            break;

        if (markerID >= 0 && markerID < numThreads) {
            SetEvent(marker_cond[markerID]);
            WaitForSingleObject(markerThreads[markerID], INFINITE);
            cout << "Array after ending the thread " << markerID << ":" << endl;
            for (int i = 0; i < arraySize; ++i) {
                cout << arr[i] << " ";
            }
            cout << endl;

            // Reset the marked flag for the current marker thread
            markerInfo[markerID].marked = false;

            // Update the array by resetting the marked elements to 0
            for (int i = 0; i < arraySize; ++i) {
                if (arr[i] == markerID + 1) {
                    arr[i] = 0;
                }
            }
        } else {
            cout << "Invalid marker thread ID. Please enter a valid ID." << endl;
        }

        SetEvent(main_cond);
    }
    cout << "Main thread is ended" << endl;
    CloseHandle(mutex);
    CloseHandle(main_cond);
    for (int i = 0; i < MAX_THREADS; ++i) {
        CloseHandle(marker_cond[i]);
    }
    delete[] arr;
    return 0;
}
