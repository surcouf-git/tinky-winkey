#include "tinky.hpp"
#include "utils.hpp"
#include <tlhelp32.h>

using namespace std;

extern tinky_t tinky;

int start(void) {
	cout	<< "Starting service...\n";

	if (!initControl())
		return (FAILURE);

	if (!StartServiceW(tinky.serviceHandler, NONE, NO_ARGS)) {
		cerr	<< "Failed to start service...";
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
			cerr	<< "Service already running !\n";
		else
			cerr	<< "Error code: " << GetLastError() << "\n";
		return (FAILURE);
	}
	cout	<< "Service started...\n";
	journalReport(L"Service started successfully\n");
	return (SUCCESS);
}