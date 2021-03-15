
#include <stddef.h>

#include "alloc.h"
#include "time.h"

#include "thread.h"


extern struct context current_ctx;


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

    // Wakeup time for sleeping thread
    uint64_t wakeup_time;

    // Stores address to place exit code when child joins.
    int* child_exit_code;

    // Threads waiting to join.
    struct thread_queue join_queue;
};

struct thread threads[MAX_THREADS];

// idle_thread loop on yield syscall forever.
// Since it doesn't exit or sleep, it's always ready.
struct thread idle_thread;

// Thread that was preempted by an interrupt.
struct thread* interrupt_thread;

// No waiting threads.
// Joining threads get put in a specific threads queue.
struct thread* running_thread;
struct thread_queue ready_threads;
struct thread_queue sleeping_threads;

int preemption_enable;




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




void thread_idle(void);
void thread_wrapper(int (*func)(void*), void* data);


void thread_init(void) {
    for (size_t i = 0; i < MAX_THREADS; i++) {
        threads[i].flags = 0;
        threads[i].tid = i;
    }

    // Init thread is always 0.
    threads[0].stack_base = NULL;
    threads[0].flags = T_USED;
    threads[0].join_queue.head = NULL;
    threads[0].join_queue.tail = NULL;

    running_thread = &threads[0];

    // Setup idle thread and make it ready.
    idle_thread.stack_base = NULL;
    idle_thread.flags = T_USED;
    idle_thread.join_queue.head = NULL;
    idle_thread.join_queue.tail = NULL;
    idle_thread.ctx.mepc = (uint32_t) thread_idle;

    thread_queue_push(&ready_threads, &idle_thread);

    preemption_enable = 1;
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


void thread_on_timeout(void) {
    uint64_t time;
    time_us(&time);

    // Wakeup sleeping threads
    struct thread_queue tmp_queue = {0};
    struct thread* node = thread_queue_pop(&sleeping_threads);
    while (node != NULL) {
        if (node->wakeup_time < time)
            thread_queue_push(&ready_threads, node);
        else
            thread_queue_push(&tmp_queue, node);

        node = thread_queue_pop(&sleeping_threads);
    }

    sleeping_threads.head = tmp_queue.head;
    sleeping_threads.tail = tmp_queue.tail;

    // Preempt threads
    if (preemption_enable)
        thread_yield();
}


void thread_exit(int exit_code) {
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


thread_t thread_create_from_ctx(int (*func)(void*), void* data,
                                struct context ctx) {
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

    // Inherit whatever context that spawned thread, and modify what
    // needs to change.
    new_thread->ctx = ctx;
    new_thread->ctx.sp = (uint32_t) (new_thread->stack_base + STACK_SIZE);
    new_thread->ctx.mepc = (uint32_t) thread_wrapper;
    new_thread->ctx.a0 = (uint32_t) func;
    new_thread->ctx.a1 = (uint32_t) data;

    thread_queue_push(&ready_threads, new_thread);

    return tid;
}

thread_t thread_create(int (*func)(void*), void* data) {
    return thread_create_from_ctx(func, data, current_ctx);
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
    preemption_enable = enable;
    return 0;
}


int thread_sleep_us(uint64_t* period) {
    uint64_t wt;
    time_us(&wt);
    wt += *period;

    running_thread->wakeup_time = wt;
    thread_queue_push(&sleeping_threads, running_thread);
    running_thread = thread_queue_pop(&ready_threads);

    return 0;
}
