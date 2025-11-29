#include <windows.h>

#include "tinky.hpp"

using namespace std;

extern tinky_t tinky;

//static void deleteAdditionalFiles(void) {

//}

// delete() was taken..lol
int uninstall(void) {
	cout	<< "Deleting service from service control manager...\n";

	if (!initControl())
		return (FAILURE);

	if (!DeleteService(tinky.serviceHandler)) {
		DWORD err = GetLastError();
		cerr	<< "Failed to delete service... ";
		if (err == ERROR_SERVICE_DOES_NOT_EXIST)
			cerr	<< "(service does not exist)" << "\n"; // Jamais atteint si initControl() fail... (ERROR_SERVICE_NOT_ACTIVE)
		else 
			cerr	<< "Error code: " << err << "\n";
		return (FAILURE);
	}

	//deleteAdditionalFiles();

	cout	<< "Service successfully deleted from service control manager\n";
	return (SUCCESS);
}