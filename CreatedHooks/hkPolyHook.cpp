#pragma once
#include "hooktables.hpp"

void hkPolyHook::InitCapstone() {
	hk::gCapstone = std::make_unique<PLH::CapstoneDisassembler>(PLH::Mode::x64);
}

void hkPolyHook::DestoryCapstone() {
	hk::gCapstone.reset();
}