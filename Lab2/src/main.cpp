#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

int averageVal, minVal, maxVal, n;

DWORD WINAPI min_max(LPVOID a){
    int *arr = reinterpret_cast<int*>(a);
    long int max = MININT32;
    long int min = MAXINT32;
    for (int i = 0; i < n; i++){
        if (arr[i] > max){
            max = arr[i];
        }
        Sleep(7);
        if (arr[i] < min){
            min = arr[i];
        }
        Sleep(7);
    }
    minVal = min;
    maxVal = max;
    cout << "Min: " << min << "; Max: " << max << "\n";
    return 0;
}

DWORD WINAPI average(LPVOID a){
    int *arr = reinterpret_cast<int*>(a);
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += arr[i];
        Sleep(12);
    }
    sum /= n;
    averageVal = sum;
    cout << "Average: " << sum << "\n";
    return 0;
}

int main(){
    cout << "Enter number of elements\n";
    cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; i++){
        cout << "Enter " << i + 1 << " number:\n";
        int n;
        cin >> arr[i];
    }
    cout << "Your array:\n";
    for (int i = 0; i < n; i++){
        cout << arr[i] << " ";
    }
    cout << "\n";
    HANDLE hMinMax = CreateThread(NULL, 0, min_max, (void *)arr, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, average, (void *)arr, 0, NULL);
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);
    for (int i = 0; i < n; i++){
        if (arr[i] == minVal || arr[i] == maxVal){
            arr[i] = averageVal;
        }
    }
    cout << "New array:\n";
    for (int i = 0; i < n; i++){
        cout << arr[i] << " ";
    }
    cout << "\n";
    CloseHandle(hMinMax);
    CloseHandle(hAverage);
    delete[] arr;
    return 0;
}
