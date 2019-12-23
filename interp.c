#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

void printgetfile(char** temp);

int main(void)
{
    int i, j;
    Program prog;
    Master library;
    Variables usrvars;
    int newf;

    int counter = 0;
    int array[MAXTOKENSIZE] = {0};
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    srand(time(0));
    prog.cw = 0; 

    library.filecount = 0;
    usrvars.intcount = 0;
    usrvars.wrdcount = 0;
    
    /* Test helper functions */
    testing();

    /* Initialise first file */
    fileclear(PROGNAME, &prog, &library, 1);
    getfiles(c, &prog, &counter);

    i = 0;

    while(c[i] != NULL)
    {
        fileclear(c[i], &prog, &library,1);
        getfiles(c, &prog, &counter);
        i++;
    }

    Prog(&library.files[0], &library, 1, &usrvars, &newf, array);
    
    printf("\nInterpretation Finished. \n");

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

