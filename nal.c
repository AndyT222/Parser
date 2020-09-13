#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "nal.h"

int argcheck(int argc, char** argv)
{
   if(argv[1] == NULL || argc < 1)
    {
        ERROR("No filename entered.");
    }
    else
    {
        return 0;
    }
}

void iterate(Program* prog, Master* library, int* counter, 
    char** c, char* masterfile)
{
    int i = 0;

    fileclear(masterfile, prog, library);
    getfiles(c, prog, counter);

    while(c[i] != NULL)
    {
        fileclear(c[i], prog, library);
        getfiles(c, prog, counter);
        i++;
    }
}

void in2str(Variables* usrvar, char* id, char* id2)
{
    char* buffer = calloc(1, sizeof(char)*MAXTOKENSIZE);
    char* buffer2 = calloc(1, sizeof(char)*MAXTOKENSIZE);
    int check1 = 0; int check2 = 0; int i = 0;
    
    scanf("%s %s", buffer, buffer2);

    if(buffer == NULL || buffer2 == NULL)
    {
        return;
    }

    if(buffer[0] == '#')
    {
        rot18(buffer);
        trimfiles(buffer);
    }

    if(buffer2[0] == '#')
    {
        rot18(buffer2);
        trimfiles(buffer2);
    }

    while (i < usrvar->wrdcount)
    {
        if(strsame(id, usrvar->wrdid[i]))
        {
            strcpy(usrvar->usrwrd[i], buffer);
            check1++;
        }

        if(strsame(id2, usrvar->wrdid[i]))
        {
            strcpy(usrvar->usrwrd[i], buffer2);
            check2++;
        }

        i++;
    }

    if(check1 == 0)
    {
        addstr(usrvar, id, buffer);
    }

    if(check2 == 0)
    {
        addstr(usrvar, id2, buffer2);
    }

    free(buffer2);
    free(buffer);

    return;
}

void findclosingbrace(Program *p)
{
    int opening = 0; int closing = 0;

    while(p->wds[p->cw][0] != '}')
    {
        if(p->wds[p->cw][0] == '{')
        {
            opening++;
        }

        p->cw = p->cw+1;
    }

    if(opening == 1)
    {
        return;
    }

    else{
        
        while(closing < opening)
        {
            if(p->wds[p->cw][0] == '{')
            {
                opening++;
            }

            if(p->wds[p->cw][0] == '}')
            {
                closing++;
            }

            p->cw = p->cw+1;
        }

        p->cw = p->cw-1;
        return;
    }
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

    return -1;  
}

char* findstr(Variables* usrvars, char* id)
{
    int i = 0;

    while (i < usrvars->wrdcount)
    {
        if(strsame(id, usrvars->wrdid[i]))
        {
            return(usrvars->usrwrd[i]);
        }

        i++;
    }

    return NULL;
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
    int i = 0; char* temp = input;

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
        if(strsame(prog->wds[i], "FILE") && 
            dupcheck(filenames, prog->wds[i+1]) == 0)
        {
            filenames[*counter] = calloc(1, sizeof(char)*MAXTOKENSIZE);
            strcpy(filenames[*counter], prog->wds[i+1]);
            *counter = *counter + 1;
        }

        i++;
    }
}

 
void trimfiles(char* input)
{
    int i = 0; int j = 0; 
    char* output = calloc(1, sizeof(char)*MAXTOKENSIZE);

    if(input[0] == '"' && input[1] == '"')
    {
        output[0] = ' ';
        strcpy(input, output);
        free(output);
        return;
    }

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
    FILE *fp = NULL; int i; char *test;
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
            if(checkendchar(prog->wds[i], '"') < 1)
            {
                makestr(prog, i, test,'"');
                strcpy(prog->wds[i], test);
            }
        }

        if(prog->wds[i][0] == '#')
        {
            if(checkendchar(prog->wds[i], '#') < 1)
            {
                makestr(prog, i, test,'#');
                rot18(test);
                strcpy(prog->wds[i], test);
            }
            else
            {
                if(countchars(prog->wds[i], '#') == 2)
                {
                    rot18(prog->wds[i]);
                }
            }
        }

        i++;
    }

    free(test);
    shiftclear(prog);
    trimall(prog);
    fclose(fp);

    strcpy(library->filenames[library->filecount], file);

    library->files[library->filecount] = *prog;
    library->filecount = library->filecount+1;
}

void trimall(Program* prog)
{
    int i = 0;
    char* buffer = calloc(1,sizeof(char)*MAXTOKENSIZE);

    while(i < MAXNUMTOKENS)
    {
        if(prog->wds[i][0] == '"')
        {
            strcpy(buffer, prog->wds[i]);
            trimfiles(buffer);
            strcpy(prog->wds[i], buffer);
        }

        i++;
    }

    free(buffer);
}

void addstr(Variables *usrvar, char* id, char* c)
{
    int i = 0;

    if(c == NULL)
    {
        return;
    }

    while(i < usrvar->wrdcount)
    {
        if(strsame(id, usrvar->wrdid[i]))
        {
            strcpy(usrvar->usrwrd[i], c);
            return;
        }

        i++;
    }

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

int countchars(char* string, char c)
{
  int i = 0; int count = 0;

  while(string[i] != '\0')
  {
      if(string[i] == c)
      {
          count++;
      }

      i++;
  }

return count;
}

int checkendchar(char* str, char b)
{
    int i = 0; int count = 0;

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
    int i = 0; int check = 0; int brace = 0;

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

        if(i < MAXNUMTOKENS-1 && strsame(prog->wds[i], "CLEARED") 
            && (strsame(prog->wds[i+1], "CLEARED") == 0 
            && prog->wds[i+1][0] != '\0'))
        {
            return 0;
        }

        if(i > 0 && strsame(prog->wds[i], "CLEARED") 
            && (strsame(prog->wds[i-1], "CLEARED") == 0 
            && prog->wds[i-1][0] != '}'))
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
    char* buffer = calloc(1,sizeof(char)*MAXTOKENSIZE);

    strcpy(test, prog->wds[i]);

    while(j < MAXNUMTOKENS)
    {
        if(checkendchar(prog->wds[i+j],x))
        {
            strcpy(buffer, test);
            sprintf(test, "%s %s", buffer, prog->wds[i+j]);
            strcpy(prog->wds[i], test);
            strcpy(prog->wds[i+j], "CLEARED");
            free(buffer);
            return;
        }

        if(checkendchar(prog->wds[i+j],x) == 0)
        {
            strcpy(buffer, test);
            sprintf(test, "%s %s", buffer, prog->wds[i+j]);
            strcpy(prog->wds[i+j], "CLEARED");
            j++;
        }

    }

    free(buffer);
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

        if(strsame(prog->wds[i], "CLEARED") && 
            prog->wds[i+1][0] != '\0')
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

void Prog(Program *p, Master *library, int mode, 
    Variables *usrvar, int *newf)
{   
    if(!strsame(p->wds[p->cw], "{"))
    {
        ERROR("No opening brace.");
    }

    p->cw = p->cw + 1;

    Code(p, library, mode, usrvar, newf);
}

void Code(Program *p, Master *library, int mode, 
    Variables *usrvar, int *newf)
{
    if(p->wds[p->cw][0] == '}' && p->wds[p->cw+1][0] == '\0')
    {
        return;
    }

    if(strsame(p->wds[p->cw], "ABORT") && mode == INTERP)
    {
        return;
    }

    Statement(p, library, mode, usrvar, newf);
    p->cw = p->cw + 1;
    Code(p, library, mode, usrvar, newf);
}

/* This is a long function but believe it's justified as
its primarily listing commands. */
void Statement(Program *p, Master *library, int mode, 
    Variables *usrvar, int *newf)
{
    char* buffer; char* c; char* k; float f = 0;

    if(p->wds[p->cw][0] == '}')
    {
        return;
    }

    if(mode == INTERP)
    {
        buffer = calloc(1,sizeof(char)*MAXTOKENSIZE);
        c = calloc(1,sizeof(char)*MAXTOKENSIZE);
        k = calloc(1,sizeof(char)*MAXTOKENSIZE);
    }

    if(p->wds[p->cw][0] == '$')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            if(mode == INTERP)
            {
                addstr(usrvar, p->wds[p->cw], 
                    p->wds[p->cw+2]);
            }

            p->cw = p->cw+2;
            return;
        }
    }

    if(p->wds[p->cw][0] == '%')
    {   
        if(p->wds[p->cw+1][0] == '=')
        {
            if(mode == INTERP)
            {
                addint(usrvar, p->wds[p->cw], 
                    atof(p->wds[p->cw+2]));
            }

            p->cw = p->cw+2;
            return;
        }
    }

    if(strsame(p->wds[p->cw], "IN2STR"))
    {
        assignin2str(p,mode,usrvar);
        return;
    }

    if(strsame(p->wds[p->cw], "IFEQUAL"))
    {
        ifequal(p,mode,usrvar,c,k);
        return;
    }

    if(strsame(p->wds[p->cw], "IFGREATER"))
    {
        ifgreater(p,mode,usrvar);
        return;
    }

    if(strsame(p->wds[p->cw], "SET"))
    {
        set(p,mode,usrvar);
        return;
    }

    if(strsame(p->wds[p->cw], "INC"))
    {
        inc(p, mode, usrvar);
        return;
    }

    if(strsame(p->wds[p->cw], "RND"))
    {
        assignrnd(p, mode, usrvar);
        return;
    }

    if(strsame(p->wds[p->cw], "INNUM"))
    {
        innum(p, mode, usrvar, c);
        return;
    }

    if(strsame(p->wds[p->cw], "FILE"))
    {
        file(p,mode,usrvar,newf,buffer,library);
        return;
    }

    if(strsame(p->wds[p->cw], "JUMP"))
    {
        jump(p, mode);
        return;
    }
    
    if(strsame(p->wds[p->cw], "PRINT") || 
        strsame(p->wds[p->cw], "PRINTN"))
    {
        assignprint(p, mode, usrvar, f, c);
        return;
    }

    if(strsame(p->wds[p->cw], "ABORT") && mode == PARSE)
    {
        return;
    }

    ERROR("Expecting a ONE or NOUGHT ?");
}

void assignprint(Program* p, int mode, Variables *usrvar, float f, char* c)
{
   if(p->wds[p->cw+1][0] == '%')
    {   
        if(mode == INTERP)
        {
            f = findfloat(usrvar, p->wds[p->cw+1]);

            if(strsame(p->wds[p->cw], "PRINT"))
            {
                printf("%.2f \n", f);
            }

            else
            {
                printf("%.2f ", f);
            }
        }

        p->cw = p->cw+1;
        return;
    }

    if(p->wds[p->cw+1][0] == '$')
    {   
        if(mode == INTERP)
        {
            c = findstr(usrvar, p->wds[p->cw+1]);
            
            if(strsame(p->wds[p->cw], "PRINT"))
            {
                printf("%s \n", c);
            }

            else
            {
                printf("%s ", c);
            }
        }

        p->cw = p->cw+1;
        return;
    }

    if(mode == INTERP)
    {

        if(strsame(p->wds[p->cw], "PRINT"))
        {
            printf("%s \n", p->wds[p->cw+1]);
        }

        else
        {
            printf("%s ", p->wds[p->cw+1]);
        }
        
    }

    p->cw = p->cw+1;
    return;
}

void file(Program* p, int mode, Variables *usrvar, int* newf, char* buffer, Master *library)
{
        if(p->wds[p->cw+1][0] != '\0')
        {   
            if(mode == INTERP)
            {
                strcpy(buffer, p->wds[p->cw+1]);
                *newf = findfile(library, buffer);
                library->files[*newf].cw = 0;
                free(buffer);
                Prog(&library->files[*newf], library, 1, usrvar, newf);
            }

            p->cw = p->cw+1;
            return;
        }

        else
        {
            ERROR("Missing opening bracket quotes for FILE.");
        }

}

void ifgreater(Program* p, int mode, Variables *usrvar)
{
    float f; float g;

    if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+5][0] == ')'
        && p->wds[p->cw+3][0] && p->wds[p->cw+3][0] == ',' &&
        p->wds[p->cw+6][0] == '{')
        {   
            if(mode == INTERP)
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

void ifequal(Program* p, int mode, Variables *usrvar, char* c, char* k)
{
    float f; float g;

    if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ',' &&
        p->wds[p->cw+5][0] == ')' &&
         p->wds[p->cw+6][0] == '{')
    {   
        if(mode == INTERP)
        {
            if(p->wds[p->cw+2][0] == '%')
            { 
                f = findfloat(usrvar, p->wds[p->cw+2]);
            }
                if(isalpha(p->wds[p->cw+2][0]) == 0 && 
                    p->wds[p->cw+2][0] != '$' && 
                    p->wds[p->cw+2][0] != '%')
                { 
                    f = atof(p->wds[p->cw+2]);
                }

                if(p->wds[p->cw+4][0] == '%')
                { 
                    g = findfloat(usrvar, p->wds[p->cw+4]);
                }

                if(isalpha(p->wds[p->cw+4][0]) == 0 && 
                    p->wds[p->cw+4][0] != '$' &&
                    p->wds[p->cw+4][0] != '%')
                { 
                    g = atof(p->wds[p->cw+4]);
                }

                /* If both are string variables */
                if((p->wds[p->cw+4][0] == '$') && 
                    (p->wds[p->cw+2][0] == '$'))
                { 
                    k = findstr(usrvar, p->wds[p->cw+2]);
                    c = findstr(usrvar, p->wds[p->cw+4]);

                    if(strsame(k,c))
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

                if(p->wds[p->cw+2][0] == '$')
                { 
                    k = findstr(usrvar, p->wds[p->cw+2]);

                    if(strsame(k,p->wds[p->cw+4]))
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

                if((int) f*HUNDRED == (int) g*HUNDRED)
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

}

void set(Program* p, int mode, Variables *usrvar)
{
    if(p->wds[p->cw+1][0] != '$')
    {
        if(p->wds[p->cw+1][0] != '%')
        {
            ERROR("Invalid set condition.");
        }
    }

    if(p->wds[p->cw+1][0] == '%' && mode == INTERP)
    {
        addint(usrvar, p->wds[p->cw+1], 
            atof(p->wds[p->cw+2]));
    }

    if(p->wds[p->cw+1][0] == '$' && mode == INTERP)
    {
        addstr(usrvar, p->wds[p->cw+1], p->wds[p->cw+2]);
    }

    p->cw = p->cw + 2;
    return;
}

void inc(Program* p, int mode, Variables *usrvar)
{
    if(p->wds[p->cw+1][0] == '(' && 
        p->wds[p->cw+3][0] == ')')
    {   
        if(mode == INTERP)
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

void assignin2str(Program* p, int mode, Variables *usrvar)
{
    if(p->wds[p->cw+1][0] == '(' && p->wds[p->cw+3][0] == ',' 
        && p->wds[p->cw+5][0] == ')')
    {   
        if(mode == INTERP)
        {
            in2str(usrvar, p->wds[p->cw+2], p->wds[p->cw+4]);
        }

        p->cw = p->cw+5;
        return;
    }

    else
    {
        ERROR("Error within INT2STR function.");
    }
}

void assignrnd(Program* p, int mode, Variables *usrvar)
{
    int temp;

    if(p->wds[p->cw+1][0] == '(' && 
        p->wds[p->cw+3][0] == ')')
    {   
        if(mode == INTERP)
        {
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

void innum(Program* p, int mode, Variables *usrvar, char* c)
{
    float f;

    if(p->wds[p->cw+1][0] == '(' && 
        p->wds[p->cw+3][0] == ')')
    {   
        if(mode == INTERP)
        {
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

void jump(Program* p, int mode)
{
    int linej = atoi(p->wds[p->cw+1]);
        
    if(linej > MAXNUMTOKENS || linej < 0)
    {
        ERROR("Invalid jump.");
    }

    if(mode == INTERP)
    {
        p->cw = linej-1;
        return;
    }

    p->cw = p->cw+1;
    return;
}

void rot18(char* input)
{
    int i = 0; char* output = calloc(1,sizeof(char)*MAXTOKENSIZE);

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

        if((input[i] <= 'z' && input[i] > 'm') || 
            (input[i] <= 'Z' && input[i] > 'M'))
        {
            output[i] = input[i] - 13;
        }

        if((input[i] >= 'a' && input[i] <= 'm') || 
            (input[i] >= 'A' && input[i] <= 'M'))
        {
            output[i] = input[i] + 13;
        }

        i++;
    }

    strcpy(input, output);
    free(output);
}

void freeall(char** c)
{
    int i = 0;

    while(c[i] != NULL)
    {
        free(c[i]);
        i++;
    }

}

void testing()
{
    int i; int j;
    char word[8] = "#Neil#";
    char word1[8] = "#ARVY#";
    char word2[8] = "\"test\"";
    char numbers[8] = "#2393#";

    rot18(word);
    assert(strsame("\"Arvy\"",word));

    rot18(numbers);
    assert(strsame("\"7848\"",numbers));

    rot18(word1);
    assert(strsame("\"NEIL\"",word1));

    trimfiles(word);
    assert(strsame("Arvy",word));

    trimfiles(word1);
    assert(strsame("NEIL",word1));

    trimfiles(word2);
    assert(strsame("test",word2));

    assert(checkendchar("test", 'e') == 0);
    assert(checkendchar("test",'"') == 0);
    assert(checkendchar("tes9",'"') == 0);
    assert(checkendchar("tes\"",'"') == 1);
    assert(checkendchar("Ges#",'#') == 1);
    assert(checkendchar("rand5",'5') == 1);

    assert(countchars("seventeen",'e') == 4);
    assert(countchars("test",'t') == 2);
    assert(countchars("#test",'#') == 1);
    assert(countchars("#test#",'#') == 2);
    assert(countchars("##",'#') == 2);

    j = 0;

    while(j < TESTVALUE)
    {
       i = rnd(); 
       assert(i<MAXRAND);
       assert(i>=0);
       j++;
    }

}