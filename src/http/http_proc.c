/*
 * http_proc.c
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */
#include "http_proc.h"
#include "string_proc.h"

//#include <regex.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

static int request_func(phead_t head, char *buff, int buff_len);
static int get_data(const char *path, char *buff, int buff_len);
static int get_execl_data(const char *path, char *buff, int buff_len);
static int get_html_data(const char *path, char *buff, int buff_len);


/* return send msg length */
int http_func(char *recv_buff, int recv_len, char *buff, int buff_len)
{
	int distance = 0;
	char agent[64] = {0};
	char *pstart = NULL;

	if ((pstart = get_head_sit(recv_buff, recv_len, &distance)) == NULL)
		return -1;

	strncpy(agent, pstart, distance);
	return request_func(analysis_head(agent), buff, buff_len);
}

int request_func(phead_t head, char *buff, int buff_len)
{
	int result = 0;  //return length if true.

	switch (head.type)
	{
	case REQUEST_GET:
	case REQUEST_POST:
		result = get_data(head.path, buff, buff_len);
		break;
	default:
		perror("request format error!");
		result = -1;
	}

	return result;
}

int get_data(const char *path, char *buff, int buff_len)
{
	int length = -1;
	char full_path[256] = DEFAULT_PATH;

	if (strncmp(path, "/", strlen(path)) == 0)
		strcat(full_path, "/index.html");
	else
		strncat(full_path, path, strlen(path));

	printf("request file path is --- %s\n", full_path);

	struct stat sta;
	if (stat(full_path, &sta) == 0)
	{
		if ((sta.st_mode & 0100) != 0)
			/* executable file */
			length = get_execl_data(full_path, buff, buff_len);
		else
			length = get_html_data(full_path, buff, buff_len);
	}

	return length;
}

int get_execl_data(const char *path, char *buff, int buff_len)
{
	int length = 0, pid = -1, status = 0;
	int pipe_id[2];

	printf("into get excel msg function.!!!!!!\n");

	if (pipe(pipe_id) == -1)
		return -1;

	pid = fork();

	if (pid == -1)
	{
		close(pipe_id[0]);
		close(pipe_id[1]);
		return -1;
	}
	else if (pid == 0)
	{
		close(pipe_id[0]);

		dup2(pipe_id[1], STDOUT_FILENO);
		execl(path, "who am i?", NULL);

		perror("child process execl error!!!!!!!!!");
	}
	else
	{
		close(pipe_id[1]);

		if (waitpid(pid, &status, WUNTRACED) == -1)
			printf("child process is error! error:%d\n", WEXITSTATUS(status));
		else
			length = read(pipe_id[0], buff, buff_len);

		printf("======I get buff is :%s\n", buff);

		close(pipe_id[0]);
	}

	return length;
}

int get_html_data(const char *path, char *buff, int buff_len)
{
	int fid, length = 0;

	printf("into get html file fucntion.!!!!!!!!\n");

	if ((fid = open(path, O_RDONLY)) == -1)
		return -1;

	length = read(fid, buff, buff_len);
	close(fid);

	return length;
}



