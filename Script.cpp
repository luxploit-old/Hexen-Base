#pragma once
#include "Script.hpp"
#include "ClassPointers.hpp"

void Script::Tick() {
	if (GetTickCount64() < wakeAt) {
		if (GetCurrentFiber() != ClassPointers::cHelpers->gMainFiber) SwitchToFiber(ClassPointers::cHelpers->gMainFiber);
		return;
	}
	else if (scriptFiber) {
		ClassPointers::cHelpers->gCurrentScript = this;
		SwitchToFiber(scriptFiber);
		ClassPointers::cHelpers->gCurrentScript = nullptr;
	}
	else
		scriptFiber = CreateFiber(NULL, [](LPVOID handler) { reinterpret_cast<Script*>(handler)->Run(); }, this);
	SwitchToFiber(ClassPointers::cHelpers->gMainFiber);
}

void Script::Run() {
	callbackFunction();
}

void Script::Wait(DWORD time) {
	if (ClassPointers::cHelpers->gMainFiber && GetCurrentFiber() != ClassPointers::cHelpers->gMainFiber)
		SwitchToFiber(ClassPointers::cHelpers->gMainFiber);
	wakeAt = GetTickCount64() + time;
}

