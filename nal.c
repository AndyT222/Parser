#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

void findclosingbrace(Program *p)
{
    while(p->wds[p->cw][0] != '}')
    {
        p->cw = p->cw+1;
    }
    return;
}

float findfloat(Variables* usrvars, char* id)
{
    int i = 0;

    while (i < usrvars->intcount)
    {
        if(strsame(id, usrvars->intid[i]))
        {
            return(usrvars->usrint[i]);
        }

        i++;
    }
    
    ERROR("Unable to amend intended variable.");
}

int rnd()
{
    int result;
    result = rand()%MAXRAND;
    return result;
}

int findfile(Master *library, char* filename)
{
    int i = 0;

    while(i < library->filecount)
    {
        if(strsame(library->filenames[i], filename))
        {
            return i;
        }

        i++;
    }

    return -1;
}

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

void getfiles(char** filenames, Program* prog, int* counter)
{
    int i = 0;

    while(prog->wds[i][0] != '\0')
    {
        if(strsame(prog->wds[i], "FILE") && dupcheck(filenames, prog->wds[i+1]) == 0)
        {
            filenames[*counter] = calloc(1, sizeof(char)*MAXTOKENSIZE);
            strcpy(filenames[*counter], prog->wds[i+1]);
            printf("GET FILES: [%d - %s] \n", *counter, filenames[*counter]);
            *counter = *counter + 1;
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

void fileclear(char* file, Program* prog, Master* library, int mode)
{
    FILE *fp = NULL; 
    int i;
    char *test;

    /* Just there to get rid of flags */
    mode = mode +1;
    mode = mode -1;

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

    strcpy(library->filenames[library->filecount], file);

    library->files[library->filecount] = *prog;
    library->filecount = library->filecount+1;

}

void testing()
{
    assert(checkchar("test", 'e') == 0);
    assert(checkchar("test",'"') == 0);
    assert(checkchar("tes9",'"') == 0);
    assert(checkchar("tes\"",'"') == 1);
}

void addstr(Variables *usrvar, char* id, char* c)
{
    strcpy(usrvar->usrwrd[usrvar->wrdcount], c);
    strcpy(usrvar->wrdid[usrvar->wrdcount], id);
    usrvar->wrdcount = usrvar->wrdcount+1;
}

void addint(Variables *usrvar, char* id, float c)
{
    int i = 0;

    while(i < usrvar->intcount)
    {
        if(strsame(id, usrvar->intid[i]))
        {
            usrvar->usrint[i] = c;
            strcpy(usrvar->intid[i], id);
            return;
        }

        i++;
    }

    usrvar->usrint[usrvar->intcount] = c;
    strcpy(usrvar->intid[usrvar->intcount], id);
    usrvar->intcount = usrvar->intcount+1;
}

void increment(Variables *usrvar, char* id)
{
    int i = 0;

    while(i < usrvar->intcount)
    {
        if(strsame(id, usrvar->intid[i]))
        {
            usrvar->usrint[i] = usrvar->usrint[i]+1;
            return;
        }

        i++;
    }

    return;
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

void makestr(Program *prog, int i, char* test, char x)
{
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

void Prog(Program *p, Master *library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE])
{   
    if(!strsame(p->wds[p->cw], "{"))
    {
        ERROR("No opening brace.");
    }

    p->cw = p->cw + 1;

    Code(p, library, mode, usrvar, newf, array);
}

void Code(Program *p, Master *library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE])
{
    /* Recursive base case - terminates with abort or } */
    if(strsame(p->wds[p->cw], "}"))
    {
        return;
    }

    if(strsame(p->wds[p->cw], "ABORT") && mode == 1)
    {
        return;
    }

    Statement(p, library, mode, usrvar, newf, array);
    p->cw = p->cw + 1;
    Code(p, library, mode, usrvar, newf, array);
}

/* Needs to be massively trimmed */
void Statement(Program *p, Master *library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE])
{
    int linej;
    int temp;
    int hold;
    char* buffer = calloc(1,sizeof(char)*MAXTOKENSIZE);
    char* c = calloc(1,sizeof(char)*MAXTOKENSIZE);
    float f; float g;

    /* Below two statements for variable 
    assignment */
    if(p->wds[p->cw][0] == '$')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            if(mode == 1)
            {
            addstr(usrvar, p->wds[p->cw], p->wds[p->cw+2]);
            printf("DECLARED STR: %s %s \n", usrvar->wrdid[usrvar->wrdcount-1], usrvar->usrwrd[usrvar->wrdcount-1]);
            }

            p->cw = p->cw+2;
            return;
        }
    }

    if(p->wds[p->cw][0] == '%')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            if(mode == 1)
            {
            addint(usrvar, p->wds[p->cw], atof(p->wds[p->cw+2]));
            printf("DECLARED FLT: %s %f \n", usrvar->intid[usrvar->intcount-1], usrvar->usrint[usrvar->intcount-1]);
            }

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
            if(mode == 1)
            {
                if(p->wds[p->cw+2][0] == '%')
                { 
                    f = findfloat(usrvar, p->wds[p->cw+2]);
                }
                    else
                    {
                        f = atof(p->wds[p->cw+2]);
                    }
                
                if(p->wds[p->cw+4][0] == '%')
                { 
                    g = findfloat(usrvar, p->wds[p->cw+4]);
                }

                    else
                    {
                        g = atof(p->wds[p->cw+4]);
                    }

                if(f == g)
                {
                    p->cw = p->cw+6;
                    return;
                }

                else
                {
                    findclosingbrace(p);
                    return;
                }

            }

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
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+5][0] == ')' && p->wds[p->cw+3][0] && p->wds[p->cw+3][0] == ',' &&
            p->wds[p->cw+6][0] == '{')
        {   
            if(mode == 1)
            {
                if(p->wds[p->cw+2][0] == '%')
                { 
                    f = findfloat(usrvar, p->wds[p->cw+2]);
                }

                    else
                    {
                        f = atof(p->wds[p->cw+2]);
                    }
                
                if(p->wds[p->cw+4][0] == '%')
                { 
                    g = findfloat(usrvar, p->wds[p->cw+4]);
                }

                    else
                    {
                        g = atof(p->wds[p->cw+4]);
                    }

                if(f > g)
                {
                    p->cw = p->cw+6;
                    return;
                }

                else
                {
                    findclosingbrace(p);
                    return;
                }
                
            }

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

        if(p->wds[p->cw+1][0] == '%')
        {
            addint(usrvar, p->wds[p->cw+1], atof(p->wds[p->cw+2]));
            printf("SET: %s %f \n", usrvar->intid[usrvar->intcount-1], usrvar->usrint[usrvar->intcount-1]);
        }

        p->cw = p->cw + 2;
        return;
    }

    if(strsame(p->wds[p->cw], "INC"))
    {
        if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ')')
        {   
            if(mode == 1)
            {
                increment(usrvar, p->wds[p->cw+2]);
            }

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
            if(mode == 1)
            {
                printf("RND ");
                temp = rnd();
                addint(usrvar, p->wds[p->cw+2], temp);
            }

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
            if(mode == 1)
            {
                printf("INNUM: ");
                scanf("%s", c);
                f = atof(c);
                addint(usrvar, p->wds[p->cw+2], f);
                free(c);
            }

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

        printf("FILE ");

        if(p->wds[p->cw+1][0] == '"')
        {   
            printf("OPENING FILE: %s \n", p->wds[p->cw+1]);

            if(mode == 1)
            {
                strcpy(buffer, p->wds[p->cw+1]);
                trimfiles(buffer);
                *newf = findfile(library, buffer);

                printf("\n%d \n", *newf);
                hold = p->cw+1;
                library->files[*newf].cw = 0;
                free(buffer);

                Prog(&library->files[*newf], library, 1, usrvar, newf, array);
                printf("\n");
            }

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

        if(mode == 1)
        {
            p->cw = linej-1;
            return;
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
            if(mode == 1)
            {
                printf("%s\n", p->wds[p->cw+1]);
            }

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
