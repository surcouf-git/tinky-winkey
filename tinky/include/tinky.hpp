#pragma once

#define UNICODE
#define _UNICODE

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define WINKEY_STOP "Global\\stop_winkey_process"  // NameSpace Global ?
#define LOCALSYS TEXT("surcouf\\LocalSystem")
#define SVC_NAME "tinky"
#define WINKEY_PATH "C:\\Users\\surcouf\\Desktop\\workspace-42\\tinky-winkey\\winkey.exe"
#define BINARY_PATH "C:\\Users\\surcouf\\Desktop\\workspace-42\\tinky-winkey\\prog.exe"
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
	WORD		onSingleProcess;
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
#define BIN_NAME ".\\winkey"
int startedBySCM(void);
void journalReport(const char *msg);