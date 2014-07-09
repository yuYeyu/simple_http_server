/*
 * thread_pool.c
 *
 *  Created on: 2014年4月28日
 *      Author: root
 */
#include <pthread.h>
#include <strings.h>
#include <stdlib.h>

#include "thread_pool.h"

threadpool_t *threadpool_create(int thread_num)
{
	threadpool_t *tp = malloc(sizeof(threadpool_t));
	bzero(tp, sizeof(threadpool_t));

	tp->thread_num = thread_num;
	threadpool_init(tp);

	return tp;
}

void threadpool_init(threadpool_t *tp)
{
	tp->threadpool_state = 1;
	tp->thread_work_num = 0;
	tp->job_head = tp->job_tail = NULL;

	tp->pid = malloc(sizeof(pthread_t) * tp->thread_num);

	pthread_mutex_init(&tp->mutex, NULL);
	pthread_cond_init(&tp->cond, NULL);

	pthread_mutex_init(&tp->queue_mutex, NULL);

	int i = 0;
	for (; i < tp->thread_num; i++)
	{
		pthread_create(&tp->pid[i], NULL, threadpool_work_thread, tp);
		tp->thread_work_num ++;
	}
}

void *threadpool_work_thread(void *tp)
{
	threadpool_t *p = (threadpool_t *)tp;

	void *arg;
	void (*routine)(void *);

	threadpool_job_t *job;

	while (1)
	{
		pthread_mutex_lock(&p->mutex);

		pthread_cond_wait(&p->cond, &p->mutex);

		pthread_mutex_unlock(&p->mutex);

		if (p->threadpool_state == 0)
		{
			p->thread_work_num --;
			break;
		}

		job = threadpool_pop_job(p);

		if (job != NULL)
		{
			routine = job->routine;
			arg = job->arg;

			routine(arg);

			free(job);
			job = NULL;
		}
	}

	return NULL;
}

threadpool_job_t *threadpool_pop_job(threadpool_t *tp)
{
	threadpool_job_t *job = NULL;

	pthread_mutex_lock(&tp->queue_mutex);

	if (tp->job_head == NULL)
		return NULL;

	job = tp->job_head;
	tp->job_head = tp->job_head->next;

	if (tp->job_head == NULL)
		tp->job_tail = NULL;

	pthread_mutex_unlock(&tp->queue_mutex);

	return job;
}

void threadpool_push_job(threadpool_t *tp, threadpool_job_t *job)
{
	threadpool_job_t *job2 = NULL;

	pthread_mutex_lock(&tp->queue_mutex);

	if (tp->job_head == NULL)
	{
		tp->job_head = tp->job_tail = job;
		tp->job_tail->next = NULL;
	}
	else
	{
		job2 = tp->job_tail->next;
		tp->job_tail->next = job;
		tp->job_tail = job;
		tp->job_tail->next = job2;
	}

	pthread_mutex_unlock(&tp->queue_mutex);
	pthread_cond_signal(&tp->cond);
}

void threadpool_distroy(threadpool_t *tp)
{
	tp->threadpool_state = 0;

	int i = 0;
	for (; i < tp->thread_num; i++)
		pthread_join(tp->pid[i], NULL);

	free(tp->pid);
	pthread_mutex_destroy(&tp->mutex);
	pthread_cond_destroy(&tp->cond);
	pthread_mutex_destroy(&tp->queue_mutex);
}
