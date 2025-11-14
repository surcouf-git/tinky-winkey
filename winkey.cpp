#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define WINKEY_STOP "Global\\stop_winkey_process"

HANDLE journalHandle = NULL;

void journalReport(const char *msg) {
	ReportEventA(
		journalHandle,
		EVENTLOG_INFORMATION_TYPE,
		0,
		0,
		NULL,
		1,
		0,
		&msg,
		NULL
	);
}

int main(int argc, char* argv[]) {
	journalHandle = RegisterEventSourceA(NULL, "Winkey");
	journalReport("Hey !\n");
	journalReport(std::string("argv[0]: " + std::string(argv[0])).c_str());


	HANDLE eventHandler = OpenEventA(
		SYNCHRONIZE,
		FALSE,
		WINKEY_STOP
	);

	DWORD err = GetLastError();
	char result[10] = {};
	itoa(err, result, 10);
	if (!eventHandler) {
		journalReport(std::string("(2) handle null: " + std::string(result) + "\n").c_str());
	} else {
		memset(result, 0, 10);
		DWORD beuteu = 0;
		while (true) {
			beuteu = WaitForSingleObject(eventHandler, 100);
			itoa(beuteu, result, 10);
			if (beuteu == 0) {
				journalReport("Signal received, shutting down\n");
				break ;
			}
			Sleep(1000);
		}
	}
	journalReport("(2) process2 termine proprement\n");
	return 0;
}