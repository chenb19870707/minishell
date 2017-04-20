#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE + 1];
char avline[MAXLINE + 1];
char *lineptr;
char *avptr;

COMMAND cmd[PIPELINE]; 
char infile[MAXNAME + 1];
char outfile[MAXNAME + 1];

int cmd_count;
int backgnd ;
int append;
int lastpid;

int main()
{
    //处理信号
    setup();

    shell_loop();

    return 0;
}