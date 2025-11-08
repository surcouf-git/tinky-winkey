#include <iostream>
#include <windows.h>

using namespace std;

int main () {
	while (true) {
		Sleep(1000);
		wcerr << L"Service launched ! \n";
	}
}