#include <gtest/gtest.h>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

TEST(MarkerThreadTest, MarkerThreadTest) {
    const int arraySize = 10;
    int* arr = new int[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        arr[i] = 0;
    }

    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    HANDLE main_cond = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE marker_cond = CreateEvent(NULL, TRUE, FALSE, NULL);

    MarkerThreadInfo markerInfo;
    markerInfo.threadID = 0;
    markerInfo.marked = false;
    HANDLE markerThreadHandle = CreateThread(NULL, 0, markerThread, &markerInfo, 0, NULL);

    SetEvent(main_cond);
    WaitForSingleObject(main_cond, INFINITE);

    EXPECT_TRUE(markerInfo.marked);

    SetEvent(marker_cond);
    WaitForSingleObject(markerThreadHandle, INFINITE);
    CloseHandle(mutex);
    CloseHandle(main_cond);
    CloseHandle(marker_cond);
    delete[] arr;
}
