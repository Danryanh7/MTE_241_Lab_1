#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_STACK_SIZE 0x4000
#define STACK_SIZE 0x400
#define THREAD_SIZE 0x400
#define RUN_FIRST_THREAD 0x3

extern uint32_t* stackptr;
extern void runFirstThread(void);

void SVC_Handler_Main( unsigned int*);
uint32_t* allocate_stack();
extern void runFirstThread(void);
