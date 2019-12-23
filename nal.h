#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 500
#define MAXRAND 100
#define MAXFILES 10

#define PROGNAME "test6.nal"
#define PROGNAME2 "test2.nal"

#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2); }

struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw; /* Current Word */
};
typedef struct prog Program;

struct variables{
    char usrwrd[MAXNUMTOKENS][MAXTOKENSIZE];
    char wrdid[MAXNUMTOKENS][MAXTOKENSIZE];

    float usrint[MAXNUMTOKENS];
    char intid[MAXNUMTOKENS][MAXTOKENSIZE];

    int intcount;
    int wrdcount;
};
typedef struct variables Variables;

struct master{
    Program files[MAXFILES];
    char filenames[MAXNUMTOKENS][MAXTOKENSIZE];
    int filecount; 
};
typedef struct master Master;

void printstr(Program *p);
void Prog(Program *p, Master* library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE]);
void Code(Program *p, Master* library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE]);
void Statement(Program *p, Master* library, int mode, Variables *usrvar, int *newf, int array[MAXTOKENSIZE]);

void makestr(Program *prog, int i, char* test, char x);
void getfiles(char** filenames, Program* prog, int* counter);
int checkchar(char* str, char b);
int clearcheck(Program *prog);
void shiftclear(Program *prog);

void rot18(char* input);
void fileclear(char* file, Program* prog, Master* library, int mode);
int findfile(Master *library, char* filename);
void trimfiles(char* input);
void freeall(Master* p);
float findfloat(Variables* usrvars, char* id);
void findclosingbrace(Program *p);
void increment(Variables *usrvar, char* id);
int rnd();

/* Interpreter Functions */
void printall(Program prog);
void testing();
void addint(Variables *usrvar, char* id, float c);
void addstr(Variables *usrvar, char* id, char* c);