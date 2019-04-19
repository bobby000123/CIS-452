#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc == 3) {
		if (link(argv[1], argv[2]) < 0)
		{
			perror("Error creating hard link.");
			exit(1);
		}
	} else if (argc == 4) {
		if (!strcmp(argv[1], "-s")) {
			if (symlink(argv[2], argv[3]) < 0)
			{
				perror("Error creating soft link.");
				exit(1);
			}
		} else {
			printf("Usage:\n./executable oldpath newpath\n./executable -s oldpath newpath");
			exit(1);
		}
	} else {
		printf("Usage:\n./executable oldpath newpath\n./executable -s oldpath newpath");
		exit(1);
	}
	return 0;


}
