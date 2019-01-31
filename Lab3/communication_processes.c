#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>

void sigHandler(int);

/*
 * Note: parent will be the only one to receive SIGUSR1/2, but
 * parent AND child receive interrupt, so it prints twice. 
 * Way around this is signal (SIGINT, SIG_IGN). However, this means
 * that the only way I know of to kill children is to use global
 * variables for the PID, and do so from the sighandler. Which is better?
 */
int main()
{
	pid_t child, parent;
	parent = getpid();
	signal(SIGUSR1, sigHandler);
	signal(SIGUSR2, sigHandler);
	signal(SIGINT, sigHandler);

	if ((child = fork()) < 0) {
		perror("Fork failed");
		exit(1);
	}

	if (child == 0) { 
//		signal(SIGINT, SIG_IGN);
		int randomTime;
		while (1)
		{
			randomTime = rand() % 5 + 1; //wait 1-5 seconds.
			sleep(randomTime);

			randomTime = rand() % 2 + 1; 
			if (randomTime == 1)
				kill(parent, SIGUSR1);
			else
				kill(parent, SIGUSR2);

		}
	} else {
		printf("Successfully created child, pid = %d\n", child);
		/* Probably not the best way to do this, but wakes up child.
		 * Pause will pause parent and children, so wake up call necessary.
		 * This may cause a race condition, not sure.
		 */
		kill(child, SIGCONT);
		while (1) {
			printf("waiting...");
			fflush(stdout);
			pause();
		}
		/* Because the child is ignoring the SIGINT signal, we have to
		 * manually send a SIGTERM to the child process.
		 */
//		kill(child, SIGTERM);
	}
	return 1;
}

void sigHandler(int sigNum)
{

	char* signal;
	switch (sigNum)
	{
		case SIGUSR1:
			signal = "SIGUSR1";
			break;
		case SIGUSR2:
			signal = "SIGUSR2";
			break;
		case SIGINT:
			signal = "SIGINT";
			break;
	}

	printf("Received %s\n", signal);
	if (sigNum == SIGINT)
		exit(0);
	
}
