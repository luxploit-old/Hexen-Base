#pragma once
#include "Script.hpp"

class Helpers {
public:
	void GetGTAWindow();
	void WndProc(HWND hWnd, uint32_t msg, WPARAM wPrm, LPARAM lPrm);

	HWND pGTAWindow{};

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
private:
	inline constexpr char ToLower(const char c) {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
}; 

