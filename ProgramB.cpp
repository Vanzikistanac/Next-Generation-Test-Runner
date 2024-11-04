#include <windows.h>
#include <iostream>
#include <string>

const int MAX_SIZE_BUFFER = 256;

BOOL GetRandom(HANDLE pipeHandle){
    int randomNumber = rand() % 100;
    if (WriteFile(
        pipeHandle,
        &randomNumber,
        sizeof(int),
        NULL,
        NULL
    ) == FALSE) return FALSE;
    std :: cout << "Sure, here is the random number : " << randomNumber << std :: endl;
    return TRUE;
}


int main() {
    LPCSTR pipeName = "\\\\.\\pipe\\JetBrainsPipe";
    HANDLE pipeHandle = CreateFileA(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (pipeHandle == INVALID_HANDLE_VALUE){
        std :: cout << "Error while creating pipe handle" << std :: endl;
        std :: cout << "Last Error : ";
        GetLastError();
        CloseHandle(pipeHandle);
        return EXIT_FAILURE;
    }
    std :: cout <<"Connected!" << std :: endl;
    char *buffer = (char*) malloc(MAX_SIZE_BUFFER);
    while (1){
        ReadFile(
            pipeHandle,
            buffer,
            MAX_SIZE_BUFFER - 1,
            NULL,
            NULL
        );
        if (strcmp(buffer, "Hi") == 0) std :: cout <<"Hi" << std :: endl;
        else if (strcmp(buffer, "GetRandom") == 0){
            if (GetRandom(pipeHandle) == FALSE){
                std :: cout <<"Last Error : ";
                GetLastError();
                CloseHandle(pipeHandle);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(buffer, "Shutdown") == 0) break;   
        std :: cout << buffer << std :: endl; 
    }

    CloseHandle(pipeHandle);
    return EXIT_SUCCESS;
}
