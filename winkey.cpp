#include <iostream>
#include <windows.h>

using namespace std;

int main () {
	SERVICE_STATUS sStatus = {
		SERVICE_USER_OWN_PROCESS, // SERVICE_WIN32_OWN_PROCESS
	};
	cout << sStatus.dwServiceType << "<->" << SERVICE_USER_OWN_PROCESS << "<-\n";
	while (true) {
		Sleep(1000);
		wcerr << L"Service launched ! \n";
	}
}