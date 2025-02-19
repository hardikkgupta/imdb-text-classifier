#include "TCB.h"
#include <stdio.h>
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include "uthread.h"
// #define _XOPEN_SOURCE 700
// #include <ucontext.h>


#ifndef STACK_SIZE
#define STACK_SIZE 16384 
#endif

// TCB::TCB(int tid, State state)
// {
//     _tid = tid;
//     _state = state;
//     _quantum = 0;
//     _stack = nullptr;


// }

TCB::TCB(int tid, void *(*start_routine)(void* arg), void *arg, State state)
{
    //initialize the TCB 
    _tid = tid;
    _state = state;
    _quantum = 0;
    _stack = nullptr;

    getcontext(&_context);
    
    if(_tid == 0) { // changes
        return;
    }
    //Allocating new stack for user-level thread
    _stack = new char[STACK_SIZE];
    
    _context.uc_stack.ss_sp = _stack;
    _context.uc_stack.ss_ssize = STACK_SIZE;
    _context.uc_stack.ss_flags = 0;    
    
    makecontext(&_context, (void(*)())stub, 2, start_routine, arg);
}

TCB::~TCB()
{
    if(_stack != nullptr) {
        delete[] _stack;
        _stack = nullptr;
    }
}

void TCB::setState(State state)
{
    _state = state;
}

State TCB::getState() const
{
    return _state;
}

int TCB::getId() const
{
    return _tid;
}

void TCB::increaseQuantum()
{
    _quantum++;
}

int TCB::getQuantum() const
{
    return _quantum;
}

//not calling them (using getcntext, setcontext directly)
// int TCB::saveContext()
// {
//     return getcontext(&_context); 
// }

// void TCB::loadContext()
// {
//    setcontext(&_context)
// }
