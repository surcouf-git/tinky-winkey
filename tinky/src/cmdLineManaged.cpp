#include "tinky.hpp"

using namespace std;

extern tinky_t tinky;

static int openService(DWORD desiredAccess) {
	tinky.serviceHandler = OpenServiceW(
		tinky.scmHandler,
		SVC_NAME,
		desiredAccess
	);

	if (!tinky.serviceHandler) {
		cerr	<< "Failed to open service, code : " << GetLastError() << "\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

BYTE initControl(void) {
	if (!openSCM(SC_MANAGER_CONNECT))
		return (FAILURE);

	if (!openService(SERVICE_ALL_ACCESS))
		return (FAILURE);

	return (SUCCESS);
}