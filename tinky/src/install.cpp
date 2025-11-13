#include "tinky.hpp"
#include <iostream>
#include "utils.hpp"

using namespace std;

extern tinky_t tinky;

static int openSCM(void) {
	tinky.HServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE); // SC_MANAGER_ALL_ACCESS
	if (!tinky.HServiceControlManager) {
		wcerr << "Error while accessing Service Control Manager\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static int createService(void) {
	if (!openSCM())
		return (FAILURE);
	
	tinky.HService = CreateServiceA(
		tinky.HServiceControlManager,
		SVC_NAME,
		SVC_NAME,
		SC_MANAGER_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, // should log on error
		BINARY_PATH, // TODO what is the final binary path + GetCurrentDirectory() ?
		NULL, // group order
		NULL, // lpLoadOrderGroup
		NULL, // dependencies
		NULL,
		NULL
	);
	if (!tinky.HService) {
		cerr << "CreateServiceA returned: " << tinky.HService << " (failure)" << '\n'; // Remove
		DWORD code = GetLastError();
		if (code == ERROR_SERVICE_EXISTS)
			wcerr << L"Service tinky already exist\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

int install(void) {
	if (!createService())
		return (FAILURE);
	return (SUCCESS);
}