#pragma once
#include <polyhook2/CapstoneDisassembler.hpp>
#include <polyhook2/Detour/x64Detour.hpp>
#include "../Memory/vmt_hook.hpp"
#include "../Patterns.hpp"
#include "../ClassPointers.hpp"
#include <dxgi.h>

namespace hk {
	/*VMT-Hook*/
	inline std::unique_ptr<vmt_hook> gDirectX = nullptr;					/*Vtable Hook for ImGui*/

	/*Detour*/
	inline std::unique_ptr<PLH::x64Detour> gGetNumberOfEvents = nullptr;	/*Main Execution Hook*/
	inline std::unique_ptr<PLH::x64Detour> gWndProc = nullptr;				/*Input Handler*/
	inline std::unique_ptr<PLH::x64Detour> gGetLabelText = nullptr;			/*Replace Text Labels*/

	/*PolyHook - v2*/
	inline std::unique_ptr<PLH::CapstoneDisassembler> gCapstone = nullptr;	/*Part of PolyHook*/
}

namespace hkPolyHook {
	void InitCapstone();
	void DestoryCapstone();
}

namespace hkDirectX {
	/*Typdefs*/
	typedef HRESULT (__stdcall* tSwapchainPresent)(IDXGISwapChain* chain, uint32_t sync, uint32_t flags);
	typedef HRESULT(__stdcall* tSwapchainResizeBuffers)(IDXGISwapChain* chain, uint32_t buffer, uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t flags);

	/*Hooks*/
	HRESULT __stdcall hkPresent(IDXGISwapChain* chain, uint32_t sync, uint32_t flags);
	HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* chain, uint32_t buffer, uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t flags);

	/*Attach- and Detaching*/
	void AttachDirectXHooks();
	void DetachDirectXHooks();
}

namespace hkWndProc {
	/*Hook*/
	LRESULT __stdcall hkWndProc(HWND hWnd, uint32_t msg, WPARAM wPrm, LPARAM lPrm);

	/*Attach- and Detaching*/
	void AttachInputHook();
	void DetachInputHook();
}

namespace hkGTA {
	/*Hooks*/
	const char* hkGetLabelText(PVOID unk, const char* label);
	int32_t hkGetNumberOfEvents(int32_t unk);

	/*Attach- and Detaching*/
	void AttachGameHooks();
	void DetachGameHooks();
}