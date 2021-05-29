#pragma once
#include <memory>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui.h>
#include <d3d11.h>

class DirectRender {
public:
	~DirectRender();
	void InitRender();
	void PresentRunner();

	bool bGuiOpen = true;
	bool bLockMouseOnOpen = true;


	ImFont* pFont[3]{};
private:
	ID3D11Device* pDevice{};
	ID3D11DeviceContext* pDeviceContext{};
};