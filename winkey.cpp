#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

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