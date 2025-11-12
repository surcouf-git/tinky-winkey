#include <windows.h>
#include <iostream>

int main(int argc, char* argv[]) {
	std::cout << "(2) argv[0]: " << argv[0] << '\n';
	if (argv[1])
		std::cout << "(2) argv[1]: " << argv[1] << '\n';
    HANDLE eventHandler = OpenEventA(
        SYNCHRONIZE,
        FALSE,
        (LPCSTR)argv[0]
    );
    if (!eventHandler) {
        std::cout << "(2) handle null code: " << GetLastError() << '\n';
    } else {
        DWORD beuteu = 0;
        while (true) {
            std::cout << "(2) in loop\n";
            beuteu = WaitForSingleObject(eventHandler, INFINITE);
            std::cout << "(2) received: " << beuteu << "\n";
            if (beuteu == 0) {
                std::cout << "Signal received, shutting down\n";
                break ;
            }
        }
    }
    std::cout << "(2) process2 termine proprement" << std::endl;
    return 0;
}