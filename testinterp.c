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
    int newf = 0; int counter = 0; int j = 0; char* temp; char* mainfile;
    char** c = calloc(1, sizeof(char**)*MAXFILES);
    
    prog.cw = library.filecount = usrvars.intcount = usrvars.wrdcount = 0;
    
    if(argcheck(argc, argv) == 0)
    {
        mainfile = argv[1];
    }

    srand(time(0));
    testing();

    iterate(&prog, &library, &counter, c, mainfile);

    /* Assigning user floats */
    j = usrvars.intcount;
    addint(&usrvars,"%CUSTOM", 44);
    j++;
    assert(j == usrvars.intcount);
    assert((int) usrvars.usrint[usrvars.intcount-1] == 44);
    assert(strsame(usrvars.intid[usrvars.intcount-1], "%CUSTOM"));

    /* Incrementing user floats */
    increment(&usrvars, usrvars.intid[usrvars.intcount-1]);
    increment(&usrvars, usrvars.intid[usrvars.intcount-1]);
    assert((int) usrvars.usrint[usrvars.intcount-1] == 46);

    j = (int) findfloat(&usrvars, "%CUSTOM");
    assert(j == 46);

    /* Assigning user strings */
    j = usrvars.wrdcount;
    addstr(&usrvars,"$NAME", "Testing");
    j++;
    
    assert(j == usrvars.wrdcount);
    assert(strsame(usrvars.usrwrd[usrvars.wrdcount-1], "Testing"));
    assert(strsame(usrvars.wrdid[usrvars.intcount-1], "$NAME"));

    addstr(&usrvars,"$NAME", NULL);
    assert(strsame(usrvars.usrwrd[usrvars.wrdcount-1], "Testing"));
    assert(strsame(usrvars.wrdid[usrvars.intcount-1], "$NAME"));

    temp = findstr(&usrvars, "$NAME");
    assert(strsame(temp, "Testing"));

    Prog(&library.files[0], &library, INTERP, &usrvars, &newf);

    freeall(c);
    free(c);
    c = NULL;
    assert(c == NULL);

    return 0;
}

