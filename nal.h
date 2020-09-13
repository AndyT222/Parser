#define MAXNUMTOKENS    1000
#define TESTVALUE       1000
#define MAXTOKENSIZE    500
#define MAXRAND         100
#define HUNDRED         100
#define MAXFILES        10

#define INTERP          1
#define PARSE           0

#define PROGNAME "escape211.nal"

#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2); }

struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw;
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

void Prog(Program *p, Master* library, int mode, 
    Variables *usrvar, int *newf);
void Code(Program *p, Master* library, int mode, 
    Variables *usrvar, int *newf);
void Statement(Program *p, Master* library, int mode, 
    Variables *usrvar, int *newf);
void iterate(Program* prog, Master* library, 
    int* counter, char** c, char* masterfile);

/* Parser & Helper Functions */
void trimall(Program* prog);
void makestr(Program *prog, int i, char* test, char x);
void rot18(char* input);
int countchars(char* string, char c);
void getfiles(char** filenames, Program* prog, int* counter);
int checkendchar(char* str, char b);
int clearcheck(Program *prog);
void shiftclear(Program *prog);
int dupcheck(char** filenames, char* input);
int argcheck(int argc, char** argv);
void fileclear(char* file, Program* prog, Master* library);
int findfile(Master *library, char* filename);
char* findstr(Variables* usrvars, char* id);
void trimfiles(char* input);

/* Interpreter Functions */
void printstr(Program *p);
float findfloat(Variables* usrvars, char* id);
void findclosingbrace(Program *p);
void set(Program* p, int mode, Variables *usrvar);
void jump(Program* p, int mode);
void inc(Program* p, int mode, Variables *usrvar);
void innum(Program* p, int mode, Variables *usrvar, char* c);
int rnd();
void increment(Variables *usrvar, char* id);
void assignrnd(Program* p, int mode, Variables *usrvar);
void assignin2str(Program* p, int mode, Variables *usrvar);
void assignprint(Program* p, int mode, 
    Variables *usrvar, float f, char* c);
void ifequal(Program* p, int mode, 
    Variables *usrvar, char* c, char* k);
void ifgreater(Program* p, int mode, Variables *usrvar);
void file(Program* p, int mode, Variables *usrvar, int* newf, char* buffer, Master *library);
void addint(Variables *usrvar, char* id, float c);
void addstr(Variables *usrvar, char* id, char* c);

void testing();
void freeall(char** c);


