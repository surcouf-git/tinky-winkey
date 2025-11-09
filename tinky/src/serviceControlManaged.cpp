#include "tinky.hpp"
#include "utils.hpp"

using namespace std;

SERVICE_STATUS			gSvcStatus = {}; // Why samples are using globals ?
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
	if (dwControl == SERVICE_CONTROL_INTERROGATE)
		return (NO_ERROR);
	(void)dwEventType; // Event type, handle only if dwControl == SERVICE_CONTROL_SESSIONCHANGE
	(void)lpEventData; // Same as above
	(void)lpContext; // Same as RegisterServiceCtrlHandlerEx lpContext (void* user data)
	return (ERROR_CALL_NOT_IMPLEMENTED);
}

/* WINAPI ServiceMain() */
VOID WINAPI serviceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors) {
	wcout << L"Demarrage du service\n";
	
	gSvcStatusHandle = RegisterServiceCtrlHandlerEx(
		SVC_NAME,
		&handlerFunctionEx,
		NULL
	);
	cout << "Status handle returned: " << gSvcStatusHandle << '\n';
	//if (!gSvcStatusHandle) {
		//report event
		//return 
	//}

	/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/ns-winsvc-service_status */
	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
	gSvcStatus.dwServiceSpecificExitCode = 0;

	if (!SetServiceStatus(gSvcStatusHandle, &gSvcStatus)) {
		cerr << "SetServiceStatus failed with code : " << GetLastError() << "\n";
	} else {
		cout << "SetServiceStatus success\n";
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
	wcout << L"Entering start\n"; // TODO delete
	SERVICE_TABLE_ENTRYA svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	if (!StartServiceCtrlDispatcherA(svcTableEntry)) {
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