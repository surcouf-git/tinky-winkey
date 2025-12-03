#include <ws2tcpip.h>

#include "reverseShell.h"

extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;

//void forceStop(void) {
//	journalReport(L"Inside forcestop\n");

//	if (shell.listenSocket) {
//		closesocket(shell.listenSocket);
//		shell.listenSocket = 0;
//	}

//	if (shell.clientSocket) {
//		closesocket(shell.clientSocket);
//		shell.clientSocket = 0;
//	}

//	if (shell.parentRead) {
//		CancelIoEx(shell.parentRead, NULL);
//		CloseHandle(shell.parentRead);
//		shell.parentRead = NULL;
//	}
	
//	if (shell.parentWrite) {
//		CancelIoEx(shell.parentWrite, NULL);
//		CloseHandle(shell.parentWrite);
//		shell.parentWrite = NULL;
//	}

//	Sleep(100);

//	if (shell.threadListener) {
//		journalReport(L"Had to force thread listen stop\n");
//		TerminateThread(shell.threadListener, 0);
//		CloseHandle(shell.threadListener);
//		shell.threadListener = NULL;
//	}
	
//	if (shell.threadSender) {
//		journalReport(L"Had to force thread sender stop\n");
//		TerminateThread(shell.threadSender, 0);
//		CloseHandle(shell.threadSender);
//		shell.threadSender = NULL;
//	}

//	if (shell.processInfo.hProcess) {
//		TerminateProcess(shell.processInfo.hProcess, 0);
//		CloseHandle(shell.processInfo.hProcess);
//		CloseHandle(shell.processInfo.hThread);
//	}
//	journalReport(L"i'm here\n");
//}

void cleanUp(std::wstring name) {
	journalReport(L"Inside cleanup, called by " + name);

	if (shell.addrResult) {
		freeaddrinfo(shell.addrResult);
		shell.addrResult = NULL;
	}

	if (shell.listenSocket) {
		closesocket(shell.listenSocket);
		shell.listenSocket = NULL;
	}

	if (shell.clientSocket) {
		closesocket(shell.clientSocket);
		shell.clientSocket = NULL;
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

	//if (shell.threadListener) {
	//	TerminateThread(shell.threadListener, 1);
	//	CloseHandle(shell.threadListener);
	//}

	//if (shell.threadSender) {
	//	TerminateThread(shell.threadSender, 0);
	//	CloseHandle(shell.threadSender);
	//}

	//if (shell.threadsHandler) {
	//	TerminateThread(shell.threadsHandler, 0);
	//	CloseHandle(shell.threadsHandler);
	//}

	WSACleanup();

	shell = {};
	journalReport(L"done cleanup\n");
}