#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 16

int main (int argc, char** argv) {
	struct termios save;
	struct termios newer;

	printf("Disabling echo\n");
	tcgetattr(0, &save);
	tcgetattr(0, &newer);
	newer.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &newer); //TCSANOW means make changes immediately

	char* input;
	input = (char*)malloc(SIZE);
	printf("Enter your secret phrase:");
	scanf ("%s", input);
	printf("\nYou entered %s\n", input);

	tcsetattr(0, TCSANOW, &save);
}
