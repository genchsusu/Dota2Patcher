#pragma once
#include <Windows.h>
#include <iostream>

namespace Paths {
	bool get_dota_path_from_reg(std::string* path);
	bool get_dota_path(std::string* path);
}

namespace Patcher {
	void Output(const char* text, ...);

	int find_offset(char* array, int array_length, BYTE* pattern, int pattern_length);
	int find_offset(std::string file_path, BYTE* pattern, int pattern_size);
	bool get_byte_array(std::string file_path, char** ret_array, int* file_size);
	void apply_patch(std::string file_path, int patch_offset, BYTE replace[], int bytes_to_replace);

	bool patch_gameinfo(bool revert);
}

namespace Globals {
	inline std::string local_version = "3.0.5.9";
	inline std::string dota_path;

	inline BYTE gameinfo_pattern[] = { 0X74, 0X00, 0X84, 0XC9, 0X75, 0X00, 0X83, 0XBF };
}
