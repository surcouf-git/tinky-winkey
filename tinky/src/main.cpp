#include "winMandatory.h"
#include <windows.h>
#include <iostream>
#include "tinky.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2)
		return (printUsage());

	char *args[] = { "install", "start", "stop", "delete" };
	int (*funcPtr[NFUNC]) (tinky_t *) = { &install, NULL, NULL, NULL };

	tinky_t	tinky = {};
	bool isJobDone = false;

	for (int i = 0; i < NFUNC; i++) {
		if (!strcmp(argv[1], args[i])) {
			cout << "Doing job (" << args[i] << ")\n";
			funcPtr[i](&tinky);
			isJobDone = true;
			cout << "Job done (" << args[i] << ")\n";
		}
	}

	if (isJobDone == false)
		return (printErr(EINARG, argv[1]));

	return (0);
}