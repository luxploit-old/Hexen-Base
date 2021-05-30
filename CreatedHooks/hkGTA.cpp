#pragma once
#include "hooktables.hpp"
#include "../CreatedThreads/threadtable.hpp"

function_types::tGetNumberOfEvents oGetNumberOfEvents;
function_types::tGetLabelText oGetLabelText;

const char* hkGTA::hkGetLabelText(PVOID unk, const char* label) {
	if (!strcmp(label, "HUD_JOINING"))
		return "Status Code: [101 -> Switching Protcols]";
	if (!strcmp(label, "HUD_TRANSP"))
		return "Hexen Base [504 -> Gateway Timeout]";
	return oGetLabelText(unk, label);
}

int32_t hkGTA::hkGetNumberOfEvents(int32_t unk) {
	ClassPointers::cScripts->tick();
	return oGetNumberOfEvents(unk);
}

void hkGTA::AttachGameHooks() {
	hk::gGetNumberOfEvents = std::make_unique<PLH::x64Detour>(reinterpret_cast<uintptr_t>(ClassPointers::cPatterns->pGetNumberOfEvents),
		reinterpret_cast<uintptr_t>(&hkGTA::hkGetNumberOfEvents), reinterpret_cast<uintptr_t*>(&oGetNumberOfEvents), *hk::gCapstone);
	hk::gGetNumberOfEvents->hook();
	printfnl("GetNumberOfEvents Hook Status Code: [201 -> Created]");

	hk::gGetLabelText = std::make_unique<PLH::x64Detour>(reinterpret_cast<uintptr_t>(ClassPointers::cPatterns->pGetLabelText),
		reinterpret_cast<uintptr_t>(&hkGTA::hkGetLabelText), reinterpret_cast<uintptr_t*>(&oGetLabelText), *hk::gCapstone);
	hk::gGetLabelText->hook();
	printfnl("GetLabelText Hook Status Code: [201 -> Created]");
}

void hkGTA::DetachGameHooks() {
	hk::gGetNumberOfEvents->unHook();
	hk::gGetNumberOfEvents.reset();
	printfnl("GetNumberOfEvents UnHook Status Code: [205 -> Reset]");

	hk::gGetLabelText->unHook();
	hk::gGetLabelText.reset();
	printfnl("GetLabelText UnHook Status Code: [205 -> Reset]");
}