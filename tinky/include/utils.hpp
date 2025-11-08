#pragma once

#include <iostream>

using namespace std;

int printUsage(void);

#define EINARG 1
int printErr(int wich, char *what);

void trackMemoryLeaks(void);