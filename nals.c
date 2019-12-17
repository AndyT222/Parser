#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 100
#define PROGNAME "test1.nal"
#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2); }

struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw; /* Current Word */
};
typedef struct prog Program;

struct variables{
    char usrwrd[MAXNUMTOKENS][MAXTOKENSIZE];
    int usrint[MAXNUMTOKENS][MAXTOKENSIZE];
};
typedef struct prog Program;

void printstr(Program *p);
void Prog(Program *p);
void Code(Program *p);
void Statement(Program *p);

void makestr(Program *prog, int i, char* test);
int checkchar(char* str, char b);
int clearcheck(Program *prog);
void shiftclear(Program *prog);
void printall(Program prog);
void testing();

int main(void)
{
    int i, j;
    FILE *fp;
    Program prog;
    char *test = malloc(sizeof(char)*MAXTOKENSIZE);

    prog.cw = 0;

    testing();

    for(i=0; i<MAXNUMTOKENS; i++)
    {
        prog.wds[i][0] = '\0';

        if(!(fp = fopen(PROGNAME, "r")))

            {
            fprintf(stderr, "Cannot open %s\n",
            PROGNAME);

            exit(2);
            }

    }

    i = 0; j = 1;

    while(fscanf(fp, "%s", prog.wds[i++]) == 1 && i<MAXNUMTOKENS)
    {
        assert(i<MAXNUMTOKENS);
    }

    i = 0;

        printf("BEFORE VALIDATION: \n");
        printall(prog);

    /* Data Validation - Tokenisation */
    while(i< MAXNUMTOKENS)
    {
        /* This only checks once */
        if(prog.wds[i][0] == '"')
        {
            makestr(&prog, i, test);
            strcpy(prog.wds[i], test);
        }

        i++;
    }

        printf("\n\nAFTER VALIDATION: \n");
        shiftclear(&prog);
    
        /* Parsing */
        Prog(&prog);
        printf("Parsed OK\n");

    return 0;
}

/* Testing of helper functions */
void testing()
{

    assert(checkchar("test", 'e') == 0);
    assert(checkchar("test",'"') == 0);
    assert(checkchar("tes9",'"') == 0);
    assert(checkchar("tes\"",'"') == 1);

}

void printall(Program prog){

    int i = 0;

    printf("PRINT ALL ELEMENTS: \n");

    while(prog.wds[i][0] != '\0')
    {
        printf("[%s] [%d] \n", prog.wds[i], i);
        i++;
    }
}

int checkchar(char* str, char b){

    int i = 0;

    while(str[i] != '\0'){

        if(str[i] == b && str[i+1] == '\0')
        {
            return 1;
        }

        i++;
    }

    return 0;
}

int clearcheck(Program *prog){

    int i = 0;
    int check = 0;
    int brace = 0;

    while(prog->wds[i][0] != '\0')
    {
        if(prog->wds[i][0] == '}')
        {
            brace++;
        }

        i++;
    }

    if(brace == 0 || brace > 1)
    {
        ERROR("No closing brace or too many closing braces detected.");
    }

    i = 0;

    while(prog->wds[i][0] != '\0'){

        if(strsame(prog->wds[i], "CLEARED"))
        {
            check++;
        }

        if(i < MAXNUMTOKENS-1 && strsame(prog->wds[i], "CLEARED") && (strsame(prog->wds[i+1], "CLEARED") == 0 && prog->wds[i+1][0] != '\0'))
        {
            return 0;
        }

        if(i > 0 && strsame(prog->wds[i], "CLEARED") && (strsame(prog->wds[i-1], "CLEARED") == 0 && prog->wds[i-1][0] != '}'))
        {
            return 0;
        }

        i++;
    }

    if(check == 0)
    {
        return 1;
    }

    return 1;
}

void makestr(Program *prog, int i, char* test){

    int j = 1;

    strcpy(test, prog->wds[i]);

    while(j < MAXNUMTOKENS)
    {
        if(checkchar(prog->wds[i+j],'"'))
        {
            sprintf(test, "%s %s", test, prog->wds[i+j]);
            strcpy(prog->wds[i], test);
            strcpy(prog->wds[i+j], "CLEARED");

            return;
        }

        if(checkchar(prog->wds[i+j],'"') == 0)
        {
            sprintf(test, "%s %s", test, prog->wds[i+j]);
            strcpy(prog->wds[i+j], "CLEARED");
            j++;
        }

    }

    ERROR("Unable to find closing quotes.");
}

void swap(char* one, char* two)
{
    char* temp = calloc(1, sizeof(char)*MAXTOKENSIZE);

    strcpy(temp, one);
    strcpy(one, two);
    strcpy(two, temp);
}

void shiftclear(Program *prog)
{
    int i = 0;

    while(clearcheck(prog) == 0)
    {   
        if(prog->wds[i][0] == '\0')
        {
            i = 0;
        }

        if(strsame(prog->wds[i], "CLEARED") && prog->wds[i+1][0] != '\0')
        {
            swap(prog->wds[i], prog->wds[i+1]);
        }

        i++;
    }

    i = 0;

        while(prog->wds[i][0] != '\0')
    {   

        if(strsame(prog->wds[i], "CLEARED"))
        {
            strcpy(prog->wds[i], "\0");
        }

        i++;
    }

}

void Prog(Program *p)
{

    if(!strsame(p->wds[p->cw], "{"))
    {
        ERROR("No opening brace.");
    }

    p->cw = p->cw + 1;

    Code(p);
}

void Code(Program *p)
{
    printf("[%s]\n", p->wds[p->cw]);

    /* Recursive base case - terminates with abort or } */
    if(strsame(p->wds[p->cw], "}"))
    {
        return;
    }

    Statement(p);
    p->cw = p->cw + 1;
    Code(p);
}

void Statement(Program *p)
{
    int linej;

    if(strsame(p->wds[p->cw], "ABORT"))
    {
        return;
    }

    if(strsame(p->wds[p->cw], "JUMP"))
    {

        linej = atoi(p->wds[p->cw+1]);

        if(linej > MAXNUMTOKENS || linej < 0)
        {
            ERROR("Invalid jump.");
        }

        p->cw = linej;
        return;
    }
    
    if(strsame(p->wds[p->cw], "PRINT"))
    {

        if(p->wds[p->cw+1][0] == '"')
        {   
            printf("[%s] - [%d] \n", p->wds[p->cw+1], p->cw+1);
            p->cw = p->cw+1;
            return;
        }

        else
        {
            ERROR("Opening quotation missing for print statement.");
        }

    }

    ERROR("Expecting a ONE or NOUGHT ?");
}

void printstr(Program *p){

    int i = 1;

    while(p->wds[p->cw][i] != '"')
    {   
        if(isalpha(p->wds[p->cw][i]))
        {
            printf("%c", p->wds[p->cw][i]);
        }

        if(p->wds[p->cw][i] == ' ')
        {
            p->cw = p->cw + 1;
            printf(" %c", p->wds[p->cw][i]);
        }

        i++;

    }

    printf("\n");

    return;
}