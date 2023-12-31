#include "Dota2Patcher.h"
#include <regex>
#include <fstream>
#include <nlohmann/json.hpp>

bool Paths::get_dota_path_from_reg(std::string* path) {
    HKEY h_key{};
    if (int error = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Classes\\dota2\\Shell\\Open\\Command", 0, KEY_QUERY_VALUE, &h_key) != ERROR_SUCCESS) {
        std::cout << "[-] Registry Read Failed at RegOpenKey, Error: " << error << std::endl;
        RegCloseKey(h_key);
        return false;
    }

    char dota_path_reg[MAX_PATH]{};
    dota_path_reg[0] = '"';
    DWORD dota_path_size = sizeof(dota_path_reg) - sizeof(char);

    if (int error = RegQueryValueExA(h_key, nullptr, nullptr, nullptr, (LPBYTE)(dota_path_reg + 1), &dota_path_size) != ERROR_SUCCESS) {
        std::cout << "[-] Registry Read Failed at RegQueryValue, Error: " << error << std::endl;
        RegCloseKey(h_key);
        return false;
    }

    RegCloseKey(h_key);
    *path = dota_path_reg;

    return true;
}

bool Paths::get_dota_path(std::string* path) {
    // Get from config.json first
    std::ifstream config_file("config.json");
    if (config_file.is_open()) {
        nlohmann::json json_obj;
        config_file >> json_obj;
        config_file.close();
        
        if (json_obj.contains("dota2_path")) {
            *path = json_obj["dota2_path"];
            std::cout << "[+] Dota2 path from config.json: " << *path << std::endl;
            return true;
        }
    }

    // if nil, get from registry
    std::string dota_path;
    if (!get_dota_path_from_reg(&dota_path)) {
        return false;
    }

    std::regex rgx{ R"(([^]:\\[^]+\\game\\))" };
    std::smatch matches;

    if (!std::regex_search(dota_path, matches, rgx)) {
        std::cout << "[-] Failed to parse Dota path!" << std::endl;
        return false;
    }

    *path = matches[1].str();
    std::cout << "[+] Dota2 path from registry: " << *path << std::endl;

    return true;
}
