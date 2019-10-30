#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winbase.h>
#include <time.h>

#include "structs.h"
#include "functions.h"

/**
Question e)
This program constantly watches all processes (every 8 Seconds) and gives a report to the user regarding which they'd want to kill
Based on high memory usage and long CPU usage.
main.c contains the heart of the program (the main function) and the function in which the thread will call to get input from the user

Regards:
* One small(non-detrimental) bug and one running condition
    * Bug: This has to do with the thread: If you input a selection every time when prompted, then as soon as you hit enter the following code in the main will execute.
    * However, if you miss an input and the program refreshes by itself than further inputs will not stop the thread and will need to wait the 8 seconds for
    * the proceeding kill code (or whatever condition gets satisfied) to execute. The program still works and I couldn't figure out the problem so here it remains.
    * Condition: If you have processes whose Name contains a space, it wont show properly or might cause a runtime failure. I uninstalled my NVIDIA programs to fix this issue
    * as they were the only ones with spaces but otherwise everything works perfectly. (I didn't bother to fix this because I had already spent lots of time
    * trying to figure out how to parse the strings (amongst solving all the other bugs this program spit my way). I found a temporary solution for this which allowed me to continue
    * progress on this project.
Author: Nicholas Pinney
Date: Sept 23, 2019
*/

//This function gets called by the thread when it gets created
DWORD WINAPI getUserInputToKillProcess(void* index)
{
    //Gets input from user which is accessed from main via void pointer
    int choice = 0;
    printf("\nWhich process would you like to kill? (-1 to exit): ");
    scanf("%d", &choice);
    *(int*)index = choice;
    return 0;
}

int main()
{
    //Some variables for the list and thread
    LIST* processesList;
    DWORD dwThreadId;

    //Variable to hold the kill choice from the user
    int* num = NULL;
    num = malloc(sizeof(int));

    //Loop condition
    bool x = true;

    while (x) {
        //Resetting the variables
        void* choice = (void*)num;
        *(int*)choice = 0;
        int i = 0;

        printf("\nRefreshing...");

        //Build list of all processes
        processesList = buildList();

        printf("\n-------------------------------------------Using more than 100,000kb------------------------------------------");

        //Build list of processes using lots of memory
        LIST* processesHighMemUsage;
        processesHighMemUsage = filterListFor(0, processesList);

        //Display the list
        NODE* n = processesHighMemUsage->head;
        while (n != NULL)
        {
            PROCESS_DETAILS* q = n->data;
            printf("\n%d:", i+1);
            printf("%s\t\t", q->pName);
            n = n->next;
            i++;
        }

        printf("\n---------------------------------------Running longer than 100 Seconds----------------------------------------");

        //Build list of processes that have been running for a long time
        LIST* processesHighCPUTime;
        processesHighCPUTime = filterListFor(1, processesList);

        //Display the list
        n = processesHighCPUTime->head;
        while (n != NULL)
        {
            PROCESS_DETAILS* q = n->data;
            printf("\n%d:", i+1);
            printf("%s\t\t", q->pName);
            n = n->next;
            i++;
        }

        //Get user input by using a thread so that the program can still refresh if no kill was requested
        HANDLE thread = CreateThread(NULL, 0, getUserInputToKillProcess, choice, 0, &dwThreadId);
        //Make sure the thread created successfully
        if (thread)
        {
            dwThreadId = WaitForSingleObject(thread, 8000);
            //Code below will be executed after 8000 milliseconds or the thread starting function has returned
            //Process the input by the user
            if ((*(int*)choice > 0) && ((*(int*)choice) <= (processesHighCPUTime->num+processesHighMemUsage->num)))
            {
                int kill;
                printf("\nA choice was selected: %d\n", (*(int*)choice));
                kill = (*(int*)choice);
                if(!killProcess(kill, processesHighMemUsage, processesHighCPUTime))
                    printf("\nSomething went wrong:\(");
            }
            else if ((*(int*)choice) == -1)
            {
                printf("\nGood Day");
                x = false;
            }
            else if ((*(int*)choice) == 0)
                printf("\nNothing selected");
            else
                printf("\nInvalid input selection!");
        }

        //Destroy the current lists so that memory can be freed and new updated ones can be created
        if(!destroyPList(processesList))
            printf("\nList didn't destroy");
        if(!destroyDList(processesHighMemUsage))
            printf("\nList didn't destroy");
        if(!destroyDList(processesHighCPUTime))
            printf("\nList didn't destroy");

        printf("\n\n\n");
    }
    //Free the num memory
    free(num);
    return 0;
}
