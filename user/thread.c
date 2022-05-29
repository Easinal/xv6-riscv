#define PGSIZE 4096
#include "thread.h"
#include "user.h"

int thread_create(void *(start_routine)(void*), void *arg){
	
	void *stack = malloc((uint) 0);
	stack = malloc((uint) PGSIZE);
	
	if (stack == 0){
		printf("stack malloc fail\n"); 
        return -1;
    }
	
	int pid = clone(stack);
	//int pid = fork();
	if(pid == 0) {
		//printf("child thread\n");
		start_routine(arg);
		exit(0);
	} else if(pid > 0) {
		//printf("parent thread: pid=%d\n", pid);
	} else {
		return -1;
	}
	// printf("thread clone finished\n"); 
	return 0;
}
void lock_init(struct lock_t* lock){
	lock->locked = 0;
}
void lock_acquire(struct lock_t* lock){
	while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
		;
}
void lock_release(struct lock_t* lock){
  __sync_lock_release(&lock->locked);
}