#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 64

int main(int argc, char** argv) {
	char* input;

	while (1) {

		//Get user input, see if user quit
		input = (char*)malloc(64);
		printf("Very superior shell, I don't know why you'd use any other shell really this is just the best one around oh right you have to enter a command here go ahead:> ");

		if(!fgets(input, MAX, stdin))
			break;
		if (!strcmp(input, "quit\n"))
			break;

		//Necessary for execvp to work properly
		size_t length = strlen(input);
		if (input[length - 1] == '\n')
			input[length - 1] = '\0';

		//Tokenize user input
		char *temp = strtok(input, " ");
		char* cmd_info[3] = {NULL, NULL, NULL};
		int i = 0;
		while (temp) {
			cmd_info[i++] = temp;
			temp = strtok(NULL, " ");
		}

		//Fork child, then wait for child to be done.
		pid_t pid;
		if ((pid = fork()) < 0) {
			perror("fork failure");
			exit(1);
		}
		else if (pid == 0) {
			execvp(cmd_info[0], cmd_info);
		}
		else {
			struct rusage buf;

			//Need to use wait, not waitpid; causes issues with child
			wait(NULL);

			//Used to get resource usage statistics for a program
			getrusage(RUSAGE_CHILDREN, &buf);
			printf("\nResource usage:\n");
			printf("CPU time: %lld.%lld sec\n", (long long)buf.ru_utime.tv_sec,(long long)buf.ru_utime.tv_usec);
			printf("Voluntary context switches: %ld\n", buf.ru_nvcsw);
			printf("Involuntary context switches: %ld\n", buf.ru_nivcsw);

		}

		//Free the input after done, just in case the user doesn't actually type quit
		free(input);
	}
}
