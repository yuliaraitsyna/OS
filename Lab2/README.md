# Lab 2 (Creating threads)

## Description

The program allows the user to input an array of integers and then creates two separate threads to perform the following tasks concurrently:
- Find the minimum and maximum values in the array.
- Calculate the average of the array elements.

After the completion of both threads, the program replaces the occurrences of minimum and maximum values in the array with the calculated average.

- The program uses Windows API functions such as `CreateThread`, `WaitForSingleObject`, and `CloseHandle` for thread management.
- Each thread performs its specific task with a specified delay to simulate computation using the `Sleep` function.
- `WaitForSingleObject` is used to wait for the threads to complete their tasks before proceeding with the main thread.

## Standards
C++ 17
