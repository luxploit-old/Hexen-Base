#pragma once
#include "Patterns.hpp"

void PatternScan::ScanPatterns() {
	pattern_batch main_batch;

	main_batch.add("GNOE", "48 83 EC 28 33 D2 85 C9", [this](ptr_manage callback) {
		pGetNumberOfEvents = callback.as<function_types::tGetNumberOfEvents>();
	});

	main_batch.add("WNPC", "48 8B 0D ? ? ? ? 48 8D 55 EF FF 15", [this](ptr_manage callback) {
		pWndProc = callback.add(-0x355).as<function_types::tWndProc>();
	});
	
	main_batch.add("GLT", "48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8", [this](ptr_manage callback) {
		pGetLabelText = callback.as<function_types::tGetLabelText>();
	});
	
	main_batch.add("DXSN", "48 8B 0D ? ? ? ? 48 8D 55 A0 48 8B 01", [this](ptr_manage callback) {
		pSwapchain = callback.add(3).rip(4).as<IDXGISwapChain*&>();
	});

	main_batch.add("NR", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [this](ptr_manage callback) {
		pNativeReg = callback.add(3).rip(4).as<rage::scrNativeRegistration**>();
	});

	main_batch.add("RS", "FF 23", [this](ptr_manage callback) {
		pRegisterSpoof = callback.as<PUCHAR>();
	});

	main_batch.run(GetModuleHandleW(nullptr));
}