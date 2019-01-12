#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 16

void type_text(char *s, unsigned ms_delay);
int main(int argc, char** argv) {
	struct termios save;
	struct termios newer;
	tcgetattr(0, &save);
	tcgetattr(0, &newer);

	newer.c_lflag &= ~ISIG; //Signal flags do nothing
	tcsetattr(0, TCSANOW, &newer); //TCSANOW means make changes immediately

	int i;
	for (i = 0; i < 100; i++)
		printf("\n");
	sleep(2);
	type_text("Huh... that didn't do much.\n", 30);
	sleep(2);
	type_text("I guess we don't get much extra credit for nothing.\n", 30);
	sleep(2);
	type_text("Go ahead and exit the program, I forgot to put in an exit here.\n", 30);
	sleep(4);
	printf("\n");
	type_text("Weird... that's not working.\n", 30);
	sleep(1);
	type_text("I guess you're just stuck here.\n", 30);
	sleep(2);
	type_text("I'll cut you a deal - a free A on the final, and I'll put back in signal flags.\n", 30);
	sleep(5);
	type_text("I'm assuming you said yes to that, because there is no other choice. Or you just waited out the 3 second timer....\n", 30);
	sleep(2);
	type_text(".\n", 30);
	sleep(1);
	type_text(".\n", 30);
	sleep(1);
	type_text(".\n", 30);
	sleep(1);
	type_text("Or just some extra credit would be nice.\n", 30);
	sleep(1);
	tcsetattr(0, TCSANOW, &save);
}


void type_text(char *s, unsigned ms_delay)
{
	unsigned usecs = ms_delay * 1000; /* 1000 microseconds per ms */

	for (; *s; s++) {
		putchar(*s);
		fflush(stdout); /* alternatively, do once: setbuf(stdout, NULL); */
		usleep(usecs);
	}	
}
