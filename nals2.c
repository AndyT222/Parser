#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 100
#define PROGNAME "01.no"
#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2); }

struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw; /* Current Word */
};
typedef struct prog Program;

void printstr(Program *p);
void Prog(Program *p);
void Code(Program *p);
void Statement(Program *p);

void makestr(Program *prog, int i, char* test);
int checkchar(char* str, char b);
void shiftclear(Program **prog, int start, int lines);
void printall(Program prog);

int main(void)
{

    int i, j;
    FILE *fp;
    Program prog;
    char *test = malloc(sizeof(char)*MAXTOKENSIZE);

    prog.cw = 0;

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

    /* Data Validation - Tokenisation */
    while(i< MAXNUMTOKENS)
    {
        /* This only checks once */
        if(prog.wds[i][0] == '"')
        {
            makestr(&prog, i, test);
            strcpy(prog.wds[i], test);
            free(test);
        }

        i++;
    }

        assert(checkchar("test",'"') == 0);
        assert(checkchar("tes\"",'"') == 1);

        printall(prog);
        
    
    /* Parsing */
        Prog(&prog);
        printf("Parsed OK\n");

    return 0;
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

        if(str[i] == b && str[i+1] == '\0'){
            return 1;
            }

        i++;
    }

    return 0;
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
            shiftclear(&prog, i+j, i);
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

void shiftclear(Program **prog, int start, int lines)
{
    int i = 0;
    int cell = start-lines+1;
    int hold = cell;
    int hold2 = lines;
    Program *h = *prog;

    printf("II == %s", h->wds[cell]);

    while(i < hold2)
    {
        strcpy(h->wds[cell], h->wds[hold+lines]);
        cell++; /*GD */
        lines++;
        i++;
    }

}

void Prog(Program *p)
{

    if(!strsame(p->wds[p->cw], "{")){
        ERROR("No opening brace.");
        }

    p->cw = p->cw + 1;

    Code(p);
}

void Code(Program *p)
{
    
    /* Recursive base case - terminates with abort or } */
    if(strsame(p->wds[p->cw], "}") || strsame(p->wds[p->cw-1], "ABORT") ){
        return;
        }

    Statement(p);
    p->cw = p->cw + 1;
    Code(p);
}

void Statement(Program *p)
{

    if(strsame(p->wds[p->cw], "ABORT")){
        return;
        }

    if(strsame(p->wds[p->cw], "ONE")){
        return;
        }

    if(strsame(p->wds[p->cw], "NOUGHT")){
        return;
        }
    
    if(strsame(p->wds[p->cw], "PRINT")){

        if(p->wds[p->cw+1][0] == '"'){
            p->cw = p->cw+2;
            return;
        }

        else{
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