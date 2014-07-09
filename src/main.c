/*
 * main.c
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdlib.h>

#include "./net/net.h"
#include "./http/http_proc.h"

int main()
{
	//create socket server

		create_server_net("127.0.0.1", 8000, 20);
		strat_accept_request(http_func);

	return 0;
}

