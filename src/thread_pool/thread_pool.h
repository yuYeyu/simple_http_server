/*
 * thread_pool.h
 *
 *  Created on: 2014年4月28日
 *      Author: root
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

typedef struct threadpool_job
{
	void (*routine)(void *);

	void *arg;

	struct threadpool_job *next;

}threadpool_job_t;

typedef struct threadpool
{
	//control start and end.
	int threadpool_state;

	int thread_num;

	int thread_work_num;

	pthread_t *pid;

	pthread_mutex_t mutex;

	pthread_cond_t cond;

	threadpool_job_t *job_head;

	threadpool_job_t *job_tail;

	pthread_mutex_t queue_mutex;

}threadpool_t;

threadpool_t *threadpool_create(int thread_num);

void threadpool_init(threadpool_t *tp);

void *threadpool_work_thread(void *tp);

threadpool_job_t *threadpool_pop_job(threadpool_t *tp);

void threadpool_push_job(threadpool_t *tp, threadpool_job_t *job);

void threadpool_distroy(threadpool_t *tp);

#endif /* THREAD_POOL_H_ */
