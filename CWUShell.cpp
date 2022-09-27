#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
using namespace std;


void cwushell_startup(string prompt)
{
	execl("Clear");
}
int main(int argc, char *argv[])
{
//startup
	cwushell_startup("");
//loop
//exit

	return EXIT_SUCCESS;
}


