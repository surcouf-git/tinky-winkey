#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

void journalReport(const char *msg) {
	ReportEventA(
		journalHandle,                    // Handle de RegisterEventSourceA
		EVENTLOG_INFORMATION_TYPE,    // Type: INFO, WARNING ou ERROR
		0,                            // Catégorie (mettez 0)
		0,                            // Event ID (mettez 0 ou un code perso)
		NULL,                         // SID (mettez NULL)
		1,                            // Nombre de strings (1 pour un message simple)
		0,                            // Taille données binaires (mettez 0)
		&msg,                     // Adresse du pointeur vers votre string
		NULL                          // Données binaires (mettez NULL)
	);
}

int main(int argc, char* argv[]) {
	std::cout << "Hey !\n";
	FILE *file = fopen ("C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\file", "w+");
	if (!file)
		std::cout << "Failed to open file\n";

	HANDLE eventHandler = OpenEventA(
		SYNCHRONIZE,
		FALSE,
		(LPCSTR)argv[1]
	);

	if (!eventHandler) {
		std::cout << "(2) handle null code: " << GetLastError() << '\n';
	} else {
		DWORD beuteu = 0;
		while (true) {
			std::cout << "(2) in loop\n";
			beuteu = WaitForSingleObject(eventHandler, 100);
			std::cout << "(2) received: " << beuteu << "\n";
			if (beuteu == 0) {
				std::cout << "Signal received, shutting down\n";
				break ;
			}
			if (!fprintf(file, "1\n"))
				std::cout << "Cant write in\n";
			fflush(file);
			std::cout << "Living my best life\n";
			Sleep(1000);
		}
	}
	std::cout << "(2) process2 termine proprement" << std::endl;
	return 0;
}