/*
 * str_proc.c
 *
 *  Created on: 2014年4月24日
 *      Author: root
 */
#include "string_proc.h"

#include <string.h>
#include <stdlib.h>

char *get_word(const char *str, char *buff)
{
	char *psit = index(str, ' ');

	if (psit == NULL)
		strncpy(buff, str, strlen(str));
	else
	{
		int distance = STRING_DISTANCE(str, psit);
		strncpy(buff, str, distance);
	}

	return psit + 1;
}

int request_type_cmp(const char *str)
{
	static char request_type[][5] = {"GET", "POST", "\0"};
	int i = 0;

	while (request_type[i][0] != '\0')
	{
		if (0 == strcasecmp(str, request_type[i]))
			break;

		i++;
	}

	return i+1;
}

char *get_head_sit(const char *str, int len, int *distance)
{
	char *sep = strpbrk(str, SEPARATOR);
	*distance = STRING_DISTANCE(str, sep);

	if (*distance > len)
	{
		*distance = 0;
		return NULL;
	}

	char *pbuff = malloc(*distance+1);
	strncpy(pbuff, str, *distance);
	pbuff[*distance] = '\0';

	return pbuff;
}

phead_t analysis_head(const char *str)
{
	phead_t head;
	const char *pstart = NULL;
	char buff[128] = {0};

	pstart = str;

	pstart = get_word(pstart, buff);
	head.type = request_type_cmp(buff);
	bzero(buff, strlen(buff));

	pstart = get_word(pstart, buff);
	strncpy(head.path, buff, strlen(buff));
	bzero(buff, strlen(buff));

	head.version = 1; //ignore

	return head;
}
