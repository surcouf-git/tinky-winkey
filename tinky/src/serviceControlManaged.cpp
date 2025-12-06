#include "tinky.hpp"
#include "utils.hpp"
#include "processes.h"

using namespace std;

extern tinky_t tinky;
extern processes_t processes;

void sendStatus(DWORD currentState, DWORD ctrlsAccepted) {
	tinky.svcStatus.dwCurrentState = currentState;
	tinky.svcStatus.dwControlsAccepted = ctrlsAccepted;
	
	SetServiceStatus(tinky.svcStatusHandle, &tinky.svcStatus);
}

/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/nc-winsvc-lphandler_function_ex */
DWORD WINAPI controlHandler(DWORD dwControl, DWORD, LPVOID, LPVOID) {
	switch(dwControl) {
		case (SERVICE_CONTROL_INTERROGATE):
			return (NO_ERROR);
		case (SERVICE_CONTROL_STOP):
			cleanUp();
			return (NO_ERROR);
		case (SERVICE_CONTROL_SHUTDOWN):
			cleanUp();
			return (NO_ERROR);
	}
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
	if (!impersonate())
		return (FAILURE);
	registerControlHandler();
	tinky.svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	sendStatus(SERVICE_START_PENDING, NONE);

	tinky.tinkyStopEventHandle = createEvent(NO_NAME);
	processes.stopEventHandle = createEvent(PROCESSES_STOP);
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
			journalReport(L"Process tinky stopped\n");
			return ;

		}
	}
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