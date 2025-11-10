#include "tinky.hpp"
#include "utils.hpp"

using namespace std;

SERVICE_STATUS			gSvcStatus = {};
SERVICE_STATUS_HANDLE	gSvcStatusHandle = {};
HANDLE					ghSvcStopEvent = NULL;

/* subject ControlService() ? */
/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/nc-winsvc-lphandler_function_ex */
DWORD WINAPI handlerFunctionEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
	// TODO s
	// Handle SERVICE_CONTROL_PAUSE ?
	// Handle SERVICE_CONTROL_SHUTDOWN !
	// Handle SERVICE_CONTROL_STOP !
	// Handle SERVICE_CONTROL_SESSIONCHANGE !
	switch(dwControl) {
		case (SERVICE_CONTROL_INTERROGATE):
			return (NO_ERROR);
		case (SERVICE_CONTROL_STOP):
			return (NO_ERROR);
	}
	(void)dwEventType; // Event type, handle only if dwControl == SERVICE_CONTROL_SESSIONCHANGE
	(void)lpEventData; // Same as above
	(void)lpContext; // Same as RegisterServiceCtrlHandlerEx lpContext (void* user data)
	return (ERROR_CALL_NOT_IMPLEMENTED);
}

static void registerServiceHandler(void) {
	gSvcStatusHandle = RegisterServiceCtrlHandlerEx(
				SVC_NAME,
				&handlerFunctionEx,
				NULL
			);
}

static void sendStatus(DWORD currentState, DWORD ctrlsAccepted) {
	gSvcStatus.dwCurrentState = currentState;
	gSvcStatus.dwControlsAccepted = ctrlsAccepted;
	
	SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

static void launchProcess(LPCWSTR processPath) {
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};

	if (CreateProcess(
		processPath,
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	)) {
		//WaitForSingleObject(pi.hProcess, INFINITE);

		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);
	}
}

void initSvc(void) {
	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	sendStatus(SERVICE_START_PENDING, 0);

	launchProcess(WINKEY_PATH);
}

/* WINAPI ServiceMain() */
VOID WINAPI serviceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors) {
	registerServiceHandler();
	initSvc();

	sendStatus(SERVICE_RUNNING, SERVICE_ACCEPT_STOP);

	if (!SetServiceStatus(gSvcStatusHandle, &gSvcStatus)) {
		LogToFile("SetServiceStatus failed with code\n");
	} else {
		LogToFile("SetServiceStatus success\n");
	}
	(void)dwNumServicesArgs, (void)lpServiceArgVectors;
}

/* SERVICE_TABLE_ENTRY = array of services (name + funcptr)*/
static void initTableEntry(SERVICE_TABLE_ENTRYA svcTableEntry[]) {
	svcTableEntry[0].lpServiceName = DEF_NAME;
	svcTableEntry[0].lpServiceProc = &serviceMain;

	svcTableEntry[1].lpServiceName = NULL;
	svcTableEntry[1].lpServiceProc = NULL;
}

/* https://learn.microsoft.com/fr-fr/windows/win32/services/service-entry-point */
int startedBySCM(tinky_t *tinky) {
	LogToFile("Entering start\n");
	SERVICE_TABLE_ENTRYA svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	if (!StartServiceCtrlDispatcherA(svcTableEntry)) {

		string str("Error while StartServiceCtrl" + GetLastError() + '\n');
		LogToFile(str.c_str());

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
	(void)tinky;
	return (SUCCESS);
}