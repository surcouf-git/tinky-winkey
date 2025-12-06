#pragma once

#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

/*
 * The first one contain one single process, 
 * the second one contains the main process winkey.exe 
 * + a vector of some eventuals bonus processes 
 * 
 */

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
