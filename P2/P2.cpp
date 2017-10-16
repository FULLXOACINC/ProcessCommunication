#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#define TIME_OUT 20000
#define SLEEP_TIME 1000
#define P2_PRINT "P2 complete\n"
#define ARG_ERROR "incorrect argument set\n"
#define ARG_SET 5



int main(int argc, char *argv[])
{
    if(argc!=ARG_SET){
        printf(ARG_ERROR);
        return -5;
    }
    char* PEventName=argv[1];
    char* P2EventName=argv[2];
    char* file=argv[3];
    char* resFile=argv[4];
    bool  isStart=true;
    char* openMode;

    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,PEventName);
    HANDLE P2Event = CreateEvent(NULL,TRUE,FALSE,P2EventName);

    FILE *fp;

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
                if(isStart){
                    openMode="w";
                    isStart=false;
                }
                else
                    openMode="a";

                fp=fopen(resFile,openMode);

                fprintf(fp, "%i\n", *((LPSTR)FileMap));
                printf(P2_PRINT);

                fclose(fp);

                ResetEvent(PEvent);
                SetEvent(P2Event);
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


