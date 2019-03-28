#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 100
int undefinedGlobal1, undefinedGlobal2;
int definedGlobal1, definedGlobal2;
void newfunction(int v1, int v2);

extern char __executable_start;
extern char __etext;

int main(int argc, char* argv){
	printf("PID: %d\n", getpid());
	//printf("Location of global variable: %p\n", SIZE);
	printf("Text section: 0x%lx, 0x%lx\n", (unsigned long)&__executable_start, (unsigned long)&__etext);
	definedGlobal1 = 1;
	definedGlobal2 = 2;
	printf("Defined Global Addresses: %p, %p\n", &definedGlobal1, &definedGlobal2);
	printf("Undefined Global Addresses: %p, %p\n", &undefinedGlobal1, &undefinedGlobal2);
	int data1, data2;
	data1 = malloc(sizeof(int));
	data2 = malloc(sizeof(int));
	printf("Run time data: %p, %p\n", &data1, &data2);
	int data3;
	int data4;
	data3 = 100000;
	data4 = 100000;
	printf("Stack data: %p, %p\n", &data3, &data4);
	newfunction(data3, data4);
	char s;
	scanf("%s", s); 

}

void newfunction(int v1, int v2){
	printf("Stack data (function): %p, %p\n", &v1, &v2);
}
