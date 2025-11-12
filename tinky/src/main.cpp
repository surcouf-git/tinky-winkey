#include "tinky.hpp"
#include "utils.hpp"

using namespace std;

void cleanExit(tinky_t *tinky) {
	if (tinky->HServiceControlManager) {
		wcerr << "Closing HSCM\n";
		if (!CloseServiceHandle(tinky->HServiceControlManager))
			wcerr << L"Failed to close the Service Control Manager Handle\n";
	}

	if (tinky->HService) {
		wcerr << "Closing SVC\n";
		if (!CloseServiceHandle(tinky->HService))
			wcerr << L"Failed to close the Service Control Manager Handle\n";
	}
}

tinky_t tinky = {};

int __cdecl main(int argc, char **argv) {

	if (argc == 1) { /* Service Console Manager Call */

		startedBySCM();

	} else if (argc == 2) { /* Console Call */

		char *args[] = { "install", "start", "stop", "delete" };
		int (*funcPtr[NFUNC]) (tinky_t *) = { &install, &start, NULL, NULL };
		
		bool isJobDone = false;

		for (int i = 0; i < NFUNC; i++) {
			if (!strcmp(argv[1], args[i])) {
				cout << "Doing job (" << args[i] << ")\n";
				funcPtr[i](&tinky);
				isJobDone = true;
				cout << "Job done (" << args[i] << ")\n";
			}
		}
		if (isJobDone == false) return (printErr(EINARG, argv[1]));
		cleanExit(&tinky);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}