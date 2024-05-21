# Process Server and Client

This project comprises a process server and multiple client processes that interact with the server to perform operations on a binary file containing employee records.

## Server

The server process undertakes the following actions:

1. Creates a binary file with employee records, where each record has the structure:
    ```c
    struct employee {
        int num;                // Employee ID
        char name[10];          // Employee name
        double hours;           // Hours worked
    };
    ```
    The filename and employee data are input from the console.
    
2. Outputs the created file to the console.

3. Launches client processes (`Client`) that access the file via a named pipe. The number of client processes is input from the console.

4. Services client process requests as follows:
    - If a client process requests modification of a file record, access to that record is blocked for other client processes until the modification is completed by the requesting client.
    - If a client process requests reading a file record, access to that record is:
        - Not blocked if other processes also request read access.
        - Blocked if other processes request write access.

5. Displays the modified file on the console after all client processes have completed their tasks.

6. Terminates its operation upon receiving a command from the console.

## Client

Each client process performs the following actions:

1. Enters a loop to repeatedly prompt the user for an operation:
    - Modify a file record.
    - Read a file record.
    - Exit the loop.

2. Accesses file records using the employee ID as the key.

3. During record modification:
    - Requests the record key.
    - Sends a request to the server.
    - Outputs the received record to the console.
    - Requests new field values.
    - Sends the modified record to the server upon user command.
    - Terminates access to the record upon user command.

4. During record reading:
    - Requests the record key.
    - Sends a request to the server.
    - Outputs the received record to the console.
    - Terminates access to the record upon user command.

## Usage

1. Compile the server and client programs using your preferred compiler.

2. Execute the server program first, providing the necessary inputs as prompted.

3. Execute the client program, specifying the desired number of client processes.

4. Follow the prompts on each client console to perform operations on the employee records file.

## Standards
C++ 17
