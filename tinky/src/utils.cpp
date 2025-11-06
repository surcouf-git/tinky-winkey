#include <iostream>
#include "utils.hpp"

using namespace std;

int printUsage(void) {
	wcerr	<< L"-> sc.exe [arg]\nList of args:\n\n"
			<< L"\t[install]: install the service svc.exe in the service control manager\n\n"
			<< L"\t[start]: start the service svc.exe in the background (need to install the service first)\n\n"
			<< L"\t[stop]: stop the running service svc.exe\n\n"
			<< L"\t[delete]: delete the service from the service control manager\n\n";
	return (EXIT_FAILURE);
}

int printErr(int wich, char *what) {
	if (wich == EINARG) {
		wcerr	<< "\"" << what << "\""
				<< L" -> invalid argument\n";
	}
	return (EXIT_FAILURE);
}