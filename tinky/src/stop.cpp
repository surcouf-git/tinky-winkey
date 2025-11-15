#include <windows.h>

#include "tinky.hpp"

using namespace std;

extern tinky_t tinky;

int stop(void) {
	cout	<< "Stopping service...\n";

	if (!initControl())
		return (FAILURE);

	if (!ControlService(tinky.serviceHandler, SERVICE_CONTROL_STOP, &tinky.svcStatus)) {
		cerr	<< "Failed to stop service... ";
		if (GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
			cerr	<< "Service not running\n";
		else
			cerr	<< "Error code: " << GetLastError() << '\n';
		return (FAILURE);
	}
	cout	<< "Service stopped successfully\n";
	return (SUCCESS);
}