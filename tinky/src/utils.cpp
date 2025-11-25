#include <iostream>
//#include "winMandatory.h"
#include <windows.h>
#include "utils.hpp"

using namespace std;

extern HANDLE journalHandle;

int printUsage(void) {
	wcerr	<< L"-> svc.exe [arg]\nList of args:\n\n"
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

void journalReport(wstring msg) {
	const wchar_t *ptr = msg.c_str();
	ReportEventW(
		journalHandle,
		EVENTLOG_INFORMATION_TYPE,
		0,
		0,
		NULL,
		1,
		0,
		&ptr,
		NULL
	);
}

wstring itostring(DWORD i) {
	char buffer[16] = {};
	_itoa_s((int)i, buffer, 10);
	return (wstring((wchar_t *)buffer));
}

wstring getServicePath(const wstring name) {
	wchar_t currentDirectory[MAX_PATH] = {};

	GetCurrentDirectoryW(MAX_PATH, currentDirectory);
	return (wstring (currentDirectory + wstring(L"\\") + name + wstring(L".exe")));
}