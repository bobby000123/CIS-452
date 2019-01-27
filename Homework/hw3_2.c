#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
	pid_t pid, pid1;
	pid = fork();
	if (pid < 0) {
		fprintf (stderr, "fork() failed\n");
		return(1);
	}
	else if (pid == 0) {
		pid1 = getpid();
		printf ("pid = %d\n", pid); // A
		printf ("pid1 = %d\n", pid1); // B
	}
	else {
		pid1 = getpid();
		printf ("pid = %d\n", pid); // C
		printf ("pid1 = %d\n", pid1); // D
		wait (NULL);
	}
	return 0;
}

