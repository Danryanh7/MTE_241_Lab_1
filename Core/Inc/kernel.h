#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define MAX_STACK_SIZE 0x4000
#define STACK_SIZE 0x400
#define THREAD_SIZE 0x400
#define RUN_FIRST_THREAD 0x3

extern void runFirstThread(void); // correct
void osKernelInitialize();
void SVC_Handler_Main(unsigned int*);
uint32_t* allocate_stack();
bool osCreateThread(void*);
void osKernelStart();

typedef struct k_thread {
    uint32_t* sp; //stack pointer
    void (*thread_function)(void*); //function pointer
} thread;
