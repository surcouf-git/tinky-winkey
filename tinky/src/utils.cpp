#include <iostream>
#include "winMandatory.h"
#include <windows.h>
#include "utils.hpp"

using namespace std;

void trackMemoryLeaks(void) {
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

int printUsage(void) {
	wcerr	<< L"-> sc.exe [arg]\nList of args:\n\n"
			<< L"\t[install]: install the service svc.exe in the service control manager\n\n"
			<< L"\t[start]: start the service svc.exe in the background (need to install the service first)\n\n"
			<< L"\t[stop]: stop the running service svc.exe\n\n"
			<< L"\t[delete]: delete the service from the service control manager\n\n";
	return (EXIT_FAILURE);
}

int printErr(int wich, char *what) {
	if (wich == EINARG) {
		wcerr	<< "\"" << what << "\""
				<< L" -> invalid argument\n";
		printUsage();
	}
	return (EXIT_FAILURE);
}

void LogToFile(const char* message) {
	FILE* f = fopen(DEBUG_FILE, "a");
	if (f) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(f, "[%02d:%02d:%02d] %s\n", 
				st.wHour, st.wMinute, st.wSecond, message);
		fclose(f);
	}
}