#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

/**
Header file containing all struct definitions for the linked list implementation
Author: Nicholas Pinney
Date: Sept 23, 2019
*/
//Used by the initial list
typedef struct processInfo {
    char *pName;
    char *processId;
    char *memUsage;
    char *cpuTime;
}PROCESS_INFO;

//Used by 'filtered' lists
typedef struct {
    char *pName;
    char *processId;
    int memUsage;
    int cpuTime;
}PROCESS_DETAILS;

//Node for the lists
typedef struct node {
    void* data;
    struct node* next;
}NODE;

//List structure/manager
typedef struct li {
    int num;
    NODE* head;
    NODE* rear;
}LIST;

#endif // STRUCTS_H_INCLUDED
