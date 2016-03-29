#include <stdio.h>
#include <errno.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>

#define the_goob(arg) syscall(325, arg)
#define init_sc(arg) syscall(326, arg)
#define get_sc(arg) syscall(327, arg)
#define smunch(arg1, arg2) syscall(329, arg1, arg2)

void my_handler(int signum)
{
	if (signum == SIGUSR1) {
		// printf("received SIGUSR1 times\n", get_sc(SIGUSR1));
		printf ("Custom handler SIGUSER1!\n");
	}
	else if (signum == SIGKILL)
	{
		printf ("Custom handler SIGKILL!\n");
	}
	else
	{
		printf ("Custom handler %d!\n", signum);
	}
}


int main ()
{
	int pid, ret, i;
	unsigned long sigmask = 0;
	ret = 0;
	switch(pid=fork()) {
	case 0:
		signal(SIGUSR1, my_handler);
		signal(SIGUSR1 + 1, my_handler);
		signal(SIGCHLD, my_handler);
		signal(SIGRTMIN, my_handler);
		signal(SIGRTMAX, my_handler);
		while (1) {
			sleep(1);
			//printf("not dead yet\n");
		}
		break;
	default:
		sleep(3);
		//kill(pid, SIGUSR1);
		//printf("sending signal 1\n");
		sleep (1);
		//kill(pid, SIGUSR1);
		//printf("sending signal 1.1\n");
		sleep (1);
		//kill(pid, SIGCHLD);
		//printf("sending signal 1.2\n");
		sleep (3);
		for (i = 0; i < 1; i++)
		{
			sigmask = 1UL << (SIGUSR1 - 1);
			sigmask |= 1UL << (SIGUSR1);
			sigmask |= 1UL << (SIGRTMIN - 1);
			sigmask |= 1UL << (SIGRTMAX - 1);
			//sigmask |= 0b100000000; //sigkill?
			
			printf ("%d\n", smunch(pid, sigmask));
			printf("sending signal 2\n");
			sleep(1);
		//	kill(pid, SIGUSR1);
		//	printf("sending signal 3\n");
			sleep(1);
		}
	}
	return ret;
}

