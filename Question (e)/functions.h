#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "structs.h"

/**
Header file containing all function prototypes for usage throughout the program
Author: Nicholas Pinney
Date: Sept 23, 2019
*/
//Function prototypes
LIST* createList();
PROCESS_INFO* createPinfo(char *name, char *pid, char *kb, char *cTime);
PROCESS_DETAILS* createDinfo(char *name, char *pid, int kb, int t);
NODE* createNode(void* dataPtr);
int createIntFromMemUsage(char* str);
PROCESS_INFO* parseInfoFromPath (char str[]);
bool addToList(LIST* list_, NODE* newNode);
LIST* buildList();
LIST* filterListFor(int a, LIST* processes);
void printInfoList (LIST* list_);
void printDetailList (LIST* list_);
bool destroyPList (LIST* list_);
bool destroyDList (LIST* list_);
bool _kill(int index, LIST* _list);
bool killProcess(int c, LIST* hMem, LIST* hTime);

#endif //FUNCTIONS_H_INCLUDED
