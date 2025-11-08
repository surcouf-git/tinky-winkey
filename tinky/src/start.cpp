#include "tinky.hpp"

using namespace std;

//void LPSERVICE_MAIN_FUNCTION(void) {
//	return (NULL);
//}

/* SERVICE_TABLE_ENTRY = array of services */
static void initTableEntry(SERVICE_TABLE_ENTRY svcTableEntry[]) {
	LPSERVICE_MAIN_FUNCTION svcMain = {};

	svcTableEntry[0].lpServiceName = BIN_NAME;
	svcTableEntry[0].lpServiceProc = svcMain; // locale ?

	svcTableEntry[1].lpServiceName = NULL;
	svcTableEntry[1].lpServiceProc = NULL;
}



int start(tinky_t *tinky) {
	wcout << L"Entering start\n";
	SERVICE_TABLE_ENTRY svcTableEntry[2] = {};

	initTableEntry(svcTableEntry);

	(void)tinky;
	return (SUCCESS);
}