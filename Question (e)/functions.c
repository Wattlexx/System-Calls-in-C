#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "structs.h"
#include "functions.h"

#define PATH_MAX 4000

/**
Source file containing all function definitions.
Functions that are more critical and perform primary tasks have been commented more thoroughly
Author: Nicholas Pinney
Date: Sept 23, 2019
*/
//Function that creates and allocates memory for a new list
LIST* createList()
{
    LIST* newListPtr;
    newListPtr = (LIST*)malloc(sizeof(LIST));

    if (newListPtr)
    {
        newListPtr->num = 0;
        newListPtr->head = NULL;
        newListPtr->rear = NULL;
    }
    return newListPtr;
}

//Function that creates and allocates memory for a PROCESS_INFO struct
PROCESS_INFO* createPinfo(char *name, char *pid, char *kb, char *cTime)
{
    PROCESS_INFO* pInfoPtr;
    pInfoPtr = (PROCESS_INFO*)malloc(sizeof(PROCESS_INFO));

    if (pInfoPtr)
    {
        pInfoPtr->pName = strdup(name);
        pInfoPtr->processId = strdup(pid);
        pInfoPtr->memUsage = strdup(kb);
        pInfoPtr->cpuTime = strdup(cTime);
    }
    return pInfoPtr;
}

//Function that creates and allocates memory for a PROCESS_DETAILS struct
PROCESS_DETAILS* createDinfo(char *name, char *pid, int kb, int t)
{
    PROCESS_DETAILS* pDetailsPtr;
    pDetailsPtr = (PROCESS_DETAILS*)malloc(sizeof(PROCESS_DETAILS));

    if (pDetailsPtr)
    {
        pDetailsPtr->pName = strdup(name);
        pDetailsPtr->processId = strdup(pid);
        pDetailsPtr->memUsage = kb;
        pDetailsPtr->cpuTime = t;
    }
    return pDetailsPtr;
}

//Function that creates and allocates memory for a NODE struct
NODE* createNode(void* dataPtr)
{
    NODE* nodePtr;
    nodePtr = (NODE*)malloc(sizeof(NODE));

    if (nodePtr)
    {
        nodePtr->data = dataPtr;
        nodePtr->next = NULL;
    }
    return nodePtr;
}

//Function which gets a string representing the memory and converts in into an int for comparison purposes
int createIntFromMemUsage(char* str)
{
    char* string1;
    char* string2;
    int mem;

    string1 = strtok(str, ",");
    string2 = strtok(NULL, " ");
    if (string2 != NULL)
        strcat(string1, string2);

    //atoi() does the requested conversion
    mem = atoi(string1);
    return mem;
}

//Function which gets a string representing the time and converts it into int seconds for comparison purposes
int createIntFromCPUTime(char* str)
{
    int t;
    t = atoi(strtok(str, ":"))*60*60;
    t += atoi(strtok(NULL, ":"))*60;
    t += atoi(strtok(NULL, " "));
    return t;
}

//Function which receives a string (path) and parses it accordingly to extract the relevant information and then creates a
//PROCESS_INFO struct with it
PROCESS_INFO* parseInfoFromPath (char str[])
{
    char *pName;
    char *processId;
    char *cpuTime;
    char *memUsage;
    char *parse;

    parse = strtok(str, " ");
    pName = parse;

    parse = strtok(NULL, " ");
    processId = parse;

    parse = strtok(NULL, " "); //Console
    parse = strtok(NULL, " "); //session
    parse = strtok(NULL, " "); //memory
    memUsage = parse;

    parse = strtok(NULL, " ");
    parse = strtok(NULL, " ");
    parse = strtok(NULL, " ");
    parse = strtok(NULL, " "); //CPUTIME
    cpuTime = parse;

    PROCESS_INFO* pInfoPtr;
    //Create PROCESS_INFO dataType
    pInfoPtr = createPinfo(pName, processId, memUsage, cpuTime);

    return pInfoPtr;
}

//Function that receives a list and a node and adds the node to the end of the list
bool addToList(LIST* list_, NODE* newNode)
{
    bool flag;
    //return if no list
    if (!list_)
    {
        flag = false;
        return flag;
    }
    //if empty list set as head
    if (list_->head == NULL)
        list_->head = newNode;
    //otherwise set as link of last node to point to last node
    else
        list_->rear->next = newNode;

    //rear points to last node
    list_->rear = newNode;
    (list_->num)++;

    flag = true;
    return flag;
}

//Function which creates the initial list of all the processes using the same FILE and pipe technique from question (d)
LIST* buildList()
{
    FILE *fp;
    char path[PATH_MAX];

    //Command whose output gets piped to a FILE fp
    fp = popen("tasklist /v /fi \"STATUS eq running\" /nh ", "r");
    if (fp == NULL)
    {
        printf( "CreateProcess failed\n");
        return 0;
    }
    int c = 0;
    //LIST to hold data
    LIST* list_;
    list_ = createList();

    PROCESS_INFO* p;
    NODE* n;

    //While there is text to read in the file
    while (fgets(path, PATH_MAX, fp) != NULL)
    {
        if (path != NULL && c != 0)
        {
            //create the process info node
            p = parseInfoFromPath(path);
            //create the list node
            n = createNode(p);
            //Add new node to the list
            if (!addToList(list_, n)){
                printf("Failed at adding to list");
            }
        }
        //Increment iterator
        c++;
    }
    return list_;
}

//Function that receives the list of all processes and return a new list based on filter parameter a
LIST* filterListFor(int a, LIST* processes)
{
    LIST* newList_;
    newList_ = createList();

    int memUsage, time;
    NODE* n;
    n = processes->head;

    //a == 0 -> create list of processes using lots of memory
    if (a == 0)
    {
        while (n->next != NULL)
        {
            PROCESS_INFO* p = n->data;
            //Get amount of time from String
            time = createIntFromCPUTime(strdup(p->cpuTime));
            //Get amount of memory from String
            memUsage = createIntFromMemUsage(strdup(p->memUsage));
            if (memUsage >= 100000)
            {
                //Use PROCESS_DETAILS to hold time and memory of type ints rather than string
                if(!addToList(newList_, createNode(createDinfo(strdup(p->pName), strdup(p->processId), memUsage, time))))
                    return 0;
            }
            n = n->next;
        }
    }
    //Otherwise create list of processes that have been running for over 100 seconds
    else
    {
        while (n->next != NULL)
        {
            PROCESS_INFO* p = n->data;
            //Get amount of time from String
            time = createIntFromCPUTime(strdup(p->cpuTime));
            //Get amount of memory from String
            memUsage = createIntFromMemUsage(strdup(p->memUsage));
            if (time > 100)
            {
                //Use PROCESS_DETAILS to hold time and memory of type ints rather than string
                if(!addToList(newList_, createNode(createDinfo(strdup(p->pName), strdup(p->processId), memUsage, time))))
                    return 0;
            }
            n = n->next;
        }
    }
    //Return the new list created
    return newList_;
}

//Unused function to print the contents of the initial processes list
void printInfoList (LIST* list_)
{
    NODE* n;
    n = list_->head;
    while (n != NULL)
    {
        PROCESS_INFO* p = n->data;
        printf("\nName: %s\n", p->pName);
        printf("PID: %s\n", p->processId);
        printf("Memory: %s\n", p->memUsage);
        printf("CPU Time: %s\n\n", p->cpuTime);
        n = n->next;
    }
}

//Unused function to print the contents of the PROCESS_DETAILS list
void printDetailList (LIST* list_)
{
    NODE* n;
    n = list_->head;
    while (n != NULL)
    {
        PROCESS_DETAILS* q = n->data;
        printf("\nName: %s\n", q->pName);
        printf("PID: %s\n", q->processId);
        printf("Memory: %d\n", q->memUsage);
        printf("CPU Time: %d seconds\n", q->cpuTime);
        n = n->next;
    }
}

//Function that frees the memory of and destroys the initial processes list
bool destroyPList (LIST* list_)
{
    bool x = false;
    NODE* deletePtr;
    if (list_)
    {
        while (list_->num > 0)
        {
            deletePtr = list_->head;

            PROCESS_INFO* p = deletePtr->data;
            free(p->pName);
            p->pName = NULL;
            free(p->processId);
            p->processId = NULL;
            free(p->memUsage);
            p->memUsage = NULL;
            free(p->cpuTime);
            p->cpuTime = NULL;
            free(deletePtr->data);

            list_->head = list_->head->next;
            free(deletePtr);
            (list_->num)--;
        }
        free(list_);
        x = true;
    }
    return x;
}

//Function that frees the memory of and destroys the PROCESS_DETAILS list
bool destroyDList (LIST* list_)
{
    bool x = false;
    NODE* deletePtr;
    if (list_)
    {
        while (list_->num > 0)
        {
            deletePtr = list_->head;

            //Since NODE data is a void pointer, the data needs to be casted to its type before it can be used
            PROCESS_DETAILS* p = deletePtr->data;
            free(p->pName);
            p->pName = NULL;
            free(p->processId);
            p->processId = NULL;
            free(&(p->memUsage));
            free(&(p->cpuTime));
            free(deletePtr->data);

            list_->head = list_->head->next;
            free(deletePtr);
            (list_->num)--;
        }
        free(list_);
        x = true;
    }
    return x;
}

//Function containing the kill command.
//Iterates through the list index times until process requested is reached
bool _kill(int index, LIST* _list)
{
    bool success = false;
    int i;
    NODE* n;
    n = _list->head;
    for (i = 0; i != index-1; i++)
    {
        n = n->next;
    }
    PROCESS_DETAILS* p = n->data;
    char kill[] = "taskkill /pid ";
    strcat(kill, p->pName);
    system(kill);
    success = true;
    return success;
}

//Function that determines which list the requested process for killing is contained in
bool killProcess(int c, LIST* hMem, LIST* hTime)
{
    bool success;
    if (c <= hMem->num)
        success =  _kill(c, hMem);
    else if (c > hMem->num)
        success = _kill((c - hMem->num), hTime);
    else
    {
        printf("Nothing was killed!");
        success = false;
    }
    return success;
}


