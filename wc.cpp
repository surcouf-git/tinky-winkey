#include <stdio.h>
#include <iostream>

int main() {
	FILE *file = fopen ("C:\\Users\\keylogger\\Desktop\\trash\\tinky-winkey\\file", "w+");

	if (!file)
		std::cout << "File open failed\n";

	std::cout << "File opened successfully\n";
	fprintf(file, "mange ta mere \n");
	fclose(file);
	return (0);
}