# Multi-Threaded Array Marking Program

This program is a multi-threaded application written in C++ that demonstrates the use of threads to mark elements in an array. Each thread attempts to mark a random element in the array, and if the chosen element is already marked, it waits for the main thread to signal before retrying.

## Usage

1. **Compilation**: Compile the program using a C++ compiler. For example, you can use `g++` on Linux or MinGW on Windows:

    ```bash
    g++ -o array_marking_program array_marking_program.cpp -lpthread
    ```

2. **Execution**: Run the compiled program:

    ```bash
    ./array_marking_program
    ```

3. **Array Size**: Enter the size of the array when prompted.

4. **Number of Marker Threads**: Enter the number of marker threads when prompted.

5. **Interacting with the Program**:

    - After starting the program, it will display the array and prompt for the ID of the marker thread to end.
    - Enter the ID of the marker thread to end or enter `-1` to exit the program.
    - If the marker thread ID is valid, the program will mark the chosen elements in the array and display the updated array.
    - Repeat the process until you choose to exit the program.

## Functionality

- **Marker Threads**: The program creates multiple marker threads, each attempting to mark a random element in the array.
- **Main Thread Interaction**: If a marker thread finds that the chosen element is already marked, it waits for a signal from the main thread before retrying.
- **Array Marking**: Marked elements in the array are represented by non-zero values. Each marker thread is identified by a unique ID, and marked elements are assigned the thread ID plus one.
- **Thread Synchronization**: The program uses mutexes and condition variables for thread synchronization.

