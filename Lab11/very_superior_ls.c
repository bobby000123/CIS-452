#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]) 
{ 
	struct stat statBuf;

	if (argc < 2) { 
		printf ("Usage: directory name required\n"); 
		exit(1); 
	}
	
	uint file_index = 1, id_flag = 0, inode_flag = 0;
	if (argc == 3)
	{
		file_index = 2;
		if (!strcmp(argv[1], "-n"))
			id_flag = 1;
		else if (!strcmp(argv[1], "-i"))
			inode_flag = 1;
		else if (!strcmp(argv[1], "-ni") || !strcmp(argv[1], "-in")) {
			id_flag = 1;
			inode_flag = 1;	
		}
	}
	if (stat (argv[file_index], &statBuf) < 0) { 
		perror ("Error opening directory."); 
		exit(1); 
	}

	if (S_ISREG(statBuf.st_mode))
	{
		//Exact words from the lab are: 
		//"Your program should accept as input the name of any directory"
		printf("Not a directory. Please provide a directory.\n");
		exit(1);
	}
	
	DIR *dirPtr;
	struct dirent *entryPtr;

	dirPtr = opendir(argv[file_index]);
	while (entryPtr = readdir(dirPtr)) {	
		if (id_flag || inode_flag) {
			stat(entryPtr->d_name, &statBuf);
			if (id_flag)
				printf("Group: %u  User: %u\t", statBuf.st_gid, statBuf.st_uid);
			if (inode_flag)
				printf("Inode #: %ld\t", entryPtr->d_ino);
		}
		printf("%s\n", entryPtr->d_name);

	}
	closedir(dirPtr);
	printf("\n");
	return 0; 
}
