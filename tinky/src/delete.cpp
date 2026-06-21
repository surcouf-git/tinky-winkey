#include <windows.h>

#include "tinky.hpp"

using namespace std;

extern tinky_t tinky;

// delete() was taken..lol
int uninstall(void**) {
	cout	<< "Deleting service from service control manager...\n";

	tinky.onDelete = TRUE;
	stop(NULL);

	if (!DeleteService(tinky.serviceHandler)) {
		DWORD err = GetLastError();
		cerr	<< "Failed to delete service... ";
		if (err == ERROR_SERVICE_DOES_NOT_EXIST)
			cerr	<< "(service does not exist)" << "\n";
		else 
			cerr	<< "Error code: " << err << "\n";
		return (FAILURE);
	}

	//deleteAdditionalFiles();

	cout	<< "Service successfully deleted from service control manager\n";
	return (SUCCESS);
}