#include "tinky.hpp"
#include "utils.hpp"
#include "processes.h"

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

tinky_t			tinky = {};
processes_t		processes = {};

HANDLE			journalHandle = NULL;

int wmain(int argc, wchar_t **argv) {

	if (argc == 1) { /* Service Console Manager Call */

		journalHandle = RegisterEventSourceA(NULL, "Tinky"); // JOURNAL
		return (startedBySCM());

	} else if (argc > 1) { /* Console Call */

		const wchar_t *args[] = { L"install", L"start", L"stop", L"delete" };
		int (*funcPtr[]) (void **) = { &install, &start, &stop, &uninstall };
		
		bool isJobDone = false;

		for (int i = 0; i < NFUNC; i++) {
			if (!wcscmp(argv[1], args[i])) {
				funcPtr[i]((void **)argv);
				isJobDone = true;
			}
		}
		cleanExit(&tinky);
		return (isJobDone == false ? printErr(EINARG, (char *)argv[1]) : EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}