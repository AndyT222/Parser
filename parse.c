#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "nal.h"

int main(void)
{
    int i, j;
    Program prog;
    Master library;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    prog.cw = 0; 
    library.filecount = 0;

    /* Test helper functions */
    testing();

    /* Initialise first file */
    i = 0;
    fileclear(PROGNAME, &prog, &library);
    Prog(&library.files[0]);
    getfiles(c, &prog);
    printall(library.files[i]);

    while(c[i] != NULL)
    {
        fileclear(c[i], &prog, &library);
        Prog(&library.files[i+1]);

        /* These two can be removed */
        printf("\n\nNEXT %d \n", i+1);
        printall(library.files[i+1]);

        getfiles(c, &prog);
        i++;
    }

    printf("\n\nParsed OK\n");
    free(c);

    return 0;
}

