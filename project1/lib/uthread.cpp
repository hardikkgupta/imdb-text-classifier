#include "uthread.h"
#include "TCB.h"
#include <cassert>
#include <deque>

using namespace std;

// Finished queue entry type
typedef struct finished_queue_entry
{
	TCB *tcb;	  // Pointer to TCB
	void *result; // Pointer to thread result (output)
} finished_queue_entry_t;

// Join queue entry type
typedef struct join_queue_entry
{
	TCB *tcb;			 // Pointer to TCB
	int waiting_for_tid; // TID this thread is waiting on
} join_queue_entry_t;

// You will need to maintain structures to track the state of threads
// - uthread library functions refer to threads by their TID so you will want
//   to be able to access a TCB given a thread ID
// - Threads move between different states in their lifetime (READY, BLOCK,
//   FINISH). You will want to maintain separate "queues" (doesn't have to
//   be that data structure) to move TCBs between different thread queues.
//   Starter code for a ready queue is provided to you
// - Separate join and finished "queues" can also help when supporting joining.
//   Example join and finished queue entry types are provided above

// Queues
static deque<TCB *> ready_queue;  //TCBs contain Tid, pr, f, arg, state,

// “Finished” queue to store threads that have fully exited
static deque<finished_queue_entry_t> finished_queue;

// “Join” queue to store threads waiting on another TID
static deque<join_queue_entry_t> join_queue;

// A table to map TID -> TCB*, so we can find any thread by its ID
static map<int, TCB*> tid_table;

static int next_tid = 1;

static int global_quantum_usecs = 0;

//current thread
static TCB* currThread = nullptr; 

// Interrupt Management --------------------------------------------------------

// Start a countdown timer to fire an interrupt
static void startInterruptTimer()
{
	// TODO
}

// Block signals from firing timer interrupt
static void disableInterrupts()
{
	// TODO
}

// Unblock signals to re-enable timer interrupt
static void enableInterrupts()
{
	// TODO
}

// Queue Management ------------------------------------------------------------

// Add TCB to the back of the ready queue
void addToReadyQueue(TCB *tcb)
{
	ready_queue.push_back(tcb);
}

// Removes and returns the first TCB on the ready queue
// NOTE: Assumes at least one thread on the ready queue
TCB *popFromReadyQueue()
{
	assert(!ready_queue.empty());
	TCB *ready_queue_head = ready_queue.front();
	ready_queue.pop_front();
	return ready_queue_head;
}

// Removes the thread specified by the TID provided from the ready queue
// Returns 0 on success, and -1 on failure (thread not in ready queue)
int removeFromReadyQueue(int tid)
{
	for (deque<TCB *>::iterator iter = ready_queue.begin(); iter != ready_queue.end(); ++iter)
	{
		if (tid == (*iter)->getId())
		{
			ready_queue.erase(iter);
			return 0;
		}
	}

	// Thread not found
	return -1;
}

// Helper functions ------------------------------------------------------------

// Switch to the next ready thread
static void switchThreads()
{
	// TODO
	if(ready_queue.empty()) {
		return;
	}

	TCB* oldThread = currThread;
	
	TCB* newThread = popFromReadyQueue();
	
	if(oldThread && oldThread->getState() == RUNNING) {
		oldThread->setState(READY);
		addToReadyQueue(oldThread);
	}

	newThread->setState(RUNNING);
	currThread = newThread

	if (oldThread != nullptr) {
        /* 
         To distinguish the two cases (first call vs. resumed call), we can use a flag.
         */
        if (getcontext(&(oldThread->context)) == 0 && !oldThread->isSwitching) {
            oldThread->flag = true;//add a variable in TCB.h
            // Transfer control to the new thread.
            setcontext(&(newThread->context));
        }
        // When this thread is resumed, clear the flag and continue execution.
        oldThread->flag = false;
    } else {
        setcontext(&(newThread->context));
    }
}

// Library functions -----------------------------------------------------------

// The function comments provide an (incomplete) summary of what each library
// function must do

// Starting point for thread. Calls top-level thread function
void stub(void *(*start_routine)(void *), void *arg)
{
	// TODO
	enableInterrupts();
	void *retVal = start_routine(arg);
	uthread_exit(retVal);
}

int uthread_init(int quantum_usecs)
{
	disableInterrupts();
	// Initialize any data structures
	global_quantum_usecs = quantum_usecs;
	// Setup timer interrupt and handler (will do it later)

	// Create a thread for the caller (main) thread
	TCB* mainTCB = new TCB();
	mainTCB->setTID(0);
	mainTCB->setState(RUNNING);
	getcontext(&(mainTCB->context))
	
	currThread = mainThread;
	tid_table[0] = mainTCB
	startInterruptTimer();
	enableInterrupts();
	return 0;
}

int uthread_create(void *(*start_routine)(void *), void *arg)
{
	disableInterrupts();
	if(next_tid >= MAX_THREAD_NUM) {
		enableInterrupts();
		return -1;
	}
	// Create a new thread and add it to the ready queue
	int new_tid = next_tid++;
	TCB* newTCB = new TCB(new_tid, (void(*)(void*))start_routine, arg, READY);
	tid_table[new_tid] = newTCB;
	addToReadyQueue(newTCB);
	enableInterrupts();
	return new_tid;
}

int uthread_join(int tid, void **retval)
{
	// If the thread specified by tid is already terminated, just return
	// If the thread specified by tid is still running, block until it terminates
	// Set *retval to be the result of thread if retval != nullptr
}

int uthread_yield(void)
{
	disableInterrupts();
	switchThreads();
	enableInterrupts();
	return 0;
}

void uthread_exit(void *retval)
{
	// If this is the main thread, exit the program
	// Move any threads joined on this thread back to the ready queue
	// Move this thread to the finished queue
}

int uthread_suspend(int tid)
{
	// Move the thread specified by tid from whatever state it is
	// in to the block queue
}

int uthread_resume(int tid)
{
	// Move the thread specified by tid back to the ready queue
}

int uthread_once(uthread_once_t *once_control, void (*init_routine)(void))
{
	// Use the once_control structure to determine whether or not to execute
	// the init_routine
	// Pay attention to what needs to be accessed and modified in a critical region
	// (critical meaning interrupts disabled)
}

int uthread_self()
{
	// TODO
}

int uthread_get_total_quantums()
{
	// TODO
}

int uthread_get_quantums(int tid)
{
	// TODO
}
