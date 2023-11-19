#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define MAX_STACK_SIZE 0x4000
#define STACK_SIZE 0x400
#define THREAD_SIZE 0x400
#define RUN_FIRST_THREAD 0x3
#define YIELD 0x4
#define MAX_THREADS 9
#define DEFAULT_TIMESLICE 5
#define SHPR2 *(uint32_t*)0xE000ED1C //for setting SVC priority, bits 31-24
#define SHPR3 *(uint32_t*)0xE000ED20 // PendSV is bits 23-16
#define _ICSR *(uint32_t*)0xE000ED04 //This lets us trigger PendSV

extern void runFirstThread(void); // correct
void osKernelInitialize(void);
void SVC_Handler_Main(unsigned int*);
uint32_t* allocate_stack(void);
bool osCreateThread(void*);
bool osCreateThreadWithDeadline(void*, uint32_t);
void osKernelStart(void);
void osSched(void);
void osYield(void);

typedef struct k_thread {
    uint32_t* sp; //stack pointer
    void (*thread_function)(void*); //function pointer
    uint32_t timeslice;
    uint32_t runtime;
} thread;
