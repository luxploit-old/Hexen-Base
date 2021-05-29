#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "Renderer.hpp"
#include "Helpers.hpp"
#include <string>
#include <STB/stb_image.h>
#include "ClassPointers.hpp"
#include "fonts.hpp"
#include "CreatedThreads/threadtable.hpp"

void DirectRender::InitRender() {
	ClassPointers::cPatterns->pSwapchain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));
	pDevice->GetImmediateContext(&pDeviceContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(ClassPointers::cHelpers->pGTAWindow);
	ImGui_ImplDX11_Init(pDevice, pDeviceContext);
	ImGui::StyleColorsClassic();

	pFont[0] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Nunito_compressed_data, Nunito_compressed_size, 22.f);
	pFont[1] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Nunito_compressed_data, Nunito_compressed_size, 15.f);
	pFont[2] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(MorningStar_compressed_data, MorningStar_compressed_size, 20.f);

	printfnl("DirectRender Status Code: [200 -> OK]");
}

DirectRender::~DirectRender() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DirectRender::PresentRunner() {
	if (bGuiOpen) {
		if (bLockMouseOnOpen) {
			ShowCursor(true);
			SetCursor(LoadCursorW(NULL, IDC_ARROW));
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CreatedThreads::GraphicsThread();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}