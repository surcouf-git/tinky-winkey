#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	std::cout << "Hey !\n";
	FILE *file = fopen ("C:\\Users\\surcouf\\Desktop\\trash\\tinky-winkey\\file", "w+");
	if (!file)
		std::cout << "Failed to open file\n";

	while (1)
	{
		if (!fprintf(file, "1\n"))
			std::cout << "Cant write in\n";
		fflush(file);
		std::cout << "Living my best life\n";
		Sleep(1000);
	}
	fclose(file);
	return (0);
}