#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>

#define READ 0 
#define WRITE 1 
#define MAX 1024


/*
 * Create some file descriptors for piping, 0 = read, 1 = write. 
 * Don't have to initialize the values, because pipe creates the file descriptors for us.
 * From the man page after using dup2, "the old and new file descriptors (arg1 and arg2)
 * can be used interchangeably".
 *
 * Parent will fork to child, then continue running.
 * Closing FD after using dup2 will replace STDIN/OUT with whatever FD pipe created,
 * which redirects the stream to our process. This is basically a really fancy print?
 */
int main() 
{ 
	int fd[2]; 
	ssize_t num; 
	pid_t pid; 
	char str[MAX];

	//0 = success, -1 = error
	if (pipe (fd) < 0) { 
		perror ("plumbing problem"); 
		exit(1); 
	} 
	// point A

	if ((pid = fork()) < 0) { 
		perror ("fork failed"); 
		exit(1); 
	} 
	// point B
	//pid == 0, means this is child.
	else if (!pid) { 
		// STDOUT_FILENO = 1, used for write syscall, this is FILENO for STDOUT.
		// Replace whatever FD we have for writing with STDOUT FD for writing.
		dup2 (fd[WRITE], STDOUT_FILENO); 
		// point C 
		// Close the file descriptors. Close makes a FD available to be reused.
		close (fd[READ]); 
		close (fd[WRITE]); 
		// point D 
		// This will write to a file, which just so happens to be STDOUT_FILENO
		// Basically a fancy print statement
		fgets (str, MAX, stdin); 
		write (STDOUT_FILENO, (const void* ) str, (size_t) strlen (str) + 1); 
		exit (0); 
	}
	
	//Replace read FD with FD for STDIN
	dup2 (fd[READ], STDIN_FILENO); 
	// point C 
	// Close the file descriptors
	close (fd[READ]); 
	close (fd[WRITE]); 
	// point D 
	// Read from some file, which just so happens to be STDIN.
	num = read (STDIN_FILENO, (void* ) str, (size_t)  sizeof (str)); 
	if (num > MAX) {
		perror ("pipe read error\n");
		exit(1);
	}
	puts (str); 
	return 0; 
}
