/*
 * str_proc.h
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */

#ifndef STR_PROC_H_
#define STR_PROC_H_

#define SEPARATOR 											"\r\n"

#define REQUEST_GET 										1
#define REQUEST_POST 									2
#define REQUEST_ERROR									3

/* pchar1, pchar2 in one string. */
#define STRING_DISTANCE(pchar1, pchar2) (pchar1 > pchar2 ? pchar1 - pchar2 : pchar2 - pchar1)

typedef struct head_t
{
	char path[256];
	int type;
	int version;
}phead_t;


char *get_head_sit(const char *str, int len, int *distance);

phead_t analysis_head(const char *str);

#endif /* STR_PROC_H_ */
