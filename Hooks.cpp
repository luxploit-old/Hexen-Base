#pragma once
#include "Hooks.hpp"
#include "CreatedHooks/hooktables.hpp"
#include "ClassPointers.hpp"

void Hooks::TriggerHook(bool b1) {
	if (b1) {
		hkPolyHook::InitCapstone();
		hkWndProc::AttachInputHook();
		hkDirectX::AttachDirectXHooks();
		hkGTA::AttachGameHooks();

		printfnl("Final Hook Status Code [200 -> OK]");
	}
	else {

		hkGTA::DetachGameHooks();
		hkDirectX::DetachDirectXHooks();
		hkWndProc::DetachInputHook();
		hkPolyHook::DestoryCapstone();

		printfnl("Final Unhook Status Code [205 -> Reset]");
	}
}