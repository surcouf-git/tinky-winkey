#include "tinky.hpp"
#include <iostream>
#include "utils.hpp"

using namespace std;

// TODO dont forget to CloseServiceHandle()

static int openSCM(tinky_t *tinky) {
	tinky->HServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE); // SC_MANAGER_ALL_ACCESS
	if (!tinky->HServiceControlManager) {
		wcerr << "Error while accessing Service Control Manager\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static int createService(tinky_t *tinky) {
	if (!openSCM(tinky))
		return (FAILURE);
	
	tinky->HService = CreateServiceA(
		tinky->HServiceControlManager,
		SVC_NAME,
		SVC_NAME,
		SC_MANAGER_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, // should log on error
		BINARY_PATH, // TODO what is the final binary path ?
		NULL, // group order
		NULL, // lpLoadOrderGroup
		NULL, // dependencies
		USER_NAME, //
		USER_PASS // TODO need to see what is LocalSystem etc..
	);

	if (!tinky->HService) {
		wcerr << L"CreateServiceA returned: " << tinky->HService << " (failure)" << '\n'; // Remove
		DWORD code = GetLastError();
		if (code == ERROR_SERVICE_EXISTS)
			wcerr << L"Service tinky already exist\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

int install(tinky_t *tinky) {
	if (!createService(tinky))
		return (FAILURE);

	return (SUCCESS);
}