#include "Dota2Patcher.h"
#include <string>

void Patcher::Output(const char* text, ...) {
	std::cout << text << std::endl;
}

int main(int argc, char* argv[]) {

    // Check Dota2 Path
    if (!Paths::get_dota_path(&Globals::dota_path)) {
        system("pause");
        return 0;
    }

	// Run patch_gameinfo directly
	if (Patcher::patch_gameinfo(false))
		Patcher::Output("[+] Gameinfo Patched");

    system("pause");
    return 0;
}