#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

const int MAX_SIZE_BUFFER = 256;
const int COUNT_OF_RANDOM_NUMBERS = 100;

int getMedian(std :: vector<int>&a){
    //Formula for Median from wikipedia, with error checking
    int n = a.size();
    if (n == 0) return -1;
    if (n % 2 == 1) return (a[n / 2]);
    return (a[n / 2] + a[(n / 2) - 1]) / 2;
}

int getAverage(std :: vector<int>&a){
    int count = a.size();
    int sum = accumulate(a.begin(), a.end(), 0); //returns sum of numbers in vector a
    return (sum / count);
}

BOOL GetRandom(HANDLE pipeHandle){
    std :: vector<int>randomNumbers;
    for (int i=0;i<COUNT_OF_RANDOM_NUMBERS;i++){
        char str[] = "GetRandom";
        if (WriteFile(
            pipeHandle,
            str,
            sizeof(str),
            NULL,
            NULL
        ) == FALSE) return FALSE;

        int randomNumber = -1;
        if (ReadFile(
            pipeHandle,
            &randomNumber,
            sizeof(int),
            NULL,
            NULL
        ) == FALSE) return FALSE;
        randomNumbers.push_back(randomNumber);
    }

    std :: sort(randomNumbers.begin(), randomNumbers.end());
    for (auto t : randomNumbers) std :: cout <<t<< ' ';
    std :: cout << std :: endl;
    std :: cout << "Median : " << getMedian(randomNumbers) << std :: endl;
    std :: cout << "Average value : " << getAverage(randomNumbers) << std :: endl;
    return TRUE;
}

BOOL Hi(HANDLE pipeHandle){
    char buffer[] = "Hi";
    return WriteFile(
        pipeHandle,
        buffer,
        sizeof(buffer),
        NULL,
        NULL
    );
}

BOOL Exit(HANDLE pipeHandle){
    char str[] = "Shutdown";
    return WriteFile(
        pipeHandle,
        str,
        sizeof(str),
        NULL,
        NULL
    );
}

int main() {
    //Creates Handle for pipe
    //Documentation says that this is the correct format for naiming pipe
    LPCSTR pipeName = "\\\\.\\pipe\\JetBrainsPipe";
    HANDLE pipeHandle = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_WAIT,
        1,
        256,
        256,
        0,
        NULL
    );

    if (pipeHandle == INVALID_HANDLE_VALUE){
        std :: cout << "Error while creating pipe handle" << std :: endl;
        return EXIT_FAILURE;
    }

    std :: cout <<"Waiting..." << std :: endl;
    
    if (ConnectNamedPipe(pipeHandle, NULL) == FALSE){
        std :: cout <<"Error while connecting to the pipe" << std :: endl;
        CloseHandle(pipeHandle);
        return EXIT_FAILURE;
    }

    std :: cout <<"Connected!" << std :: endl;
    std :: cout << std :: endl;
    std :: cout <<"Input one of the following commands :" << std :: endl;
    std :: cout <<"GetRandom" << std :: endl;
    std :: cout <<"Hi" << std :: endl;
    std :: cout <<"Shutdown" << std :: endl;
    std :: cout << std :: endl;
    //If other program is connected to our pipe, we can start
    //asking for instructions
    char buffer[MAX_SIZE_BUFFER];
    while (1){
        std :: cin >> buffer;
        if (strcmp(buffer, "Hi") == 0){
            if (Hi(pipeHandle) == FALSE){
                std :: cout <<"Last Error : ";
                GetLastError();
                CloseHandle(pipeHandle);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(buffer, "GetRandom") == 0){
            if (GetRandom(pipeHandle) == FALSE){
                std :: cout <<"Last Error : ";
                GetLastError();
                CloseHandle(pipeHandle);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(buffer, "Shutdown") == 0){
            if (Exit(pipeHandle) == FALSE){
                std :: cout <<"Last Error : ";
                GetLastError();
                CloseHandle(pipeHandle);
                return EXIT_FAILURE;
            }
            break;
        }
    }
    //Closing hanlde before exiting!
    CloseHandle(pipeHandle);
    return EXIT_SUCCESS;
}
