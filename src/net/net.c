/*
 * net.c
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "net.h"
#include "../thread_pool/thread_pool.h"

static void request_func(void *arg);


typedef struct parameter{
	int sock;
	struct sockaddr_in addr;
	int (*func)(char *, int, char *, int);
}parameter_t;


int server_sock = -1;


void request_func(void *arg)
{
	parameter_t *par = (parameter_t *)arg;

	/* improve to memory pool */
	int recv_len = 0, send_len = SEND_BUFF_LEN, send_result = 0;
	char *recv_buff = malloc(RECV_BUFF_LEN);
	bzero(recv_buff, RECV_BUFF_LEN);
	char *send_buff = malloc(SEND_BUFF_LEN);
	bzero(send_buff, SEND_BUFF_LEN);

	recv_len = recv(par->sock, recv_buff, RECV_BUFF_LEN, 0);
	int result = par->func(recv_buff, recv_len, send_buff, send_len);

	if (-1 != result)
	{
		printf("I'm send msg!!!------msg:\n%s\n------\nlen is %d\n", send_buff, result);

		/* if not -1, as send length */
		if ((send_result = send(par->sock, send_buff, result, 0)) == -1)
			printf("send msg is error!!!!!  errno:%d\n", errno);
		else
			printf("send msg length is %d\n", send_result);
	}

	free(recv_buff);
	free(send_buff);
	free(par);

	recv_buff = NULL;
	send_buff = NULL;
	par = NULL;
	arg = NULL;
}

int create_server_net(char *strIp, short port, int listen_count)
{
	if (-1 == server_sock)
	{
		server_sock = socket(AF_INET, SOCK_STREAM, 0);

		int opt = 1; //port reuse
		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		inet_pton(AF_INET, strIp, &addr.sin_addr);
		addr.sin_port = htons(port);
		bind(server_sock, (struct sockaddr*)&addr, sizeof(addr));

		listen(server_sock, listen_count);

		return 0;
	}

	return -1;
}

int strat_accept_request(int (*proc_func)(char *, int, char *, int))
{
	parameter_t *par = NULL;
	socklen_t addrlen;
	threadpool_t *tp = NULL;
	threadpool_job_t *job = NULL;

	tp = threadpool_create(8);

	while (1)
	{
		par = malloc(sizeof(parameter_t));
		bzero(par, sizeof(parameter_t));

		if ((par->sock = accept(server_sock, (struct sockaddr*)&par->addr, &addrlen)) != -1)
			{
				par->func = proc_func;

				job = malloc(sizeof(threadpool_job_t));
				job->routine = request_func;
				job->arg = par;

				threadpool_push_job(tp, job);

				job = NULL;
			}
	}

	return 0;
}
