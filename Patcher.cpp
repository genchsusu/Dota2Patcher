#include "Dota2Patcher.h"
#include <fstream>
#include <string>
#include <sstream>

int Patcher::find_offset(char* array, int array_length, BYTE* pattern, int pattern_length) {
    for (int haystack_index = 0; haystack_index < array_length; haystack_index++) {
        bool needle_found = true;
        for (size_t needle_index = 0; needle_index < pattern_length; needle_index++) {
            char haystack_character = array[haystack_index + needle_index];
            char needle_character = pattern[needle_index];
            if (needle_character == 0x00 || haystack_character == needle_character)
                continue;
            else {
                needle_found = false;
                break;
            }
        }
        if (needle_found)
            return haystack_index;
    }

    return 0;
}

bool Patcher::get_byte_array(std::string file_path, char** ret_array, int* file_size) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    int file_length = static_cast<int>(file.tellg());
    if (file_length == 0) {
        std::cout << "[-] File Size is NULL!" << std::endl;
        return false;
    }

    char* array = new char[file_length];
    file.rdbuf()->pubseekoff(0, std::ios_base::beg);
    file.read(array, file_length);
    file.close();

    *file_size = file_length;
    *ret_array = array;

    return true;
}

int Patcher::find_offset(std::string file_path, BYTE* pattern, int pattern_size) {
    char* array = nullptr;
    int file_size = 0;

    if (!get_byte_array(file_path, &array, &file_size))
        return 0;

    int patch_offset = find_offset(array, file_size, pattern, pattern_size);

    return patch_offset;
}

void Patcher::apply_patch(std::string file_path, int patch_offset, BYTE replace[], int bytes_to_replace) {
    FILE* pFile;
    fopen_s(&pFile, file_path.c_str(), "r+b");

    for (int i = 0; i < bytes_to_replace; i++) {
        fseek(pFile, patch_offset + i, SEEK_SET);
        fputc(replace[i], pFile);
    }

    fclose(pFile);
}

// client.dll - gameinfo.gi CRC check bypass
bool Patcher::patch_gameinfo() {
    std::string client_path = Globals::dota_path + "dota\\bin\\win64\\client.dll";

    BYTE Replace[] = { 0xEB } ;

    int client_patch_offset = Patcher::find_offset(client_path, Globals::gameinfo_pattern, sizeof(Globals::gameinfo_pattern));
    if (!client_patch_offset) {
        Output("[-] Gameinfo Bypass Offset is already patched!");
        return false;
    }

    Patcher::apply_patch(client_path, client_patch_offset, Replace, sizeof(Replace));

    Output("[+] Gameinfo Patched");
    return true;
}

// gameinfo.gi - do modifications
bool Patcher::find_keyword(std::string file_path, std::string keyword) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(keyword) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void Patcher::update_gameinfo_gi(std::string file_path) {
    std::ifstream fileIn(file_path);
    std::stringstream buffer;
    buffer << fileIn.rdbuf();
    std::string content = buffer.str();
    fileIn.close();

    std::size_t pos = content.find("Game_Language");
    if (pos != std::string::npos) {
        std::size_t endOfLine = content.find("\n", pos);
        if (endOfLine != std::string::npos) {
            std::string toInsert = "\n\t\t\tGame\t\t\t\tauto_accept // Added\n\t\t\tMod\t\t\t\t\tauto_accept // Added";
            content.insert(endOfLine + 1, toInsert);
        }

    }

    std::ofstream fileOut(file_path);
    fileOut << content;
    fileOut.close();
}

bool Patcher::patch_gameinfo_gi() {
    std::string gameinfo_path = Globals::dota_path + "dota\\gameinfo.gi";

    int found = Patcher::find_keyword(gameinfo_path, "auto_accept");
    if (found) {
        Output("[-] Gameinfo.io is already patched!");
        return false;
    }

    Patcher::update_gameinfo_gi(gameinfo_path);
    Output("[+] auto_accept has been added to gameinfo.gi successfully.");
    return true;
}

void Patcher::Output(const char* text, ...) {
	std::cout << text << std::endl;
}