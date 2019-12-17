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
void makestr(Program prog, int i);
int checkchar(char* str, char b);

int main(void)
{

    int i, j;
    FILE *fp;
    Program prog;

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

    while(i< MAXNUMTOKENS)
    {
        if(prog.wds[i][0] == '"')
        {
            makestr(prog, i);
        }

        i++;
    }

        assert(checkchar("test",'"') == 0);
        assert(checkchar("tes\"",'"') == 1);
        
        Prog(&prog);
        printf("Parsed OK\n");

    return 0;
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

void makestr(Program prog, int i){

    int j = 1;
    char *test = malloc(sizeof(char)*100);

    strcpy(test, prog.wds[i]);

    while(j < MAXNUMTOKENS){

        if(checkchar(prog.wds[i+j],'"'))
        {
            sprintf(test, "%s %s", test, prog.wds[i+j]);
            printf("%s", test);
            return;
        }

        if(checkchar(prog.wds[i+j],'"') == 0)
        {
            sprintf(test, "%s %s", test, prog.wds[i+j]);
            j++;
        }

    }

    ERROR("Unable to find closing quotes.");

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
        printf("1\n");
        return;
        }

    if(strsame(p->wds[p->cw], "NOUGHT")){
        printf("0\n");
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