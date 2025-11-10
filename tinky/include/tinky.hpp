#pragma once

#define UNICODE
#define _UNICODE

#include "winMandatory.h"
#include <windows.h>
#include <iostream>

//#include "unicode.h"

// =============== Magic variables ========== //
#define USER_NAME TEXT(".\\surcouf")
#define LOCALSYS TEXT("surcouf\\LocalSystem")
#define SVC_NAME TEXT("tinky")
#define DEF_NAME ""
#define WINKEY_PATH TEXT("C:\\Users\\surcouf\\Desktop\\trash\\tinky-winkey\\winkey.exe")
#define BINARY_PATH TEXT("C:\\Users\\surcouf\\Desktop\\trash\\tinky-winkey\\prog.exe")
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