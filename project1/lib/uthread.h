#ifndef _UTHREADS_H
#define _UTHREADS_H

/* From IBM OS 3.1.0 -z/OS C/C++ Runtime Library Reference */
#ifdef _LP64
#define STACK_SIZE 2097152 + 16384 /* large enough value for AMODE 64 */
#else
#define STACK_SIZE 16384 /* AMODE 31 addressing */
#endif

#define MAX_THREAD_NUM 100 /* maximal number of threads */

/* External interface */
typedef enum Priority
{
    RED,
    ORANGE,
    GREEN
} Priority;

typedef enum
{
    UTHREAD_ONCE_NOT_EXECUTED = 0,
    UTHREAD_ONCE_EXECUTED = 1
} execution_status_t;

typedef struct
{
    execution_status_t execution_status;
} uthread_once_t;

#define UTHREAD_ONCE_INIT ((uthread_once_t){.execution_status = UTHREAD_ONCE_NOT_EXECUTED})

/* Initialize the thread library */
// Return 0 on success, -1 on failure
int uthread_init(int quantum_usecs);

/* Create a new thread whose entry point is f */
// Return new thread ID on success, -1 on failure
int uthread_create(void *(*start_routine)(void *), void *arg);

/* Join a thread */
// Return 0 on success, -1 on failure
int uthread_join(int tid, void **retval);

/* yield */
// Return 0 on success, -1 on failure
int uthread_yield(void);

/* Terminate this thread */
// Does not return to caller. If this is the main thread, exit the program
void uthread_exit(void *retval);

/* Suspend a thread */
// Return 0 on success, -1 on failure
int uthread_suspend(int tid);

/* Resume a thread */
// Return 0 on success, -1 on failure
int uthread_resume(int tid);

/* Get the id of the calling thread */
// Return the thread ID
int uthread_self();

/* Execute the init_routine only once for a set of threads */
// Return 0 if the init_routine is executed otherwise 1
int uthread_once(uthread_once_t *once_control, void (*init_routine)(void));

/* Get the total number of library quantums (times the quantum has been set) */
// Return the total library quantum set count
int uthread_get_total_quantums();

/* Get the number of thread quantums (times the quantum has been set for this thread) */
// Return the thread quantum set count
int uthread_get_quantums(int tid);

#endif
