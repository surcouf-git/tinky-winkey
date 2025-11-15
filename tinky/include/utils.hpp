#pragma once

#include <iostream>

using namespace std;

int printUsage(void);

#define EINARG 1
int printErr(int wich, char *what);

#define DEBUG_FILE "C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\svc_debug.log"
void LogToFile(const char* message);

void journalReport(string msg);

string itostring(DWORD i);

string getServicePath(const string name);