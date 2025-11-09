#pragma once

#include <iostream>

using namespace std;

int printUsage(void);

#define EINARG 1
int printErr(int wich, char *what);

void trackMemoryLeaks(void);
#define DEBUG_FILE "C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\svc_debug.log"
void LogToFile(const char* message);