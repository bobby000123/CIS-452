#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>

void sigHandler1(int, siginfo_t*, void*);
void sigHandler2(int, siginfo_t*, void*);
void exitSigHandler(int, siginfo_t*, void*);

/*
 * Note: parent will be the only one to receive SIGUSR1/2, but
 * parent AND child receive interrupt, so it prints twice. 
 * Way around this is signal (SIGINT, SIG_IGN). However, this means
 * that the only way I know of to kill children is to use global
 * variables for the PID, and do so from the sighandler. Which is better?
 */
int main()
{
	pid_t child1, child2, parent;
	parent = getpid();

	struct sigaction usr1, usr2, quitsig;

	usr1.sa_sigaction = sigHandler1;
	usr2.sa_sigaction = sigHandler2;
	quitsig.sa_sigaction = exitSigHandler;

	usr1.sa_flags = SA_SIGINFO;
	usr2.sa_flags = SA_SIGINFO;
	quitsig.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &usr1, NULL);
	sigaction(SIGUSR2, &usr2, NULL);
	sigaction(SIGINT, &quitsig, NULL);

	if (((child1 = fork()) | (child2 = fork())) < 0) {
		perror("Fork failed");
		exit(1);
	}

	if (child1 == 0) { 
//		signal(SIGINT, SIG_IGN);
		int randomTime1;
		while (1)
		{
			randomTime1 = rand() % 5 + 1; //wait 1-5 seconds.
			sleep(randomTime1);

			kill(parent, SIGUSR1);
		}
	} else if (child2 == 0) {
//		signal(SIGINT, SIG_IGN);
		int randomTime2;
		while (1)
		{
			randomTime2 = rand() % 5 + 1; //wait 1-5 seconds.
			sleep(randomTime2);

			kill(parent, SIGUSR2);
		}

	} else {
		printf("Successfully created children, pid = %d, %d\n", child1, child2);
		/* Probably not the best way to do this, but wakes up child.
		 * Pause will pause parent and children, so wake up call necessary.
		 * This may cause a race condition, not sure.
		 */
		kill(child1, SIGCONT);
		kill(child2, SIGCONT);
		while (1) {
			printf("waiting...");
			fflush(stdout);
			pause();
		}
		/* Because the child is ignoring the SIGINT signal, we have to
		 * manually send a SIGTERM to the child process.
		 */
//		kill(child1, SIGTERM);
//		kill(child2, SIGTERM);
	}
	return 1;
}

void sigHandler1(int sigNum, siginfo_t *siginfo, void *context)
{
	printf("PID %ld sent USR1\n", (long)siginfo->si_pid); 
}

void sigHandler2(int sigNum, siginfo_t *siginfo, void *context)
{
	printf("PID %ld sent USR2\n", (long)siginfo->si_pid);
}

void exitSigHandler(int sigNum, siginfo_t *siginfo, void *context)
{
	printf("PID %ld sent SIGINT\n", (long)siginfo->si_pid);
	exit(0);
}
