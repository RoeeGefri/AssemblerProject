#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "functions.h"
#include "allMethods.c"

int main(int argc, char** argv)
{	
	
	int i;
	for(i = 1 ; i < argc ; i++)
	{
		run(argv[i]);
	}
	return 0;
}
