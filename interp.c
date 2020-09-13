#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

int main(int argc, char **argv)
{  
    Program prog; Master library; Variables usrvars;
    char* mainfile;
    int newf = 0; int counter = 0; 
    char** c = calloc(1, sizeof(char**)*MAXFILES);
    
    prog.cw = library.filecount = 0;
    usrvars.intcount = usrvars.wrdcount = 0;
    
    if(argcheck(argc, argv) == 0)
    {
        mainfile = argv[1];
    }
    
    srand(time(0));
    iterate(&prog, &library, &counter, c, mainfile);
    Prog(&library.files[0], &library, INTERP, 
        &usrvars, &newf);

    freeall(c);
    free(c);
    return 0;
}

