#pragma once
#include "Memory/pattern.hpp"
#include "GTA-Rage/ScrNative.hpp"
#include <dxgi.h>

namespace function_types {
	typedef int32_t(*tGetNumberOfEvents)(int32_t unknown);
	typedef int64_t(*tWndProc)(HWND windowid, uint32_t msg, WPARAM wPrm, LPARAM lPrm);
	typedef const char*(*tGetLabelText)(PVOID thisptr, const char* label);
	typedef void(*tFixVectors)(rage::scrNativeCallContext* callcontext);
}

class PatternScan {
public:
	void ScanPatterns();

	function_types::tGetNumberOfEvents pGetNumberOfEvents;
	function_types::tWndProc pWndProc;
	function_types::tGetLabelText pGetLabelText;
	function_types::tFixVectors pFixVectors;

	IDXGISwapChain* pSwapchain;
	rage::scrNativeRegistration** pNativeReg;
	PUCHAR pRegisterSpoof;
};