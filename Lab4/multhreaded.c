#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cerrno>
#include <signal.h>
#include <iostream>
void* findFile (void* arg);
void sigHandler(int);

#define NUM_THREADS 25
#define FILE_NAME_SIZE 1024
int filesFound = 0;
int totalTime;
int status[NUM_THREADS];
pthread_mutex_t lock;

/*
 * Used to pass multiple arguments to thread.
 * There may be some better way to do this, but I decided with this.
 * Why? I don't know. That's why.
 * You may also be wondering why I used C and C++. I don't know. I just don't.
 */
struct arguments
{
	//Used to set thread status when done.
	int thread_id;
	//File name to print when thread is done.
	char file_name[FILE_NAME_SIZE];
};


int main(){
	signal (SIGINT, sigHandler);
	//It's reasonable to say that there is a limited # of worker threads.
	pthread_t worker_threads[NUM_THREADS];
	int loopStatus = 1;
	//Arguments to send to pthread when created.
	struct arguments thread_info;

	//Initialize all thread statuses.
	for (int i = 0; i < NUM_THREADS; i++)
		status[i] = 1;

	while(loopStatus){
		//Get the file name
		std::cout << "Enter a file name to search: ";
		std::cin >> thread_info.file_name;

		//If user typed exit, then exit.
		if(!strcmp(thread_info.file_name, "exit"))
			loopStatus = 0;
		else
		{
			//Set thread_id to -1, so if one isn't available, we know.
			thread_info.thread_id = -1;
			//Find first available thread.
			for (int i = 0; i < NUM_THREADS; i++)
				//thread is free if 0, not if 1
				if (status[i])
				{
					thread_info.thread_id = i;
					status[i] = 0;
					break;
				}
			if (thread_info.thread_id == -1)
			{
				std::cerr << "All threads busy"	<< std::endl;
				continue;
			}

			if((status[thread_info.thread_id] = pthread_create (&worker_threads[thread_info.thread_id], NULL,  findFile, &thread_info)) != 0)
			{
				std::cerr << "join error " <<  status << ": " <<  std::endl;
				exit(1);
			}	
		}	
	}
}

void* findFile (void* arg)
{
	//Create a temp struct to hold the arg, then assign to a local copy.
	struct arguments *thread_info = (struct arguments*) arg;
	char file_name[FILE_NAME_SIZE];
       	strcpy(file_name,(*thread_info).file_name);
	int thread_id = (*thread_info).thread_id;

	int sleepTime, random_wait = rand() % 5 + 1;
	if (random_wait > 0 && random_wait < 5)
		sleep(sleepTime = rand() % 2 + 1);
	else
		sleep(sleepTime = rand() % 10 + 7);

	std::cout << file_name << " found. Returning." << std::endl;

	pthread_mutex_lock(&lock);
	filesFound++;
	totalTime+=sleepTime;
	status[thread_id] = 1;
	pthread_mutex_unlock(&lock);
	return NULL;
}

void sigHandler(int sigNum)
{
	printf("\n\nUser is exiting. \nFiles found: %d\nTotal sleep time: %d\nAverage sleep time: %f\n", filesFound, totalTime, ((float)totalTime/(float)filesFound));
	exit(0);
}
