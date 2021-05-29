#pragma once
#include "hooktables.hpp"
#include "../ClassPointers.hpp"
#include <imgui_impl_dx11.h>

HRESULT __stdcall hkDirectX::hkPresent(IDXGISwapChain* chain, uint32_t sync, uint32_t flags) {
	if (!bUnInject) ClassPointers::cDX->PresentRunner();
	return hk::gDirectX->get_original<tSwapchainPresent>(8)(chain, sync, flags);
}

HRESULT __stdcall hkDirectX::hkResizeBuffers(IDXGISwapChain* chain, uint32_t buffer, uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t flags) {
	auto og = hk::gDirectX->get_original<tSwapchainResizeBuffers>(13)(chain, buffer, width, height, format, flags);
	if (SUCCEEDED(og))
		ImGui_ImplDX11_CreateDeviceObjects();
	return og;
}

void hkDirectX::AttachDirectXHooks() {
	hk::gDirectX = std::make_unique<vmt_hook>(ClassPointers::cPatterns->pSwapchain, 18);
	hk::gDirectX->hook(8, &hkDirectX::hkPresent);
	hk::gDirectX->hook(13, &hkDirectX::hkResizeBuffers);
	hk::gDirectX->enable();

	printfnl("DirectX Hook Status Code [201 -> Created]");
}

void hkDirectX::DetachDirectXHooks() {
	hk::gDirectX->unhook(8);
	hk::gDirectX->unhook(13);
	hk::gDirectX->disable();
	hk::gDirectX.reset();

	printfnl("DirectX UnHook Status Code [205 -> Reset]");
}