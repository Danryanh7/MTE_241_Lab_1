#include "stm32f4xx_hal.h"
#include "kernel.h"

uint32_t* MSP_INIT_VAL;
uint32_t* LAST_STACK;
int nThreads = 0;
thread lab3Thread;
extern void runFirstThread(void);

void SVC_Handler_Main( unsigned int *svc_args ) {
  unsigned int svc_number;
  /*
  * Stack contains:
  * r0, r1, r2, r3, r12, r14, the return address and xPSR
  * First argument (r0) is svc_args[0]
  */
  svc_number = ( ( char * )svc_args[ 6 ] )[ -2 ] ;
  switch( svc_number )
  {
    case RUN_FIRST_THREAD:
      __set_PSP((uint32_t) stackptr);
      runFirstThread();
      break;
    case 17: //17 is sort of arbitrarily chosen
      printf("Success!\r\n");
      break;
    case 21:
      printf("Failure!\r\n");
      break;
    default: /* unknown SVC */
      break;
  }
}

// Allocate a new stack
uint32_t* allocate_stack() {
    if ((nThreads * THREAD_SIZE) + STACK_SIZE > MAX_STACK_SIZE) {
        return NULL;
    }

    LAST_STACK -= THREAD_SIZE;
    nThreads += 1;

    uint32_t* topOfStack = LAST_STACK + THREAD_SIZE; 

    return topOfStack;
}

// Creating a new thread
bool osCreateThread((void*) fncPtr) {
    fncPtr = (uint32_t*) fncPtr;
    uint32_t* stackPtr = allocate_stack();

    if (stackPtr == NULL) {
        return false;
    }

    // Setting up the stack from lab 2
    *(--stackPtr) = 1<<24; // xPSR
    *(--stackptr) = (uint32_t)fncPtr; //the function name
    for (int i = 0; i < 14; i++) {
        *(--stackptr) = 0xA; //An arbitrary number
    }

    lab3Thread.sp = stackPtr;
    lab3Thread.thread_function = fncPtr;
    return true;
}