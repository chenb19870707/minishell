#ifndef _DEF_H_
#define _DEF_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
  do \
  { \
    perror(m); \
	exit(EXIT_FAILURE); \
  } \
  while (0)

#define MAXLINE 1024 /*输入行最大长度 */
#define MAXARG 20    /*每个简单命令所带参数的最大个数*/
#define PIPELINE 5   /*一个管道行中最多简单命令个数*/
#define MAXNAME 100      /*IO重定向文件名的最大长度*/

struct COMMAND
{
    char *args[MAXARG + 1]; /*解析出来的命令参数列表*/
    int infd;
    int outfd;
};

#endif