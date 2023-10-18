#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_STACK_SIZE 0x4000
#define STACK_SIZE 0x400
#define THREAD_SIZE 0x400
#define RUN_FIRST_THREAD 0x3

// extern uint32_t* stackptr;

void SVC_Handler_Main(unsigned int*);
uint32_t* allocate_stack();
extern void runFirstThread(void); // correct
bool osCreateThread(void*);
void osKernelInitialize();
void osKernelStart();

typedef struct k_thread {
    uint32_t* sp; //stack pointer
    void (*thread_function)(void*); //function pointer
} thread;
