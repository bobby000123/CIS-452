#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define FOO 4096
#define SIZE 1024

int main ()
{
	int count, shmId, loopFlag = -1;
	char *shmPtr = (char*)malloc(SIZE), *data = (char*)malloc(SIZE);


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
//	printf("What the heck goes here %s\n", shmPtr + SIZE); 

	//	}

	printf ("value a: %p\t value b: %p\n", (void *) shmPtr, (void *) shmPtr + FOO);
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
