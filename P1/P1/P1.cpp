#include <windows.h>
#include <stdio.h>

#define TIME_OUT 20000
#define SLEEP_TIME 1000
#define P_PRINT "P1 %i\n"
#define ARG_ERROR "incorrect argument set\n"
#define ARG_SET 4

int main(int argc, char *argv[])
{
    if(argc!=ARG_SET){
        printf(ARG_ERROR);
        return -5;
    }
    char* PEventName=argv[1];
    char* P1EventName=argv[2];
    char* file=argv[3];

    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,PEventName);
    HANDLE P1Event = CreateEvent(NULL,TRUE,FALSE,P1EventName);

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



