#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define FOO 4096
#define SIZE 1024

char* shmPtr, *data;
int shmId;
void sigHandler(int);
void* readInput(void*);
int main ()
{
	int status, loopFlag = 1;
	data = (char*)malloc(SIZE);
	signal(SIGINT, sigHandler);
	pthread_t reader;

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
	if ((status = pthread_create(&reader, NULL,  readInput, NULL)) != 0)
		fprintf(stderr, "Failed to create reader thread. Computer will now self destruct.\n");
	
	shmPtr[SIZE + (sizeof(int) * 2)]++;
	while (loopFlag)
	{
		printf ("Please input your word: ");
		scanf ("%s", data);
		if (!strcmp(data, "exit"))
			loopFlag = 0;
		strcpy(shmPtr, data);
		shmPtr[SIZE] = '1'; // lock: 1 = available, 0 = not
		shmPtr[SIZE + sizeof(int)] = 0; // how many readers have accessed data
		printf("Word is in shared memory. Please wait . . .\n"); 
		while (shmPtr[SIZE + sizeof(int)] != shmPtr[SIZE + (sizeof(int)*2)]); // offset of 2 ints is # of readers/writers
		shmPtr[SIZE + sizeof(int)] = 0;
	}
	shmPtr[SIZE + (sizeof(int) * 2)]--;

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
	printf("\n\nUser exiting.\n");
	if (shmdt (shmPtr) < 0) {

		exit (1);
	}
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		exit(1);
	}
	free(data);
	exit(0);
}

void* readInput(void* arg)
{
	int count, shmId, loopFlag = -1;

	key_t key = ftok("shmfile",65);
	if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
	{
		perror("could not get");
		exit(1);
	}
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
		perror ("can't attach\n");
		exit (1);
	}
	
	while (loopFlag)
	{
		//wait for value to be available
		//wait for lock to be 0
		//set lock to 0
		//read value
		//increment count
		//set lock to 1
		//while word hasn't changed, sleep
		while (shmPtr[SIZE] == 0);

		while (strcmp(data, shmPtr) == 0)
			strcpy(data, shmPtr);

		shmPtr[SIZE] = 1;
		count = (int)*(shmPtr + SIZE + sizeof(int)) + 1;

		shmPtr[SIZE + sizeof(int)] = count;
		shmPtr[SIZE] = 0;
		strcpy(data, shmPtr);
		printf("word is %s\n", data);
	}
	printf ("value a: %p\t value b: %p\n", (void *) shmPtr, (void *) shmPtr + FOO);
	return NULL;
}
