#pragma once

#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

typedef struct process {
	wstring					processName;

	STARTUPINFOW			processStartupInfo;
	PROCESS_INFORMATION		processProcessInfo;

}	process_t;

typedef struct processes {
	HANDLE					systemToken;
	HANDLE					stopEventHandle;

	STARTUPINFOW			winkeyStartupInfo;
	PROCESS_INFORMATION		winkeyProcessInfo;

	vector<process_t>		processesVector;

}	processes_t;
