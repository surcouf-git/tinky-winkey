#include "tinky.hpp"

using namespace std;

VOID WINAPI MyServiceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors) {
	wcout << L"Service démarré !\n";
		
	// Tu dois appeler RegisterServiceCtrlHandler ici
	// puis faire le travail de ton service...
	(void)dwNumServicesArgs, (void)lpServiceArgVectors;
}
/* SERVICE_TABLE_ENTRY = array of services */
static void initTableEntry(SERVICE_TABLE_ENTRY svcTableEntry[]) {
	svcTableEntry[0].lpServiceName = "";
	svcTableEntry[0].lpServiceProc = &MyServiceMain; // locale ?

	svcTableEntry[1].lpServiceName = NULL;
	svcTableEntry[1].lpServiceProc = NULL;
}

int start(tinky_t *tinky) {
	wcout << L"Entering start\n";
	SERVICE_TABLE_ENTRY svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);
	bool result = StartServiceCtrlDispatcher(svcTableEntry);

	wcout << L"CtrlDispatcher done with bool: " << result << L" - Last error: " << GetLastError() << "\n";
	(void)tinky;
	return (SUCCESS);
}