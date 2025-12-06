#include "tinky.hpp"
#include "utils.hpp"
#include "processes.h"

using namespace std;

extern tinky_t		tinky;
extern processes_t	processes;
extern HANDLE 		journalHandle;

static void terminateProcesses(void) {
	if (!TerminateProcess(processes.winkeyProcessInfo.hProcess, NONE)) {
		journalReport(wstring(L"Winkey terminate process failed with code: ") + itostring(GetLastError()) + wstring(L"\n"));
	}
	if (processes.winkeyProcessInfo.hProcess)
		CloseHandle(processes.winkeyProcessInfo.hProcess);

	if (processes.winkeyProcessInfo.hThread)
		CloseHandle(processes.winkeyProcessInfo.hThread);

	for (size_t i = 0; i < processes.processesVector.size(); i++) {
		process_t *process = &processes.processesVector[i];
	
		if (process->processProcessInfo.hProcess) {
			if (!TerminateProcess(process->processProcessInfo.hProcess, NONE)) {
				journalReport(wstring(L"generic terminate process failed with code: ") + itostring(GetLastError()) + wstring(L"\n"));
			}
		}
		if (process->processProcessInfo.hProcess)
			CloseHandle(processes.winkeyProcessInfo.hProcess);

		if (process->processProcessInfo.hThread)
			CloseHandle(process->processProcessInfo.hThread);
	}

}

static void closeHandles(void) {
	if (processes.systemToken)
		CloseHandle(processes.systemToken);

	if (tinky.tinkyStopEventHandle)
		CloseHandle(tinky.tinkyStopEventHandle);

	if (processes.stopEventHandle)
		CloseHandle(processes.stopEventHandle);

	if (journalHandle)
		CloseHandle(journalHandle);
}

static void stopAllProcesses(void) {
	SetEvent(tinky.tinkyStopEventHandle);
	SetEvent(processes.stopEventHandle);

	terminateProcesses();
	closeHandles();

	sendStatus(SERVICE_STOPPED, NONE);
}

void cleanUp(void) {
	stopAllProcesses();
}