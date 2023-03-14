#pragma once

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    int id;
    void (*function)(void *);
    void *args;
    int completed;
} Task;

typedef struct {
    Task* tasks;
    int tail;
    int size;
    int capacity;
} TaskQueue;

typedef struct {
    pthread_t id;
    int id_int;
    int waiting;
} Thread;

typedef struct {
    TaskQueue queue;
    Thread* threads;
    int should_exit;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int num_workers;
} ThreadPool;

void thread_pool_cleanup();
void thread_pool_init(int num_workers, int capacity);
void finished();
void add_task(Task task);
