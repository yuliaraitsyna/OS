#include <gtest/gtest.h>
#include <windows.h>
#include <iostream>
#include <vector>

TEST_F(ArrayManipulationTest, MinMaxTest) {
    int arr[] = { 5, 2, 10, 3, 8 };
    int n = sizeof(arr) / sizeof(arr[0]);
    
    DWORD threadId;
    HANDLE hMinMax = CreateThread(NULL, 0, min_max, (void*)arr, 0, &threadId);
    WaitForSingleObject(hMinMax, INFINITE);

    EXPECT_EQ(minVal, 2);
    EXPECT_EQ(maxVal, 10);

    CloseHandle(hMinMax);
}

TEST_F(ArrayManipulationTest, AverageTest) {
    int arr[] = { 5, 2, 10, 3, 8 };
    int n = sizeof(arr) / sizeof(arr[0]);

    DWORD threadId;
    HANDLE hAverage = CreateThread(NULL, 0, average, (void*)arr, 0, &threadId);
    WaitForSingleObject(hAverage, INFINITE);

    EXPECT_EQ(averageVal, 5);

    CloseHandle(hAverage);
}