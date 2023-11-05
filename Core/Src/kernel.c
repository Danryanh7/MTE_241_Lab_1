#include "kernel.h"

uint32_t* MSP_INIT_VAL;
uint32_t* LAST_STACK;
int nThreads;
int currentThread;
thread threadArray[MAX_THREADS];

void osSched() {
  // Round robin implementation
  threadArray[currentThread].sp = (uint32_t*)(__get_PSP() - 8*4);
  currentThread = (currentThread+1)%nThreads;
  __set_PSP((uint32_t) threadArray[currentThread].sp);
  return;
}

void osYield(void) {
  __asm("SVC #4");
}

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
      __set_PSP((uint32_t) threadArray[0].sp);
      runFirstThread();
      break;
    case YIELD:
      //Pend an interrupt to do the context switch
      _ICSR |= 1<<28;
      __asm("isb");
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
    if (((nThreads+1) * THREAD_SIZE) + STACK_SIZE > MAX_STACK_SIZE) {
        return NULL;
    }

    LAST_STACK -= THREAD_SIZE;

    uint32_t* topOfStack = LAST_STACK + THREAD_SIZE; 

    return topOfStack;
}

// Creating a new thread
bool osCreateThread(void* fncPtr) {
    uint32_t* stackPtr = allocate_stack();

    if (stackPtr == NULL) {
        return false;
    }

    // Setting up the stack from lab 2
    *(--stackPtr) = 1<<24; // xPSR
    *(--stackPtr) = (uint32_t)fncPtr; //the function name
    for (int i = 0; i < 14; i++) {
        *(--stackPtr) = 0xA; //An arbitrary number
    }

    threadArray[nThreads].sp = stackPtr;
    threadArray[nThreads].thread_function = fncPtr;
    nThreads += 1;
    return true;
}

// Initialize a new thread
void osKernelInitialize() {
  MSP_INIT_VAL = *(uint32_t**) 0x0;
  LAST_STACK = MSP_INIT_VAL - STACK_SIZE; // questionable... I think its bypassing the main stack
  nThreads = 0;
  threadArray[nThreads].sp = LAST_STACK;
  threadArray[nThreads].thread_function = NULL;

  //set the priority of PendSV to almost the weakest
  SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
  SHPR2 |= 0xFDU << 24; //Set the priority of SVC higher than PendSV
  currentThread = nThreads;
}

// Start the thread
void osKernelStart() {
  __asm("SVC #3");
}
