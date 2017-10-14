#include <windows.h>
#include <stdio.h>
#include "process.h"
#include <time.h>

#define TIME_OUT 20000
#define SLEEP_TIME 3000
#define NUMBER 20

int main()
{
    srand(time(NULL));
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset (&si, 0, sizeof(si));
    memset (&pi, 0, sizeof(pi));
    GetStartupInfo(&si);
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;

    char CommandLine[1024];
    char* P1path=".\\P1.exe";
    char* P2path=".\\P2.exe";

    sprintf(CommandLine, "P1.exe %s", "PEvent P1Event test.txt");
    if (CreateProcess (P1path , CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) != 0) {
            printf ("Start P1\n");
       }
    else {
       printf ("Create Process P1 Failed\n");
       return -1;
       }
    sprintf(CommandLine, "P2.exe %s", "PEvent P2Event test.txt");
    if (CreateProcess (P2path , CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) != 0) {
            printf ("Start P2\n");
       }
    else {
       printf ("Create Process P2 Failed\n");
       return -2;
       }
    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,"PEvent");
    HANDLE P1Event = CreateEvent(NULL,TRUE,FALSE,"P1Event");
    HANDLE P2Event = CreateEvent(NULL,TRUE,FALSE,"P2Event");
    int number;
    FILE *fp;
    while(true){
        Sleep(SLEEP_TIME);
        number=rand()%NUMBER;
        printf ("P  %i\n",number);


        fp = fopen("test.txt", "w+");
        fprintf(fp,"%d",number);

        fclose(fp);

        SetEvent(PEvent);
        WaitForSingleObject(P1Event,TIME_OUT);
        WaitForSingleObject(P2Event,TIME_OUT);
        if(!(WaitForSingleObject(P1Event,TIME_OUT) ||WaitForSingleObject(P2Event,TIME_OUT)) ){
            ResetEvent(P1Event);
            ResetEvent(P2Event);
        }
        else
            return -3;


    }

}


