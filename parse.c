#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "nal.h"

void printgetfile(char** temp);

int main(void)
{
    int i, j;
    int counter = 0;
    Program prog;
    Master library;
    Variables usrvars;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    prog.cw = 0; 
    library.filecount = 0;
    usrvars.intcount = 0;
    usrvars.wrdcount = 0;

    /* Test helper functions */
    testing();

    /* Initialise first file */
    fileclear(PROGNAME, &prog, &library, 0);
    Prog(&library.files[0], &library, 0, &usrvars);
    getfiles(c, &prog, &counter);

    i = 0;

    while(c[i] != NULL)
    {
        printgetfile(c);
        fileclear(c[i], &prog, &library,0);
        Prog(&library.files[i+1], &library, 0, &usrvars);
        getfiles(c, &prog, &counter);
        i++;
    }

    printf("\n\nParsed OK\n");

    /* Free the filenames */
    i = 0;

    while(c[i] != NULL)
    {
        free(c[i]);
        i++;
    }

    free(c);

    return 0;
}

void printgetfile(char** temp)
{

    int i = 0;
    
    printf("CUSTOM LOOP \n");

    while(temp[i] != NULL)
    {
        printf("%i %s \n", i, temp[i]);
        i++;
    }
}

