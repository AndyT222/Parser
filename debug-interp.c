#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

void printgetfile(char** temp);

int main(int argc, char **argv)
{
    Program prog; Master library; Variables usrvars;
    int newf = 0; int counter = 0;  int i = 0; char* mainfile;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    prog.cw = library.filecount = usrvars.intcount = usrvars.wrdcount = 0;

    if(argv[1] == NULL || argc == 1)
    {
        ERROR("No filename entered.");
    }
    else{
        mainfile = argv[1];
    }

    srand(time(0));
    testing();

    printf("DEBUG VERSION. \n");
    fileclear(mainfile, &prog, &library);
    getfiles(c, &prog, &counter);

    i = 0;

    while(c[i] != NULL)
    {
        printgetfile(c);
        fileclear(c[i], &prog, &library);
        getfiles(c, &prog, &counter);
        i++;
    }

    Prog(&library.files[0], &library, INTERP, &usrvars, &newf);

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
    
    printf("FILES LOADED: \n");

    while(temp[i] != NULL)
    {
        printf("%i %s \n", i, temp[i]);
        i++;
    }
}

