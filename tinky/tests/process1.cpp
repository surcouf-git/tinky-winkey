#include <windows.h>
#include <iostream>
#include <stdio.h>

#define PROCESS_PATH TEXT("C:\\Users\\surcouf\\Desktop\\workspace-42\\tinky-winkey\\tinky\\tests\\process2.exe")
#define EVENTNAME TEXT("Global\\stop_winkey_process")

HANDLE hEvent = {};

static void createProcess() {
    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};

    std::string first("C:\\Users\\surcouf\\Desktop\\workspace-42\\tinky-winkey\\tinky\\tests\\process2.exe");
    std::string sec("Global\\stop_winkey_process");
    std::string both = first + ' ' + sec;

    if (CreateProcess(
        PROCESS_PATH,
        (LPSTR)sec.c_str(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cout << "(1) sleeping\n";
        Sleep(1000);
        std::cout << "(1) waking up\n";
        SetEvent(hEvent);
        std::cout << "Signal sent\n";
        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cout << "(1) process not launched with error code: " << GetLastError() << "\n";
    }
    std::cout << "(1) launch process done\n";
}

static void createHandle() {
    SECURITY_ATTRIBUTES secAttribute = {
        sizeof(SECURITY_ATTRIBUTES),
        NULL,
        FALSE
    };

    hEvent = CreateEvent(
        &secAttribute,
        FALSE,
        FALSE,
        EVENTNAME
    );
    if (!hEvent) {
        std::cout << "(1) hEvent NULL last error: " << GetLastError() << '\n';
    }
}

int main(int argc, char **argv) {
    createHandle();
    createProcess();
}