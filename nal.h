#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 500
#define MAXFILES 10

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
    int wrdcount;

    int usrint[MAXNUMTOKENS];
    int intcount;
};
typedef struct variables Variables;

struct master{
    Program files[MAXFILES];
    int filecount; 
};
typedef struct master Master;

void printstr(Program *p);
void Prog(Program *p);
void Code(Program *p);
void Statement(Program *p);

void makestr(Program *prog, int i, char* test, char x);
void getfiles(char** filenames, Program* prog);
int checkchar(char* str, char b);
int clearcheck(Program *prog);
void shiftclear(Program *prog);
char* rot18(char* input);

void fileclear(char* file, Program* prog, Master* library);
char* trimfiles(char* input);
void freeall(Master* p);

/* Interpreter Functions */
void printall(Program prog);
void testing();
void addint(Variables *usrvar, int c);