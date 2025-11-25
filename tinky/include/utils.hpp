#pragma once

#include <iostream>
#include <string>

using namespace std;

int printUsage(void);

#define EINARG 1
int printErr(int wich, char *what);

void journalReport(wstring msg);

wstring itostring(DWORD i);

wstring getServicePath(const wstring name);