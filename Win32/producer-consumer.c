#include <stdio.h>
#include <math.h>
#include "windows.h"

#define BUFFER_SIZE 16                          // Buffer with 16 slots total
#define ARRAY_SIZE 50                           // Default array size

DWORD initializeSemaphore(void);                // Initialize all the semaphores
DWORD WINAPI produce(LPVOID lpParam);           // The producer will produce item in this function
DWORD WINAPI consume(LPVOID lpParam);           // The consumer will consume item in this function

HANDLE mutex;                                   // Handle to mutex
HANDLE full;                                    // Keep track of full slots
HANDLE empty;                                   // Keep track of empty slots
int buffer[BUFFER_SIZE];                        // The buffer to store items produced

int main(int argc, char *argv[]) {
    
    HANDLE hThread;                             // Handle to thread
    HANDLE *hThreadArray[ARRAY_SIZE];           // Array of thread
    int i;                                      // Index
    int producer_count = 0;                     // Number of producers
    int consumer_count = 0;                     // Number of consumers

    if (argc != 4) {
        return 1;
    }

    producer_count = pow(2, atoi(argv[1]));                         // Store the number of producers
    consumer_count = pow(2, atoi(argv[2]));                         // Store the number of consumers

    // Compute the number of items each consumer needs to consume
    max_item_consume = ((max_item_produce * producer_count) / consumer_count);        

    if (!initializeSemaphore())                                     // Exit program if we can't initialize semaphores
        return 1;

    // Start to create producer threads
    for (i = 0; i < producer_count; i++) {
        hThreadArray[i] = CreateThread(NULL,
                               0,                                   // Use default stack size
                               produce,                             // Produce function
                               NULL,                                // Pass in producer as the argument
                               0,                                   // Default creation flags
                               NULL);                               // Don't return thread identifier

        if (hThreadArray[i] == NULL) {
            printf("Failed to create thread\n");
            ExitProcess(3);
        }
    }

    // Start to create consumer threads
    for (; i < consumer_count + producer_count; i++) {
        hThreadArray[i] = CreateThread(NULL,
                               0,                                   // Use default stack size
                               consume,                             // Consume function
                               NULL,                                // No function argument
                               0,                                   // Default creation flags
                               NULL);                               // Don't return thread identifier

        if (hThreadArray[i] == NULL) {
            printf("Failed to create thread\n");
            ExitProcess(3);
        }
    }

    printf("Completed\n");

    return 0;
}

DWORD initializeSemaphore(void) {

    // Initialize the buffer to all -1
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = -1;

    if ((mutex = CreateMutex(NULL,                                  // Default security attributes
                             FALSE,                                 // Does not obtain ownership of the mutex
                             NULL)) != NULL) {                      // Unnamed mutex
        printf("Failed to create mutex\n");
        return 1;
    }

    if ((full = CreateSemaphore(NULL,                               // Default security attributes
                                0,                                  // Initial count
                                BUFFER_SIZE,                        // Maximum count
                                NULL)) != NULL) {                   // Unnamed semaphore
        printf("Failed to create semaphore\n");
        return 1;
    }

    if ((empty = CreateSemaphore(NULL,
                                 BUFFER_SIZE,
                                 BUFFER_SIZE,
                                 NULL)) != NULL) {
        printf("Failed to create semaphore\n");
        return 1;
    }

    return 0;
}

DWORD WINAPI produce(LPVOID lpParam) {

    int counter = 0;
    
    while (1) {                            

        WaitForSingleObject(empty, INFINITE);                       // Wait for the semaphore empty
        WaitForSingleObject(mutex, INFINITE);                       // Wait for the mutex lock
        
        // Release mutex lock
        if (!ReleaseMutex(mutex)) {                 
            printf("Failed to release ownership of the mutex object\n");
        }

        // Signal the semaphore full
        if (!ReleaseSemaphore(full,                                 // Handle to semaphore
                              1,                                    // Increase count by one
                              NULL)) {                              // Not interested in previous count
            printf("Failed to release semaphore full\n");
        }
    }

    return 0;
}

DWORD WINAPI consume(LPVOID lpParam) {

    while (1) {
        WaitForSingleObject(full, INFINITE);                        // Wait for the semaphore full
        WaitForSingleObject(mutex, INFINITE);                       // wait for the mutex lock
   
        // Release the mutex lock
        if (!ReleaseMutex(mutex)) {
            printf("Failed to release ownership of the mutex object\n");
        }

        // Signal the semaphore empty
        if (!ReleaseSemaphore(empty,                                // Handle to semaphore
                              1,                                    // Increase count by one
                              NULL)) {                              // Not interested in previous count
            printf("Failed to release semaphore full\n");
        }
    }
    
    return 0;
}

/*
 The completed source code will be posted after 4/22/15
 */
