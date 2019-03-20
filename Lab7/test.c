#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

void get_max_nsems();
void get_max_sem_count();
void get_max_shared_mem_size();

int main(int argc, char** argv)
{
	//	get_max_nsems();
	//	get_max_sem_count();
	get_max_shared_mem_size();
}

void get_max_nsems()
{
	int count = 1;
	int semid;
	while (errno == 0)
	{
		semid = semget(IPC_PRIVATE, count++, 0600);
		semctl(semid, 0, IPC_RMID);
		if (count % 1000 == 0)
			printf("%d, %d\n", semid, errno);

	}
	printf("%d, %d\n", count, errno);
}
void get_max_sem_count()
{
	unsigned int count = 1;
	int val = 0;
	sem_t sem;
	sem_init(&sem, 0, count++);
	while (errno == 0)
	{
		sem_init(&sem, 0, count++);
		//		sem_post(&sem);
		//printf("%d\n", sem_getvalue(&sem, &val));
		//		if (count % 100000 == 0)
		//			printf("%d, %d\n", count, errno);
		//		printf("%d\n", val);
		sem_destroy(&sem);
	}
	sem_getvalue(&sem, &val);
	printf("%d, %d\n", val, errno);
	sem_destroy(&sem);
}

void get_max_shared_mem_size()
{

	int shmId, size = 1;
	char *shmPtr;

	while (errno == 0)

	{
			printf("%d\n", size);
		if ((shmId = shmget (IPC_PRIVATE, size++, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
			perror ("i can't get no..\n");
			printf("%d\n", size);

			exit (1);
		}
		if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
			perror ("can't attach\n");
			printf("%d\n", size);
			exit (1);
		}

		if (shmdt (shmPtr) < 0) {

			printf("%d\n", size);
			perror ("just can't let go\n");
			exit (1);
		}
		struct shmid_ds buf;
		if (shmctl (shmId, IPC_RMID | IPC_STAT, &buf) < 0) {
			printf("%d\n", size);
			perror ("can't deallocate\n");
			exit(1);
		}
	}
}
