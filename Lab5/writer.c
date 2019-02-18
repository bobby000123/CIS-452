#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

#define FOO 4096
#define SIZE 1024

char* shmPtr, *data;
int shmId;
void sigHandler(int);
int main ()
{
	int loopFlag = 1;
	data = (char*)malloc(SIZE);
	signal(SIGINT, sigHandler);

	key_t key = ftok("shmfile",65);
	printf("made it past here\n");
	if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror ("i can't get no..\n");
		exit (1);
	}
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
		perror ("can't attach\n");
		exit (1);
	}

	printf ("value a: %p\t value b: %p, %d\n", (void *) shmPtr, (void *) shmPtr + FOO, shmId);
	while (loopFlag)
	{
		printf ("Please input your word: ");
		scanf ("%s", data);
		strcpy(shmPtr, data);
		shmPtr[SIZE] = '1';

		printf("Word is in shared memory. Please wait . . .\n"); 
	}

	if (shmdt (shmPtr) < 0) {

		perror ("just can't let go\n");
		exit (1);
	}
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}
	free(data);
	return 0;
} 

void sigHandler(int sigNum)
{
	printf("User exiting\n");
	if (shmdt (shmPtr) < 0) {

		perror ("just can't let go\n");
		exit (1);
	}
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}
	free(data);
}
