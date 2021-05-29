#pragma once
#include "hooktables.hpp"

function_types::tWndProc oWndProc = nullptr;

#define KeyPressed(x) if(GetAsyncKeyState(x) & 0x8000)
LRESULT __stdcall hkWndProc::hkWndProc(HWND hWnd, uint32_t msg, WPARAM wPrm, LPARAM lPrm) {
	KeyPressed(88) //X Key
		ClassPointers::cDX->bLockMouseOnOpen = !ClassPointers::cDX->bLockMouseOnOpen;
	KeyPressed(115) //F4 Key
		ClassPointers::cDX->bGuiOpen = !ClassPointers::cDX->bGuiOpen;
	ClassPointers::cHelpers->WndProc(hWnd, msg, wPrm, lPrm);
	return oWndProc(hWnd, msg, wPrm, lPrm);
}
#undef KeyPressed

void hkWndProc::AttachInputHook() {
	hk::gWndProc = std::make_unique<PLH::x64Detour>(reinterpret_cast<uintptr_t>(ClassPointers::cPatterns->pWndProc),
		reinterpret_cast<uintptr_t>(&hkWndProc::hkWndProc), reinterpret_cast<uintptr_t*>(&oWndProc), *hk::gCapstone);
	hk::gWndProc->hook();
	printfnl("WndProc Hook Status Code [201 -> Created]");
}

void hkWndProc::DetachInputHook() {
	hk::gWndProc->unHook();
	hk::gWndProc.reset();
	printfnl("WndProc UnHook Status Code [205 -> Reset]");
}