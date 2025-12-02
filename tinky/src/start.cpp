#include "tinky.hpp"
#include "utils.hpp"
#include <tlhelp32.h>

using namespace std;

extern tinky_t tinky;

static wchar_t **cleanArgumentLine(wchar_t **args) {
	return (args[2] ? &args[2] : NULL);
}

static DWORD argNumber(wchar_t **args) {
	DWORD i = 0;

	while (args && args[i])
		i++;
	return (i);
}

int start(void **args) {
	wchar_t **realArgs = cleanArgumentLine((wchar_t **)args);

	cout	<< "Starting service...\n";

	if (realArgs == NULL) cout << "Real args null\n";

	if (!initControl())
		return (FAILURE);

	if (!StartServiceW(tinky.serviceHandler, argNumber(realArgs), (LPCWSTR*)realArgs)) {
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