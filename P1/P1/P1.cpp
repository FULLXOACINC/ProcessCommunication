#include <windows.h>
#include <stdio.h>

#define TIME_OUT 20000
#define SLEEP_TIME 1000
#define P_PRINT "P1 %i\n"

int main(int argc, char *argv[])
{

    char* PEventName=argv[1];
    char* P1EventName=argv[2];
    char* file=argv[3];

    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,PEventName);
    HANDLE P1Event = CreateEvent(NULL,TRUE,FALSE,P1EventName);
    int number=0;

    HANDLE FileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, file);
    if(FileMapping==NULL){
        return -3;
    }

    LPVOID FileMap = MapViewOfFile(FileMapping,FILE_MAP_READ, 0, 0, 0);
    if(FileMap == 0)
        return -4;


    while(true){
        switch(WaitForSingleObject(PEvent,TIME_OUT)){
        case WAIT_OBJECT_0:
            {
                Sleep(SLEEP_TIME);
                printf(P_PRINT,*((LPSTR)FileMap));

                ResetEvent(PEvent);
                SetEvent(P1Event);
                break;
            }

        case WAIT_FAILED:
            {
                UnmapViewOfFile(FileMap);
                CloseHandle(FileMapping);
                return -1;
            }

        case WAIT_TIMEOUT:
            {
                UnmapViewOfFile(FileMap);
                CloseHandle(FileMapping);
                return -2;
            }
        }
    }
    return 0;
}



