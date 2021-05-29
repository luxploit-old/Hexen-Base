#include <Windows.h>
#include "pattern.hpp"
#include <functional>
#include <sstream>
#include "../ClassPointers.hpp"

int total_pattern_count = 0;
int found_pattern_count = 0;

ptr_manage::ptr_manage(void* hand) { m_Ptr = hand; }
ptr_manage::ptr_manage(std::uintptr_t hand) { m_Ptr = reinterpret_cast<void*>(hand); }

ptr_manage ptr_manage::add(int offset) {
	return ptr_manage(as<std::uintptr_t>() + offset);
}
ptr_manage ptr_manage::rip(int offset) {
	if (!m_Ptr) return nullptr;
	return add(as<std::int32_t&>()).add(offset);
}

sModule::sModule(HMODULE hMod) :
	m_begin(hMod), m_end(nullptr), m_size(0) {
	auto dosHeader = ptr_manage(m_begin).as<IMAGE_DOS_HEADER*>();
	auto ntHeader = ptr_manage(m_begin).add(dosHeader->e_lfanew).as<IMAGE_NT_HEADERS*>();
	m_size = ntHeader->OptionalHeader.SizeOfImage;
	m_end = ptr_manage(m_begin.add(m_size));
}

sModule::sModule(std::string name) :
	sModule(GetModuleHandleA(name.c_str())) {}

ptr_manage sModule::get_begin() {
	return m_begin;
}

ptr_manage sModule::get_end() {
	return m_end;
}

ptr_manage sModule::get_export(std::string proc_name) {
	return ptr_manage(GetProcAddress(m_begin.as<HMODULE>(), proc_name.c_str()));
}

find_pattern::find_pattern(const char* pattern) {
	auto toUpper = [](char c) -> char {
		return c >= 'a' && c <= 'z' ? static_cast<char>(c + ('A' - 'a')) : static_cast<char>(c);
	};

	auto isHex = [&](char c) -> bool {
		switch (toUpper(c)) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return true;
		default:
			return false;
		}
	};

	do {
		if (*pattern == ' ')
			continue;
		if (*pattern == '?') {
			Element e = Element({}, true);
			m_Elements.push_back(e);
			continue;
		}

		if (*(pattern + 1) && isHex(*pattern) && isHex(*(pattern + 1))) {
			char str[3] = { *pattern, *(pattern + 1), '\0' };
			auto data = std::strtol(str, nullptr, 16);

			Element e = Element(static_cast<std::uint8_t>(data), false);
			m_Elements.push_back(e);
		}
	} while (*(pattern++));
}

ptr_manage find_pattern::Scan(sModule region) {
	auto compareMemory = [](std::uint8_t* data, Element* elem, std::size_t num) -> bool {
		for (std::size_t i = 0; i < num; ++i) {
			if (!elem[i].m_Wildcard)
				if (data[i] != elem[i].m_Data)
					return false;
		}
		return true;
	};

	for (std::uintptr_t i = region.get_begin().as<std::uintptr_t>(), end = region.get_end().as<std::uintptr_t>(); i != end; ++i) {
		if (compareMemory(reinterpret_cast<std::uint8_t*>(i), m_Elements.data(), m_Elements.size())) {
			return ptr_manage(i);
		}
	}
	return nullptr;
}

pattern_hisnt::pattern_hisnt(std::string name, find_pattern pattern, std::function<void(ptr_manage)> callback) :
	m_name(std::move(name)),
	m_pattern(std::move(pattern)),
	m_callback(std::move(callback))
{}

void pattern_batch::add(std::string name, find_pattern pattern, std::function<void(ptr_manage)> callback) {
	m_patterns.emplace_back(name, pattern, callback);
	total_pattern_count++;
}

void pattern_batch::run(std::string moduleName, bool silent) {
	run(GetModuleHandleA(moduleName.c_str()), silent);
}

void pattern_batch::run(HMODULE hModule, bool silent) {
	sModule* pMod = new sModule(hModule);
	bool all_found = true;
	for (auto &entry : m_patterns) {
		if (auto result = entry.m_pattern.Scan(*pMod)) {
			if (entry.m_callback) {
				if (result.as<uintptr_t>() != NULL) {
					found_pattern_count++;
					std::invoke(std::move(entry.m_callback), result);
					if (!silent && !entry.m_name.empty())
						printfnl("%s Pattern Status Code: [202 -> Accepted]", entry.m_name);
				}
				else if (!silent)
					printfnl("%s Pattern Status Code: [405 -> Method not Allowed]", entry.m_name);
			}
			else {
				all_found = false;
				if (!silent) 
					printfnl("%s Pattern Status Code: [422 -> Unprocessable Entry]", entry.m_name);
			}
		}
		else if (!silent)
			printfnl("%s Pattern Status Code: [404 -> Not Found]", entry.m_name);
	}

	m_patterns.clear();
	delete pMod;

	if (!all_found)
		printfnl("Final Pattern Status Code: [417 -> Expectation Failed]");
	else
		printfnl("Final Pattern Status Code: [200 -> OK]");

	printfnl("Final Pattern Result of Scanning: %i out of %i", found_pattern_count, total_pattern_count);
}