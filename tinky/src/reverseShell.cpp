#include <windows.h>

#include "reverseShell.h"
#include "tinky.hpp"

int initShell(void) {
	static BYTE alreadyDone = 0;

	if (alreadyDone == TRUE)
		return (SUCCESS);
	alreadyDone = TRUE;
	return (SUCCESS);
}