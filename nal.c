#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "nal.h"

int dupcheck(char** filenames, char* input)
{
    int i = 0;
    char* temp = input;
    trimfiles(temp);

    while(filenames[i] != NULL)
    {
        if(strsame(filenames[i], temp))
        {
            return 1;
        }

        i++;
    }

    return 0;
}

void getfiles(char** filenames, Program* prog)
{
    int i = 0; static int j = 0;

    while(prog->wds[i][0] != '\0')
    {
        if(strsame(prog->wds[i], "FILE") && dupcheck(filenames, prog->wds[i+1]) == 0)
        {
            filenames[j] = prog->wds[i+1];
            trimfiles(filenames[j]);
            printf("RESULT IS ___ [%s] \n", filenames[j]);
            j++;
        }

        i++;
    }

}

void trimfiles(char* input)
{
    int i = 0; int j = 0;
    char* output = calloc(1, sizeof(char)*MAXTOKENSIZE);

    while(input[i] != '\0')
    {
        if(input[i] != '"')
        {
            output[j] = input[i];
            j++;
        }

        i++;
    }

    strcpy(input, output);
    free(output);
}

void fileclear(char* file, Program* prog, Master* library)
{
    FILE *fp = NULL; 
    int i;
    char *test;
    fp = fopen(file, "r");

    if(fp == NULL)
    {
        ERROR("File open error");
    }

    else
    {
        for(i=0; i<MAXNUMTOKENS; i++)
        {
            prog->wds[i][0] = '\0';
        }
    }

    test = malloc(sizeof(char)*MAXTOKENSIZE);
    i = 0;

    while(fscanf(fp, "%s", prog->wds[i++]) == 1 && i<MAXNUMTOKENS)
    {
        assert(i<MAXNUMTOKENS);
    }

    /* Data Validation - Tokenisation */
    i = 0;
    while(i< MAXNUMTOKENS)
    {
        if(prog->wds[i][0] == '"')
        {
            if(checkchar(prog->wds[i], '"') < 1)
            {
                makestr(prog, i, test,'"');
                strcpy(prog->wds[i], test);
            }
        }

        if(prog->wds[i][0] == '#')
        {
            if(checkchar(prog->wds[i], '#') < 1)
            {
                makestr(prog, i, test,'#');
                rot18(test);
                strcpy(prog->wds[i], test);
            }
        }

        i++;
    }

    free(test);
    shiftclear(prog);
    fclose(fp);

    library->files[library->filecount] = *prog;
    library->filecount = library->filecount+1;

}

/* Testing of helper functions */
void testing()
{
    assert(checkchar("test", 'e') == 0);
    assert(checkchar("test",'"') == 0);
    assert(checkchar("tes9",'"') == 0);
    assert(checkchar("tes\"",'"') == 1);
}

void addint(Variables *usrvar, int c)
{
    usrvar->usrint[usrvar->intcount] = c;
    usrvar->intcount = usrvar->intcount+1;
}

void printall(Program prog)
{
    int i = 0;

    printf("PRINT ALL ELEMENTS: \n");

    while(prog.wds[i][0] != '\0')
    {
        printf("[%s] [%d] \n", prog.wds[i], i);
        i++;
    }
}

/* Note only checks end char! */
int checkchar(char* str, char b)
{
    int i = 0;
    int count = 0;

    while(str[i] != '\0')
    {
        if(str[i] == b && str[i+1] == '\0')
        {
            count++;
        }

        i++;
    }

    return count;
}

int clearcheck(Program *prog)
{
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

    i = 0;

    while(prog->wds[i][0] != '\0')
    {

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

void makestr(Program *prog, int i, char* test, char x){

    int j = 1;

    strcpy(test, prog->wds[i]);

    while(j < MAXNUMTOKENS)
    {
        if(checkchar(prog->wds[i+j],x))
        {
            sprintf(test, "%s %s", test, prog->wds[i+j]);
            strcpy(prog->wds[i], test);
            strcpy(prog->wds[i+j], "CLEARED");
            return;
        }

        if(checkchar(prog->wds[i+j],x) == 0)
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

    free(temp);
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

    /* Below two statements for variable 
    assignment */
    if(p->wds[p->cw][0] == '$')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            p->cw = p->cw+2;
            return;
        }
    }

    if(p->wds[p->cw][0] == '%')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            p->cw = p->cw+2;
            return;
        }
    }

    if(strsame(p->wds[p->cw], "IN2STR"))
    {
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ',' && p->wds[p->cw+5][0] == ')')
        {   
            p->cw = p->cw+5;
            return;
        }

        else
        {
            ERROR("Error within INT2STR function.");
        }
    }

    if(strsame(p->wds[p->cw], "IFEQUAL"))
    {

        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ',' &&
            p->wds[p->cw+5][0] == ')' &&
            p->wds[p->cw+6][0] == '{')
        {   
            p->cw = p->cw+6;
            return;
        }

        else
        {
            ERROR("Error within IFEQUAL function.");
        }

        return;
    }

    if(strsame(p->wds[p->cw], "IFGREATER"))
    {

        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+5][0] == ')' && p->wds[p->cw+3][0] &&
            p->wds[p->cw+6][0] == '{')
        {   
            p->cw = p->cw+6;
            return;
        }

        else
        {
            ERROR("Opening bracket missing for IFGREATER.");
        }

    }

    if(strsame(p->wds[p->cw], "SET"))
    {
        if(p->wds[p->cw+1][0] != '$')
        {
            if(p->wds[p->cw+1][0] != '%')
            {
                ERROR("Invalid set condition.");
            }
        }

        p->cw = p->cw + 2;
        return;
    }

    if(strsame(p->wds[p->cw], "INC"))
    {
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ')')
        {   
            p->cw = p->cw+3;
            return;
        }

        else
        {
            ERROR("Error within INC function.");
        }
    }

    if(strsame(p->wds[p->cw], "RND"))
    {
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ')')
        {   
            p->cw = p->cw+3;
            return;
        }

        else
        {
            ERROR("Error within RND function.");
        }
    }

    if(strsame(p->wds[p->cw], "INNUM"))
    {
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ')')
        {   
            p->cw = p->cw+3;
            return;
        }

        else
        {
            ERROR("Error within INNUM function.");
        }
    }

    if(strsame(p->wds[p->cw], "FILE"))
    {
        if(p->wds[p->cw+1][0] == '"')
        {   
            printf("OPEN: %s \n", p->wds[p->cw+1]);
            p->cw = p->cw+1;
            return;
        }

        else
        {
            ERROR("Missing opening bracket quotes for FILE.");
        }

        return;
    }

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

        p->cw = p->cw+1;
        return;
    }
    
    if(strsame(p->wds[p->cw], "PRINT") || strsame(p->wds[p->cw], "PRINTN"))
    {

        if(p->wds[p->cw+1][0] == '%')
        {   
            p->cw = p->cw+1;
            return;
        }

        if(p->wds[p->cw+1][0] == '$')
        {   
            p->cw = p->cw+1;
            return;
        }

        if(p->wds[p->cw+1][0] == '"')
        {   
            p->cw = p->cw+1;
            return;
        }

        if(p->wds[p->cw+1][0] == '#')
        {   
            p->cw = p->cw+1;
            return;
        }

        ERROR("Opening quotation missing for print statement.");

    }

    ERROR("Expecting a ONE or NOUGHT ?");
}

void rot18(char* input)
{
    int i = 0;
    char* output = calloc(1,sizeof(char)*MAXTOKENSIZE);

    while(input[i] != '\0')
    {
        if(input[i] == '#')
        {
            output[i] = '"';
        }

        if(isalpha(input[i]) == 0 && input[i] != '#')
        {
            output[i] = input[i];
        }

        if(input[i] >= '0' && input[i] <= '4')
        {
            output[i] = input[i] + 5;
        }

        if(input[i] >= '5' && input[i] <= '9')
        {
            output[i] = input[i] - 5;
        }

        /* These can be put together */
        if(input[i] <= 'z' && input[i] > 'm')
        {
            output[i] = input[i] - 13;
        }

        if(input[i] >= 'a' && input[i] <= 'm')
        {
            output[i] = input[i] + 13;
        }

        if(input[i] <= 'Z' && input[i] > 'M')
        {
            output[i] = input[i] - 13;
        }

        if(input[i] >= 'A' && input[i] <= 'M')
        {
            output[i] = input[i] + 13;
        }

        i++;
    }

    strcpy(input, output);
    free(output);
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
