#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "nal.h"

/* Note: this test file is intended &
hardcoded for the escape211.nal example */

int main(void)
{
    Program prog; Master library; Variables usrvars;
    int i, newf; int counter = 0; int j = 1;
    char** c = calloc(1, sizeof(char**)*MAXFILES);

    prog.cw = library.filecount = usrvars.intcount = usrvars.wrdcount = 0;

    testing();
    fileclear(PROGNAME, &prog, &library);
    Prog(&library.files[0], &library, PARSE, &usrvars, &newf);

    assert(library.filecount == j);
    j++;

    getfiles(c, &prog, &counter);
    assert(counter == 1);
    assert(dupcheck(c, "escape211.nal") == 0);

    i = 0;

    while(c[i] != NULL)
    {
        fileclear(c[i], &prog, &library);
        Prog(&library.files[i+1], &library, PARSE, &usrvars, &newf);
        assert(library.filecount == j);

        getfiles(c, &prog, &counter);
        i++; j++;
    }
    
    assert(findfile(&library, "labse.nal") == 3);
    assert(findfile(&library, "labne.nal") == 5);
    assert(findfile(&library, "nonex.nal") == -1);
    assert(findfile(&library, "labnex.nal") == -1);

    assert(dupcheck(c, "escape211.nal") == 1);
    assert(dupcheck(c, "labne.nal") == 1);
    assert(dupcheck(c, "random.nal") == 0);
    assert(dupcheck(c, "other.nal") == 0);

    printf("Test Parsed OK\n");

    freeall(c);
    free(c);

    c = NULL;
    assert(c == NULL);

    return 0;
}
