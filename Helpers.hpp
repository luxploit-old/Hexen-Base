#pragma once
#include "Script.hpp"

class Helpers {
public:
	void GetGTAWindow();
	void add_script(ScriptFunc script, const char* name);
	void remove_scripts();
	void WndProc(HWND hWnd, uint32_t msg, WPARAM wPrm, LPARAM lPrm);

	HANDLE gMainFiber{};
	std::vector<Script*> vScripts{};
	std::vector<Script*> vDirectXScripts{};
	Script* gCurrentScript{};
	HWND pGTAWindow{};
}; 

inline constexpr char ToLower(const char c) {
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

inline constexpr uint32_t Joaat(const char* str) {
	uint32_t hash = 0;
	while (*str) {
		hash += ToLower(*str++);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}