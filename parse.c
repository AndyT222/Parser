#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "nal.h"

int main(int argc, char **argv)
{
    static Program prog; Master library; Variables usrvars;
    int i, newf; char* mainfile; int counter = 0;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    if(argcheck(argc, argv) == 0)
    {
        mainfile = argv[1];
    }

    prog.cw = library.filecount = 0;
    usrvars.intcount = usrvars.wrdcount = 0;

    fileclear(mainfile, &prog, &library);
    Prog(&library.files[0], &library, PARSE, &usrvars, 
        &newf);
    getfiles(c, &prog, &counter);

    i = 0;

    /* Loop reads and parses each file as
    it is encountered */
    while(c[i] != NULL)
    {
        fileclear(c[i], &prog, &library);
        Prog(&library.files[i+1], &library, PARSE, 
            &usrvars, &newf);
        getfiles(c, &prog, &counter);
        i++;
    }

    printf("Parsed OK\n");
    freeall(c);
    free(c);
    return 0;
}
