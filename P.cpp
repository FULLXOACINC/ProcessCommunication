#include <windows.h>
#include <stdio.h>
#include "process.h"
#include <time.h>

#define TIME_OUT 20000
#define SLEEP_TIME 1000
#define NUMBER 20
#define P1_START "P1 Start\n"
#define P2_START "P2 Start\n"
#define CREATE_FAILED "Create Process Failed\n"
#define P_PRINT "\nP  %i\n"
#define SIZE 1024
#define ARG_ERROR "incorrect argument set\n"
#define ARG_SET 8



int main(int argc, char *argv[])
{
    if(argc!=ARG_SET){
        printf(ARG_ERROR);
        return -6;
    }
    srand(time(NULL));
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset (&si, 0, sizeof(si));
    memset (&pi, 0, sizeof(pi));
    GetStartupInfo(&si);
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;

    char CommandLine[SIZE];
    char* P1Name=argv[1];
    char* P2Name=argv[2];
    char* PEventName=argv[3];
    char* P1EventName=argv[4];
    char* P2EventName=argv[5];
    char* file=argv[6];
    char* P2res=argv[7];

    HANDLE map_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int),file);
    if(map_handle==NULL)
        return -1;
    LPVOID lpFileMap = MapViewOfFile(map_handle,FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if(lpFileMap == 0)
        return -2;

    sprintf(CommandLine,"%s %s %s %s",P1Name,PEventName,P1EventName,file);
    if (CreateProcess (P1Name , CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) != 0) {
            printf (P1_START);
       }
    else {
        printf (CREATE_FAILED);
        UnmapViewOfFile(lpFileMap);
        CloseHandle(map_handle);
        return -3;
       }
    sprintf(CommandLine,"%s %s %s %s %s",P2Name,PEventName,P2EventName,file,P2res);
    if (CreateProcess (P2Name , CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) != 0) {
            printf (P2_START);
       }
    else {
        printf (CREATE_FAILED);
        UnmapViewOfFile(lpFileMap);
        CloseHandle(map_handle);
        return -4;
       }
    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,PEventName);
    HANDLE P1Event = CreateEvent(NULL,TRUE,FALSE,P1EventName);
    HANDLE P2Event = CreateEvent(NULL,TRUE,FALSE,P2EventName);
    int number;

    while(true){
        Sleep(SLEEP_TIME);

        number=(rand()%NUMBER);
        CopyMemory(lpFileMap, &number,sizeof(int));
        printf(P_PRINT,*((LPSTR)lpFileMap));

        SetEvent(PEvent);
        WaitForSingleObject(P1Event,TIME_OUT);
        WaitForSingleObject(P2Event,TIME_OUT);
        if(!(WaitForSingleObject(P1Event,TIME_OUT) || WaitForSingleObject(P2Event,TIME_OUT)) ){
            ResetEvent(P1Event);
            ResetEvent(P2Event);
        }
        else{
            UnmapViewOfFile(lpFileMap);
            CloseHandle(map_handle);
            return -5;
        }

    }

}
