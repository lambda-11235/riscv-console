
#include <stddef.h>

#include "alloc.h"
#include "video.h"

#include "thread.h"


#define MAX_THREADS 256
#define STACK_SIZE 4096


enum THREAD_FLAGS {
    T_USED = 1
};

struct thread;

struct thread_queue {
    struct thread* head;
    struct thread* tail;
};

struct thread {
    int flags;
    int tid;
    struct thread* next;

    void* stack_base;
    struct context ctx;

    // Stores address to place exit code when child joins.
    int* child_exit_code;

    // Threads waiting to join.
    struct thread_queue join_queue;
};


extern struct context current_ctx;

struct thread threads[MAX_THREADS];

// Thread that was preempted by an interrupt.
struct thread* interrupt_thread;

// No waiting threads.
// Joining threads get put in a specific threads queue.
struct thread* running_thread;
struct thread_queue ready_threads;


int thread_queue_push(struct thread_queue* queue, struct thread* thread) {
    thread->next = NULL;

    if (queue->head == NULL) {
        queue->head = thread;
        queue->tail = thread;
    } else {
        queue->tail->next = thread;
        queue->tail = thread;
    }

    return 0;
}


/**
 * @return Next thread in queue, or NULL if queue is empty.
 */
struct thread* thread_queue_pop(struct thread_queue* queue) {
    struct thread* thread = queue->head;

    if (thread != NULL) {
        queue->head = thread->next;

        if (queue->head == NULL)
            queue->tail = NULL;
    }

    return thread;
}


/**
 * @return 1 if empty, 0 if not.
 */
int thread_queue_empty(struct thread_queue* queue) {
    if (queue->head == NULL)
        return 1;
    else
        return 0;
}




void thread_wrapper(int (*func)(void*), void* data);


void thread_init(void) {
    for (size_t i = 0; i < MAX_THREADS; i++) {
        threads[i].flags = 0;
        threads[i].tid = i;
    }

    threads[0].stack_base = NULL;
    threads[0].flags = T_USED;
    threads[0].join_queue.head = NULL;
    threads[0].join_queue.tail = NULL;

    running_thread = &threads[0];
}


void thread_enter_int(void) {
    interrupt_thread = running_thread;
}

void thread_exit_int(void) {
    if (running_thread == NULL) {
        fault("No threads ready to run");
    } else {
        interrupt_thread->ctx = current_ctx;
        current_ctx = running_thread->ctx;
    }
}


void thread_exit(int exit_code) {
    fault("exit");
    struct thread* next = thread_queue_pop(&running_thread->join_queue);

    while (next != NULL) {
        *next->child_exit_code = exit_code;
        thread_queue_push(&ready_threads, next);
        next = thread_queue_pop(&running_thread->join_queue);
    };

    running_thread->flags = 0;
    mem_free(running_thread->stack_base);

    running_thread = thread_queue_pop(&ready_threads);
}


thread_t thread_create(int (*func)(void*), void* data) {
    struct thread* new_thread;
    thread_t tid = 0;

    while (tid < MAX_THREADS && threads[tid].flags & T_USED)
        tid++;

    if (tid >= MAX_THREADS)
        return -1;

    new_thread = &threads[tid];

    new_thread->stack_base = mem_alloc(STACK_SIZE);
    new_thread->flags = T_USED;
    new_thread->join_queue.head = NULL;
    new_thread->join_queue.tail = NULL;

    new_thread->ctx.sp = (uint32_t) (new_thread->stack_base + STACK_SIZE);
    new_thread->ctx.gp = current_ctx.gp;  // Inherit gp
    new_thread->ctx.mepc = (uint32_t) thread_wrapper;
    new_thread->ctx.a0 = (uint32_t) func;
    new_thread->ctx.a1 = (uint32_t) data;

    thread_queue_push(&ready_threads, new_thread);

    return tid;
}


int thread_yield(void) {
    if (!thread_queue_empty(&ready_threads)) {
        thread_queue_push(&ready_threads, running_thread);
        running_thread = thread_queue_pop(&ready_threads);
    }

    return 0;
}


int thread_join(thread_t t, int* exit_code) {
    if (running_thread->tid != 0)
        fault("join by non 0");
    
    if (t < 0 || t > MAX_THREADS) {
        return -1;
    } else if (!(threads[t].flags & T_USED)) {
        return -1;
    }

    thread_queue_push(&threads[t].join_queue, running_thread);
    running_thread->ctx = current_ctx;
    running_thread->child_exit_code = exit_code;

    running_thread = thread_queue_pop(&ready_threads);

    return 0;
}


int thread_set_preemption(int enable) {
    return 0;
}
