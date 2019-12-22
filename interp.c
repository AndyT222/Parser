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
    Program prog;
    Master library;
    Variables usrvars;

    char** c = calloc(1, sizeof(char**)*MAXFILES);
    int counter = 0;

    prog.cw = 0; 
    library.filecount = 0;
    usrvars.intcount = 0;
    usrvars.wrdcount = 0;

    /* Test helper functions */
    testing();

    addint(&usrvars, "%D", 5);
    printf("%s - %f \n", usrvars.intid[0], usrvars.usrint[0]);

    addint(&usrvars, "%G", 9);
    printf("%s - %f \n", usrvars.intid[1], usrvars.usrint[1]);

    /* Initialise first file */
    fileclear(PROGNAME, &prog, &library, 1);
    Prog(&library.files[0], 1, &usrvars);
    getfiles(c, &prog, &counter);

    i = 0;

    while(c[i] != NULL)
    {
        printgetfile(c);
        fileclear(c[i], &prog, &library,1);
        Prog(&library.files[i+1],1, &usrvars);
        getfiles(c, &prog, &counter);
        i++;
    }

    printf("%s - %f \n", usrvars.intid[2], usrvars.usrint[2]);
    printf("%s - %f \n", usrvars.intid[3], usrvars.usrint[3]);
    printf("%s - %s \n", usrvars.wrdid[0], usrvars.usrwrd[0]);
    printf("\nParsed OK\n");

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

