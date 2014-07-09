/*
 * net.h
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */

#ifndef NET_H_
#define NET_H_

#define RECV_BUFF_LEN 1024
#define SEND_BUFF_LEN 4096

/* create server socket */
int create_server_net(char *strIp, short port, int listen_count);

/* accept link for client and process of proc_func */
int strat_accept_request(int (*proc_func)(char *, int, char *, int));

#endif /* NET_H_ */
