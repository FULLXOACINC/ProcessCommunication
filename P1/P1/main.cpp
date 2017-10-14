#include <windows.h>
#include <stdio.h>
#define TIME_OUT 20000
#define SLEEP_TIME 3000


int main(int argc, char *argv[])
{
    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,argv[1]);
    HANDLE P1Event = CreateEvent(NULL,TRUE,FALSE,argv[2]);
    FILE *fp;
    int number=0;

    while(true){
        switch(WaitForSingleObject(PEvent,TIME_OUT)){
        case WAIT_OBJECT_0:
            {
                Sleep(SLEEP_TIME);

                fp = fopen(argv[3], "r");
                fscanf(fp, "%d", &number);
                printf("P1 %i\n",number);
                fclose(fp);;

                ResetEvent(PEvent);
                SetEvent(P1Event);
                break;
            }

        case WAIT_FAILED:
            return -1;
        case WAIT_TIMEOUT:
            return -2;
        }
    }
    return 0;
}



