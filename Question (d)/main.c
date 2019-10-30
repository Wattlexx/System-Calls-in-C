#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define PATH_MAX 20000

/**
Question d)
This program kills a notepad as soon as one is opened
Author: Nicholas Pinney
Date: Sept,23 2019
*/
int main()
{
    printf("Open a Notepad, I dare you...\n");

    //Virtual file that will hold the output of the shell command
    FILE *fp;
    //Buffer that will hold the piped information line by line
    char path[PATH_MAX];

    //Variables to hold the parsed information
    char *pLine, *parse;

    //Always running to detect a notepad process
    while (true) {

        //Command to execute to kill the notepad
        char kill[] = "taskkill /pid ";
        parse = NULL;
        pLine = NULL;

        //Piping the output from the command into the file
        fp = popen("tasklist", "r");
        //If it failed...
        if (fp == NULL)
        {
            printf( "CreateProcess failed\n");
            return 0;
        }

        //While loop to scan through the file
        while (fgets(path, PATH_MAX, fp) != NULL)
        {
            //Gets the line notepad.exe resides in
            pLine = strstr(path, "notepad.exe");
            //Check to make sure
            if (pLine != NULL)
            {
                printf("%s", pLine);

                parse = strtok(pLine, " ");
                parse = strtok(NULL, " "); //PID

                //Add the PID to the kill command
                strcat(kill, parse);

                //Execute the kill command using system()
                system(kill);
            }
        }
    }
    return 0;
}
