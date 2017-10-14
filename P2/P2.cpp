#include <windows.h>
#include <stdio.h>
#define TIME_OUT 20000
#define SLEEP_TIME 3000

int main(int argc, char *argv[])
{
    HANDLE PEvent = CreateEvent(NULL,TRUE,FALSE,argv[1]);
    HANDLE P2Event = CreateEvent(NULL,TRUE,FALSE,argv[2]);

    FILE *fp;
    FILE *fp1;
    const char *path="P2res.txt";
    int number=0;

    while(true){
        switch(WaitForSingleObject(PEvent,TIME_OUT)){
        case WAIT_OBJECT_0:
            {
                Sleep(SLEEP_TIME);

                fp = fopen(argv[3], "r");
                fp1=fopen(path, "a");

                fscanf(fp, "%d", &number);
                fprintf(fp1, "%i\n", number);
                printf("P2 complete\n");

                fclose(fp);
                fclose(fp1);

                ResetEvent(PEvent);
                SetEvent(P2Event);
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
