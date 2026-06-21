#include "tinky.hpp"
#include "utils.hpp"
#include <tlhelp32.h>
#include "processes.h"

using namespace std;

extern tinky_t tinky;
extern processes_t processes;

static DWORD findProcessId(const wchar_t *process_name)
{
	HANDLE process_snap = {};
	PROCESSENTRY32 process_entry = {};
	DWORD result = 0;

	process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (INVALID_HANDLE_VALUE == process_snap)
		return(FAILURE);

	process_entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(process_snap, &process_entry)) {
		journalReport(L"Failed to find pid... Error code: " + itostring(GetLastError()));
		CloseHandle(process_snap);
		return (FAILURE);
	}
	
	do {
		if (!wcscmp(process_name, process_entry.szExeFile)) {
			std::cout << "Process name: " << process_name << '\n';
			result = process_entry.th32ProcessID;
			break ;
		}
	} while (Process32Next(process_snap, &process_entry));

	CloseHandle(process_snap);
	return (result);
}

BYTE impersonate(void) {
	DWORD pid = findProcessId(L"winlogon.exe");
	HANDLE winlogonHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);

	if (winlogonHandle == INVALID_HANDLE_VALUE) {
		journalReport(L"Failed to open process... Error code: " + itostring(GetLastError()) + L"\n");
		return (FAILURE);
	}

	HANDLE tempToken = NULL;

	if (!OpenProcessToken(winlogonHandle, TOKEN_DUPLICATE | TOKEN_QUERY, &tempToken)) {
		journalReport(L"Failed to open process token... Error code: " + itostring(GetLastError()) + L"\n");
		CloseHandle(winlogonHandle);
		return (FAILURE);
	}

	if (!DuplicateTokenEx(
		tempToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,
		&processes.systemToken
	)) {
		journalReport(L"Failed to duplicate token... Error code: " + itostring(GetLastError()) + L"\n");
		CloseHandle(tempToken);
		CloseHandle(winlogonHandle);
		return (FAILURE);
	}

	CloseHandle(tempToken);
	CloseHandle(winlogonHandle);
	journalReport(L"Token impersonate success\n");
	return (SUCCESS);
}