// lab10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <string>


int main()
{
	SYSTEM_INFO info;
	MEMORY_BASIC_INFORMATION mbi;
	GetSystemInfo(&info);

	void* data = malloc(600000 * sizeof(char));
	int query_return = VirtualQuery(data, &mbi, sizeof(mbi));
	std::string state = "";

	if (mbi.State == 4096)
		state = "MEM_COMMIT";
	else if (mbi.State == 65536)
		state = "MEM_FREE";
	else if (mbi.State == 8192)
		state = "MEM_RESERVE";
	std::cout << "Page size: " << info.dwPageSize << std::endl;
	std::cout << "Data state before free: " << state << std::endl;

	free(data);

	query_return = VirtualQuery(data, &mbi, sizeof(mbi));
	if (mbi.State == 4096)
		state = "MEM_COMMIT";
	else if (mbi.State == 65536)
		state = "MEM_FREE";
	else if (mbi.State == 8192)
		state = "MEM_RESERVE";
	std::cout << "Data state after free: " << state << std::endl;
	getchar();
    return 0;
}


