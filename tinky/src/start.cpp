#include "tinky.hpp"

using namespace std;

/* subject ControlService() ? */
/* https://learn.microsoft.com/fr-fr/windows/win32/api/winsvc/nc-winsvc-lphandler_function_ex */
DWORD WINAPI handlerFunctionEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
	(void)dwControl, (void)dwEventType, (void)lpEventData, (void)lpContext;
	if (dwControl == SERVICE_CONTROL_INTERROGATE)
		return (NO_ERROR);
	return (ERROR_CALL_NOT_IMPLEMENTED);
}

/* WINAPI ServiceMain() */
VOID WINAPI serviceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors) {
	wcout << L"Demarrage du service\n";
	
	SERVICE_STATUS_HANDLE serviceStatusHandle = RegisterServiceCtrlHandlerExA(
		SVC_NAME,
		&handlerFunctionEx,
		NULL
	);
	cout << "Status handle returned: " << serviceStatusHandle << '\n';

	SERVICE_STATUS sStatus = {
		.dwServiceType = SERVICE_USER_OWN_PROCESS, // SERVICE_WIN32_OWN_PROCESS
		.
	}
	if (!SetServiceStatus(serviceStatusHandle, )) {
		cerr << "SetServiceStatus failed with code : " << GetLastError() << "\n";
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
int start(tinky_t *tinky) {
	wcout << L"Entering start\n";
	SERVICE_TABLE_ENTRYA svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	if (!StartServiceCtrlDispatcherA(svcTableEntry)) {
		wcerr << L"Failed to start service: tinky (last error= " << GetLastError() << ")\n";
		return (FAILURE);
	}

	(void)tinky;
	return (SUCCESS);
}