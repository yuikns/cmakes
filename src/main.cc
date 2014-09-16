#include <cstdio>

#include "crfpp.h" // crfpp
#include "config.h"

#include <iostream>

int main(int argc, char *argv[])
{
	printf("cmake sample starting ... in  version : %d.%.2f , compiled by  %s @ %s (%s) %s \n",GLO_VERSION_MAJOR,GLO_VERSION_MINOR,CMAKE_CXX_COMPILER_ID,CMAKE_SYSTEM,CMAKE_SYSTEM_NAME,CMAKE_SYSTEM_PROCESSOR);
	return 0;
}
