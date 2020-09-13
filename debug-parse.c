#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

int main(int argc, char **argv)
{
    Program prog; Master library; Variables usrvars; char* mainfile;
    int newf = 0; int counter = 0;  int i = 0; int j = 1;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    prog.cw = library.filecount = usrvars.intcount = usrvars.wrdcount = 0;

    if(argcheck(argc, argv) == 0)
    {
        mainfile = argv[1];
    }

    srand(time(0));
    testing();

    printf("DEBUG VERSION - PARSER. \n");
    fileclear(mainfile, &prog, &library);
    Prog(&library.files[0], &library, PARSE, &usrvars, &newf);
    getfiles(c, &prog, &counter);

    assert(library.filecount == j);
    j++;

    i = 0;

    while(c[i] != NULL)
    {
        fileclear(c[i], &prog, &library);
        Prog(&library.files[i+1], &library, PARSE, &usrvars, &newf);
        assert(library.filecount == j);
        getfiles(c, &prog, &counter);
        i++; j++;
    }

    printf("DEBUG VERSION - PARSED OK. \n");

    freeall(c);
    free(c);

    c = NULL;
    assert(c == NULL);
    return 0;
}

