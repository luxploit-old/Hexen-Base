#pragma once
#include "ClassPointers.hpp"

void printfnl(const char* msg, ...) {
	char buffer[4096]{};
	va_list args{};

	va_start(args, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, args) + 1;
	printf(buffer);
	printf("\n");
	va_end(args);
}

namespace ClassPointers {
	std::unique_ptr<Hooks> cHooks = std::make_unique<Hooks>();
	std::unique_ptr<PatternScan> cPatterns = std::make_unique<PatternScan>();
	std::unique_ptr<Invoker> cInvoker = std::make_unique<Invoker>();
	std::unique_ptr<Helpers> cHelpers = std::make_unique<Helpers>();
	std::unique_ptr<DirectRender> cDX = std::make_unique<DirectRender>();

	void ResetPointers() {
		cHooks.reset();
		cPatterns.reset();
		cInvoker.reset();
		cHelpers.reset();
		cDX.reset();
	}
}