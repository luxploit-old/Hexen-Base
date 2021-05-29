#pragma once
#include "Helpers.hpp"
#include "ClassPointers.hpp"

IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Helpers::GetGTAWindow() {
	pGTAWindow = FindWindowA("grcWindow", "Grand Theft Auto V");
	printfnl("FindWindow Status Code: [200 -> OK]");
}

void Helpers::add_script(ScriptFunc script, const char* name) {
	vScripts.push_back(new Script(script, name));
	printfnl("%s Thread Creation Status Code: [200 -> OK]", name);
}

void Helpers::remove_scripts() {
	for (auto ss : vScripts)
		delete ss;
	printfnl("Final Thread Removal Status Code: [205 -> Reset]");
}


void Helpers::WndProc(HWND hWnd, uint32_t msg, WPARAM wPrm, LPARAM lPrm) {
	if (ImGui::GetCurrentContext())
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wPrm, lPrm);
}

/*
rage::GtaThread* Helpers::FindScriptThread(uint32_t hash) {
	for (auto thread : ClassPointers::cPointer->fnGTAThreads())
		if (thread && thread->m_context.ThreadID && thread->m_handler && thread->m_script_hash == hash)
			return thread;
	return nullptr;
}*/