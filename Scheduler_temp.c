#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
 
void round_robin_scheduler()
{
 
	pid_t c1_pid, c2_pid;
 
	(c1_pid = fork()) && (c2_pid = fork()); // Creates two children
 
	if(c1_pid != 0 && c2_pid != 0)
	{
		while(1)
		{
			// think of condintion to break the while loop
			kill(c1_pid,resume);
			//after 2ms pause child1 and resume child2
			sleep(2);
			kill(c1_pid,pause);
			kill(c2_pid,resume);
			//after 2ms pause child2
			sleep(2);
			kill(c2_pid,pause);
		}
	}
 
	if (c1_pid == 0) 
	{
	    /* Child 1*/
	    //exec into P1
	} 
	else if (c2_pid == 0) 
	{
	    /* Child 2 */
	    // exec into P2
	} 
	// else 
	// {
	//      //Parent code goes here 
	// }
}
 
int main() {
   
   round_robin_scheduler();
 
 
 
 
       return 0;
}s