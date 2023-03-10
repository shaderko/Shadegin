#include "thread_pool.h"

static ThreadPool thread_pool = {0};

void thread_pool_cleanup() {
    thread_pool.should_exit = 1;
    pthread_cond_broadcast(&thread_pool.cond);

    for (int i = 0; i < thread_pool.num_workers; i++) {
        pthread_join(thread_pool.threads[i].id, NULL);
    }

    free(thread_pool.threads);
    free(thread_pool.queue.tasks);
}

int get_index() {
    int index = (thread_pool.queue.tail - thread_pool.queue.size) % thread_pool.queue.capacity;
    if (index < 0) {
        index += thread_pool.queue.capacity;
    }
    return index;
}

Task get_task() {
    Task task = {0};

    if (thread_pool.queue.size <= 0) {
        return task;
    }
    task = thread_pool.queue.tasks[get_index()];
    thread_pool.queue.size--;

    return task;
}

Thread* get_self(pthread_t thread_id) {
    for (int i = 0; i < thread_pool.num_workers; i++) {
        if (thread_id == thread_pool.threads[i].id) {
            return &thread_pool.threads[i];
        }
    }
    return NULL;
}

void* worker(void* arg) {
    pthread_t thread_id = pthread_self();
    Thread* thread = get_self(thread_id);
    if (thread == NULL) {
        printf("worker not found!\n");
        return NULL;
    }
    while (1) {
        pthread_mutex_lock(&thread_pool.mutex);
        while (thread_pool.queue.size <= 0 && !thread_pool.should_exit) {
            thread->waiting = 1;
            pthread_cond_wait(&thread_pool.cond, &thread_pool.mutex);
        }
        thread->waiting = 0;
        if (thread_pool.should_exit) {
            pthread_mutex_unlock(&thread_pool.mutex);
            pthread_exit(NULL);
        }

        Task task = get_task();

        pthread_mutex_unlock(&thread_pool.mutex);
        if (task.function != NULL && !task.completed) {
            task.completed = 1;
            task.function(task.args);
            free(task.args);
        }
    }
}

void thread_pool_init(int workers, int capacity) {
    thread_pool.num_workers = workers;
    thread_pool.threads = calloc(thread_pool.num_workers, sizeof(Thread));
    thread_pool.queue.capacity = capacity;
    thread_pool.queue.tasks = malloc(sizeof(Task) * capacity);

    if (pthread_mutex_init(&thread_pool.mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }
    if (pthread_cond_init(&thread_pool.cond, NULL) != 0)
    {
        printf("\n cond init failed\n");
    }

    for (long i = 0; i < thread_pool.num_workers; i++) {
        pthread_create(&thread_pool.threads[i].id, NULL, worker, (void*)i);
    }
}

void sleep(int ms)
{
    struct timespec waittime;

    waittime.tv_sec = (ms / 1000);
    ms = ms % 1000;
    waittime.tv_nsec = ms * 1000 * 1000;

    nanosleep(&waittime, NULL);
}

void await_workers() {
    pthread_cond_broadcast(&thread_pool.cond);
    int free_workers = 0;
    for (int i = 0; i < thread_pool.num_workers; i++) {
        if (thread_pool.threads[i].waiting == 1) {
            free_workers++;
        }
    }
    if (free_workers == thread_pool.num_workers) {
        return;
    }
    sleep(1);
    await_workers();
}

void wait_free_worker() {
    pthread_cond_broadcast(&thread_pool.cond);
    for (int i = 0; i < thread_pool.num_workers; i++) {
        if (thread_pool.threads->waiting == 1) {
            return;
        }
    }
    sleep(1);
    wait_free_worker();
    return;
}

void finished() {
    while (thread_pool.queue.size != 0) {
        sleep(1);
    }
    await_workers();
    return;
}

void add_task(Task task) {
    if (thread_pool.queue.size == thread_pool.queue.capacity - 1) {
        printf("waiting for worker to become available\n");
        wait_free_worker();
        printf("worker freed, continuing\n");
        add_task(task);
        return;
    }

    pthread_mutex_lock(&thread_pool.mutex);
    task.id = thread_pool.queue.tail;
    thread_pool.queue.tasks[thread_pool.queue.tail] = task;
    thread_pool.queue.tail = (thread_pool.queue.tail + 1) % thread_pool.queue.capacity;
    thread_pool.queue.size++;

    pthread_mutex_unlock(&thread_pool.mutex);
    pthread_cond_signal(&thread_pool.cond);
}
