#pragma once

#include "winMandatory.h"
#include <windows.h>

#define NFUNC 0x4

// =============== Structs =============== //
typedef struct tinky {
	SC_HANDLE	HService;
}	tinky_t;

// =============== createService.cpp ===============// 
int createService(tinky_t *tinky);