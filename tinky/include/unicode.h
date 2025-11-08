#pragma once

#include "winMandatory.h"
#include <windows.h>

#ifdef UNICODE
#define CreateService  CreateServiceW
#else
#define CreateService  CreateServiceA
#endif // !UNICODE

#ifdef UNICODE
typedef SERVICE_TABLE_ENTRYW SERVICE_TABLE_ENTRY;
typedef LPSERVICE_TABLE_ENTRYW LPSERVICE_TABLE_ENTRY;
#else
typedef SERVICE_TABLE_ENTRYA SERVICE_TABLE_ENTRY;
typedef LPSERVICE_TABLE_ENTRYA LPSERVICE_TABLE_ENTRY;
#endif // !UNICODE

#ifdef UNICODE
#define LPSERVICE_MAIN_FUNCTION LPSERVICE_MAIN_FUNCTIONW
#else
#define LPSERVICE_MAIN_FUNCTION LPSERVICE_MAIN_FUNCTIONA
#endif // !UNICODE

#ifdef UNICODE
#define StartServiceCtrlDispatcher  StartServiceCtrlDispatcherW
#else
#define StartServiceCtrlDispatcher  StartServiceCtrlDispatcherA
#endif // !UNICODE

#ifdef UNICODE
#define RegisterServiceCtrlHandlerEx  RegisterServiceCtrlHandlerExW
#else
#define RegisterServiceCtrlHandlerEx  RegisterServiceCtrlHandlerExA
#endif // !UNICODE