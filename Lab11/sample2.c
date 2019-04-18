#include <stdio.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>

int main() 
{ 
	struct stat statBuf;
	DIR *dirPtr; 
	struct dirent *entryPtr; 

	dirPtr = opendir (".");
	printf("%-20sSize (in bytes)\n", "File name"); 
	while ((entryPtr = readdir (dirPtr))) {
		stat(entryPtr->d_name, &statBuf);
		printf ("%-20s%ld\n", entryPtr->d_name, statBuf.st_size);
	}
	closedir (dirPtr); 
	return 0; 
}
