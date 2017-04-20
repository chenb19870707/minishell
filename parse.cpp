#include "parse.h"
#include "init.h"
#include <stdio.h>
#include "extern.h"
#include "execute.h"
#include "builtin.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <fcntl.h>


void get_command(int i);
void getname(char *name);
void print_command();
int execute_command(void);

/*
 * shell 主循环
 */ 
void shell_loop(void)
{
    while(1)
    {
        init();
        if(-1 == read_command())
        {
            break;
        }
        parse_command();
        //print_command();
        execute_command();
    }
    printf("\nexit\n");
}

/*
 * 读取命令
 * 成功返回0，失败或者读取到文件结束符(EOF)返回-1
 */ 
int read_command(void)
{
    if(NULL == fgets(cmdline, MAXLINE , stdin))
    {
        return -1;
    }

    return 0;
}

/*
 * 解析命令
 * 成功返回0，失败返回-1
 */ 
int parse_command(void)
{
    /* cat < test.txt | grep -n public > test2.txt & */
    if(check("\n"))
        return 0;

	/* 判断是否内部命令并执行它 */
	if (builtin())
        return 0;
	
	/* 1、解析第一条简单命令 */
	get_command(0);
	/* 2、判定是否有输入重定向符 */
	if (check("<"))
		getname(infile);
	/* 3、判定是否有管道 */
    int i;
    for(i = 1;i < PIPELINE;++i)
    {
        if(check("|"))
            get_command(i);
        else
            break;
    }
    /* 4、判定是否有输出重定向符 */
    if(check(">"))
    {
        if(check(">"))
        {
            append = 1;
        }
        getname(outfile);
    }
        
    /* 5、判定是否后台作业 */
    if(check("&"))
        backgnd = 1;
    /* 6、判定命令结束 \n */
    if(check("\n"))
    {
        cmd_count = i;
        return cmd_count;
    }
    else
    {
        fprintf(stderr,"Command line syntax eroor\n");
        return -1;
    }
}

/*
 * 执行命令
 * 成功返回0，失败返回-1
 */ 
int execute_command(void)
{
	/* 先判定是否内部命令 */

	return execute_disk_command();
}

/*
 * 解析简单命令至cmd[i]
 * 提取cmdline中的命令参数到avline数组中，
 * 并且将COMMAND结构中的args[]中的每个指针指向这些字符串
 */
void get_command(int i)
{
	/*   cat < test.txt | grep -n public > test2.txt & */

	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		/* 去除空格 */
		while (*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

		/* 将第i条命令第j个参数指向avptr */
		cmd[i].args[j] = avptr;
/*		cmd[i].infd = 0;
        cmd[i].outfd = 1;*/

		printf("infd %d,outfd %d \n",cmd[i].infd ,cmd[i].outfd);

		/* 提取参数 */
		while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '>'
			&& *lineptr != '<'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
                /* 参数提取至avptr指针所向的数组avline */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0';
		switch (*lineptr)
		{
		case ' ':
		case '\t':
			inword = 0;
			j++;
			break;
		case '<':
		case '>':
		case '|':
		case '&':
		case '\n':
			if (inword == 0)
			cmd[i].args[j] = NULL;
			return;
		default: /* for '\0' */
			return;
		}
	}
}

/*
 * 将lineptr中的字符串与str进行匹配
 * 成功返回1，lineptr移过所匹配的字符串
 * 失败返回0，lineptr保持不变
 */
int check(const char *str)
{
    char *p;
    /* 去除空格 */
    while (*lineptr == ' ' || *lineptr == '\t')
        lineptr++;

    p = lineptr;

    while(*str != '\0' && *str == *p)
    {
        str++;
        p++;
    }

    if(*str == '\0' )
    {
        /* lineptr移过所匹配的字符串 */
        lineptr = p;
        return 1;
    }
    else
    {
        /* lineptr保持不变 */
        return 0;
    }
}

void getname(char *name)
{
    /* 去除空格 */
    while (*lineptr == ' ' || *lineptr == '\t')
        lineptr++;

    while (*lineptr != '\0'
    && *lineptr != ' '
    && *lineptr != '\t'
    && *lineptr != '>'
    && *lineptr != '<'
    && *lineptr != '|'
    && *lineptr != '&'
    && *lineptr != '\n')
    {
        *name++ = *lineptr++;
    }

    *name = '\0';
}

void print_command()
{
	int i;
	int j;
	printf("cmd_count = %d\n", cmd_count);
	if (infile[0] != '\0')
		printf("infile=[%s]\n", infile);
	if (outfile[0] != '\0')
		printf("outfile=[%s]\n", outfile);

	for (i=0; i<cmd_count; ++i)
	{
		j = 0;
		while (cmd[i].args[j] != NULL)
		{
			printf("[%s] ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
	}
}

