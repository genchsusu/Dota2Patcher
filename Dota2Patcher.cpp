#include "Dota2Patcher.h"
#include <string>

int main(int argc, char* argv[]) {
    if (!Paths::get_dota_path(&Globals::dota_path)) {
        system("pause");
        return 0;
    }

    Patcher::patch_gameinfo();
    Patcher::patch_gameinfo_gi();
    
    system("pause");
    return 0;
}